# Indexes — What They Cost, When They Help, EXPLAIN — Practice

> **Track:** BE · **Type:** concept · **Scheduled:** Week 3 · Thu · **Runtime:** PostgreSQL 17 · **Generated:** 2026-10-24

## How to Use This File

Every answer is collapsed — commit to a plan shape (or write the SQL) first, open after.
Warm-Ups and Self-Checks are from memory; Build Its are written cold against the scaled
`shop` database from the [deep dive](deep-dive.md) → Why It Exists, with the deep dive's
indexes in place, and verified with your own `EXPLAIN ANALYZE`. Timings in the collapsed
outputs will not match yours — the plan shape and row counts are the answer, never the ms.
Log attempts in `prep/notes.md` in your own words; the method and what "done" means are in
[how-to-study.md](../../../prep/how-to-study.md).

## Warm-Up

For each: say the plan's top node (`Seq Scan`, `Index Scan`, `Index Only Scan`, `Bitmap` pair)
and why, before opening.

### W1 — A thousand rows, one id

`products` has 1,000 rows and its primary-key index.

```sql
EXPLAIN ANALYZE SELECT * FROM products WHERE id = 500;
```

<details>
<summary>Plan and why</summary>

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Index Scan using products_pkey on products  (cost=0.28..8.29 rows=1 width=25) (actual .
.time=0.380..0.380 rows=1 loops=1)
   Index Cond: (id = 500)
 Planning Time: 0.576 ms
 Execution Time: 0.408 ms
(4 rows)
```

`Index Scan` — 1,000 rows is already past the point where the walk beats reading every
page (#2, #5). Compare the 3-row `categories` in the deep dive, where the same planner
picks `Seq Scan`: "small table ignores its index" is about *tiny*, and the planner, not a
rule, draws the line.

</details>

### W2 — The second column, alone

`idx_orders_customer_placed (customer_id, placed_at)` exists.

```sql
EXPLAIN ANALYZE SELECT count(*) FROM orders
WHERE placed_at >= timestamptz '2026-07-24 00:00+00';
```

<details>
<summary>Plan and why</summary>

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Aggregate  (cost=20167.90..20167.91 rows=1 width=8) (actual time=41.500..41.501 rows=1.
. loops=1)
   ->  Seq Scan on orders  (cost=0.00..19853.04 rows=125945 width=0) (actual time=0.006.
...38.701 rows=126098 loops=1)
         Filter: (placed_at >= '2026-07-24 00:00:00+00'::timestamp with time zone)
         Rows Removed by Filter: 873905
 Planning Time: 0.394 ms
 Execution Time: 41.528 ms
(6 rows)
```

`Seq Scan` — `placed_at` is the composite's *second* column, scattered across every
customer's run of entries; the leftmost-prefix rule says the index cannot serve it (#8).
(And note the estimate holding up: `rows=125945` vs actual `126098` — healthy statistics,
#5.)

</details>

### W3 — The other case

`idx_customers_email_lower (lower(email))` and the unique index on `email` both exist.

```sql
EXPLAIN ANALYZE SELECT id FROM customers
WHERE upper(email) = 'CUSTOMER4242@EXAMPLE.COM';
```

<details>
<summary>Plan and why</summary>

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Seq Scan on customers  (cost=0.00..2522.00 rows=500 width=8) (actual time=0.814..18.03.
.2 rows=1 loops=1)
   Filter: (upper(email) = 'CUSTOMER4242@EXAMPLE.COM'::text)
   Rows Removed by Filter: 99999
 Planning Time: 0.287 ms
 Execution Time: 18.050 ms
(5 rows)
```

`Seq Scan` — an index matches an exact expression, not an idea (#7). The index is on
`lower(email)`; the query asks about `upper(email)`; no match, 99,999 rows read for one.
The lesson behind the gotcha: pick *one* canonical form for case-insensitive matching and
use it on both sides, everywhere.

</details>

### W4 — Almost everyone is NULL

`customers.phone` has a unique index (declared Tuesday). Exactly one customer has a phone.

```sql
EXPLAIN ANALYZE SELECT count(*) FROM customers WHERE phone IS NULL;
```

<details>
<summary>Plan and why</summary>

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Aggregate  (cost=2272.00..2272.01 rows=1 width=8) (actual time=6.936..6.936 rows=1 loo.
.ps=1)
   ->  Seq Scan on customers  (cost=0.00..2022.00 rows=100000 width=0) (actual time=0.0.
.04..4.792 rows=99999 loops=1)
         Filter: (phone IS NULL)
         Rows Removed by Filter: 1
(6 rows)
```

`Seq Scan` — not a shape problem (a B-tree does store NULLs); a selectivity problem:
99,999 of 100,000 rows match, and fetching 99.999% of a table through an index is the
slowest way to read it (#6). The planner's statistics told it so before it ran (#5).

</details>

### W5 — Newest five, no Sort

```sql
EXPLAIN ANALYZE SELECT id, placed_at FROM orders
WHERE customer_id = 4242 ORDER BY placed_at DESC LIMIT 5;
```

<details>
<summary>Plan and why</summary>

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Limit  (cost=0.42..22.33 rows=5 width=16) (actual time=0.011..0.019 rows=5 loops=1)
   ->  Index Scan Backward using idx_orders_customer_placed on orders  (cost=0.42..48.6.
.2 rows=11 width=16) (actual time=0.010..0.017 rows=5 loops=1)
         Index Cond: (customer_id = 4242)
 Planning Time: 0.304 ms
 Execution Time: 0.029 ms
(5 rows)
```

`Index Scan Backward`, and **no `Sort` node anywhere**: within customer 4242 the composite's
entries are already in `placed_at` order, so walking them backward *is* `ORDER BY placed_at
DESC` (#8), and the `LIMIT` stops after five entries. Sorting without sorting is the
composite's quiet second job.

</details>

### W6 — Nothing left to fetch

```sql
EXPLAIN ANALYZE SELECT customer_id, placed_at FROM orders WHERE customer_id = 77;
```

<details>
<summary>Plan and why</summary>

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Index Only Scan using idx_orders_customer_placed on orders  (cost=0.42..4.62 rows=11 w.
.idth=16) (actual time=0.054..0.055 rows=9 loops=1)
   Index Cond: (customer_id = 77)
   Heap Fetches: 0
 Planning Time: 1.095 ms
 Execution Time: 0.079 ms
(5 rows)
```

`Index Only Scan` with `Heap Fetches: 0` — both selected columns live in the index, so the
heap is never visited (#10). Change the select list to `*` and this silently becomes a plan
with heap fetches again; covering is exact, not approximate.

</details>

## Spot It

Name what is going on — and what you would do — within 60 seconds, no code.

### S1 — Everyone's dashboard is slow

The admin dashboard's "revenue by month, all time" chart recomputes a full-table aggregate
over `order_items` on every page load, for every admin. Indexes on `order_items` are
already sensible, and `EXPLAIN ANALYZE` shows a tight plan — it reads every row because the
*question* covers every row, and takes 2 s. The team proposes finding a better index. Is an
index the fix?

<details>
<summary>What it is</summary>

No — this is the index/cache boundary. The plan already reads only what the question needs;
no location structure helps a question about *everything* (C1's lesson). A repeated,
expensive, identical answer wants to be computed once and reused — an application cache, or
a precomputed summary; that is Week 10's slot ("Redis cache on the products API"). The
tell: "would a perfect index still read most of the table?" If yes, stop shopping for
indexes.

</details>

### S2 — "I indexed it and nothing changed"

A teammate adds `CREATE INDEX ON orders (shipped)` because the warehouse app filters on it,
re-runs their query — `WHERE shipped = true` — and the plan still says `Seq Scan`. They
conclude Postgres indexing is unreliable.

<details>
<summary>What it is</summary>

Selectivity (#6) plus planner-as-estimator (#5): 95% of orders are shipped, and fetching
95% of a table through an index is slower than scanning it — the planner is protecting
them. The real warehouse query is about the *unshipped* few; the deep dive's answer is a
partial index on `WHERE NOT shipped` (#11), smaller and maintained only for rows anyone
searches.

</details>

### S3 — The nightly cleanup crawls

A nightly job deletes retired products. Each `DELETE FROM products WHERE id = …` takes tens
of milliseconds even for products with no sales, and the job's total runtime grows with the
`order_items` table, which the job never touches.

<details>
<summary>What it is</summary>

An unindexed foreign key (#9): every product delete fires the `order_items.product_id` FK
check, which seq-scans a million rows per delete without an index on the referencing
column. The deep dive measured exactly this: 49 ms → 0.2 ms per delete after
`CREATE INDEX … ON order_items (product_id)`. The giveaway in evidence: `EXPLAIN ANALYZE`'s
`Trigger for constraint …` lines carrying the time, not the plan itself.

</details>

### S4 — Writes got slower every sprint

Checkout inserts into `orders` were snappy at launch. Three sprints later they are
noticeably slower. Nothing about checkout changed — but each sprint shipped a new reporting
feature, and each feature shipped "its" index on `orders`. There are now nine.

<details>
<summary>What it is</summary>

The write tax compounding (#4): every insert maintains all nine B-trees — the deep dive
measured 5.4× for just three. The fix is an audit, not a tweak: list the real queries, keep
the smallest index set that serves them (leftmost-prefix means one composite often replaces
two singles — #8, C3), and drop the rest. Indexes are also the first suspects when the
answer to "what changed?" is "nothing, we only added things".

</details>

### S5 — Fast for users, slow for support

Login (`WHERE email = $1`) is instant. The support tool's search — "find user by email,
case-insensitive" (`WHERE lower(email) = lower($1)`) — takes 20 ms and climbs with the user
count. Same table, same column.

<details>
<summary>What it is</summary>

Shape mismatch (#7): the unique index is sorted by `email`, and `lower(email)` is a
different expression — the support query seq-scans. Fix: an expression index on
`lower(email)` (the deep dive builds exactly this), and then use `lower(…)` consistently on
both paths so there is one canonical form instead of two code paths with different
performance.

</details>

### S6 — Two open carts

Despite "the app only ever opens one cart per customer", production has customers with two
open carts, and the checkout code trips over them. A teammate proposes a `UNIQUE (customer_id)`
constraint on `carts` — but then a customer could never have *past* carts.

<details>
<summary>What it is</summary>

A conditional uniqueness rule — Tuesday's docs named it and deferred it here: a **partial
unique index**, `UNIQUE … ON carts (customer_id) WHERE status = 'open'` (#11). Uniqueness
enforced among open carts only; history untouched; the race the app-level check loses is
closed by the database. (The deep dive's run shows the duplicate insert failing and the slot
freeing when the cart closes.)

</details>

## Build It

The plan schedules the real follow-ons: the Week 9 BE slot builds the "tested products API"
these queries come from, and Week 11's milestone is "indexed filter queries" proven with
`EXPLAIN` — in the plan's own words. The drills below are those muscles on this doc's data.
Rebuild the scaled `shop` database from the deep dive's Why It Exists block first if you no
longer have it.

### B1 — Index the order-list endpoint

*Unscheduled drill (feeds Week 11's "indexed filter queries") · Mode: cold, then verified
with EXPLAIN*

**Task.** The API endpoint is: *a customer's unshipped orders, newest first, first page* —

```sql
SELECT id, placed_at FROM orders
WHERE customer_id = $1 AND NOT shipped
ORDER BY placed_at DESC LIMIT 20;
```

Design the best index for it, create it, and prove with `EXPLAIN ANALYZE` (use
`customer_id = 77`) that the plan has **no `Sort` node and no `Filter` line**. State what
each part of your index contributes before you check.

<details>
<summary>Hint 1 — where to look</summary>

Run `EXPLAIN ANALYZE` *before* adding anything. The deep dive's composite already serves
`customer_id` — so what work is left in the plan, and which two features from Level 3
remove exactly those two lines?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

The `WHERE NOT shipped` belongs in the index's own `WHERE`; the `ORDER BY placed_at DESC`
belongs in the index's column order. Equality column first, range/order column second —
and only the rows the endpoint can ever return.

</details>

<details>
<summary>Solution</summary>

**Approach.** A partial composite: rows restricted to `NOT shipped` (#11), sorted by
`customer_id`, then `placed_at DESC` (#8) — the index *is* the endpoint's result order.

Before (deep dive's indexes only): the composite finds customer 77's 9 orders, but the plan
must still filter out the shipped ones and sort the rest:

```text
 Limit  (cost=47.38..47.38 rows=1 width=16) (actual time=0.288..0.289 rows=1 loops=1)
   ->  Sort  (cost=47.38..47.38 rows=1 width=16) (actual time=0.287..0.287 rows=1 loops.
.=1)
         Sort Key: placed_at DESC
         Sort Method: quicksort  Memory: 25kB
         ->  Bitmap Heap Scan on orders  (cost=4.51..47.37 rows=1 width=16) (actual tim.
.e=0.263..0.275 rows=1 loops=1)
               Recheck Cond: (customer_id = 77)
               Filter: (NOT shipped)
               Rows Removed by Filter: 8
               Heap Blocks: exact=9
               ->  Bitmap Index Scan on idx_orders_customer_placed  (cost=0.00..4.51 ro.
.ws=11 width=0) (actual time=0.235..0.235 rows=9 loops=1)
                     Index Cond: (customer_id = 77)
```

The fix:

```sql
CREATE INDEX idx_orders_cust_unshipped
  ON orders (customer_id, placed_at DESC)
  WHERE NOT shipped;

EXPLAIN ANALYZE SELECT id, placed_at FROM orders
WHERE customer_id = 77 AND NOT shipped
ORDER BY placed_at DESC LIMIT 20;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Limit  (cost=0.29..8.31 rows=1 width=16) (actual time=0.011..0.011 rows=1 loops=1)
   ->  Index Scan using idx_orders_cust_unshipped on orders  (cost=0.29..8.31 rows=1 wi.
.dth=16) (actual time=0.009..0.009 rows=1 loops=1)
         Index Cond: (customer_id = 77)
 Planning Time: 0.381 ms
 Execution Time: 0.019 ms
(5 rows)
```

No `Sort`, no `Filter`: the partial `WHERE` absorbed the filter, the column order absorbed
the sort, and the `LIMIT` reads at most 20 entries.

**Edge cases handled.** A customer with zero unshipped orders returns instantly (the walk
finds no entries); shipped-order queries ignore this index entirely (the planner proves the
query implies the index predicate, or does not use it); the index stays small — only ~5% of
rows qualify.

**Uses ideas:** #7, #8, #10, #11

</details>

### B2 — The slow account deletion

*Unscheduled drill · Mode: cold, then verified with EXPLAIN*

**Task.** Support says deleting a customer account is "randomly slow", even for brand-new
accounts with no orders. Find a customer with no orders and no carts (write the query),
measure the delete with `EXPLAIN ANALYZE` inside `BEGIN … ROLLBACK`, read which constraint
is burning the time, and fix it. `orders.customer_id` is already indexed.

<details>
<summary>Hint 1 — where to look</summary>

The plan node will be innocent. Look *below* the plan — which lines of `EXPLAIN ANALYZE`
output carry time that the plan itself does not explain?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Three tables hold foreign keys into `customers` — `orders`, `carts`, and `customers`
itself (`referred_by`). Each FK fires a check on delete; each check without an index on the
*referencing* column is a seq scan.

</details>

<details>
<summary>Solution</summary>

**Approach.** Find an unreferenced customer, measure, read the trigger lines, index the
referencing columns that show real time.

```sql
SELECT c.id FROM customers c
LEFT JOIN orders o  ON o.customer_id  = c.id
LEFT JOIN carts  ct ON ct.customer_id = c.id
WHERE o.id IS NULL AND ct.id IS NULL
ORDER BY c.id LIMIT 1;
```

```text
  id
------
 3346
(1 row)
```

```sql
BEGIN;
EXPLAIN ANALYZE DELETE FROM customers WHERE id = 3346;
ROLLBACK;
```

```text
                                       QUERY PLAN
----------------------------------------------------------------------------------------
 Delete on customers  (cost=0.29..8.31 rows=0 width=0) (actual time=0.072..0.072 rows=0.
. loops=1)
   ->  Index Scan using customers_pkey on customers  (cost=0.29..8.31 rows=1 width=6) (.
.actual time=0.028..0.029 rows=1 loops=1)
         Index Cond: (id = 3346)
 Planning Time: 0.320 ms
 Trigger for constraint customers_referred_by_fkey: time=5.345 calls=1
 Trigger for constraint orders_customer_id_fkey: time=0.562 calls=1
 Trigger for constraint carts_customer_id_fkey: time=0.166 calls=1
 Execution Time: 6.214 ms
(8 rows)
```

The delete itself: 0.07 ms. The bill: `customers_referred_by_fkey` at 5.3 ms — the
self-referencing `referred_by` column has no index, so the "does anyone point at this
customer?" check scans all 100,000 customers. (`orders` is cheap because its FK *is*
indexed; `carts` is tiny.) The fix, and the re-run:

```sql
CREATE INDEX idx_customers_referred_by ON customers (referred_by);
CREATE INDEX idx_carts_customer_id     ON carts (customer_id);
```

```text
 Trigger for constraint customers_referred_by_fkey: time=0.108 calls=1
 Trigger for constraint orders_customer_id_fkey: time=0.289 calls=1
 Trigger for constraint carts_customer_id_fkey: time=0.180 calls=1
 Execution Time: 0.686 ms
```

6.2 ms → 0.7 ms per delete — 50× on the hot constraint, and it scales with the customers
table, so it only gets worse without the index.

**Edge cases handled.** The customer hunt itself (anti-join per Tuesday's docs); delete
measured under `BEGIN … ROLLBACK` so nothing is lost (#3's trap); the already-indexed
`orders` FK confirmed cheap rather than blindly re-indexed.

**Uses ideas:** #3, #9

</details>

### B3 — One active session per user

*Unscheduled drill · Mode: from memory*

**Task.** From a blank psql prompt, build a `user_sessions` table (id, customer, `active`
flag) that the *database* guarantees can hold at most one active session per customer while
keeping unlimited inactive history. Prove it: second login for the same customer fails;
logout then login succeeds and history remains.

<details>
<summary>Hint 1 — where to look</summary>

A plain `UNIQUE (customer_id)` forbids the history rows too. What kind of index can state a
rule about only *some* rows?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

A partial unique index: uniqueness enforced only where the index's `WHERE` holds — the same
pattern as the deep dive's one-open-cart rule, from memory this time.

</details>

<details>
<summary>Solution</summary>

**Approach.** Same shape as the carts rule: unique on `customer_id`, restricted to
`WHERE active`.

```sql
CREATE TABLE user_sessions (
  id          bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  customer_id bigint NOT NULL REFERENCES customers (id),
  active      boolean NOT NULL DEFAULT true
);

CREATE UNIQUE INDEX one_active_session_per_customer
  ON user_sessions (customer_id)
  WHERE active;

INSERT INTO user_sessions (customer_id) VALUES (2);
INSERT INTO user_sessions (customer_id) VALUES (2);
```

```text
ERROR:  duplicate key value violates unique constraint "one_active_session_per_customer"
DETAIL:  Key (customer_id)=(2) already exists.
```

```sql
UPDATE user_sessions SET active = false WHERE customer_id = 2 AND active;
INSERT INTO user_sessions (customer_id) VALUES (2);
SELECT id, customer_id, active FROM user_sessions ORDER BY id;
```

```text
 id | customer_id | active
----+-------------+--------
  1 |           2 | f
  3 |           2 | t
(2 rows)
```

**Edge cases handled.** Duplicate active login rejected by the database (run shown);
logout frees the slot; history preserved (row 1 stays); the skipped id 3 shows the failed
insert consumed a sequence number — expected, harmless. Bonus the index gives free: "find
this customer's active session" is an indexed lookup.

**Uses ideas:** #9, #11

</details>

## Self-Check Questions

### Q1 — The two structures

A table has an index. What are the two separate on-disk structures involved, and what does
each hold?

<details>
<summary>Answer</summary>

The **heap** — the table's pages, holding full rows in no useful order — and the **B-tree
index** — sorted guide pages over sorted leaf entries, each entry holding only the indexed
value and the address (page, slot) of its heap row (#1). The index never holds the row;
that is why most index reads still end with a heap visit (#2).

</details>

### Q2 — Why the walk is fast

Why is finding 9 rows via an index `O(log n)` plus 9 fetches, rather than `O(log n)` flat?

<details>
<summary>Answer</summary>

The walk down the guide pages to the right leaf is logarithmic (#2) — but leaf entries only
*point*. Each of the 9 addresses is a separate heap-page visit at an arbitrary location.
That trailing per-row cost is why unselective predicates lose (#6) and why an index that
contains every selected column — index-only (#10) — is such a win: it deletes exactly that
term.

</details>

### Q3 — The guess and the receipt

In `(cost=4.51..47.37 rows=11 width=25) (actual time=… rows=9 …)` — which numbers are
estimates, where do they come from, and which command added the actuals?

<details>
<summary>Answer</summary>

`cost`, `rows=11` and `width` are the planner's estimates, computed from the statistics
`ANALYZE` maintains (row counts, common values, histograms) (#3, #5). The `(actual …)`
group exists only under `EXPLAIN ANALYZE`, which **executes** the query and measures. A
large gap between estimated and actual `rows` is the standard smell of stale statistics.

</details>

### Q4 — Quote the tax

A reviewer asks "what does adding an index actually cost?" Give the three costs, with one
measured number.

<details>
<summary>Answer</summary>

(1) Writes: every `INSERT`/`UPDATE`/`DELETE` maintains every index — the deep dive measured
the same million-row insert at 839 ms bare vs 4,560 ms under three indexes, 5.4× (#4).
(2) Storage and cache: the scaled `orders` carries 68 MB of indexes on 57 MB of data.
(3) Build: plain `CREATE INDEX` blocks writes while it builds (`CONCURRENTLY` in
production).

</details>

### Q5 — Defend the planner

Your dev database query shows `Seq Scan` despite a perfectly good index. Name the four
innocent explanations before you blame Postgres.

<details>
<summary>Answer</summary>

Small table — everything fits in a page or two, scanning is genuinely cheaper (#5);
unselective predicate — the condition matches most rows (#6); shape mismatch — a function,
cast or leading wildcard over the column (#7), or querying a non-leftmost composite column
(#8); stale statistics — the planner is pricing fiction until `ANALYZE` runs (#5). Only
after those four is the plan itself suspect.

</details>

### Q6 — The boolean column

When is an index on a two-value column like `shipped` worth having, and in what form?

<details>
<summary>Answer</summary>

Only when queries target the *rare* value — and then as a **partial index** on exactly
those rows (`WHERE NOT shipped`), which the deep dive measured at a sixth of the full
index's size (#6, #11). A full index on a 95/5 boolean spends 95% of its space and write
tax on rows no query touches through it.

</details>

### Q7 — The login bug that isn't a bug

`WHERE email = $1` is fast; `WHERE lower(email) = $1` is slow. Why, and what are the two
fixes?

<details>
<summary>Answer</summary>

The B-tree is sorted by `email`; `lower(email)` is a different expression, so the index's
order says nothing about it and the planner scans (#7). Fixes: an expression index on
`lower(email)` — a B-tree sorted by exactly that — or normalising the data so the column
itself is stored lowercased and the plain index serves. Either way: one canonical form,
used everywhere.

</details>

### Q8 — Design the composite

For `WHERE customer_id = $1 AND placed_at >= $2`, why is `(customer_id, placed_at)` right
and `(placed_at, customer_id)` wrong — and which other queries does the right one serve
for free?

<details>
<summary>Answer</summary>

Equality first: the walk pins customer `$1` exactly, then reads one contiguous, already
sorted slice of dates (#8). Range-first would land in a huge date slice and filter
customers inside it. Free extras from leftmost-prefix and order: plain
`WHERE customer_id = $1`, and `ORDER BY placed_at [DESC] LIMIT n` within a customer with no
`Sort` node (W5's backward scan) — plus index-only scans when only those two columns are
selected (#10).

</details>

### Q9 — What a foreign key doesn't do

Which constraint declarations create indexes automatically, and what exactly goes slow when
a foreign key's referencing column has none?

<details>
<summary>Answer</summary>

Only `PRIMARY KEY` and `UNIQUE` (#9 — they are *enforced by* B-trees). A foreign key
creates nothing on the referencing side, so two things degrade: parent→child joins, and
every **parent delete/key-update**, whose referential check seq-scans the child table —
visible as a fat `Trigger for constraint …` line under `EXPLAIN ANALYZE` (49 ms → 0.2 ms in
the deep dive's run once indexed).

</details>

### Q10 — Index-only, honestly

What two conditions must hold for an `Index Only Scan`, and what does a non-zero
`Heap Fetches:` tell you?

<details>
<summary>Answer</summary>

The index must contain **every** column the query selects, and the pages involved must be
marked all-visible in the visibility map, which `VACUUM` maintains (#10). Non-zero
`Heap Fetches` means visibility had to be checked in the heap for that many rows — the
table wants a vacuum, or the "covering" index quietly is not covering the query any more.

</details>

## Explain It Back

### E1 — The two-minute teach

Explain indexes to a new teammate in two minutes, starting from the picture-book story and
ending with the one command they should run before ever creating one.

<details>
<summary>Rubric — a complete answer covers</summary>

- The back-of-book list: separate, sorted, points at pages — doesn't change any answer
  (#1, #2).
- Sorted is the trick: a few hops instead of reading everything (`O(log n)` — the Week 2
  connection), then you still *go to the page* (heap fetch).
- The librarian's burden: every change to the book updates every list — writes pay for
  reads, with a measured multiple (#4).
- The database *decides* whether to use the list, by cost — and is usually right when it
  declines (#5, #6).
- The closing command: `EXPLAIN ANALYZE` — read what was read vs what was returned, then
  decide (#3).

</details>

### E2 — "Why is it ignoring my index?"

An interviewer shows you a `Seq Scan` over a table that has the "right" index and asks you
to think aloud. Give the full diagnostic, out loud, in order.

<details>
<summary>Rubric — a complete answer covers</summary>

- First: is the plan actually wrong? Read returned-vs-read and the actual time before
  assuming (#3).
- Size: tiny tables scan because scanning is cheaper (#5) — and dev-DB plans prove nothing
  about prod.
- Selectivity: what fraction matches? Most-of-the-table predicates *should* scan (#6).
- Shape: function/cast over the column, leading wildcard, or non-leftmost composite column
  (#7, #8).
- Statistics: estimated vs actual rows wildly apart → `ANALYZE`, then re-explain (#5).
- Senior close: name the fix matched to the branch found — expression index, partial index,
  reordered composite, or "the seq scan is correct; this wants a cache/summary instead".

</details>

### E3 — Defend the diff

In review you rejected a PR titled "add indexes to orders" that created six single-column
indexes, and replaced it with one partial composite. Justify both halves to the author —
including why their endpoint gets *faster* under your version.

<details>
<summary>Rubric — a complete answer covers</summary>

- The cost case against six: every write maintains every index — measured 5.4× under just
  three; plus 68 MB-vs-57 MB as the storage picture (#4).
- Redundancy: leftmost-prefix means `(customer_id, placed_at)` already serves
  `customer_id` alone — the single-column twin is pure tax (#8, C3).
- The endpoint's actual query (filter + order + limit) is served *better* by one partial
  composite: filter absorbed by the index `WHERE`, sort absorbed by column order, no
  `Sort`/`Filter` nodes left (B1's before/after plans) (#8, #11).
- Proof over opinion: both versions measured with `EXPLAIN ANALYZE`, decided by read-vs-
  returned, not by index count (#3).
- The standing rule offered, not imposed: new index PRs name the query they serve and
  attach its plan.

</details>
