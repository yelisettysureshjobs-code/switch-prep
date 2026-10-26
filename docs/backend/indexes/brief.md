# Indexes — What They Cost, When They Help, EXPLAIN — Brief

> **Track:** BE · **Type:** concept · **Scheduled:** Week 3 · Thu · **Runtime:** PostgreSQL 17 · **Generated:** 2026-10-24

## Explain It Like I'm 5

You want every dinosaur picture in a big, thick picture book. One way: turn every single
page and look. The other way: the back of the book has a special list — all the topics in
alphabetical order, each with its page numbers. You flip to "dinosaurs", it says page 48 and
page 112, and you go straight there. So fast! But the list is not magic: it does not hold
the pictures, only where they are — you still turn to the page. And if you paste a new
picture into the book, someone has to fix the list too. Every list the book keeps. The more
lists, the longer every paste takes.

In real terms: an index is a sorted structure that maps values to row locations, making
lookups fast at a cost on every write — and `EXPLAIN` shows whether it is being used.

## What It Is & Where It Shows Up

An index is a separate, sorted on-disk structure (in Postgres, a B-tree by default) that
maps column values to row addresses, so matching rows are found by a short walk instead of a
full-table scan.

It solves the problem that a table — the heap — keeps rows in no useful order, so without an
index *every* search reads *every* row. Someone who does not hold the model ships endpoints
that scan a million rows to return nine, "fixes" them with indexes the planner then rightly
ignores, indexes every column and wonders why writes crawl, and never learns that deletes
can be slow because of *another* table's missing index.

It shows up under every `WHERE`, every `JOIN`, every `ORDER BY … LIMIT` in every API you
will build — the plan's own project hits it at Week 9 ("tested products API") and Week 11
("indexed filter queries"). Interviews ask it as "this endpoint is slow — what do you do?"
and score whether your first move is `EXPLAIN ANALYZE`, and the follow-ups probe exactly
this doc: when the planner ignores an index, composite column order, and what indexes cost.

## Glossary

| Term              | In plain words                                              | Precise definition                                                                  |
| ----------------- | ------------------------------------------------------------ | ------------------------------------------------------------------------------------ |
| heap              | The book itself — pages of rows in no useful order           | The table's data pages; rows placed wherever space allows, order never maintained     |
| B-tree index      | The sorted list at the back of the book                      | The default index: sorted leaf entries under a few levels of guide pages              |
| row address (TID) | "Page 48" — where to find it, not the thing itself           | The (page, slot) pointer each leaf entry holds to its heap row                        |
| seq scan          | Reading the whole book page by page                          | The plan node that reads every heap page and filters                                  |
| planner           | The part that decides *how* to answer, before answering      | The component that prices candidate plans using statistics and picks the cheapest     |
| statistics        | What the planner knows about your data                       | Per-column summaries (counts, common values, histogram) maintained by `ANALYZE`       |
| `EXPLAIN`         | "Show me your plan" — a guess, nothing runs                  | Prints the chosen plan with estimated cost, rows and width                            |
| `EXPLAIN ANALYZE` | "Run it and show receipts" — it really executes              | Executes the statement and adds actual times and row counts per node                  |
| selectivity       | How few rows the question keeps                              | The fraction of rows a predicate matches; small fraction = selective = index-friendly |
| composite index   | A list sorted by two things — surname, then first name       | One B-tree over multiple columns, sorted by the first, then the next within it        |
| leftmost prefix   | You can search by surname, never by first name alone         | A composite serves queries on its leading column(s) only                              |
| expression index  | A list of the *lowercased* topics                            | An index built on an expression (`lower(email)`), serving predicates on exactly it    |
| partial index     | A list of only the pop-up pages                              | An index with a `WHERE` clause, holding and taxing only the matching rows             |
| index-only scan   | The list alone answers — no page-turning at all              | A plan served entirely from the index, heap skipped via the visibility map            |
| visibility map    | The librarian's note: "these pages have no stale stickers"   | Per-page all-visible flags maintained by `VACUUM`; required for index-only scans      |

## Core Ideas

| #  | Idea                                | Level        | In one line                                                                     | Taught in                                                      | Tested by              |
| -- | ----------------------------------- | ------------ | -------------------------------------------------------------------------------- | ---------------------------------------------------------------- | ---------------------- |
| 1  | A sorted copy you pay to keep       | basic        | Heap unordered; the index is a separate sorted B-tree of value → row address      | [deep-dive](deep-dive.md) → Level 2 — What the index physically is | Q1, E1                 |
| 2  | It changes finding, not answers     | basic        | Scan-everything becomes a log-time walk — plus a heap visit per row found         | [deep-dive](deep-dive.md) → Level 2 — What the index physically is | W1, Q2, E1             |
| 3  | EXPLAIN guesses, ANALYZE executes   | basic        | Node type + estimated vs actual rows is the reading skill; ANALYZE runs for real  | [deep-dive](deep-dive.md) → Level 2 — Reading EXPLAIN             | W2, Q3, B2, E2         |
| 4  | Every index taxes every write       | intermediate | Three indexes made the same insert 5.4× slower — measured, standing, per write    | [deep-dive](deep-dive.md) → Level 2 — Reading EXPLAIN             | S4, Q4, E3             |
| 5  | The planner is a cost estimator     | intermediate | Statistics price the plans; small tables and stale stats explain "ignored" indexes | [deep-dive](deep-dive.md) → Level 3 — The planner                 | W1, S2, Q5, E2         |
| 6  | Selectivity decides                 | intermediate | An index pays only when the predicate keeps few rows; 95%-match predicates scan   | [deep-dive](deep-dive.md) → Level 3 — Selectivity                 | W4, S2, Q6             |
| 7  | The shape must match                | intermediate | A function, cast or leading wildcard over the column silently forfeits the index  | [deep-dive](deep-dive.md) → Level 3 — Shape                       | W3, S5, Q7             |
| 8  | Composite = leftmost prefix         | intermediate | `(a, b)` serves `a` and `a`+`b`, never `b` alone; equality first, range last      | [deep-dive](deep-dive.md) → Level 3 — Composite indexes           | W2, W5, Q8, B1, E3     |
| 9  | Foreign keys index nothing          | intermediate | The referencing column is your job — joins and parent deletes seq-scan without it | [deep-dive](deep-dive.md) → Level 3 — Foreign keys                | S3, B2, Q9             |
| 10 | Index-only scans skip the heap      | intermediate | All selected columns in the index + fresh visibility map → `Heap Fetches: 0`      | [deep-dive](deep-dive.md) → Level 3 — Index-only scans            | W6, Q10                |
| 11 | Partial indexes index what you query | advanced    | A `WHERE` on the index shrinks size and tax — and states conditional uniqueness   | [deep-dive](deep-dive.md) → Level 3 — Partial indexes             | S6, B1, B3, Q6         |

## Know This Cold

The rules:

1. The heap is unordered; an index is a **separate sorted B-tree** of value → row address —
   it speeds finding and never changes an answer.
2. An index pays only when the predicate is **selective**; the planner decides by estimated
   cost, and a declined index usually means small table, unselective predicate, wrong shape,
   or stale statistics.
3. `EXPLAIN` prints the plan and its estimates; `EXPLAIN ANALYZE` **executes** — actuals
   beside estimates, and `BEGIN … ROLLBACK` around any write you analyze.
4. Every index taxes **every write** (measured: 5.4× under three) and takes storage (this
   doc's indexes outgrew their table).
5. The predicate must match the index's **shape**: the exact column or expression, no
   leading wildcard.
6. Composite indexes serve the **leftmost prefix** only; equality columns first, the range
   or `ORDER BY` column last.
7. `PRIMARY KEY`/`UNIQUE` auto-create their indexes; **foreign keys never do** — index the
   referencing column yourself.

The canonical read (copied from [deep-dive.md](deep-dive.md) → Level 2 — Reading EXPLAIN;
the same query cost 46.452 ms as a seq scan before the index):

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

Read it cold: inner node first (the index found 9 addresses), parent visits 9 heap blocks;
`rows=11` is the statistics' guess, `rows=9` the truth; 0.066 ms against 46 ms, and no
`Rows Removed by Filter` anywhere.

## Costs & Limits

| What it costs                                                                      | When it bites                                                            |
| ----------------------------------------------------------------------------------- | -------------------------------------------------------------------------- |
| Write tax: the same 1M-row insert, 839 ms bare → 4,560 ms under three indexes (measured in the deep dive) | Every `INSERT`/`UPDATE`/`DELETE`, forever — compounding with each index added |
| Storage & cache: 68 MB of indexes on a 57 MB table (five indexes, measured)          | Disk, backups, and RAM the heap itself wants                               |
| Build lock: plain `CREATE INDEX` blocks writes while building (225 ms at 1M rows)    | Production tables — use `CREATE INDEX CONCURRENTLY`                        |
| A heap visit per row found through the index                                         | Unselective predicates — fetching most of a table by index loses to a scan |

Hard limits: no B-tree serves a leading wildcard or substring search (that is GIN/`pg_trgm`
territory — named in the deep dive's Variants, beyond this topic); a composite is blind to
its non-leading columns; index-only scans decay as the visibility map staled (`VACUUM`
restores); and no index helps a question that genuinely touches the whole table — that is a
cache or precomputation problem (Week 10).

## Connections

- **Prerequisites:**
  [Postgres — Schema Design, Normalization & Joins](../postgresSchemaJoins/brief.md) — the
  `shop` schema these docs scale up, and the keys whose auto-indexes this topic explains.
  SQL basics (no doc yet) — 8-line primer at the top of the [deep dive](deep-dive.md).
- **Confusables:**
  Keys — Tuesday's signal stands: removing it could change an *answer* → key; only a
  *time* → index.
  Application cache (Redis, Week 10) — the signal: would a perfect index still read most of
  the table? Then it is a cache/precompute problem; an index stores locations, a cache
  stores answers.
- **Unlocks:**
  "Redis cache on the products API" — Week 10 (BE).
  "indexed filter queries" on the project — Week 11 (BE), the milestone this doc trains for.
  DBMS days, SQL by hand — Week 19.

## Mastery Checklist

- [ ] You can read an unseen `EXPLAIN ANALYZE` output aloud — node by node, estimated vs
  actual, what was read vs returned — without notes.
- [ ] You can list the four innocent reasons a plan shows `Seq Scan` past a usable-looking
  index, without notes.
- [ ] You can design a composite index for a filter + order + limit endpoint and say why
  each column is where it is, without notes.
- [ ] You can state the write tax with a measured number and name the two other costs,
  without notes.
- [ ] You can name the two places an unindexed foreign key hurts, and find the evidence in
  `EXPLAIN ANALYZE`'s trigger lines, without notes.
- [ ] You can write a partial unique index for a "one open X per Y" rule from a blank
  prompt, from memory.
- [ ] You can explain indexes to someone else in two minutes, starting from the picture
  book.

## 2-Minute Revision

1. Heap = rows in no useful order; index = separate **sorted** B-tree of value → address.
2. Walk is `O(log n)`, then one heap visit per row found.
3. `EXPLAIN` = the guess; `EXPLAIN ANALYZE` = executes, shows actuals. Writes →
   `BEGIN … ROLLBACK`.
4. Declined index: small table · unselective · wrong shape · stale stats.
5. Shape: exact column/expression only — `lower(email)` needs its own index; `LIKE '%…'`
   never.
6. Composite `(a, b)`: serves `a`, `a`+`b`; equality first, range last; backward walk =
   free `DESC`.
7. Index-only: all selected columns in the index + `VACUUM`-fresh map → `Heap Fetches: 0`.
8. Partial: index only queried rows — and "one open cart per customer" as partial UNIQUE.
9. FKs create no index — referencing columns are yours; slow parent deletes = trigger
   lines.
10. Costs: writes 5.4× under three indexes · 68 MB indexes vs 57 MB heap · build locks.
11. The snippet: 46 ms seq scan → 0.066 ms, `rows=11` guessed, `rows=9` real (Know This
    Cold).
12. First move on any slow query: `EXPLAIN ANALYZE`, then read — never "add an index" first.
