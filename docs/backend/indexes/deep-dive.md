# Indexes — What They Cost, When They Help, EXPLAIN — Deep Dive

> **Track:** BE · **Type:** concept · **Scheduled:** Week 3 · Thu · **Runtime:** PostgreSQL 17 · **Generated:** 2026-10-24

> **How to read the outputs.** Every output below came from `psql -q` on PostgreSQL 17.5
> with `\pset null NULL` and `\pset format wrapped` at 88 columns — a plan line longer than
> the page wraps, with a `.` marking the break. Parallel workers are off for the session
> (`SET max_parallel_workers_per_gather = 0`) so plans stay one node per decision; on
> default settings a big seq scan would show the same decision wrapped in a `Gather` node
> with workers. The data is Tuesday's `shop` schema ([Postgres — Schema Design, Normalization
> & Joins](../postgresSchemaJoins/deep-dive.md)), scaled up as shown in Why It Exists, with
> the random seed pinned so a re-run builds the same rows. **Timings vary a few ms between
> runs and between machines — the stable, teachable part of every output is the plan shape
> and the row counts, and no exercise asks you to predict a time.** Autovacuum is off on this
> scratch cluster so that the one `ANALYZE` lesson is explicit; a real server would run it
> for you within a minute or so. One more wobble to expect: `rows=` *estimates* can differ
> by a few percent between rebuilds of the data, because `ANALYZE` samples — the plan's
> decisions are what reproduce.

> **Primer — SQL, just enough for this topic** (the full basics have no doc yet):
> `CREATE TABLE orders (…)` declares a table; `INSERT INTO orders VALUES (…)` adds rows.
> `SELECT id, placed_at FROM orders WHERE customer_id = 4242` reads: from the `orders`
> table, keep the rows whose `customer_id` is 4242, and return their `id` and `placed_at`.
> `ORDER BY placed_at DESC LIMIT 20` sorts the kept rows, newest first, and returns 20.
> `DELETE FROM orders WHERE …` removes the matching rows.
> `JOIN` matches rows across tables — taught properly in
> [Tuesday's deep dive](../postgresSchemaJoins/deep-dive.md).

## Why It Exists

Tuesday's `shop` schema holds a handful of hand-checkable rows. Real tables do not. Scale it
to a year of traffic — one new column (`shipped`) and four inserts; the seed makes the random
data reproducible:

```sql
ALTER TABLE orders ADD COLUMN shipped boolean NOT NULL DEFAULT true;

DO $$ BEGIN PERFORM setseed(0.42); END $$;

INSERT INTO customers (email, name)
SELECT 'customer' || n || '@example.com', 'Customer ' || n
FROM generate_series(4, 100000) AS n;

INSERT INTO products (name, price)
SELECT 'Product ' || n, (random() * 4000 + 99)::numeric(10, 2)
FROM generate_series(5, 1000) AS n;

INSERT INTO orders (customer_id, placed_at, shipped)
SELECT (random() * 99999 + 1)::bigint,
       timestamptz '2024-10-24 00:00+00' + random() * 730 * interval '1 day',
       random() < 0.95
FROM generate_series(1, 1000000);

INSERT INTO order_items (order_id, product_id, quantity, unit_price)
SELECT id, (random() * 999 + 1)::bigint, 1, 499.00
FROM orders
WHERE id > 3;

ANALYZE;
```

100,000 customers, a million orders. Now the most ordinary query a products API will ever
run — one customer's orders — and `EXPLAIN ANALYZE`, which runs a query and reports what it
did:

```sql
EXPLAIN ANALYZE SELECT * FROM orders WHERE customer_id = 4242;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Seq Scan on orders  (cost=0.00..19853.04 rows=11 width=25) (actual time=3.104..46.427 .
.rows=9 loops=1)
   Filter: (customer_id = 4242)
   Rows Removed by Filter: 999994
 Planning Time: 0.219 ms
 Execution Time: 46.452 ms
(5 rows)
```

Read the two damning numbers: the query needed **9 rows**, and to find them the database
checked every row it has and threw away **999,994** — a `Seq Scan`, 46 milliseconds of
reading a million rows to keep nine. Not because Postgres is naive, but because, as the
table stands, there is **no faster way to know where those 9 rows are**: Tuesday's docs
built what the rows *mean*; nothing yet says where any of them *live*. At one query this is
46 ms; at an API endpoint called on every page load it is the whole latency budget
(`150 KB of JS at 1.5 Mbps ≈ 800 ms` was the frontend budget — this one query would be 6%
of it, for nine rows).

An index is the fix: a separate, *sorted* structure that maps each `customer_id` to the
addresses of its rows. This doc is what that structure is, what it costs you everywhere
else, and how to read `EXPLAIN` — the tool that tells you whether any of it worked.

## Level 1 — Intuition

The story is the picture book from the [brief](brief.md) → Explain It Like I'm 5: to find
the dinosaur pages you either flip through the whole book, or you turn to the back — a
**sorted** list of topics, each with its page numbers — jump straight to "dinosaurs", and go
exactly where it points.

The whole topic is in that back-of-book list:

- It is **separate** from the pages — adding it changes no story, no picture, no answer.
- It is **sorted**, and sorted is the superpower: you can find "dinosaurs" in a 1,000-topic
  list in a few hops (the Week 2 way — binary search), not 1,000 reads.
- It points, it does not contain — you still **go to the page** for the actual picture.
- And it is not free: paste one new picture into the book and someone must fix the list —
  **every list the book keeps**.

In the database, one statement builds the list:

```sql
CREATE INDEX idx_orders_customer_id ON orders (customer_id);
```

```text
Time: 225.509 ms
```

A quarter-second once (the librarian reads the whole book one time to write the list), and
the 46 ms query above drops to 0.066 ms — shown with its plan in Level 2.

**Recap:** an index is a sorted side-structure mapping values to row addresses; it makes
*finding* fast, changes no answers, and must be maintained on every change to the table.

**Check yourself (C1):** the back-of-book index helps you find "dinosaurs". Why does it not
help you answer "how many pages does this book have?"

<details>
<summary>Answer</summary>

Because that question is about *every* page, not about finding a few: you would still have
to count them all, and the list adds nothing (worse — reading the list *and* the book is
more work). Same in SQL: a query that touches most of the table gains nothing from knowing
where each row is — the seq scan was never the problem. That is the selectivity rule,
Level 3.

</details>

## Level 2 — How It Actually Works

### What the index physically is

*(Idea 1)* The table itself — the **heap** — is just pages of rows in no useful order (rows
land where there is space; the end of this doc proves even *that* order shifts). The index
is a separate structure on disk, a **B-tree**: a few levels of sorted guide pages on top,
and at the bottom a sorted row of **leaf entries**, each one `value → row address`:

```text
                      idx_orders_customer_id — a B-tree (sketch)

                          [ guide:   ≤ 31400 | ≤ 62800 | … ]          root page
                            /            |
               [ … ≤ 4210 | ≤ 8457 … ]   …                            guide pages
                      |
   … | 4241 | 4242 | 4242 | … | 4242 | 4243 | …                       leaf pages (sorted)
              │      │          │
              ▼      ▼          ▼
        (page 1537, row 4) (page 2209, row 1) …                       heap row addresses

   heap = the orders table: 57 MB of pages, rows in no useful order
```

(The guide values are a sketch — Postgres does not print its tree — but the shape is exact:
sorted leaves, a few guide levels, one address per entry.) The index holds **only** the
indexed column and addresses, which is why it is a fraction of the table's size:

```sql
SELECT pg_size_pretty(pg_relation_size('orders'))                 AS table_size,
       pg_size_pretty(pg_relation_size('idx_orders_customer_id')) AS index_size;
```

```text
 table_size | index_size
------------+------------
 57 MB      | 9088 kB
(1 row)
```

*(Idea 2)* Finding `customer_id = 4242` is now a **walk, not a scan**: start at the root,
each guide page says which child covers 4242, land on the leaf, read the handful of matching
entries — about 3 page reads instead of thousands, `O(log n)` instead of `O(n)`. Then one
more step the book analogy keeps honest: the index only *points*, so for `SELECT *` the
database still visits the heap page behind each address to fetch the actual row. Cheap for
9 addresses — and the root of two rules later: scattered addresses make unselective index
use expensive (Level 3 → Selectivity), and an index that already *contains* everything the
query wants can skip the heap trip entirely (Level 3 → Index-only scans).

### Reading EXPLAIN — the guess and the receipt

*(Idea 3)* `EXPLAIN` shows the **plan**: what the planner intends to do and what it
estimates. It does not run the query. Same query as Why It Exists, after the index:

```sql
EXPLAIN SELECT * FROM orders WHERE customer_id = 4242;
```

```text
                                      QUERY PLAN
--------------------------------------------------------------------------------------
 Bitmap Heap Scan on orders  (cost=4.51..47.37 rows=11 width=25)
   Recheck Cond: (customer_id = 4242)
   ->  Bitmap Index Scan on idx_orders_customer_id  (cost=0.00..4.51 rows=11 width=0)
         Index Cond: (customer_id = 4242)
(4 rows)
```

Four numbers per node, and they are the reading skill:

| Number          | Meaning                                                                  |
| --------------- | ------------------------------------------------------------------------ |
| `cost=4.51..47.37` | Estimated cost: to the first row, then to the last — abstract units, comparable only to other plans for the same query |
| `rows=11`       | Estimated row count **from statistics** — a guess, not a measurement       |
| `width=25`      | Estimated bytes per row                                                   |
| (inner → outer) | Plans read inside-out: the indented node runs first and feeds its parent  |

`EXPLAIN ANALYZE` is the receipt: it **actually executes the query** and adds
`(actual …)` beside every estimate:

```sql
EXPLAIN ANALYZE SELECT * FROM orders WHERE customer_id = 4242;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Bitmap Heap Scan on orders  (cost=4.51..47.37 rows=11 width=25) (actual time=0.023..0..
.043 rows=9 loops=1)
   Recheck Cond: (customer_id = 4242)
   Heap Blocks: exact=9
   ->  Bitmap Index Scan on idx_orders_customer_id  (cost=0.00..4.51 rows=11 width=0) (.
.actual time=0.017..0.017 rows=9 loops=1)
         Index Cond: (customer_id = 4242)
 Planning Time: 0.225 ms
 Execution Time: 0.066 ms
(7 rows)
```

46.452 ms → 0.066 ms — **700× faster** — and the `Rows Removed by Filter: 999994` line is
simply gone: nothing was read that was not wanted. Estimated `rows=11` vs actual `rows=9`
is the planner's statistics being close but not exact — Level 3 shows where that guess
comes from and what happens when it goes stale.

The node here is not plain `Index Scan` but a pair — that is the planner choosing *how* to
use the index, and the node names are worth knowing cold:

| Node                          | What it does                                                        | You see it when                                    |
| ----------------------------- | -------------------------------------------------------------------- | --------------------------------------------------- |
| `Seq Scan`                    | Reads the whole heap, filters                                        | No usable index — or the index would not pay (Level 3) |
| `Index Scan`                  | Walks the index; visits the heap per entry, in index order           | Few rows, or the index order serves an `ORDER BY`   |
| `Bitmap Index Scan` + `Bitmap Heap Scan` | Collects all matching addresses first, sorts them by page, then visits each heap page once | More rows, scattered — one visit per page instead of per row |
| `Index Only Scan`             | Answers from the index alone, heap skipped                           | The index covers every selected column (Level 3)    |

*(Idea 4)* And the cost side of the ledger. The librarian fixes the list on **every**
paste — each `INSERT`, `UPDATE` or `DELETE` maintains every index the table has, and it is
measurable, not folklore. The same million-row insert into a copy of `orders`, first with no
indexes, then with three:

```sql
CREATE TABLE orders_copy (LIKE orders);
\timing on
INSERT INTO orders_copy SELECT * FROM orders;
```

```text
Time: 839.313 ms
```

```sql
TRUNCATE orders_copy;
CREATE INDEX ON orders_copy (customer_id);
CREATE INDEX ON orders_copy (placed_at);
CREATE INDEX ON orders_copy (shipped);
\timing on
INSERT INTO orders_copy SELECT * FROM orders;
```

```text
Time: 4560.049 ms (00:04.560)
```

839 ms → 4,560 ms: three indexes made the same write **5.4× slower** (the exact ratio moves
run to run; the multiple does not go away). An index is a standing tax on every write to the
table, paid whether or not anyone ever reads it. That is why "just index everything" is not
a strategy — every index must earn its keep against this number, and the end of this doc
shows a table whose indexes outweigh the table itself.

**Recap:** the heap holds rows in no useful order; a B-tree index is a sorted side-structure
of `value → address` entries, walked in `O(log n)`. `EXPLAIN` shows the plan with estimated
cost/rows; `EXPLAIN ANALYZE` executes and shows actuals. Every index taxes every write —
measured here at 5.4× for three of them.

**Check yourself (C2):** in the `EXPLAIN ANALYZE` output above, which two values would you
quote to prove the index "worked", and which value is still only a guess?

<details>
<summary>Answer</summary>

Proof it worked: `Execution Time: 0.066 ms` against the pre-index 46.452 ms, and the
disappearance of `Rows Removed by Filter: 999994` (nothing unwanted was read — `Heap
Blocks: exact=9`). Still a guess: `rows=11` in the `cost=…` parenthesis — that is the
statistics-based estimate; the measured truth is the `rows=9` inside `(actual …)`.

</details>

## The Analogy, Mapped

| In the story                                   | In reality                                           | Idea # |
| ---------------------------------------------- | ----------------------------------------------------- | ------ |
| The book's pages, in page order                | The heap — the table's rows, in no useful order       | 1      |
| The sorted topic list at the back              | The B-tree index, sorted leaf entries                 | 1      |
| "dinosaurs → p. 48, 112" points, doesn't show  | Leaf entries hold `value → row address`, not the row  | 2      |
| Jumping straight to "d…" in the sorted list    | The `O(log n)` walk down guide pages to the leaf      | 2      |
| Still turning to page 48 for the picture       | The heap visit behind every index hit                 | 2      |
| Fixing every list after pasting in a picture   | Every write maintains every index — the 5.4× run      | 4      |
| A list of *only* the pop-up pages              | A partial index (Level 3)                             | 11     |

**Where the analogy breaks:**

- A book's index is updated by hand and drifts out of date; the database's never lies — it
  is maintained in the same transaction as the write, which is exactly why writes pay.
- Books have one index. Tables can have many, and each multiplies the write tax.
- A book index lists topics once; a B-tree holds one entry per *row*, so it grows with the
  table, not with the number of distinct values.
- Nobody chooses whether to use a book's index — the database *decides*, by cost, and may
  rightly ignore it (Level 3). The list existing does not mean the list gets used.

## Level 3 — The Formal Picture

Postgres's own vocabulary for what Level 2 showed: `CREATE INDEX` builds a **B-tree** by
default (the PostgreSQL docs' index-type chapter; the other types are in Variants below).
Leaf entries point at heap rows by **TID** (page number, slot number) — the "addresses"
above. `PRIMARY KEY` and `UNIQUE` constraints are *enforced* by B-tree indexes Postgres
creates automatically (that is how `customers_email_key` below exists without anyone
creating it — and the key-vs-index boundary itself is Tuesday's:
[a key decides what may exist; an index decides how fast it is reached](../postgresSchemaJoins/brief.md)).

### The planner is a cost estimator, not a rule follower

*(Idea 5)* Nothing in SQL says "use the index". The **planner** prices every way it knows
to answer the query — using **statistics** that `ANALYZE` gathers per column (row count,
distinct values, most-common values, histogram) — and runs the cheapest plan. Two honest
consequences follow.

A tiny table ignores its index, correctly — `categories` has three rows and a primary-key
index, and:

```sql
EXPLAIN ANALYZE SELECT * FROM categories WHERE id = 2;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Seq Scan on categories  (cost=0.00..1.04 rows=1 width=15) (actual time=0.848..0.848 ro.
.ws=1 loops=1)
   Filter: (id = 2)
   Rows Removed by Filter: 2
 Planning Time: 0.227 ms
 Execution Time: 0.864 ms
(5 rows)
```

Three rows fit in one page; reading that page is one I/O, while walking even the smallest
B-tree is more. "Postgres is not using my index" on a small (or test!) table is the planner
being right — which is also why a query plan from your 50-row dev database proves nothing
about production.

And the estimates are only as good as the statistics. A freshly loaded table, before
`ANALYZE` has seen it:

```sql
CREATE TABLE events (id bigint, kind text);
INSERT INTO events
SELECT n, CASE WHEN n % 100 = 0 THEN 'error' ELSE 'click' END
FROM generate_series(1, 100000) AS n;

EXPLAIN ANALYZE SELECT * FROM events WHERE kind = 'error';
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Seq Scan on events  (cost=0.00..1352.50 rows=325 width=40) (actual time=0.014..47.732 .
.rows=1000 loops=1)
   Filter: (kind = 'error'::text)
   Rows Removed by Filter: 99000
 Planning Time: 0.083 ms
 Execution Time: 47.787 ms
(5 rows)
```

```sql
ANALYZE events;
EXPLAIN ANALYZE SELECT * FROM events WHERE kind = 'error';
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Seq Scan on events  (cost=0.00..1791.00 rows=1063 width=14) (actual time=0.009..4.537 .
.rows=1000 loops=1)
   Filter: (kind = 'error'::text)
   Rows Removed by Filter: 99000
 Planning Time: 0.078 ms
 Execution Time: 4.568 ms
(5 rows)
```

Before: estimated `rows=325` against an actual 1,000 — a guess from defaults, off 3×. After
one `ANALYZE`: `rows=1063`, within 7%. A planner working from stale statistics prices plans
on fiction, and a wildly wrong `rows=` estimate against actual is the signature to look for
in any slow plan. (On a normal server autovacuum re-analyzes busy tables for you; this
scratch cluster has it off so the lesson is visible.)

### Selectivity — when the index would not pay

*(Idea 6)* Walking the index costs per *row found* (walk, then a heap visit at a random
page); a seq scan costs per *row stored* but reads pages in order. So the index wins only
when the query keeps a small fraction — the predicate is **selective**. The same index, two
predicates, opposite decisions. `shipped` is true for 95% of the million orders:

```sql
CREATE INDEX idx_orders_shipped ON orders (shipped);
EXPLAIN ANALYZE SELECT * FROM orders WHERE shipped;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Seq Scan on orders  (cost=0.00..17353.03 rows=949270 width=25) (actual time=0.009..52..
.806 rows=949957 loops=1)
   Filter: shipped
   Rows Removed by Filter: 50046
 Planning Time: 0.269 ms
 Execution Time: 70.371 ms
(5 rows)
```

```sql
EXPLAIN ANALYZE SELECT * FROM orders WHERE NOT shipped;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Index Scan using idx_orders_shipped on orders  (cost=0.42..6720.41 rows=50733 width=25.
.) (actual time=0.020..15.496 rows=50046 loops=1)
   Index Cond: (shipped = false)
 Planning Time: 0.253 ms
 Execution Time: 16.451 ms
(4 rows)
```

Fetching 950K of 1M rows through the index would mean ~950K random heap hops — the planner
refuses and scans. Fetching the rare 5% uses it. The statistics (most-common values) are
what let it tell the two apart *before* running. Rule of thumb the output supports: an index
on a column where the searched value covers a large share of the table is dead weight for
that query — if only the rare value is ever searched, a partial index (below) does the same
job at a fraction of the cost.

### The query must match the index's shape

*(Idea 7)* A B-tree on `email` is sorted by `email` — exactly `email`, not by anything
computed *from* it. The auto-created unique index serves the exact match:

```sql
EXPLAIN ANALYZE SELECT id, name FROM customers
WHERE email = 'customer4242@example.com';
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Index Scan using customers_email_key on customers  (cost=0.42..8.44 rows=1 width=22) (.
.actual time=0.024..0.024 rows=1 loops=1)
   Index Cond: (email = 'customer4242@example.com'::text)
 Planning Time: 1.557 ms
 Execution Time: 0.044 ms
(4 rows)
```

Wrap the column in a function — the case-insensitive login lookup every app writes — and
the sort order the index stores no longer answers the question being asked:

```sql
EXPLAIN ANALYZE SELECT id, name FROM customers
WHERE lower(email) = 'customer4242@example.com';
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Seq Scan on customers  (cost=0.00..2522.00 rows=500 width=22) (actual time=0.828..18.8.
.07 rows=1 loops=1)
   Filter: (lower(email) = 'customer4242@example.com'::text)
   Rows Removed by Filter: 99999
 Planning Time: 0.232 ms
 Execution Time: 18.824 ms
(5 rows)
```

Back to 100,000 rows read for 1. The fix is an index on the **expression** itself — a
B-tree sorted by `lower(email)`:

```sql
CREATE INDEX idx_customers_email_lower ON customers (lower(email));
EXPLAIN ANALYZE SELECT id, name FROM customers
WHERE lower(email) = 'customer4242@example.com';
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Bitmap Heap Scan on customers  (cost=20.29..879.42 rows=500 width=22) (actual time=0.0.
.14..0.014 rows=1 loops=1)
   Recheck Cond: (lower(email) = 'customer4242@example.com'::text)
   Heap Blocks: exact=1
   ->  Bitmap Index Scan on idx_customers_email_lower  (cost=0.00..20.17 rows=500 width.
.=0) (actual time=0.010..0.010 rows=1 loops=1)
         Index Cond: (lower(email) = 'customer4242@example.com'::text)
 Planning Time: 0.267 ms
 Execution Time: 0.027 ms
(7 rows)
```

Same trap, other costume: a **leading wildcard**. A sorted list is useless to someone who
only knows how the word *ends*:

```sql
EXPLAIN ANALYZE SELECT count(*) FROM customers
WHERE email LIKE '%42@example.com';
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Aggregate  (cost=2277.05..2277.06 rows=1 width=8) (actual time=7.375..7.376 rows=1 loo.
.ps=1)
   ->  Seq Scan on customers  (cost=0.00..2272.00 rows=2020 width=0) (actual time=0.008.
...7.349 rows=1000 loops=1)
         Filter: (email ~~ '%42@example.com'::text)
         Rows Removed by Filter: 99000
 Planning Time: 1.231 ms
 Execution Time: 7.401 ms
(6 rows)
```

No B-tree can serve `LIKE '%…'`; substring search needs a different index family entirely
(`pg_trgm` + GIN — named in Variants, beyond this topic's depth). The general rule: the
planner uses an index when the predicate is a direct condition **on the indexed expression**
— a bare column compared to a value, or the exact expression the index was built on. A
function, a cast or a leading wildcard over the column silently turns the query back into a
scan, with no error and no warning — only `EXPLAIN` tells you.

### Composite indexes — sorted by two things

*(Idea 8)* `ON orders (customer_id, placed_at)` builds **one** B-tree sorted by
`customer_id`, and *within* each customer, by `placed_at` — a phone directory: surname,
then first name. One directory serves "all Raos" and "Raos named Priya", but it is useless
for "everyone named Priya" — first names are scattered across every surname. Same here:

```sql
CREATE INDEX idx_orders_customer_placed ON orders (customer_id, placed_at);
EXPLAIN ANALYZE SELECT * FROM orders
WHERE customer_id = 4242 AND placed_at >= timestamptz '2025-06-01 00:00+00';
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Bitmap Heap Scan on orders  (cost=4.51..35.84 rows=8 width=25) (actual time=0.016..0.0.
.33 rows=7 loops=1)
   Recheck Cond: ((customer_id = 4242) AND (placed_at >= '2025-06-01 00:00:00+00'::time.
.stamp with time zone))
   Heap Blocks: exact=7
   ->  Bitmap Index Scan on idx_orders_customer_placed  (cost=0.00..4.50 rows=8 width=0.
.) (actual time=0.011..0.011 rows=7 loops=1)
         Index Cond: ((customer_id = 4242) AND (placed_at >= '2025-06-01 00:00:00+00'::.
.timestamp with time zone))
 Planning Time: 0.356 ms
 Execution Time: 0.057 ms
(7 rows)
```

Both conditions landed in `Index Cond` — the walk jumps to customer 4242 *and* starts at
June 2025 within that customer's run of entries. Now the second column **alone**:

```sql
EXPLAIN ANALYZE SELECT count(*) FROM orders
WHERE placed_at >= timestamptz '2026-10-17 00:00+00';
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Aggregate  (cost=19877.07..19877.08 rows=1 width=8) (actual time=39.746..39.747 rows=1.
. loops=1)
   ->  Seq Scan on orders  (cost=0.00..19853.04 rows=9615 width=0) (actual time=0.009...
.39.540 rows=9558 loops=1)
         Filter: (placed_at >= '2026-10-17 00:00:00+00'::timestamp with time zone)
         Rows Removed by Filter: 990445
 Planning Time: 0.260 ms
 Execution Time: 39.769 ms
(6 rows)
```

Seq scan — `placed_at` values are scattered across every customer in that tree, exactly like
first names in the directory. The **leftmost-prefix rule**: a composite on `(a, b)` serves
queries on `a`, and on `a` + `b`; a query on `b` alone needs its own index. Column-order
discipline follows from the walk: put **equality** columns first and the **range** column
last — the walk pins down the exact customer, then reads one contiguous slice of dates; a
range first would leave equality conditions filtering inside a huge slice.

One more thing the sorted-within-customer structure gives away free: order. "This customer's
latest orders" needs no `Sort` node — the plan just walks the leaf entries backward
(`Index Scan Backward` — it appears in [practice](practice.md) W5).

### Index-only scans — when the heap trip disappears

*(Idea 10)* The composite index *contains* `customer_id` and `placed_at`. If those are all
the query selects, the heap has nothing left to add:

```sql
VACUUM orders;
EXPLAIN ANALYZE SELECT placed_at FROM orders WHERE customer_id = 4242;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Index Only Scan using idx_orders_customer_placed on orders  (cost=0.42..4.62 rows=11 w.
.idth=8) (actual time=0.037..0.038 rows=9 loops=1)
   Index Cond: (customer_id = 4242)
   Heap Fetches: 0
 Planning Time: 0.137 ms
 Execution Time: 0.056 ms
(5 rows)
```

`Index Only Scan` with `Heap Fetches: 0` — the strongest plan a B-tree can produce. The
`VACUUM` line is not decoration: the index does not know which rows are still visible to
your transaction (old row versions linger in the heap until `VACUUM` — that machinery is
beyond this topic), so index-only works only for pages `VACUUM` has marked all-visible in
the **visibility map**. Practical reading: `Heap Fetches: 0` is the goal; a large
`Heap Fetches:` number on an `Index Only Scan` means the table needs a vacuum. This is also
the honest version of "covering index": selecting one extra column the index does not hold
(`SELECT *`) silently demotes the plan back to heap visits.

### Partial indexes — index only what you query

*(Idea 11)* The selectivity section left a loose end: `idx_orders_shipped` spends most of
its 6.8 MB indexing the 95% of rows nobody searches by. A **partial index** takes a `WHERE`
clause and indexes only the rows that match it:

```sql
CREATE INDEX idx_orders_unshipped ON orders (placed_at) WHERE NOT shipped;

SELECT pg_size_pretty(pg_relation_size('idx_orders_shipped'))   AS full_on_shipped,
       pg_size_pretty(pg_relation_size('idx_orders_unshipped')) AS partial_unshipped;
```

```text
 full_on_shipped | partial_unshipped
-----------------+-------------------
 6792 kB         | 1112 kB
(1 row)
```

A sixth of the size, maintained only when an unshipped row changes — and it serves the real
query ("the oldest unshipped orders", the warehouse screen) directly:

```sql
EXPLAIN ANALYZE SELECT id, placed_at FROM orders
WHERE NOT shipped ORDER BY placed_at LIMIT 10;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Limit  (cost=0.29..6.35 rows=10 width=16) (actual time=0.012..0.033 rows=10 loops=1)
   ->  Index Scan using idx_orders_unshipped on orders  (cost=0.29..30724.27 rows=50733.
. width=16) (actual time=0.011..0.032 rows=10 loops=1)
 Planning Time: 0.373 ms
 Execution Time: 0.042 ms
(4 rows)
```

No `Sort`, no filter: the index holds exactly the unshipped rows, already in `placed_at`
order. The planner applies it only when the query's `WHERE` provably implies the index's
`WHERE` — ask for shipped rows and this index is invisible.

And a partial **unique** index settles a promise from Tuesday: "one *open* cart per
customer" is a rule no plain constraint can say, because it is conditional:

```sql
CREATE TABLE carts (
  id          bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  customer_id bigint NOT NULL REFERENCES customers (id),
  status      text   NOT NULL DEFAULT 'open'
);

CREATE UNIQUE INDEX one_open_cart_per_customer
  ON carts (customer_id)
  WHERE status = 'open';

INSERT INTO carts (customer_id) VALUES (1);
INSERT INTO carts (customer_id) VALUES (1);
```

```text
ERROR:  duplicate key value violates unique constraint "one_open_cart_per_customer"
DETAIL:  Key (customer_id)=(1) already exists.
```

```sql
UPDATE carts SET status = 'ordered' WHERE customer_id = 1 AND status = 'open';
INSERT INTO carts (customer_id) VALUES (1);
SELECT id, customer_id, status FROM carts ORDER BY id;
```

```text
 id | customer_id | status
----+-------------+---------
  1 |           1 | ordered
  3 |           1 | open
(2 rows)
```

Uniqueness enforced among open carts only; a closed cart frees the slot. (Note `id 3`, not
2 — the failed insert still consumed an identity number; sequences do not roll back.)

### Foreign keys bring no index — the quiet one

*(Idea 9)* Tuesday's docs made the promise: a `REFERENCES` constraint creates **no index**
on the referencing column. Postgres indexes the referenc**ed** side for you (it is a key);
the referenc**ing** side is your job, and the bill arrives in two places — every join from
parent to children, and, more surprisingly, every **delete of a parent**, because the
database must check for referencing children. `order_items.product_id` has a foreign key
and no index (the table's primary key `(order_id, product_id)` cannot help — `product_id`
is not its leftmost column). Watch a delete of a brand-new, never-referenced product:

```sql
BEGIN;
INSERT INTO products (name, price) VALUES ('Prototype', 0.00);
EXPLAIN ANALYZE DELETE FROM products WHERE name = 'Prototype';
ROLLBACK;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Delete on products  (cost=0.00..20.50 rows=0 width=0) (actual time=0.095..0.095 rows=0.
. loops=1)
   ->  Seq Scan on products  (cost=0.00..20.50 rows=1 width=6) (actual time=0.071..0.07.
.2 rows=1 loops=1)
         Filter: (name = 'Prototype'::text)
         Rows Removed by Filter: 1000
 Planning Time: 0.094 ms
 Trigger for constraint product_categories_product_id_fkey: time=0.226 calls=1
 Trigger for constraint order_items_product_id_fkey: time=49.058 calls=1
 Execution Time: 49.444 ms
(8 rows)
```

The delete itself is trivial — the cost hides in the **trigger lines**, which `EXPLAIN
ANALYZE` prints below the plan. The `product_categories` check took 0.2 ms (its primary key
*starts with* `product_id`, so the leftmost-prefix rule makes that check indexed for free);
the `order_items` check took **49 ms** — a full scan of a million rows to confirm that
nothing references a product that was never referenced. One index ends it:

```sql
CREATE INDEX idx_order_items_product_id ON order_items (product_id);
```

```text
 Trigger for constraint product_categories_product_id_fkey: time=0.253 calls=1
 Trigger for constraint order_items_product_id_fkey: time=0.226 calls=1
 Execution Time: 0.944 ms
```

(The re-run of the same `BEGIN … ROLLBACK` block, trigger lines shown; 49.058 → 0.226 ms —
**200×**.) The working rule: when you declare a foreign key, decide about an index on the
referencing column(s) in the same breath — the columns you join on and the columns you
cascade through are the first indexes a schema needs after its keys.

**Recap:** the planner prices plans with statistics — small tables and unselective
predicates make seq scans correct, and `ANALYZE` keeps the estimates honest. An index is
used only when the predicate matches its shape (exact expression, leftmost prefix, no
leading wildcard). Composites sort by column one then column two; equality columns first.
Index-only scans skip the heap when the index covers the query and `VACUUM` has kept the
visibility map fresh. Partial indexes shrink the tax to the rows you actually query — and a
partial unique index states a conditional rule. Foreign keys index nothing by themselves.

**Check yourself (C3):** with `idx_orders_customer_placed (customer_id, placed_at)` in
place, a teammate drops `idx_orders_customer_id` to save write cost. Does the Why It Exists
query (`WHERE customer_id = 4242`) get slower? Why or why not?

<details>
<summary>Answer</summary>

No — `customer_id` is the composite's **leftmost** column, so the leftmost-prefix rule says
the composite serves pure-`customer_id` queries just as well (the walk pins the customer and
reads that whole run of entries). Dropping the single-column index is the right call: it was
fully redundant, and every write was paying for both. The reverse would not hold — a
composite `(placed_at, customer_id)` would *not* replace it.

</details>

## Variants & Environment Differences

| Variant or environment      | What changes                                                                                       | Since / where                       |
| --------------------------- | --------------------------------------------------------------------------------------------------- | ----------------------------------- |
| B-tree (the default)        | Everything in this doc; equality and range on sortable values                                        | PostgreSQL docs — index types       |
| GIN                         | Indexes *elements inside* a value — arrays, `jsonb`, full-text, trigram substring search (`pg_trgm`) | Beyond this topic's depth; know the name |
| GiST / BRIN / hash          | Geometric & nearest-neighbour / huge append-only tables by block range / equality only               | Beyond this topic's depth; know the names |
| `CREATE INDEX CONCURRENTLY` | Builds without blocking writes — the production way; plain `CREATE INDEX` locks writes for the build (225 ms here, minutes on big tables) | PostgreSQL docs — CREATE INDEX      |
| MySQL (InnoDB)              | The table itself is stored in primary-key order (a clustered index); secondary indexes point at the PK, not a heap address — per the MySQL docs. Postgres heaps are never clustered this way | MySQL vs PostgreSQL                 |
| `UNIQUE` / `PRIMARY KEY`    | Auto-create the B-tree that enforces them — never add a duplicate index on the same column           | PostgreSQL docs — unique indexes    |

## Compared With What You Know

**Keys** (Tuesday's topic — [the full comparison lives there](../postgresSchemaJoins/brief.md),
one line here): a key is a *rule* about what may exist; an index is a *speed* structure.
The tell-apart stays Tuesday's: removing it changes an answer → key; only a time → index.
The overlap that confuses people: keys happen to be *enforced by* indexes, so every key
gives you one index for free — but no foreign key gives you anything (Idea 9).

**An application cache (Redis — scheduled Week 10)** — both answer "this read is too slow":

| Aspect              | Cache (Redis, memoized query)                        | Index                                               |
| ------------------- | ----------------------------------------------------- | ---------------------------------------------------- |
| What it stores      | **Answers** — the computed result                     | **Locations** — where the rows live                  |
| Can it be stale?    | Yes — invalidation is your problem                    | Never — maintained in the same transaction           |
| Helps which queries | Exactly the one you cached                            | Every query whose shape matches (Idea 7)             |
| Cost                | Memory + staleness risk                               | Write tax + storage (Ideas 4, 1)                     |
| First resort when   | The query is expensive *even with* good indexes, and repeats | The query reads far more rows than it returns  |

Tell them apart by the plan: if `EXPLAIN ANALYZE` shows the database discarding most of what
it reads, you have an index problem; if the plan is already tight but the query is hot and
heavy, that is Week 10's problem.

## Where It's Used

- **Every `PRIMARY KEY` and `UNIQUE` you declared on Tuesday** — each is backed by an
  auto-created B-tree (`customers_email_key` above); your schema already runs on indexes.
  *Documented* — PostgreSQL docs on unique indexes.
- **Login and lookup paths** — `WHERE email = …` on users/customers is the canonical
  single-row index hit, and its case-insensitive variant is the canonical shape trap
  (Idea 7). *Illustrative*.
- **The plan's own project** — Week 9's "tested products API" grows into Week 11's "indexed
  filter queries" (the plan's wording): category filters, price sorts and order lists are
  exactly the composite/partial decisions in this doc. *Illustrative* (scheduled).
- **Feed and list endpoints everywhere** — any `WHERE user_id = ? ORDER BY created_at DESC
  LIMIT 20` endpoint is the composite-plus-backward-walk pattern (Idea 8). *Illustrative*.
- **Your own code** — no SQL exists in this repo yet to point at; the first real target is
  the Week 9 products API, and [practice](practice.md) B1 is written against it.

## Misconceptions & Traps

(No learner SQL exists in the repo yet, so none of these cite your files — they are the
standard traps, each with the run that kills it.)

**"Indexes are free speed — index every column."**

*Why it seems right:* reads get faster and nothing visibly breaks; dev-size data hides the
cost.

*Why it is wrong:* the Level 2 measurement — the same insert went 839 ms → 4,560 ms under
three indexes (5.4×), and the size summary below shows this table's indexes (68 MB) now
outweigh the table (57 MB).

*The correct model:* every index is a standing tax on every write and a claim on disk and
cache, paid forever, whether or not any query uses it (Idea 4). An index must be justified
by a query — named, and checked with `EXPLAIN` — not by a column existing.

---

**"The planner is broken — it's ignoring my index."**

*Why it seems right:* you created it, the query names that column, and the plan still says
`Seq Scan`.

*Why it is wrong:* the two Level 3 runs — three-row `categories` seq-scans past its primary
key (cheapest possible plan), and `WHERE shipped` seq-scans past `idx_orders_shipped`
because fetching 95% of a table through an index would be slower than reading it.

*The correct model:* the planner is a cost estimator (Idea 5); an unused index usually means
small table, unselective predicate (Idea 6), shape mismatch (Idea 7) — or stale statistics,
which is the one case where the fix is just `ANALYZE`.

---

**"The foreign key created an index."**

*Why it seems right:* keys create indexes — primary and unique ones really do — and the FK
*is* enforced somehow.

*Why it is wrong:* the Idea 9 run — deleting one never-referenced product cost 49 ms,
because the FK check seq-scanned a million `order_items` rows. The constraint exists; the
index does not.

*The correct model:* only `PRIMARY KEY` and `UNIQUE` auto-create indexes. A foreign key is
enforced by lookups that *need* an index you must create yourself — on the referencing
column (Idea 9).

---

**"I have an index on `(customer_id, placed_at)`, so date queries are covered."**

*Why it seems right:* `placed_at` is right there in the index.

*Why it is wrong:* the Idea 8 run — `WHERE placed_at >= …` alone seq-scanned a million rows
with the composite sitting idle.

*The correct model:* a composite is sorted by its first column, then the second *within*
the first — leftmost prefix or nothing (Idea 8). `(a, b)` serves `a` and `a, b`; `b` needs
its own index.

---

**"`EXPLAIN ANALYZE` is a read-only diagnostic — safe to run on anything."**

*Why it seems right:* `EXPLAIN` is read-only, and `ANALYZE` sounds like "analysis".

*Why it is wrong:*

```sql
BEGIN;
SELECT count(*) FROM orders;
EXPLAIN (ANALYZE, COSTS OFF, TIMING OFF) DELETE FROM orders WHERE customer_id = 4242;
SELECT count(*) FROM orders;
ROLLBACK;
```

```text
  count
---------
 1000003
(1 row)

                                     QUERY PLAN
-------------------------------------------------------------------------------------
 Delete on orders (actual rows=0 loops=1)
   ->  Bitmap Heap Scan on orders (actual rows=9 loops=1)
         Recheck Cond: (customer_id = 4242)
         Heap Blocks: exact=9
         ->  Bitmap Index Scan on idx_orders_customer_placed (actual rows=9 loops=1)
               Index Cond: (customer_id = 4242)
 Planning Time: 0.056 ms
 Trigger for constraint order_items_order_id_fkey: calls=9
 Execution Time: 4.543 ms
(9 rows)

 count
--------
 999994
(1 row)
```

Nine orders *gone* inside the transaction — `EXPLAIN ANALYZE` **executes** the statement
(Idea 3); only the `ROLLBACK` saved them.

*The correct model:* `EXPLAIN` plans without running; `EXPLAIN ANALYZE` runs for real. On a
write statement, wrap it in `BEGIN … ROLLBACK` — every data-changing demo in this doc does.

---

**"Rows come back in insertion order anyway — `ORDER BY` is optional."**

*Why it seems right:* small fresh tables do return rows in insertion order, every time you
look.

*Why it is wrong:*

```sql
CREATE TABLE tiny (id integer, val text);
INSERT INTO tiny VALUES (1, 'one'), (2, 'two'), (3, 'three');
SELECT * FROM tiny;
UPDATE tiny SET val = 'ONE' WHERE id = 1;
SELECT * FROM tiny;
```

```text
 id |  val
----+-------
  1 | one
  2 | two
  3 | three
(3 rows)

 id |  val
----+-------
  2 | two
  3 | three
  1 | ONE
(3 rows)
```

One `UPDATE` and row 1 is now *last* — the new row version landed at the end of the heap
(Idea 1: the heap has no useful order, and keeps none).

*The correct model:* without `ORDER BY`, row order is whatever the plan happened to produce
— heap order, index order, anything — and it changes under updates and plan changes. Order
you rely on is order you declare; with the right index, declaring it is free (the
`Index Scan Backward` in [practice](practice.md) W5 sorts without sorting).

## Limits & Trade-offs

- **The write tax never sleeps.** 5.4× on bulk insert under three indexes (measured,
  Level 2); every additional index adds its share on every `INSERT`, `UPDATE`, `DELETE` —
  the right number of indexes is the smallest set your real queries need, proven by
  `EXPLAIN`, re-audited when queries change.
- **Storage and cache are real money.** This doc's `orders` ended at 57 MB of heap and
  **68 MB of indexes**:

  ```sql
  SELECT pg_size_pretty(pg_relation_size('orders')) AS heap,
         pg_size_pretty(pg_indexes_size('orders'))  AS all_indexes;
  ```

  ```text
   heap  | all_indexes
  -------+-------------
   57 MB | 68 MB
  (1 row)
  ```

  Five indexes (the PK plus this doc's four) and the "speed structures" outweigh the data.
  Every one of those pages also competes for the same RAM the table wants.
- **An index cannot fix an unselective question.** Low selectivity (Idea 6), leading
  wildcards and arbitrary substring search (Idea 7) are outside a B-tree's power — the
  first wants a partial index or no index; the others want GIN/`pg_trgm` (named, not
  taught, in Variants).
- **Index-only is conditional.** It needs the visibility map fresh (`VACUUM`) and *every*
  selected column in the index — one stray column, or `SELECT *`, and the heap trips are
  back (Idea 10).
- **Build-time locks.** Plain `CREATE INDEX` blocks writes while it builds — 225 ms at this
  size, minutes at production size; `CREATE INDEX CONCURRENTLY` exists for exactly that
  (Variants).

Verdict: indexes are the single highest-leverage performance tool a 2 YOE full-stack
engineer controls — a 700× read win from one statement in this very doc — but they are
bought with writes, storage and maintenance. The discipline that keeps the trade honest is
always the same: name the query, run `EXPLAIN ANALYZE`, look at what was read versus what
was returned, and let that output — not habit — decide.

## Interview Notes

- **How it opens:** rarely "what is an index". At 2 YOE it is "this endpoint is slow —
  walk me through what you'd do", and the scored answer starts with `EXPLAIN ANALYZE`, not
  with "add an index": read the plan, find the node reading far more than it returns, then
  propose the index that removes exactly that.
- **The follow-up ladder:** why might the planner ignore an index? (small table /
  unselective / shape mismatch / stale stats — Ideas 5–7) → composite `(a, b)`: which
  queries can use it? (leftmost prefix — Idea 8) → what does an index cost? (quote a
  measured write multiple and the 68 MB > 57 MB size fact — Idea 4) → does a foreign key
  create one? (no — and the delete-trigger story lands the point, Idea 9) → what is an
  index-only scan? (covered columns + visibility map, `Heap Fetches: 0` — Idea 10).
- **The senior tells:** reaching for `EXPLAIN ANALYZE` before opining; saying "estimated
  rows versus actual" out loud; refusing an index ("this predicate matches 95% of the
  table — a partial index on the rare value, or nothing"); knowing deletes get slow from
  *other* tables' unindexed foreign keys.
- **The two-minute version, out loud:** a table is a heap with no useful order; an index is
  a separate sorted B-tree of value → row address, so a lookup is a log-time walk plus heap
  fetches instead of a full scan — 46 ms to 0.07 ms on a million rows in my runs. It is paid
  for on every write — three indexes made the same bulk insert five times slower — so every
  index must be justified by a query and checked with `EXPLAIN ANALYZE`: estimated versus
  actual rows, and what was read versus returned. The planner may rightly ignore it — small
  table, unselective predicate, or a shape mismatch like a function over the column or a
  non-leftmost composite column. Composites go equality-first-range-last; covering columns
  give index-only scans; partial indexes shrink the tax to the rows you query; and foreign
  keys never index themselves — then the book's back-index story if they want it simpler.
