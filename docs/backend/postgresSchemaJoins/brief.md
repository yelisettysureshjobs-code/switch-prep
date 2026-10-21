# Postgres — Schema Design, Normalization & the Joins You'll Actually Use — Brief

> **Track:** BE · **Type:** concept · **Scheduled:** Week 3 · Tue · **Runtime:** PostgreSQL 17 · **Generated:** 2026-10-20

## Explain It Like I'm 5

Mia sells lemonade and writes every sale in one big notebook: the friend's name, where they live, what they bought, what it cost. One day Ravi moves house. Mia has to find every page with Ravi on it and fix it, and she misses one. Now the notebook says Ravi lives in two places. So Mia starts three small notebooks: friends, drinks, sales. Every friend and every drink gets a number. A sale page only says "friend 2 bought drink 3, paid 5". To know who that was, Mia flips to friend number 2. And she writes down the price she charged that day, because next week lemonade might cost more.

In real terms: a Postgres schema stores each fact once, in its own table, and links tables with keys; normalization is the set of rules that says what "once" means, and a join is how a query follows the keys to put the facts back together.

## What It Is & Where It Shows Up

Schema design is deciding which tables exist, what identifies a row in each, and how rows point at each other — so that every fact is stored exactly once and the database itself refuses data that breaks the rules.

It explains why a "simple" flat table slowly corrupts itself (two emails for one customer, a customer who vanishes with his last order), why a report can double count without any error, and why a `LEFT JOIN` can silently lose the rows it was written to keep. Someone who does not hold this model ships those bugs and cannot see them in the output, because every one of them is a plausible-looking result set rather than an exception.

It shows up in every table you create for the Express + Postgres API (the Month 1 exit check), in the cart, checkout and order history of the Week 9–15 project, and in every "per customer", "per category", "with no orders" query an endpoint or a dashboard needs. Interviews ask it three ways: "design the tables for X", "what is normalization — give an example", and a query to write cold — "customers with no orders this month", "top products in each category" — with "why not `NOT IN`?" and "where would you denormalize?" as the follow-ups.

## Glossary

| Term                  | In plain words                                                          | Precise definition                                                                                               |
| --------------------- | ----------------------------------------------------------------------- | ---------------------------------------------------------------------------------------------------------------- |
| table                 | A notebook for one kind of thing                                        | A named set of rows sharing the same columns, each row describing one entity of that kind                       |
| row                   | One line in the notebook — one thing                                    | One tuple of the table; one fact-bundle about one entity                                                         |
| column                | One box on the line — one value                                         | A named, typed attribute present in every row of the table                                                       |
| primary key           | The number each thing gets, which never changes                         | The column(s) that uniquely identify a row; implies `UNIQUE` and `NOT NULL`                                      |
| surrogate key         | A made-up number used as the primary key                                | A system-generated identifier with no business meaning (identity column)                                         |
| natural key           | A real-world identifier, like an email                                  | A business attribute that is unique but may change; kept `UNIQUE`, not used as the primary key                   |
| join                  | Flipping to the matching number in another notebook                     | An operation that pairs rows of two tables according to a condition and emits one row per qualifying pair       |
| foreign key           | A column that holds another notebook's number                           | A column (or columns) constrained to equal a primary or unique key in a referenced table                        |
| one-to-many           | One of these has many of those                                          | A relationship where a parent row is referenced by any number of child rows via a foreign key on the child      |
| referential action    | What happens to the children when the parent is deleted                 | The `ON DELETE` / `ON UPDATE` behaviour of a foreign key: `NO ACTION` (default), `RESTRICT`, `CASCADE`, `SET NULL`, `SET DEFAULT` |
| many-to-many          | Each of these has many of those, and the other way round                | A relationship that needs a third table because neither side can hold a single key to the other                  |
| junction table        | A notebook that lists pairs                                             | A table whose rows are (key A, key B) pairs, keyed by the pair, optionally carrying the pair's own attributes    |
| constraint            | A rule the table itself enforces, every time                            | A declared condition (`NOT NULL`, `UNIQUE`, `CHECK`, `PRIMARY KEY`, `FOREIGN KEY`) that rejects violating writes |
| snapshot              | Writing today's price on the sale page                                  | A deliberately stored copy of a value whose later divergence from its source is correct, not an error           |
| denormalization       | Keeping a copy on purpose                                               | Storing a redundant or derived value for read convenience, accepting the cost of keeping it right               |
| aggregate             | Folding many lines into one number                                      | A function (`SUM`, `COUNT`, `AVG`, `string_agg`) over a group of rows defined by `GROUP BY`                      |
| join condition        | The matching rule                                                       | The `ON` clause: the predicate that decides whether a left row and a right row form a pair                      |
| inner join            | Keep only the pairs that match                                          | `[INNER] JOIN`: emits exactly the row pairs for which the join condition is true                                 |
| left join             | Keep every left row; fill the gaps with blanks                          | `LEFT [OUTER] JOIN`: the inner join plus one row per unmatched left row with the right columns set to `NULL`    |
| fan-out               | One line copied for every match on another side                         | Row multiplication from joining a parent to more than one one-to-many child set; aggregates over it are inflated |
| functional dependency | Knowing this tells you that                                             | `X → Y`: rows with equal `X` always have equal `Y`                                                               |
| candidate key         | The smallest set of boxes that tells you which line you're on           | A minimal column set that functionally determines every column of the row                                       |
| normal form           | A test for "each fact in one place"                                     | 1NF: atomic cells, keyed rows; 2NF: no dependency on part of the key; 3NF: no dependency between non-key columns |
| anomaly               | A way the notebook lies after an ordinary edit                          | An update, insert or delete that leaves stored facts inconsistent or lost, caused by redundancy                 |
| three-valued logic    | True, false — or "don't know"                                           | SQL predicates evaluate to true, false or unknown; comparisons involving `NULL` are unknown; `WHERE` keeps only true |
| anti-join             | The things with no match                                                | Selecting rows of A with no matching row in B: `NOT EXISTS`, or `LEFT JOIN … WHERE b.key IS NULL`                |
| cross join            | Every possible pair                                                     | `CROSS JOIN`: the Cartesian product, `N × M` rows; an inner join is a cross join filtered by the condition       |

## Core Ideas

| #  | Idea                                   | Level        | In one line                                                                                   | Taught in                                                            | Tested by                 |
| -- | -------------------------------------- | ------------ | --------------------------------------------------------------------------------------------- | -------------------------------------------------------------------- | ------------------------- |
| 1  | One row, one fact                      | basic        | A cell holds one value; a list in a cell or a repeated fact is the flat-table disease (1NF)   | [deep-dive](deep-dive.md) → Level 1 — Intuition                      | S1, Q2, E1                |
| 2  | Every table has a primary key          | basic        | A generated number that never changes; a natural key such as email becomes `UNIQUE`          | [deep-dive](deep-dive.md) → Level 1 — Intuition                      | B1, Q1, E1                |
| 3  | A relationship is a foreign key        | basic        | On the many side, enforced both ways; `ON DELETE` defaults to refuse — cascade is a choice   | [deep-dive](deep-dive.md) → Level 2 — Relationships are foreign keys | W2, B1, B2, Q2, Q10       |
| 4  | Many-to-many needs a third table       | intermediate | A junction table keyed by the pair, which can carry the pair's own facts                      | [deep-dive](deep-dive.md) → Level 2 — Many-to-many needs a third table | B1, B3, B4, Q11         |
| 5  | Redundancy causes anomalies            | intermediate | Update, insert and delete anomalies; 1NF–3NF remove them: key, whole key, nothing but the key | [deep-dive](deep-dive.md) → Level 3 — Functional dependencies and the normal forms | S1, Q3, Q4, E1, E2 |
| 6  | The schema rejects bad data            | intermediate | `NOT NULL`, `UNIQUE`, `CHECK`, identity, `numeric` for money, `timestamptz` — for every code path | [deep-dive](deep-dive.md) → Level 2 — The database refuses bad data | W2, B1, B2, B4, Q11, Q12 |
| 7  | Denormalize on purpose                 | intermediate | A copy is fine when the two values are allowed to differ — the price paid vs the price now    | [deep-dive](deep-dive.md) → Level 2 — Denormalize on purpose         | B1, B4, Q5, E1, E2        |
| 8  | A join is a row-matching rule          | basic        | One output row per matching pair; rows with no match are simply not emitted                   | [deep-dive](deep-dive.md) → Level 1 — Intuition                      | W1, B3, E1                |
| 9  | `LEFT JOIN` keeps the unmatched        | intermediate | As `NULL`-padded rows — and a `WHERE` on the right side silently makes it inner again; use `ON` | [deep-dive](deep-dive.md) → Level 2 — Joins, step by step           | W3, W4, B3, Q6, Q7        |
| 10 | Joins multiply                         | intermediate | Two one-to-many joins fan out and `SUM`/`COUNT` double count; aggregate each side first       | [deep-dive](deep-dive.md) → Level 2 — Joins, step by step            | W5, S2, B3, Q8, E3        |
| 11 | The anti-join                          | intermediate | "Rows with no match" is `NOT EXISTS` or `LEFT JOIN … IS NULL`; `NOT IN` breaks on one `NULL` | [deep-dive](deep-dive.md) → Level 3 — The anti-join                 | W6, S3                    |
| 12 | `NULL` is unknown, not a value         | advanced     | Fails every comparison, slips past `UNIQUE`, skipped by `COUNT(col)`, dropped by `WHERE`      | [deep-dive](deep-dive.md) → Level 3 — NULL is not a value            | W3, W4, W6, S3, B2, Q7, Q9 |

## Know This Cold

The rules:

1. One row, one fact; one cell, one value. Every table gets a generated numeric primary key; a natural key such as email is `UNIQUE` on the side.
2. A foreign key sits on the many side and the database enforces it both ways. `ON DELETE` defaults to refuse; cascade only where the child is meaningless without its parent.
3. A many-to-many is a table of pairs, keyed by the pair, carrying the pair's own facts (`quantity`, `unit_price`).
4. Normalize to 3NF — key, whole key, nothing but the key — to remove update, insert and delete anomalies. Keep a copy only when the two values are allowed to differ: that is a snapshot.
5. Constraints and types are rules for every code path: `NOT NULL`, `UNIQUE`, `CHECK`, `numeric` for money, `timestamptz` for time.
6. An inner join keeps matched pairs; a left join adds unmatched left rows as `NULL`s. A condition on the right-hand table goes in `ON`, not `WHERE`. Two one-to-many joins multiply — aggregate each side to one row per parent first, and count with `COUNT(col)`.
7. `NULL` is unknown: it fails every comparison, slips past `UNIQUE`, and makes `NOT IN` return nothing. "Rows with no match" is `NOT EXISTS`.

The canonical snippet (from [deep-dive.md](deep-dive.md) → Level 2 — Joins, step by step; it runs against the shop schema created at the end of Why It Exists):

```sql
SELECT c.name, COUNT(*) AS rows_out, COUNT(o.id) AS orders
FROM customers c
LEFT JOIN orders o ON o.customer_id = c.id
GROUP BY c.name
ORDER BY c.name;
```

```text
 name  | rows_out | orders
-------+----------+--------
 Asha  |        2 |      2
 Meera |        1 |      0
 Ravi  |        1 |      1
(3 rows)
```

## Costs & Limits

| What it costs                                                   | When it bites                                                                                                   |
| --------------------------------------------------------------- | --------------------------------------------------------------------------------------------------------------- |
| Every read of a normalized fact is a join                       | The product list touches three tables; done as one aggregate-first query it is 1 round trip, done as a loop in app code it is `1 + 20 = 21` for a page of 20 (the N+1 pattern) |
| Every child insert checks that its parent exists; every parent delete checks for children | Bulk loads and deletes of parents with many children — and the foreign-key column has no index unless you add one (Thursday) |
| Constraint errors surface in the database, not the validator    | The API must translate a unique or foreign-key violation into a 4xx response instead of a 500                   |
| A wrong join is silent                                          | Fan-out reports `4` sold instead of `2`; a `WHERE` on a left join drops the rows it should keep; `NOT IN` over a nullable column returns 0 rows — all without an error |

Hard limits: 3NF only removes anomalies from single-column dependencies — higher forms exist and are beyond the target depth; a rule that spans tables ("stock must cover every pending order line") cannot be a constraint and needs a transaction (Week 15); and a conditional uniqueness rule ("one *open* cart per customer") needs a partial index, which is Thursday's topic.

## Connections

**Prerequisites**

- SQL basics — `CREATE TABLE`, `INSERT`, `SELECT` (no doc yet); every statement the docs use is shown in full in the deep dive
- REST resources — Week 1 · Thu BE: "resources, verbs, status codes" (no doc yet); each resource becomes a table

**Confusables**

- Key vs index — the signal: could removing it change an *answer*? Then it is a key; if it could only change the *time*, it is an index.
- Normalized tables vs document embedding — the signal: if the two copies disagree, is one of them wrong? Then normalize; if not, embedding is harmless.
- `UNION` vs `JOIN` — the signal: do you want the result taller (same kind of thing stacked) or wider (related things paired)?

**Unlocks**

- Indexes — what they cost, when they help, `EXPLAIN` on your own queries — Week 3 · Thu (BE)
- Auth: `users` and `refresh_tokens` tables — Week 4 · Tue/Thu (SD/BE); Practice B2
- Express + Postgres API with working JWT auth — Month 1 exit check
- Tested products API — Week 9 (BE); indexed filter queries, proved with `EXPLAIN` — Week 11 (BE)
- Cart endpoints with a version column — Week 12 (BE); Practice B4
- Checkout in one Postgres transaction, the last-item race — Week 15 (BE)
- DBMS: normalization 1NF–3NF, ACID, SQL by hand — Week 19
- Backend HLD "Data model" step — SQL vs NoSQL with the reason — Week 16 and Week 18 (SD)

## Mastery Checklist

- [ ] You can draw the shop schema — six tables, keys, foreign-key directions, the two junction tables — from a blank page without notes
- [ ] You can name the three anomalies and show each one on the flat table in one sentence
- [ ] You can state the snapshot test and apply it to "price on a line" (keep) versus "email on an order" (never)
- [ ] You can write the inner, left, aggregate-first and anti-join queries the products API needs and predict their row counts before running them
- [ ] You can explain why a `LEFT JOIN` lost rows and fix it by moving the condition from `WHERE` to `ON`
- [ ] You can explain why a `NOT IN` report returned nothing and rewrite it as `NOT EXISTS`
- [ ] You can say what `numeric`, `timestamptz` and `CHECK` each prevent, with the run that shows it
- [ ] You can explain it to someone else in two minutes, starting from the notebook story

## 2-Minute Revision

1. One table per kind of thing; one row, one fact; one cell, one value.
2. Every table: a generated numeric primary key. Natural keys are `UNIQUE`, never the key.
3. Foreign key on the many side; the database refuses orphans and refuses to delete a parent in use.
4. `ON DELETE` defaults to refuse. Cascade only for children that mean nothing alone — lines, junction rows, tokens.
5. Many-to-many = a table of pairs, keyed by the pair.
6. 3NF: key, whole key, nothing but the key. It removes the update, insert and delete anomalies.
7. Snapshot test: may the two values differ? Price paid — yes, store it. Email on an order — no, join for it.
8. Inner join: matched pairs. Left join: plus unmatched left rows, right side `NULL`.
9. The snippet: `COUNT(*)` gives Meera 1, `COUNT(o.id)` gives her 0 (Know This Cold).
10. Trap: a right-side condition in `WHERE` turns a left join inner. Put it in `ON`.
11. Trap: two one-to-many joins multiply — aggregate each side to one row per parent first.
12. `NULL` is unknown: it fails `=`, slips past `UNIQUE`, empties `NOT IN`. Write `NOT EXISTS`.
