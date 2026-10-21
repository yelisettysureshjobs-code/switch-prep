# Postgres — Schema Design, Normalization & the Joins You'll Actually Use — Practice

> **Track:** BE · **Type:** concept · **Scheduled:** Week 3 · Tue · **Runtime:** PostgreSQL 17 · **Generated:** 2026-10-20

## How to Use This File

Every query here runs against the `shop` schema from [deep-dive.md](deep-dive.md) → Why It Exists (the `CREATE TABLE` and `INSERT` blocks at its end) — create and seed it once in a scratch database; outputs are shown with `\pset null NULL`. Every answer is collapsed; take the smallest hint you need. Warm-Up: predict the rows — count and values — on paper before running anything. Spot It: name the idea in 60 seconds, no code. Build It: SQL from a blank file, in the mode on each item's italic line. Self-Check and Explain It Back are answered out loud. Log results in `prep/notes.md` in your own words; this slot is a 1-hour BE block on the learn-first track, and the method lives in [how-to-study.md](../../../prep/how-to-study.md).

## Warm-Up

### W1 — Orders with names

```sql
SELECT c.name, o.id AS order_id
FROM customers c
JOIN orders o ON o.customer_id = c.id
ORDER BY o.id;
```

<details>
<summary>Output and why</summary>

```text
 name | order_id
------+----------
 Asha |        1
 Asha |        2
 Ravi |        3
(3 rows)
```

An inner join emits one row per matching pair: Asha matches two orders, Ravi one, Meera none — so three rows, and Meera is absent without any error (Idea #8).

</details>

### W2 — Two inserts and a count

```sql
INSERT INTO order_items (order_id, product_id, quantity, unit_price)
VALUES (3, 1, 0, 499.00);

INSERT INTO order_items (order_id, product_id, quantity, unit_price)
VALUES (9, 1, 1, 499.00);

SELECT COUNT(*) AS rows_in_order_items FROM order_items;
```

<details>
<summary>Output and why</summary>

```text
ERROR:  new row for relation "order_items" violates check constraint …
DETAIL:  Failing row contains (3, 1, 0, 499.00).
ERROR:  insert or update on table "order_items" violates foreign key constraint …
DETAIL:  Key (order_id)=(9) is not present in table "orders".
 rows_in_order_items
---------------------
                   4
(1 row)
```

The first insert breaks `CHECK (quantity > 0)`; the second references order 9, which does not exist. Both are refused before anything is stored, so the count is still the seed's 4. The schema enforces the rules on every code path, not only the ones that remembered to validate (Ideas #3, #6).

</details>

### W3 — Early orders

```sql
SELECT c.name, o.id AS order_id
FROM customers c
LEFT JOIN orders o ON o.customer_id = c.id
WHERE o.placed_at < '2026-10-03'
ORDER BY c.name;
```

<details>
<summary>Output and why</summary>

```text
 name | order_id
------+----------
 Asha |        1
(1 row)
```

One row, not three. The left join does keep Meera, with `placed_at = NULL` — and then `WHERE` tests `NULL < '2026-10-03'`, which is unknown, and drops her along with Ravi (order 3 is on the 7th) and Asha's order 2. A `WHERE` on the right-hand table turns a left join back into an inner join; the date belongs in `ON` if the goal was "every customer, with their early orders" (Idea #9, #12).

</details>

### W4 — Orders per customer

```sql
SELECT c.name, COUNT(*) AS orders
FROM customers c
LEFT JOIN orders o ON o.customer_id = c.id
GROUP BY c.name
ORDER BY c.name;
```

<details>
<summary>Output and why</summary>

```text
 name  | orders
-------+--------
 Asha  |      2
 Meera |      1
 Ravi  |      1
(3 rows)
```

Meera has no orders and is reported with one. `COUNT(*)` counts output rows, and the left join produced one `NULL`-padded row for her. `COUNT(o.id)` counts only rows where `o.id` is not `NULL` and gives the 0 you meant (Ideas #9, #12).

</details>

### W5 — Asha's basket

```sql
SELECT c.name, SUM(oi.quantity) AS items_bought
FROM customers c
JOIN orders o              ON o.customer_id = c.id
JOIN order_items oi        ON oi.order_id = o.id
JOIN product_categories pc ON pc.product_id = oi.product_id
WHERE c.name = 'Asha'
GROUP BY c.name;
```

<details>
<summary>Output and why</summary>

```text
 name | items_bought
------+--------------
 Asha |            5
(1 row)
```

Asha bought 4 items: two T-shirts, one pair of Sneakers, one pair of Jeans. The Sneakers line matches two `product_categories` rows (footwear and sale), so it is emitted twice and counted twice — fan-out (Idea #10). The category join contributes nothing to the question and should not be there; without it the same query gives the truth:

```sql
SELECT c.name, SUM(oi.quantity) AS items_bought
FROM customers c
JOIN orders o       ON o.customer_id = c.id
JOIN order_items oi ON oi.order_id = o.id
WHERE c.name = 'Asha'
GROUP BY c.name;
```

```text
 name | items_bought
------+--------------
 Asha |            4
(1 row)
```

</details>

### W6 — Two never-lists

```sql
SELECT name FROM products
WHERE id NOT IN (SELECT product_id FROM order_items);

SELECT name FROM customers
WHERE id NOT IN (SELECT referred_by FROM customers);
```

<details>
<summary>Output and why</summary>

```text
 name
------
 Cap
(1 row)

 name
------
(0 rows)
```

Same shape, opposite outcomes. `order_items.product_id` is `NOT NULL`, so the first `NOT IN` works and finds Cap. `customers.referred_by` is nullable and Asha's is `NULL`, so every `id NOT IN (1, 1, NULL)` is unknown and the second query returns nobody — though Ravi and Meera have referred no one. `NOT EXISTS` is immune to this (Ideas #11, #12).

</details>

## Spot It

### S1 — The customer list has a double

Support reports that the customer list shows "Asha" twice, with two different email addresses, since an agent corrected her email "from the order screen" yesterday. No error was logged. Which idea explains it, and where in the schema is the fault?

<details>
<summary>Answer</summary>

An update anomaly (Idea #5): the email is stored on each order row (or copied onto it) instead of once on the customer. The order-screen edit changed one copy. The fault is the schema, not the agent — a fact about the customer must live on `customers` only, and the order holds `customer_id` (Ideas #1, #3).

</details>

### S2 — The dashboard overshoots

Finance says the "units sold per product, with categories" dashboard doubled every product that is also in the `sale` category, and the grand total of "revenue by category" is higher than total revenue. Which idea, and which of the two complaints is actually a bug?

<details>
<summary>Answer</summary>

Fan-out (Idea #10). "Units per product with categories" joins a product to two one-to-many children — lines and categories — so each line is repeated once per category; aggregate each side to one row per product first. "Revenue by category" double counting a product that sits in two categories is by design — a category total is a category total — but its grand total is therefore not total revenue, and the dashboard should say so or sum `order_items` directly.

</details>

### S3 — The campaign reached nobody

A marketing email to "customers who have never placed an order" was sent to zero people, although analytics shows thousands of such accounts. The query uses `NOT IN (SELECT customer_id FROM orders)`, and last month guest checkout was added — guest orders have `customer_id = NULL`. Which idea?

<details>
<summary>Answer</summary>

The `NOT IN` trap (Ideas #11, #12). One `NULL` among the subquery's values makes every `NOT IN` comparison unknown, so `WHERE` drops every row and the query returns nothing, with no error. Rewrite as `NOT EXISTS (SELECT 1 FROM orders o WHERE o.customer_id = c.id)`, which never matches a `NULL`.

</details>

### S4 — The slow search

Product search by name takes four seconds on two million rows. The team's first proposal is to "fix the schema" — it is already in 3NF, with proper keys. Is this topic the answer?

<details>
<summary>Answer</summary>

No. Nothing about keys, normal forms or joins changes how fast one row is *found*; that is an index, Thursday's BE slot. The schema is fine; the query needs an index on the column it searches (and `EXPLAIN` to prove it). A key decides what may exist; an index decides how fast it is reached.

</details>

### S5 — One hundred and forty columns

Each product category has its own attributes — shoes have sizes, phones have storage, shirts have sleeve length — and the team keeps adding nullable columns to `products`. It is at 140 columns, mostly `NULL`. Someone proposes normalizing: an `attributes` table with one row per product per attribute. Is this topic the answer?

<details>
<summary>Answer</summary>

Not really. Those attributes are never shared between rows or joined on, so there is no redundancy for normalization to remove — the attribute-per-row table (an "entity-attribute-value" design) just moves the sprawl into a join that is slow and hard to query. A per-row bag of attributes — Postgres's `jsonb` column, or a document shape — fits data that varies per item and is read whole. Keep the shared facts (name, price, categories) normalized; put the per-kind attributes in one flexible column. The boundary is a Week 16 HLD conversation.

</details>

### S6 — One list from two tables

The newsletter needs a single list of email addresses from `customers` and from `newsletter_signups`, which have the same columns. A teammate joined the two tables and got a cross product. Is a join what this needs?

<details>
<summary>Answer</summary>

No — `UNION`. The two tables hold the *same kind* of thing, and the result should be taller, not wider. `SELECT email FROM customers UNION SELECT email FROM newsletter_signups` stacks the rows and removes duplicates. A join relates different kinds of things through a matching rule, and with no meaningful rule it degenerates into every pair.

</details>

## Build It

### B1 — The shop schema, from a blank file

*Scheduled: Week 3 · Tue — this slot, "Postgres — schema design, normalization, the joins you'll actually use" · Mode: from memory, then run it*

**Task.** From a blank `.sql` file, write and run the schema for the plan's commerce app: customers, products, categories (a product can be in many categories), orders, and order lines. Requirements: every table has a generated numeric key; a customer's email is unique; prices are exact and never negative; a line's quantity is positive; a line stores the price actually paid; deleting a customer who has orders is refused; deleting an order removes its lines; the same product cannot appear twice on one order. Then prove it: write four inserts that must fail (an order for a customer that does not exist, a line with quantity 0, a second customer with an existing email, a duplicate product–category pair) and show that each is rejected.

<details>
<summary>Hint 1 — where to look</summary>

List the nouns in the task. Which pairs of nouns are "one has many" and which are "many have many"? The second kind needs something the first does not.

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Every foreign key goes on the "many" side; a many-to-many gets a table of pairs whose primary key is the pair; and the price on a line is a snapshot, so it is a column on the line, not a join to the product.

</details>

<details>
<summary>Solution</summary>

**Approach.** Six tables: `customers`, `products`, `categories`, `product_categories` (the junction), `orders`, `order_items` (a junction that carries `quantity` and `unit_price`). `ON DELETE CASCADE` only on the two junction tables' links to their owners; the default `NO ACTION` everywhere else.

The schema and the seed are at the end of [deep-dive.md](deep-dive.md) → Why It Exists; the four rejected inserts are in Level 2 (Relationships are foreign keys, Many-to-many needs a third table, The database refuses bad data). Compare your `CREATE TABLE`s against it line by line — the differences are the lesson.

**Edge cases handled.** Orphan order refused; `quantity = 0` refused; duplicate email refused; duplicate `(product_id, category_id)` refused; a hand-written `id` refused by `GENERATED ALWAYS`; deleting an order removes its lines; deleting a customer with orders is refused — each one was run in the deep dive.

**Uses ideas:** #1, #2, #3, #4, #6, #7

</details>

### B2 — Auth tables for Week 4

*Scheduled: Week 4 · Tue — SD/BE: "auth — JWT vs sessions, refresh token flow, where tokens live"; Week 4 · Thu — "wire auth into your Express API end to end" · Mode: from memory*

**Task.** Design the two tables the refresh-token flow needs: users, and the refresh tokens issued to them. One user has many tokens (one per device); a token can be revoked without deleting it; a token's hash must be unique; deleting a user must remove every token that was theirs. Then run a scenario: create a user with two tokens, revoke one, list the tokens that are still usable on 2026-10-21, delete the user, and show that no tokens remain.

<details>
<summary>Hint 1 — where to look</summary>

Which table holds the foreign key, and what should happen to its rows when the user row goes away? And what is the difference between a token that is revoked and a token that is deleted?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

"Revoked" is a fact about the token — a nullable timestamp on its row — and "usable" is a query: not revoked and not expired. The user → tokens link is the one place where `ON DELETE CASCADE` is exactly right, because a token is meaningless without its user.

</details>

<details>
<summary>Solution</summary>

**Approach.** `users` with a unique email and a password hash; `refresh_tokens` with `user_id` cascading, a unique `token_hash`, `expires_at`, and a nullable `revoked_at`. Store hashes, never raw tokens — the same reason you store a password hash.

```sql
CREATE TABLE users (
  id            bigint      GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  email         text        NOT NULL UNIQUE,
  password_hash text        NOT NULL,
  created_at    timestamptz NOT NULL DEFAULT now()
);

CREATE TABLE refresh_tokens (
  id         bigint      GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  user_id    bigint      NOT NULL REFERENCES users (id) ON DELETE CASCADE,
  token_hash text        NOT NULL UNIQUE,
  expires_at timestamptz NOT NULL,
  revoked_at timestamptz
);

BEGIN;
INSERT INTO users (email, password_hash) VALUES ('asha@example.com', '$2b$10$hash-goes-here');
INSERT INTO refresh_tokens (user_id, token_hash, expires_at) VALUES
  (1, 'hash-laptop', '2026-11-20 00:00+00'),
  (1, 'hash-phone',  '2026-11-20 00:00+00');

UPDATE refresh_tokens SET revoked_at = '2026-10-21 00:00+00' WHERE token_hash = 'hash-phone';

SELECT token_hash
FROM refresh_tokens
WHERE user_id = 1 AND revoked_at IS NULL AND expires_at > '2026-10-21 00:00+00';

DELETE FROM users WHERE email = 'asha@example.com';
SELECT COUNT(*) AS tokens_left FROM refresh_tokens;
ROLLBACK;
```

```text
 token_hash
-------------
 hash-laptop
(1 row)

 tokens_left
-------------
           0
(1 row)
```

**Edge cases handled.** Revoked token excluded by `revoked_at IS NULL` (a `NULL` test, not `= NULL`); expiry compared as `timestamptz`; a second device is a second row, not a second column; deleting the user cascaded to both tokens; the password hash is `NOT NULL` so a user can never exist without a credential.

**Uses ideas:** #3, #6, #12

</details>

### B3 — The queries the products API needs

*Scheduled: Week 9 · Tue/Thu — BE: "tested products API" (and Week 9's "scaffold … routing") · Mode: from memory*

**Task.** Against the shop schema, write the four queries the first endpoints need, each returning exactly one row per thing: (a) the products in the `clothing` category, with price; (b) one customer's orders (customer 1), newest first, each with its number of lines and its total — an order with no lines must still appear with 0; (c) every customer ranked by total spend, including customers who have spent nothing; (d) revenue per category, highest first — and then say in one sentence why the category figures do not add up to total revenue, and show total revenue.

<details>
<summary>Hint 1 — where to look</summary>

For each query, ask: which table should contribute exactly one row to the output, and which joins could multiply it or drop it?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

"Must still appear" means `LEFT JOIN` plus `COALESCE` for the sum and `COUNT(child column)` for the count; "per category" through a junction table counts a product once per category it is in, on purpose.

</details>

<details>
<summary>Solution</summary>

**Approach.** (a) is a two-hop inner join through the junction table. (b) and (c) are left joins aggregated per parent, with `COALESCE(SUM(…), 0)` so the `NULL` of "no match" reads as 0. (d) is a two-hop join from categories through `product_categories` to `order_items`; a product in two categories counts in both.

```sql
SELECT p.id, p.name, p.price
FROM products p
JOIN product_categories pc ON pc.product_id = p.id
JOIN categories c          ON c.id = pc.category_id
WHERE c.name = 'clothing'
ORDER BY p.name;
```

```text
 id |  name   |  price
----+---------+---------
  4 | Cap     |  299.00
  2 | Jeans   | 1999.00
  1 | T-shirt |  499.00
(3 rows)
```

```sql
SELECT o.id AS order_id, o.placed_at::date AS placed_on,
       COUNT(oi.product_id) AS lines, COALESCE(SUM(oi.quantity * oi.unit_price), 0) AS total
FROM orders o
LEFT JOIN order_items oi ON oi.order_id = o.id
WHERE o.customer_id = 1
GROUP BY o.id, o.placed_at
ORDER BY o.placed_at DESC;
```

```text
 order_id | placed_on  | lines |  total
----------+------------+-------+---------
        2 | 2026-10-05 |     1 | 1999.00
        1 | 2026-10-01 |     2 | 3497.00
(2 rows)
```

```sql
SELECT c.name, COALESCE(SUM(oi.quantity * oi.unit_price), 0) AS spent
FROM customers c
LEFT JOIN orders o       ON o.customer_id = c.id
LEFT JOIN order_items oi ON oi.order_id = o.id
GROUP BY c.id, c.name
ORDER BY spent DESC, c.name;
```

```text
 name  |  spent
-------+---------
 Asha  | 5496.00
 Ravi  | 2499.00
 Meera |       0
(3 rows)
```

```sql
SELECT c.name AS category, SUM(oi.quantity * oi.unit_price) AS revenue
FROM categories c
JOIN product_categories pc ON pc.category_id = c.id
JOIN order_items oi        ON oi.product_id = pc.product_id
GROUP BY c.name
ORDER BY revenue DESC;
```

```text
 category | revenue
----------+---------
 footwear | 4998.00
 sale     | 4998.00
 clothing | 2997.00
(3 rows)
```

```sql
SELECT SUM(quantity * unit_price) AS revenue FROM order_items;
```

```text
 revenue
---------
 7995.00
(1 row)
```

The categories sum to 12,993 against 7,995 of real revenue because both Sneakers sales sit in `footwear` and in `sale`: a per-category figure counts a product once per category by design, so category figures are not partitions of the total and must never be summed as if they were.

**Edge cases handled.** (b) `COUNT(oi.product_id)` not `COUNT(*)`, so an order with no lines would show 0, and `COALESCE` turns its `NULL` sum into 0; (c) Meera appears with 0 through two left joins; (c) chains `orders → order_items` as one-to-many of one-to-many, which does not fan out because each line belongs to exactly one order; (d) names its fan-out instead of hiding it.

**Uses ideas:** #4, #8, #9, #10, #12

</details>

### B4 — The cart

*Scheduled: Week 12 · Tue/Thu — BE: "cart endpoints with a version column — reject stale writes with 409" (the version column is Week 12's own lesson; this item is the shape under it) · Mode: from memory*

**Task.** Add the cart to the shop schema: each customer has at most one cart; a product appears at most once per cart; adding a product that is already in the cart increments its quantity instead of adding a row; removing a customer removes their cart and its contents. Then run: give Meera a cart, add Cap twice, show the cart with line totals, and show that a second cart for Meera is refused.

<details>
<summary>Hint 1 — where to look</summary>

Two of the rules are about what may not exist twice. Which constraints say that, and on which columns?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

"At most one per customer" is `UNIQUE` on the foreign key; "at most once per cart" is a composite primary key on the pair — and with that key in place, "add or increment" is one statement that uses the conflict on it.

</details>

<details>
<summary>Solution</summary>

**Approach.** `carts` with `customer_id UNIQUE` (one-to-one, enforced), and `cart_items` as a junction table keyed by `(cart_id, product_id)`. The increment is `INSERT … ON CONFLICT (cart_id, product_id) DO UPDATE` — the composite key is what makes the conflict detectable. Cart lines do not snapshot the price: a cart shows today's price, unlike an order.

```sql
CREATE TABLE carts (
  id          bigint      GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  customer_id bigint      NOT NULL UNIQUE REFERENCES customers (id) ON DELETE CASCADE,
  updated_at  timestamptz NOT NULL DEFAULT now()
);

CREATE TABLE cart_items (
  cart_id    bigint  NOT NULL REFERENCES carts (id)    ON DELETE CASCADE,
  product_id bigint  NOT NULL REFERENCES products (id) ON DELETE CASCADE,
  quantity   integer NOT NULL CHECK (quantity > 0),
  PRIMARY KEY (cart_id, product_id)
);

BEGIN;
INSERT INTO carts (customer_id) VALUES (3);

INSERT INTO cart_items (cart_id, product_id, quantity) VALUES (1, 4, 1)
ON CONFLICT (cart_id, product_id) DO UPDATE SET quantity = cart_items.quantity + 1;
INSERT INTO cart_items (cart_id, product_id, quantity) VALUES (1, 4, 1)
ON CONFLICT (cart_id, product_id) DO UPDATE SET quantity = cart_items.quantity + 1;

SELECT p.name, ci.quantity, p.price * ci.quantity AS line_total
FROM cart_items ci
JOIN products p ON p.id = ci.product_id
WHERE ci.cart_id = 1;

INSERT INTO carts (customer_id) VALUES (3);
ROLLBACK;
```

```text
 name | quantity | line_total
------+----------+------------
 Cap  |        2 |     598.00
(1 row)

ERROR:  duplicate key value violates unique constraint "carts_customer_id_key"
DETAIL:  Key (customer_id)=(3) already exists.
```

**Edge cases handled.** Second cart for the same customer refused by `UNIQUE`; the same product twice became one line with quantity 2; a product deleted from the catalogue disappears from carts (`CASCADE` on `product_id`) where on `order_items` it is refused — a cart is not history, an order is; the cart joins to `products` for the current price instead of storing a snapshot.

**Uses ideas:** #3, #4, #6, #7

</details>

## Self-Check Questions

### Q1 — Why is the primary key a generated number and not the email, which is unique anyway?

<details>
<summary>Answer</summary>

Because the key is written into every row that points at the customer, and it must never change. An email is a natural key: unique today, but a person can change it, and then every referencing row is stale or must be rewritten. The generated number has no meaning to change; the email stays `UNIQUE` on the side (Idea #2).

</details>

### Q2 — In a one-to-many, which table holds the foreign key, and why can it not be the other one?

<details>
<summary>Answer</summary>

The "many" side. A customer row would need a list of order ids to point the other way, and a list in a cell is the first normal form breaking — you could not join on it, index it, or constrain it. One order has exactly one customer, so one column on the order says which (Ideas #1, #3).

</details>

### Q3 — Name the three anomalies, one sentence each, using the shop.

<details>
<summary>Answer</summary>

Update: Asha's email on every order row — change one and she has two emails. Insert: a new product nobody has ordered has no row to live in when products exist only inside orders. Delete: cancel Ravi's only order and Ravi himself disappears. All three are one fact stored in more than one place, or several facts in one (Idea #5).

</details>

### Q4 — What does "the key, the whole key, and nothing but the key" test, form by form?

<details>
<summary>Answer</summary>

"The key": every row is identified by a key and every cell is atomic — 1NF. "The whole key": no non-key column depends on only part of a composite key — 2NF (a product name on a line keyed by order and product). "Nothing but the key": no non-key column depends on another non-key column — 3NF (a customer's name depending on the customer's email on an order row) (Idea #5).

</details>

### Q5 — Why does `order_items` store `unit_price` but `orders` does not store `total`?

<details>
<summary>Answer</summary>

Apply one question to each: if the stored value and the recomputed value disagree, is one of them wrong? For `unit_price`, no — the price paid is allowed to differ from today's catalogue price, so it is a separate fact, a snapshot. For `total`, yes — a total that disagrees with its lines is a bug, so it is derived and recomputed with `SUM` (Idea #7).

</details>

### Q6 — What is the difference between a condition in `ON` and the same condition in `WHERE`, and when does it matter?

<details>
<summary>Answer</summary>

`ON` is evaluated while matching: it decides which pairs count as a match. `WHERE` is evaluated after the join on the finished rows: it decides which survive. For an inner join the result is identical. For an outer join a `WHERE` on the nullable side also removes the `NULL`-padded rows the outer join added — it turns a left join back into an inner one (Idea #9).

</details>

### Q7 — Why does `COUNT(*)` report one order for Meera while `COUNT(o.id)` reports zero?

<details>
<summary>Answer</summary>

The left join emitted one row for Meera with every `orders` column `NULL`. `COUNT(*)` counts rows, so that row counts. `COUNT(o.id)` counts non-`NULL` values of `o.id`, and hers is `NULL`. "How many orders" is a question about order rows, so it is `COUNT(o.id)` (Ideas #9, #12).

</details>

### Q8 — You join `products` to both `order_items` and `reviews` and take `SUM(quantity)`. What happens, and what is the fix?

<details>
<summary>Answer</summary>

Each product's lines are repeated once per review, so the sum is multiplied by its review count — fan-out. Aggregate lines per product and reviews per product in separate subqueries, each returning one row per product, then join those; or compute each aggregate in its own query (Idea #10).

</details>

### Q9 — Why is `NULL = NULL` not true, and which two SQL features behave surprisingly because of it?

<details>
<summary>Answer</summary>

`NULL` means unknown, and comparing two unknowns gives unknown, not true — three-valued logic. Consequences: `UNIQUE` cannot see two `NULL`s as duplicates, so an optional unique column may hold many `NULL`s; and `NOT IN` against a list containing `NULL` is unknown for every row, so the query returns nothing. Also `WHERE col <> x` drops `NULL` rows, and `COUNT(col)` skips them (Idea #12).

</details>

### Q10 — Why is `NO ACTION` the default for `ON DELETE`, and when do you choose `CASCADE` or `SET NULL`?

<details>
<summary>Answer</summary>

Because refusing is the only action that cannot destroy data you did not mean to touch; the designer must opt into anything else. `CASCADE` when the child is meaningless without the parent — order lines, junction rows, refresh tokens. `SET NULL` when the child is valid on its own and the link was optional — a customer's referrer. Never cascade into money or history; make that an explicit workflow (Idea #3).

</details>

### Q11 — What goes wrong if `product_categories` gets its own `id` key and no unique constraint on the pair?

<details>
<summary>Answer</summary>

The same product–category pair can be inserted twice with different ids. Every join through the table then fans out — the product appears twice in its category listing and double counts in any aggregate — and nothing reports an error. The pair *is* the identity of the row, so the pair is the key, or at least `UNIQUE` (Ideas #4, #6).

</details>

### Q12 — What would change if `price` were `double precision` instead of `numeric(10, 2)`?

<details>
<summary>Answer</summary>

Most decimal prices would be stored as the nearest binary fraction, and sums of many lines would drift from the sum of the receipts by fractions of a paisa that accumulate; `0.1 + 0.2 = 0.3` is already false. The database would accept it silently, so the error surfaces as a reconciliation mismatch months later. `numeric` stores decimal digits exactly and is the manual's recommendation for money (Idea #6).

</details>

## Explain It Back

### E1 — Explain schema design to a new teammate in two minutes, starting from the notebook story

<details>
<summary>Rubric — what a complete answer covers</summary>

- One notebook per kind of thing, one line per thing, one value per box — and what went wrong with the single big notebook (a fact in many places).
- Every line gets a number that never changes; other notebooks write the number, never the name.
- A many-to-many is a notebook of pairs.
- The three anomalies, named, as the reason for all of this.
- The one deliberate copy — the price on the sale page — and the test that allows it.
- A join is flipping to the matching number: one output row per match, none for a thing with no match unless you ask to keep it.

</details>

### E2 — Your interviewer asks: "Why not store each order as one JSON document with its lines and the customer's details inside it?"

<details>
<summary>Rubric — what a complete answer covers</summary>

- Acknowledge what it buys: one read, no joins, the shape the API returns.
- Separate the two kinds of embedded data: line prices are snapshots (fine to embed), the customer's email is a shared fact (not fine — the update anomaly).
- Name what is lost: constraints across documents (an order for a non-existent product), and "all orders containing product X" or "revenue per category" become scans.
- Concede the hybrid: a normalized schema can still keep a per-order `jsonb` for genuinely per-order free-form data.
- Close with the rule: embed what is allowed to diverge; normalize what must agree.

</details>

### E3 — Out loud, walk through why this report double counts and fix it as you talk

The query from the deep dive that reports Sneakers sold as 4: `products` joined to `order_items` and to `product_categories` and `categories`, then `SUM(quantity)` grouped by product.

<details>
<summary>Rubric — what a complete answer covers</summary>

- State the row-pair definition of a join and that two one-to-many joins multiply (`2 lines × 2 categories = 4 rows`).
- Show the four intermediate rows before the `GROUP BY`, in words.
- Name the rule: one row per parent on each side before joining.
- Give the fixed shape: a lines-per-product subquery and a categories-per-product subquery, both left-joined to `products`.
- Mention `COALESCE` for products with no sales and why `LEFT` rather than inner (Cap must appear).
- Note which aggregates fan-out silently spares (`AVG`) and which it breaks (`SUM`, `COUNT`), so "the averages look fine" is not evidence the report is right.

</details>
