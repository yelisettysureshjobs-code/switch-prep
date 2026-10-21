# Postgres — Schema Design, Normalization & the Joins You'll Actually Use — Deep Dive

> **Track:** BE · **Type:** concept · **Scheduled:** Week 3 · Tue · **Runtime:** PostgreSQL 17 · **Generated:** 2026-10-20

> **How to read the outputs.** Every output below came from `psql -q` on PostgreSQL 17.5 with `\pset null NULL`, so a NULL cell prints as `NULL` instead of blank, and statement tags such as `INSERT 0 3` are hidden. Every query from Level 1 on runs against the `shop` schema created at the end of Why It Exists. A block that changes data is wrapped in `BEGIN … ROLLBACK` so the data is identical for every later block; what a transaction guarantees is Week 15's topic, not this one. Where an `ERROR` line is longer than this page is wide it is cut with `…`; the `DETAIL` line under it is complete.

## Why It Exists

Prerequisite: the REST API from the Week 1–2 BE slots — see [month-01.md](../../../prep/daily/month-01.md) (Week 1 · Thu: "REST design — resources, verbs, status codes"). This topic needs one idea from it: each resource the API exposes (`/customers`, `/products`, `/orders`) is a kind of thing the database has to store.

The first instinct for storing orders is a spreadsheet: one row per order, with everything you know about that order written on the row.

```sql
CREATE TABLE sales_flat (
  order_id       integer,
  customer_name  text,
  customer_email text,
  items          text,
  total          numeric(10, 2)
);

INSERT INTO sales_flat VALUES
  (1, 'Asha', 'asha@example.com', 'T-shirt x2, Sneakers x1', 3497.00),
  (2, 'Asha', 'asha@example.com', 'Jeans x1',                1999.00),
  (3, 'Ravi', 'ravi@example.com', 'Sneakers x1',             2499.00);
```

Asha changes her email. The support tool she is using is looking at order 2, so that is the row it updates:

```sql
UPDATE sales_flat SET customer_email = 'asha.k@example.com' WHERE order_id = 2;

SELECT DISTINCT customer_name, customer_email
FROM sales_flat
ORDER BY customer_name, customer_email;
```

```text
 customer_name |   customer_email
---------------+--------------------
 Asha          | asha.k@example.com
 Asha          | asha@example.com
 Ravi          | ravi@example.com
(3 rows)
```

Nothing failed, and the database now holds two emails for one person. Whichever one the next query picks up is right by luck. Then Ravi cancels his only order:

```sql
DELETE FROM sales_flat WHERE order_id = 3;

SELECT DISTINCT customer_name FROM sales_flat;
```

```text
 customer_name
---------------
 Asha
(1 row)
```

Ravi is gone — not only his order, the customer. There is no row left that knows he exists. And a question the shop asks daily, "how many Sneakers have we sold?", has no honest answer:

```sql
SELECT order_id, items FROM sales_flat WHERE items LIKE '%Sneakers%';
```

```text
 order_id |          items
----------+-------------------------
        1 | T-shirt x2, Sneakers x1
(1 row)
```

The quantity is trapped inside a string, and order 3's Sneakers went with the delete. A fourth problem never even shows up as an error: a new product nobody has ordered yet has nowhere to live in this table at all.

Four failures, one cause: the same fact is written in more than one place, and several facts are written in one place. Schema design is the discipline of storing each fact exactly once, in a table built for it. Joins are how a query puts the facts back together on demand. Everything in this doc is one of those two skills, and the Month 1 exit check — "Express + Postgres API with working JWT auth" — stands on both.

Here is the same data stored properly — the `shop` schema that every query in these three files runs against. Read it once now for its shape; Level 2 explains each line.

```sql
CREATE TABLE customers (
  id          bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  email       text   NOT NULL UNIQUE,
  name        text   NOT NULL,
  phone       text   UNIQUE,
  referred_by bigint REFERENCES customers (id)
);

CREATE TABLE products (
  id    bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  name  text           NOT NULL,
  price numeric(10, 2) NOT NULL CHECK (price >= 0)
);

CREATE TABLE categories (
  id   bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  name text   NOT NULL UNIQUE
);

CREATE TABLE product_categories (
  product_id  bigint NOT NULL REFERENCES products (id)   ON DELETE CASCADE,
  category_id bigint NOT NULL REFERENCES categories (id) ON DELETE CASCADE,
  PRIMARY KEY (product_id, category_id)
);

CREATE TABLE orders (
  id          bigint      GENERATED ALWAYS AS IDENTITY PRIMARY KEY,
  customer_id bigint      NOT NULL REFERENCES customers (id),
  placed_at   timestamptz NOT NULL DEFAULT now()
);

CREATE TABLE order_items (
  order_id   bigint         NOT NULL REFERENCES orders (id) ON DELETE CASCADE,
  product_id bigint         NOT NULL REFERENCES products (id),
  quantity   integer        NOT NULL CHECK (quantity > 0),
  unit_price numeric(10, 2) NOT NULL,
  PRIMARY KEY (order_id, product_id)
);
```

And the rows it holds for the rest of the doc — small on purpose, so every result can be checked by hand. Meera has no orders and Cap has never been ordered; both are there to make the join behaviour visible.

```sql
INSERT INTO customers (email, name, phone, referred_by) VALUES
  ('asha@example.com',  'Asha',  '9000000001', NULL),
  ('ravi@example.com',  'Ravi',  NULL,         1),
  ('meera@example.com', 'Meera', NULL,         1);

INSERT INTO products (name, price) VALUES
  ('T-shirt',   499.00),
  ('Jeans',    1999.00),
  ('Sneakers', 2499.00),
  ('Cap',       299.00);

INSERT INTO categories (name) VALUES ('clothing'), ('footwear'), ('sale');

INSERT INTO product_categories (product_id, category_id) VALUES
  (1, 1), (2, 1), (3, 2), (3, 3), (4, 1);

INSERT INTO orders (customer_id, placed_at) VALUES
  (1, '2026-10-01 10:00+00'),
  (1, '2026-10-05 18:30+00'),
  (2, '2026-10-07 09:15+00');

INSERT INTO order_items (order_id, product_id, quantity, unit_price) VALUES
  (1, 1, 2,  499.00),
  (1, 3, 1, 2499.00),
  (2, 2, 1, 1999.00),
  (3, 3, 1, 2499.00);
```

## Level 1 — Intuition

The notebook story — Mia's one big notebook, then three small ones — is in [brief.md](brief.md); here is what it maps to.

*(Idea 1)* A **table** is a notebook for one kind of thing: customers, products, orders. Each **row** is one of those things, and each **column** holds one value about it. "One row, one fact; one cell, one value" is the whole rule. `sales_flat` broke it twice: the `items` cell held two facts, and Asha's email — a fact about Asha — was written on every one of her order rows instead of once on an Asha row. Every failure above traces back to one of those two breaks.

*(Idea 2)* Every row gets a number that never changes: its **primary key**. Mia numbers her friends because names repeat (two Ashas) and change (a wedding, a typo fix). A number the system makes up for this purpose is a **surrogate key**. A real-world identifier such as an email is a **natural key** — it is unique today, but a person can change it, and then every notebook that wrote it down is wrong. So the number is the key, and the email is kept unique on the side (Level 2 shows how).

With the two notebooks numbered, a sale no longer writes the customer's name. It writes the customer's number — this is `customers` and `orders` from the schema above, trimmed to the columns that matter here:

```text
customers                          orders
id  name   email                   id  customer_id  placed_on
──  ─────  ─────────────────       ──  ───────────  ──────────
1   Asha   asha@example.com        1   1            2026-10-01
2   Ravi   ravi@example.com        2   1            2026-10-05
3   Meera  meera@example.com       3   2            2026-10-07
    ▲                                  │
    └──────── "customer 2" ────────────┘   the order holds a number, never a name
```

*(Idea 8)* A **join** is Mia flipping to the matching number. The query names the two notebooks and the rule for matching — "this order's `customer_id` equals that customer's `id`" — and the database produces one output row for every pair that matches:

```sql
SELECT o.id AS order_id, c.name
FROM orders o
JOIN customers c ON c.id = o.customer_id
ORDER BY o.id;
```

```text
 order_id | name
----------+------
        1 | Asha
        2 | Asha
        3 | Ravi
(3 rows)
```

Asha appears twice because two orders match her. Meera appears zero times because no order does. The join did not lose her — it only ever produces rows where a match exists. Keeping her anyway is a different join, and the single most common bug in this topic lives exactly there (Level 2).

**Recap:** A table holds one kind of thing, one fact per row, one value per cell; every row has a number that never changes. Other tables write that number, and a join follows the number back — one output row per match, none for a row that matches nothing.

**Check yourself (C1):** Mia's orders notebook writes "Asha" instead of "customer 1". What goes wrong on the day Asha changes her name, and which idea is broken?

<details>
<summary>Answer</summary>

Every sale page that says "Asha" has to be found and rewritten; miss one and the notebook now contains two different customers. The name is a natural key that can change, which is why the key must be a number that never does (Idea 2). It is also Idea 1 breaking: "Asha's name" is one fact, and it was written on many rows.

</details>

## Level 2 — How It Actually Works

### The shop schema

The six tables created at the end of Why It Exists are the running example for the rest of the topic — the data model of the commerce app the plan builds from Week 9 (products → cart → checkout → orders). Drawn as boxes, with every arrow sitting on the "many" side:

```text
┌─ customers ──────────────────────┐  ┌─ products ───────────────────────┐
│ id           PK                  │  │ id     PK                        │
│ email        NOT NULL, UNIQUE    │  │ name   NOT NULL                  │
│ name         NOT NULL            │  │ price  numeric, CHECK >= 0       │
│ phone        UNIQUE, nullable    │  └──────────────────────────────────┘
│ referred_by  FK ▶ customers.id   │
└──────────────────────────────────┘  ┌─ categories ─────────────────────┐
                                      │ id     PK                        │
┌─ orders ─────────────────────────┐  │ name   NOT NULL, UNIQUE          │
│ id           PK                  │  └──────────────────────────────────┘
│ customer_id  FK ▶ customers.id   │
│ placed_at    timestamptz         │  ┌─ product_categories ─────────────┐
└──────────────────────────────────┘  │ product_id   PK, FK ▶ products   │
                                      │ category_id  PK, FK ▶ categories │
┌─ order_items ────────────────────┐  └──────────────────────────────────┘
│ order_id     PK, FK ▶ orders     │
│ product_id   PK, FK ▶ products   │  ▶ = points at: the arrow sits on the
│ quantity     CHECK > 0           │      "many" side; the row it points at
│ unit_price   price when bought   │      must exist.
└──────────────────────────────────┘
```

The seed rows as stored — every later output can be checked by hand against these three tables:


```sql
SELECT id, email, name, phone, referred_by FROM customers ORDER BY id;
```

```text
 id |       email       | name  |   phone    | referred_by
----+-------------------+-------+------------+-------------
  1 | asha@example.com  | Asha  | 9000000001 |        NULL
  2 | ravi@example.com  | Ravi  | NULL       |           1
  3 | meera@example.com | Meera | NULL       |           1
(3 rows)
```

```sql
SELECT id, customer_id, placed_at::date AS placed_on FROM orders ORDER BY id;
```

```text
 id | customer_id | placed_on
----+-------------+------------
  1 |           1 | 2026-10-01
  2 |           1 | 2026-10-05
  3 |           2 | 2026-10-07
(3 rows)
```

```sql
SELECT order_id, product_id, quantity, unit_price FROM order_items ORDER BY order_id, product_id;
```

```text
 order_id | product_id | quantity | unit_price
----------+------------+----------+------------
        1 |          1 |        2 |     499.00
        1 |          3 |        1 |    2499.00
        2 |          2 |        1 |    1999.00
        3 |          3 |        1 |    2499.00
(4 rows)
```

### Relationships are foreign keys

*(Idea 3)* `orders.customer_id` is a **foreign key**: a column that holds another table's primary key, declared with `REFERENCES customers (id)`. One customer, many orders — a **one-to-many** relationship — and the column always sits on the "many" side, because a customer row cannot hold a list of orders without breaking Idea 1. The declaration is not documentation. The database enforces it on every write:

```sql
INSERT INTO orders (customer_id) VALUES (42);
```

```text
ERROR:  insert or update on table "orders" violates foreign key constraint …
DETAIL:  Key (customer_id)=(42) is not present in table "customers".
```

An order for a customer who does not exist — an orphan — is rejected before it is stored. If the rule were not enforced, the first bug in the API would create orders that join to nothing, and no later query could tell them from real ones.

The same rule guards the other direction. Deleting a customer who still has orders:

```sql
DELETE FROM customers WHERE name = 'Ravi';
```

```text
ERROR:  update or delete on table "customers" violates foreign key constraint …
DETAIL:  Key (id)=(2) is still referenced from table "orders".
```

That is the default **referential action**, `NO ACTION`: refuse. You choose a different one per foreign key with `ON DELETE`. `order_items` cascades from `orders` — a line item has no meaning without its order:

```sql
BEGIN;
DELETE FROM orders WHERE id = 2;
SELECT order_id, product_id FROM order_items ORDER BY order_id, product_id;
ROLLBACK;
```

```text
 order_id | product_id
----------+------------
        1 |          1
        1 |          3
        3 |          3
(3 rows)
```

Order 2's line went with it. The choice is a design decision, made per relationship:

| The child row is…                                    | ON DELETE        | Example here                                   |
| ---------------------------------------------------- | ---------------- | ---------------------------------------------- |
| meaningless without the parent                       | `CASCADE`        | `order_items` → `orders`; `product_categories` |
| history or money — must never vanish silently        | `NO ACTION` / `RESTRICT` | `orders` → `customers`; `order_items` → `products` |
| valid on its own, the link was optional              | `SET NULL`       | `customers.referred_by` (if the referrer leaves) |

`RESTRICT` and `NO ACTION` both refuse; the difference is only that `NO ACTION` can be checked at the end of a transaction instead of immediately (PostgreSQL manual, section 5.5.5 — opened for this doc).

### Many-to-many needs a third table

*(Idea 4)* A product can be in several categories and a category holds many products: **many-to-many**. Neither side can hold the other's key — a `category_id` column on `products` allows one category, and a list in a cell is `sales_flat` again. The only shape that keeps Idea 1 is a third table whose rows are the pairs: a **junction table**. Its primary key is the pair itself, so the same pair cannot be stored twice:

```sql
INSERT INTO product_categories (product_id, category_id) VALUES (3, 2);
```

```text
ERROR:  duplicate key value violates unique constraint "product_categories_pkey"
DETAIL:  Key (product_id, category_id)=(3, 2) already exists.
```

Reading across it is a two-hop join — categories to the pairs, pairs to the products:

```sql
SELECT c.name AS category, p.name AS product
FROM categories c
JOIN product_categories pc ON pc.category_id = c.id
JOIN products p            ON p.id = pc.product_id
ORDER BY c.name, p.name;
```

```text
 category | product
----------+----------
 clothing | Cap
 clothing | Jeans
 clothing | T-shirt
 footwear | Sneakers
 sale     | Sneakers
(5 rows)
```

`order_items` is a junction table too — orders to products — and it shows the second thing a junction table does: it carries the facts that belong to the pair and to nothing else. "Two T-shirts" is not a fact about order 1 alone, nor about T-shirts alone; it is a fact about the pair, so `quantity` lives on the pair's row.

### The database refuses bad data

*(Idea 6)* A **constraint** is a rule the table enforces on every row, forever, no matter which code path wrote it. The schema above uses four kinds, and each one replaces a check that would otherwise have to be repeated in every endpoint — and would be forgotten in one of them.

`UNIQUE` keeps the natural key honest without making it the key:

```sql
INSERT INTO customers (email, name) VALUES ('asha@example.com', 'Asha K');
```

```text
ERROR:  duplicate key value violates unique constraint "customers_email_key"
DETAIL:  Key (email)=(asha@example.com) already exists.
```

`CHECK` states a business rule in the one place it cannot be bypassed:

```sql
INSERT INTO order_items (order_id, product_id, quantity, unit_price) VALUES (1, 4, 0, 299.00);
```

```text
ERROR:  new row for relation "order_items" violates check constraint …
DETAIL:  Failing row contains (1, 4, 0, 299.00).
```

`GENERATED ALWAYS AS IDENTITY` hands out the surrogate key and refuses a hand-picked one, so two code paths can never invent the same id:

```sql
INSERT INTO customers (id, email, name) VALUES (99, 'x@example.com', 'X');
```

```text
ERROR:  cannot insert a non-DEFAULT value into column "id"
DETAIL:  Column "id" is an identity column defined as GENERATED ALWAYS.
HINT:  Use OVERRIDING SYSTEM VALUE to override.
```

And the column type is a constraint too. Money is `numeric`, never `float`, because binary floating point cannot hold most decimal fractions exactly:

```sql
SELECT 0.1::float8 + 0.2::float8   AS as_float,
       0.1::numeric + 0.2::numeric AS as_numeric,
       (0.1::float8 + 0.2::float8) = 0.3 AS float_equal;
```

```text
      as_float       | as_numeric | float_equal
---------------------+------------+-------------
 0.30000000000000004 |        0.3 | f
(1 row)
```

The PostgreSQL manual says it directly: `numeric` "is especially recommended for storing monetary amounts and other quantities where exactness is required" (section 8.1.2 — opened for this doc). The same reasoning picks `timestamptz` for `placed_at`: the manual states a `timestamp with time zone` value "is stored internally as UTC" and is converted to the session's time zone on output, while plain `timestamp` "will silently ignore any time zone indication" in its input (section 8.5.1 — opened). An order placed at 18:30 in Bengaluru must still be 18:30 in Bengaluru when a server in another region reads it.

### Denormalize on purpose

*(Idea 7)* `order_items.unit_price` looks like the redundancy Level 1 just banned: the price is already on `products`. It is not. Watch what happens when the product's price changes after the order:

```sql
BEGIN;
UPDATE products SET price = 2999.00 WHERE name = 'Sneakers';

SELECT o.id AS order_id, p.name, oi.unit_price AS paid, p.price AS price_now
FROM order_items oi
JOIN orders o   ON o.id = oi.order_id
JOIN products p ON p.id = oi.product_id
WHERE p.name = 'Sneakers'
ORDER BY o.id;
ROLLBACK;
```

```text
 order_id |   name   |  paid   | price_now
----------+----------+---------+-----------
        1 | Sneakers | 2499.00 |   2999.00
        3 | Sneakers | 2499.00 |   2999.00
(2 rows)
```

"What Asha paid" and "what Sneakers cost now" are two different facts that are allowed to differ — and must, or every old invoice would change when the catalogue does. A copy kept so that it can diverge is a **snapshot**, and it is the one form of **denormalization** that needs no apology. The test is a single question: *if the two values ever disagree, is one of them wrong?* If yes, you have stored one fact twice — store it once. If no, they are two facts, and each gets its own column.

Order totals are the opposite case. They can always be recomputed from the lines, so the schema does not store them:

```sql
SELECT o.id AS order_id, c.name, SUM(oi.quantity * oi.unit_price) AS total
FROM orders o
JOIN customers c    ON c.id = o.customer_id
JOIN order_items oi ON oi.order_id = o.id
GROUP BY o.id, c.name
ORDER BY o.id;
```

```text
 order_id | name |  total
----------+------+---------
        1 | Asha | 3497.00
        2 | Asha | 1999.00
        3 | Ravi | 2499.00
(3 rows)
```

That query is the first **aggregate** in the doc: `SUM` folds many line rows into one number per group, and `GROUP BY` says what a group is — here, one order. Every column in the `SELECT` is either grouped or aggregated; that rule is what makes "one row per order" guaranteed.

### Joins, step by step

The **join condition** is the `ON` clause. An **inner join** (`JOIN` on its own means `INNER JOIN`) keeps a pair only when the condition is true. A **left join** (`LEFT JOIN`, short for `LEFT OUTER JOIN`) does the inner join first, then adds back every left-side row that found no partner, with `NULL` in all the right-side columns. The trace is the same for both until the last step:

```sql
SELECT c.name, o.id AS order_id
FROM customers c
LEFT JOIN orders o ON o.customer_id = c.id
ORDER BY c.name, o.id;
```

```text
 name  | order_id
-------+----------
 Asha  |        1
 Asha  |        2
 Meera |     NULL
 Ravi  |        3
(4 rows)
```

```text
rows out: 4 with LEFT JOIN · an inner join stops after step 3 and emits 3

step  left row  matching orders   emits           why
1     1 Asha    order 1, order 2  Asha|1, Asha|2  two matches → two rows
2     2 Ravi    order 3           Ravi|3          one match → one row
3     3 Meera   none              -               inner join: nothing to emit
4     3 Meera   none              Meera|NULL      LEFT keeps the orphan; right side NULL
```

*(Idea 9)* Step 4 is where the bug lives. The `NULL`s it writes are not values; they are the absence of a match. Now filter the result by an orders column — "customers and their orders from the 5th onwards":

```sql
-- WRONG — the WHERE on an orders column throws away every customer without a match
SELECT c.name, o.id AS order_id
FROM customers c
LEFT JOIN orders o ON o.customer_id = c.id
WHERE o.placed_at >= '2026-10-05'
ORDER BY c.name, o.id;
```

```text
 name | order_id
------+----------
 Asha |        2
 Ravi |        3
(2 rows)
```

Meera is gone, and so is the reason you wrote `LEFT`. `WHERE` runs after the join is complete; Meera's row has `placed_at = NULL`, `NULL >= '2026-10-05'` is not true, and `WHERE` keeps only rows where the condition is true. The left join quietly became an inner one. The fix is to say what you meant: the date is part of the *matching rule*, so it belongs in `ON`:

```sql
SELECT c.name, o.id AS order_id
FROM customers c
LEFT JOIN orders o ON o.customer_id = c.id AND o.placed_at >= '2026-10-05'
ORDER BY c.name, o.id;
```

```text
 name  | order_id
-------+----------
 Asha  |        2
 Meera |     NULL
 Ravi  |        3
(3 rows)
```

Asha's order 1 no longer matches, so it is simply not emitted; Meera still matches nothing and is still kept. The rule: *a condition in `ON` decides what counts as a match; a condition in `WHERE` decides which finished rows survive.* For an inner join the two are interchangeable. For an outer join they never are.

The same `NULL` decides how you count. `COUNT(*)` counts rows; `COUNT(column)` counts rows where that column is not `NULL`:

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

Meera's one row is the `NULL`-padded row from step 4. "Orders per customer" is `COUNT(o.id)`; `COUNT(*)` gives her a phantom order.

*(Idea 10)* The last mechanic is the one that produces numbers that are wrong but look plausible. Joins multiply: one left row with two matches becomes two rows (step 1 above). Join a product to *two* different one-to-many children — its order lines and its categories — and every line is repeated once per category:

```sql
SELECT p.name, c.name AS category, oi.order_id, oi.quantity
FROM products p
JOIN order_items oi        ON oi.product_id = p.id
JOIN product_categories pc ON pc.product_id = p.id
JOIN categories c          ON c.id = pc.category_id
WHERE p.name = 'Sneakers'
ORDER BY c.name, oi.order_id;
```

```text
   name   | category | order_id | quantity
----------+----------+----------+----------
 Sneakers | footwear |        1 |        1
 Sneakers | footwear |        3 |        1
 Sneakers | sale     |        1 |        1
 Sneakers | sale     |        3 |        1
(4 rows)
```

Two order lines × two categories = four rows. This is called **fan-out**, and nothing about it is an error — each row is a true combination. The damage appears the moment you aggregate over it:

```sql
-- WRONG — two one-to-many joins on products multiply each other
SELECT p.name, string_agg(c.name, ', ' ORDER BY c.name) AS categories, SUM(oi.quantity) AS sold
FROM products p
JOIN order_items oi        ON oi.product_id = p.id
JOIN product_categories pc ON pc.product_id = p.id
JOIN categories c          ON c.id = pc.category_id
GROUP BY p.id, p.name
ORDER BY p.name;
```

```text
   name   |           categories           | sold
----------+--------------------------------+------
 Jeans    | clothing                       |    1
 Sneakers | footwear, footwear, sale, sale |    4
 T-shirt  | clothing                       |    2
(3 rows)
```

Sneakers sold 2, not 4, and Cap is missing entirely because inner joins dropped a product with no sales. The fix is a rule you can apply without thinking: *bring each one-to-many side down to one row per parent before joining it in.* Aggregate the lines per product, aggregate the categories per product, then join the two one-row-per-product results — with `LEFT JOIN` so a product with nothing on one side survives:

```sql
SELECT p.name, cats.names AS categories, COALESCE(sales.sold, 0) AS sold
FROM products p
LEFT JOIN (
  SELECT product_id, SUM(quantity) AS sold
  FROM order_items
  GROUP BY product_id
) sales ON sales.product_id = p.id
LEFT JOIN (
  SELECT pc.product_id, string_agg(c.name, ', ' ORDER BY c.name) AS names
  FROM product_categories pc
  JOIN categories c ON c.id = pc.category_id
  GROUP BY pc.product_id
) cats ON cats.product_id = p.id
ORDER BY p.name;
```

```text
   name   |   categories   | sold
----------+----------------+------
 Cap      | clothing       |    0
 Jeans    | clothing       |    1
 Sneakers | footwear, sale |    2
 T-shirt  | clothing       |    2
(4 rows)
```

`COALESCE(sales.sold, 0)` turns the `NULL` of "no sales row matched" into the `0` the API should return. This is the query the Week 9 products endpoint needs, and it is the shape of every "list parents with a summary of each child set" query you will write.

**Recap:** A foreign key sits on the many side and the database enforces it both ways; a many-to-many is a table of pairs; constraints and types are rules the schema enforces for every code path; a copy that is allowed to differ is a snapshot, not a mistake. `ON` decides matches, `WHERE` decides survivors — and on an outer join that difference is everything. Two one-to-many joins multiply; aggregate each side to one row per parent first.

**Check yourself (C2):** You add `reviews` (one product, many reviews) and join `products → order_items → reviews` to show `SUM(quantity)` and `AVG(rating)` per product. The averages come out right but the sales are too high. Why does only one of them break?

<details>
<summary>Answer</summary>

Fan-out (Idea 10): every order line is repeated once per review, and every review once per order line. `SUM` adds the repeated quantities, so it is inflated by the number of reviews. `AVG` is a ratio: each rating is repeated the same number of times (once per order line), so the repeats cancel and the mean is unchanged. That is what makes fan-out dangerous — some aggregates survive it by accident, so the report looks half right. Aggregate reviews per product and lines per product separately, then join.

</details>

## The Analogy, Mapped

| In the story                                          | In reality                                                       | Idea # |
| ----------------------------------------------------- | ---------------------------------------------------------------- | ------ |
| One big notebook with everything on every page        | `sales_flat` — the un-normalized table                           | 1, 5   |
| One notebook per kind of thing                        | a table                                                          | 1      |
| One line on a page                                    | a row                                                            | 1      |
| The number each friend and each drink gets            | the primary key, a surrogate                                     | 2      |
| A sale page that says "friend 2", not a name          | a foreign key on the many side                                   | 3      |
| Flipping to friend number 2                           | a join                                                           | 8      |
| A friend with no sale pages                           | an unmatched row — a left join keeps it, an inner join drops it  | 9      |
| Writing today's price on the sale page                | the snapshot — deliberate denormalization                        | 7      |
| Fixing a moved friend's address on every page         | the update anomaly                                               | 5      |

**Where the analogy breaks:**

- A notebook does not refuse a wrong number. Mia can write "friend 42" with no such friend; the database cannot be made to (Ideas 3, 6).
- Mia flips one page at a time. The database matches whole tables at once and chooses its own order of work — how it does that fast is Thursday's topic, indexes.
- A page can be torn out quietly. A row that other rows point at cannot be deleted without saying what happens to them (Idea 3).
- A drink that is on two menus has no natural notebook. The story needs a fourth notebook that only lists pairs, and that is less obvious on paper than it is in a schema (Idea 4).
- One Mia writes. Many API requests write at the same time, and what keeps them from corrupting each other is the Week 15 topic, transactions.

## Level 3 — The Formal Picture

### Functional dependencies and the normal forms

*(Idea 5)* The precise tool behind "one row, one fact" is the **functional dependency**: `X → Y` means that any two rows with the same `X` always have the same `Y` — `X` determines `Y`. A **candidate key** is a smallest set of columns that determines the whole row; the primary key is the candidate key you chose. The **normal forms** are tests on which dependencies a table is allowed to contain, and each one names the anomaly it prevents:

| Form | The rule                                                                 | Broken in the flat design by                          | The anomaly it causes                               |
| ---- | ------------------------------------------------------------------------ | ----------------------------------------------------- | --------------------------------------------------- |
| 1NF  | every cell holds one atomic value; no repeating groups; rows have a key  | `items = 'T-shirt x2, Sneakers x1'`                   | "how many Sneakers?" is unanswerable (insert/query) |
| 2NF  | 1NF, and every non-key column depends on the **whole** key               | `product_name` on a line keyed by `(order_id, product_id)` | rename a product → fix every line (update)     |
| 3NF  | 2NF, and no non-key column depends on another non-key column             | `customer_email → customer_name` on an order row      | two emails for Asha; Ravi vanishes (update/delete)  |

The mnemonic is the one every interviewer knows: *every non-key column depends on the key, the whole key, and nothing but the key.* Drawn on the flat table:

```text
sales_flat (order_id, customer_name, customer_email, items, total)

order_id ──▶ customer_email ──▶ customer_name     transitive: the name depends on the
    │                                              email, not on the order     ✗ 3NF
    ├──────▶ items = "T-shirt x2, Sneakers x1"     a list in one cell          ✗ 1NF
    └──────▶ total                                 derived from the items      (store? no)

order_lines_bad (order_id, product_id, product_name, quantity)

(order_id, product_id) ──▶ quantity                depends on the whole key    ✓
             product_id ──▶ product_name           depends on PART of the key  ✗ 2NF
```

Normalizing is applying the three tests in order; each step moves one dependency into the table whose key is its left-hand side:

```text
step  do                                         new tables                     removes
1     one line per row, not a list per cell      order_lines(order_id, product, 1NF break
                                                 qty, price)
2     product facts depend on the product alone  products(id, name, price)      2NF break
      → move out, keep product_id on the line    order_items(order_id,
                                                 product_id, quantity,
                                                 unit_price)
3     customer facts depend on the customer      customers(id, email, name)     3NF break
      alone → move out, keep customer_id         orders(id, customer_id,
      on the order                               placed_at)
```

`unit_price` survives step 2 on purpose: it depends on the whole key `(order_id, product_id)` — *this* line's price — not on `product_id` alone. That is the formal statement of the snapshot rule from Level 2. Higher forms (BCNF and beyond) exist; at the target depth — the Week 19 DBMS day says "normalization 1NF–3NF" — 3NF is where you stop, and you say why: it removes every anomaly that single-column dependencies can cause.

### Join algebra

The PostgreSQL manual defines each join in terms of the row pairs it emits (section 7.2.1.1 — opened for this doc): `CROSS JOIN` yields "every possible combination of rows" — `N × M`; `INNER JOIN` keeps, for each left row, "a row for each row in T2 that satisfies the join condition"; `LEFT OUTER JOIN` does the inner join and then adds "for each row in T1 that does not satisfy the join condition with any row in T2, a joined row … with null values in columns of T2"; `RIGHT` is the mirror; `FULL` does both. On the seed data:

```sql
SELECT
  (SELECT COUNT(*) FROM customers CROSS JOIN orders)                              AS cross_join,
  (SELECT COUNT(*) FROM customers c JOIN orders o       ON o.customer_id = c.id)  AS inner_join,
  (SELECT COUNT(*) FROM customers c LEFT JOIN orders o  ON o.customer_id = c.id)  AS left_join,
  (SELECT COUNT(*) FROM customers c RIGHT JOIN orders o ON o.customer_id = c.id)  AS right_join,
  (SELECT COUNT(*) FROM customers c FULL JOIN orders o  ON o.customer_id = c.id)  AS full_join;
```

```text
 cross_join | inner_join | left_join | right_join | full_join
------------+------------+-----------+------------+-----------
          9 |          3 |         4 |          3 |         4
(1 row)
```

| Join      | Rows out                                         | Here           | Bound                     |
| --------- | ------------------------------------------------ | -------------- | ------------------------- |
| `CROSS`   | every pair                                       | `3 × 3 = 9`    | exactly `N × M`           |
| `INNER`   | pairs where `ON` is true                         | 3              | `0 … N × M`               |
| `LEFT`    | inner + each unmatched left row, right side NULL | `3 + 1 = 4`    | at least `N`              |
| `RIGHT`   | inner + each unmatched right row                 | `3 + 0 = 3`    | at least `M`              |
| `FULL`    | inner + unmatched from both sides                | `3 + 1 + 0 = 4`| at least `max(N, M)`      |

The `ON`-versus-`WHERE` rule from Level 2 is stated by the manual in one sentence: "a restriction placed in the `ON` clause is processed *before* the join, while a restriction placed in the `WHERE` clause is processed *after* the join. That does not matter with inner joins, but it matters a lot with outer joins." Fan-out follows from the definition of `INNER`: *for each* left row, *a row for each* match — so two independent child sets of sizes `a` and `b` produce `a × b` rows per parent.

### NULL is not a value

*(Idea 12)* `NULL` means *unknown*, and SQL comparisons use **three-valued logic**: true, false, or unknown. Anything compared with `NULL` using `=`, `<>`, `<` or `>` is unknown — including `NULL` itself:

```sql
SELECT NULL = NULL              AS eq,
       NULL <> NULL             AS neq,
       NULL IS NULL             AS is_null,
       NULL IS DISTINCT FROM NULL AS distinct_from;
```

```text
  eq  | neq  | is_null | distinct_from
------+------+---------+---------------
 NULL | NULL | t       | f
(1 row)
```

Only `IS NULL` and `IS DISTINCT FROM` give a definite answer. Everything else in SQL inherits this. `IN` is a chain of `=` joined by `OR`, so one `NULL` in the list poisons a non-match:

```sql
SELECT 2 IN (1, NULL) AS in_list, 2 NOT IN (1, NULL) AS not_in_list;
```

```text
 in_list | not_in_list
---------+-------------
 NULL    | NULL
(1 row)
```

`2 IN (1, NULL)` is `2 = 1 OR 2 = NULL` = `false OR unknown` = unknown; `NOT unknown` is still unknown. The manual states the consequence for `NOT IN` exactly: when "at least one right-hand expression yields null, the result of the `NOT IN` construct will be null, not true as one might naively expect" (section 9.25.2 — opened). And `WHERE` keeps only rows whose condition is *true*, so unknown rows vanish without a sound:

```sql
SELECT name, phone FROM customers WHERE phone <> '9000000001';
```

```text
 name | phone
------+-------
(0 rows)
```

"Everyone except Asha" returned nobody: Asha fails on `=`, Ravi and Meera fail on unknown. The null-safe form asks the question you meant:

```sql
SELECT name, phone FROM customers WHERE phone IS DISTINCT FROM '9000000001' ORDER BY id;
```

```text
 name  | phone
-------+-------
 Ravi  | NULL
 Meera | NULL
(2 rows)
```

The same logic is why `UNIQUE` lets many rows have no phone: the manual says "two null values are not considered equal in this comparison", so a unique constraint cannot see them as duplicates (section 5.5.3 — opened; the opt-in `UNIQUE NULLS NOT DISTINCT` arrived in PostgreSQL 15, per its release notes — opened):

```sql
BEGIN;
INSERT INTO customers (email, name, phone) VALUES ('dev@example.com', 'Dev', NULL);
SELECT COUNT(*) AS customers_without_phone FROM customers WHERE phone IS NULL;
ROLLBACK;
```

```text
 customers_without_phone
-------------------------
                       3
(1 row)
```

A third missing phone is accepted; a second *actual* phone is not:

```sql
INSERT INTO customers (email, name, phone) VALUES ('dev@example.com', 'Dev', '9000000001');
```

```text
ERROR:  duplicate key value violates unique constraint "customers_phone_key"
DETAIL:  Key (phone)=(9000000001) already exists.
```

Four consequences to carry: a `NULL` foreign key never matches anything in a join; `COUNT(col)` skips `NULL` (Level 2); `WHERE col <> x` drops the `NULL` rows along with `x`; `UNIQUE` allows any number of `NULL`s. Primary key columns cannot be `NULL` at all — the manual: a primary key "requires that the values be both unique and not null" (section 5.5.4 — opened).

### The anti-join

*(Idea 11)* "Rows with no match" — customers who never ordered, products nobody bought — is the **anti-join**, and it is the question most often asked of a schema after "join these". Two forms are always correct:

```sql
SELECT c.name
FROM customers c
WHERE NOT EXISTS (SELECT 1 FROM orders o WHERE o.customer_id = c.id);
```

```text
 name
-------
 Meera
(1 row)
```

```sql
SELECT c.name
FROM customers c
LEFT JOIN orders o ON o.customer_id = c.id
WHERE o.id IS NULL;
```

```text
 name
-------
 Meera
(1 row)
```

The second one is Level 2's step 4 used on purpose: keep the unmatched rows, then keep *only* them. The third form everyone reaches for first is correct only by luck. "Customers who have never referred anyone" — `referred_by` is nullable, and Asha's is `NULL`:

```sql
-- WRONG — one NULL in the subquery and NOT IN matches nobody
SELECT name
FROM customers
WHERE id NOT IN (SELECT referred_by FROM customers);
```

```text
 name
------
(0 rows)
```

Ravi and Meera referred nobody, and the query says there is no such person. Every `id NOT IN (…, NULL, …)` is unknown (Idea 12), so `WHERE` drops every row. `NOT EXISTS` has no such failure mode, because it asks "is there a matching row?" and a `NULL` simply never matches:

```sql
SELECT c.name
FROM customers c
WHERE NOT EXISTS (SELECT 1 FROM customers r WHERE r.referred_by = c.id)
ORDER BY c.id;
```

```text
 name
-------
 Ravi
 Meera
(2 rows)
```

| You want "rows with no match in B"                  | Use                                  | Why                                              |
| --------------------------------------------------- | ------------------------------------ | ------------------------------------------------ |
| the default                                         | `NOT EXISTS (SELECT 1 FROM b WHERE …)` | null-safe; reads as the question; stays correct if a column becomes nullable |
| you already have the `LEFT JOIN` for other columns  | `LEFT JOIN b … WHERE b.id IS NULL`   | one scan serves both purposes                    |
| the subquery column is `NOT NULL` and you know it   | `NOT IN (SELECT …)` — or better, still `NOT EXISTS` | a later `ALTER TABLE … DROP NOT NULL` turns it into a silent 0-row bug |

**Recap:** Normal forms are tests on functional dependencies — key, whole key, nothing but the key — and each one names the anomaly it prevents. A join's row count follows from its definition: cross is `N × M`, inner is the true pairs, outer adds the orphans back. `NULL` is unknown, not a value: it fails every comparison, slips through `UNIQUE`, is skipped by `COUNT(col)`, and turns `NOT IN` into a query that returns nothing. The anti-join is `NOT EXISTS`.

**Check yourself (C3):** Write "products in no category" in three ways, and say which one you would ship.

<details>
<summary>Answer</summary>

`NOT EXISTS (SELECT 1 FROM product_categories pc WHERE pc.product_id = p.id)`; `LEFT JOIN product_categories pc ON pc.product_id = p.id WHERE pc.product_id IS NULL`; `p.id NOT IN (SELECT product_id FROM product_categories)`. All three return the same rows today because `product_categories.product_id` is `NOT NULL`. Ship `NOT EXISTS`: it states the question, and it stays correct if that column is ever made nullable, where `NOT IN` would start returning zero rows with no error (Ideas 11, 12).

</details>

## Variants & Environment Differences

| Variant or environment                  | What changes                                                                                                                                      | Since / where                                                          |
| --------------------------------------- | ------------------------------------------------------------------------------------------------------------------------------------------------- | ---------------------------------------------------------------------- |
| `serial` vs identity column             | `id serial PRIMARY KEY` is the older spelling; identity columns are the SQL-standard form, and `GENERATED ALWAYS` rejects a hand-written id (shown in Level 2) where `BY DEFAULT` accepts one | identity columns: PostgreSQL 10 (release notes — opened)               |
| `text` vs `varchar(n)` vs `char(n)`     | "There is no performance difference among these three types" in PostgreSQL; `char(n)` is usually the slowest. Use `text`, and a `CHECK` if a length is a business rule | PostgreSQL manual, section 8.3 (opened); MySQL and others differ here  |
| `timestamptz` vs `timestamp`            | `timestamptz` converts input to UTC and output to the session zone; `timestamp` silently drops the offset you pass it                             | PostgreSQL manual, section 8.5.1 (opened)                              |
| `ON DELETE` default                     | `NO ACTION` — refuse. Nothing cascades unless you say so                                                                                          | PostgreSQL manual, section 5.5.5 (opened)                              |
| `UNIQUE` and `NULL`                     | many `NULL`s allowed by default; `UNIQUE NULLS NOT DISTINCT` makes them collide                                                                  | PostgreSQL 15 (release notes — opened)                                 |
| MySQL                                   | the `JOIN` grammar on MySQL's reference page has no `FULL JOIN` — emulate it with `LEFT JOIN … UNION … RIGHT JOIN`; auto ids are `AUTO_INCREMENT`; `<=>` plays the role of `IS NOT DISTINCT FROM` | MySQL 8.4 reference, JOIN page (opened); the `<=>` operator `(recalled — verify)` |
| SQLite                                  | foreign keys are "disabled by default" and must be switched on per connection with `PRAGMA foreign_keys = ON` — the orphan insert in Level 2 silently succeeds otherwise | sqlite.org, "SQLite Foreign Key Support", section 2 (opened)           |

The only line that changes between the `serial` and identity spellings, as a fragment of the `customers` block that was run whole:

```sql
  id          bigint GENERATED ALWAYS AS IDENTITY PRIMARY KEY,   -- this doc
  id          bigserial PRIMARY KEY,                             -- older spelling, same effect
```

## Compared With What You Know

You have already written a join — in JavaScript. The Two Sum trigger in your log ("store elements in a hash map to find complements in one pass", [notes.md](../../../prep/notes.md) row 5) is exactly how a hash join works: build a `Map` from one array keyed by `id`, loop the other array, `get()` the match. An inner join is that loop with `continue` when `get()` returns `undefined`; a left join is the same loop with `?? null` instead. What the database adds is that it enforces the keys you are matching on, picks the matching strategy itself, and does it in a set at a time instead of a loop you maintain.

| Aspect                     | Index (Thursday's topic)                                         | Key (this topic)                                                   |
| -------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------ |
| What it is about           | speed — how fast a row can be found                              | identity and truth — which rows may exist                          |
| What it changes            | query time, never the result                                     | which inserts and deletes are allowed                              |
| Dropping it                | queries get slower, answers stay the same                        | duplicates and orphans become possible                             |
| Relationship               | a `PRIMARY KEY` or `UNIQUE` *creates* an index to enforce itself | a foreign key column gets no index unless you add one              |

*Tell them apart:* if removing it could change an answer, it is a key; if it could only change the time, it is an index.

| Aspect                     | Document embedding (NoSQL)                                       | Normalized tables                                                  |
| -------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------ |
| Where a child lives        | inside its parent — the order holds its lines and a copy of the customer | in its own table, pointing at the parent by key              |
| Reading one order          | one lookup, no join                                              | two or three joins                                                 |
| Changing a customer's email | every embedded copy, or accept that old orders show the old one | one row                                                            |
| When it is the right call  | the copy is a snapshot anyway, or the data is read far more than changed | the fact is shared and must stay consistent — the default    |

*Tell them apart:* if two copies of a value are allowed to disagree, embedding is harmless; if they must agree, you want one row and a join.

| Aspect                     | `UNION`                                                          | `JOIN`                                                             |
| -------------------------- | ---------------------------------------------------------------- | ------------------------------------------------------------------ |
| Direction                  | stacks rows — the result is *taller*                             | pairs rows — the result is *wider*                                 |
| Shape requirement          | both sides have the same columns                                 | sides can be anything; a condition relates them                    |
| Output size                | at most `N + M` rows (`UNION` also removes duplicates)           | `0 … N × M` rows                                                   |

```sql
SELECT name FROM customers
UNION
SELECT name FROM products
ORDER BY name;
```

```text
   name
----------
 Asha
 Cap
 Jeans
 Meera
 Ravi
 Sneakers
 T-shirt
(7 rows)
```

*Tell them apart:* "combine two lists of the same kind of thing" is `UNION`; "attach facts about one thing to facts about a related thing" is a `JOIN`.

## Where It's Used

1. **The plan's commerce app** (Weeks 9–15) — the products endpoint is the aggregate-first query from Level 2; the cart is a junction table (`carts` ↔ `products`, Week 12); checkout writes `order_items` with the snapshot price (Week 15). Why not one table: the first price change would rewrite history. *Illustrative.*
2. **Shopify's Order resource** — an order carries its own `line_items`, each with a `price`, and the line's `product_id` "can be `null` if the original product associated with the order is deleted at a later date" (Shopify Admin REST API reference, Order — opened). The line survives the product: the shape of Idea 7. That the stored `price` is frozen at order time `(recalled — verify)` — the page describes the field without saying when it is captured. *Documented* for the structure, as stated.
3. **Auth tables** (Week 4) — `users` one-to-many `refresh_tokens`, with `ON DELETE CASCADE` so deleting an account revokes every session, and `token_hash UNIQUE`. Why not a `tokens` array on the user row: Idea 1, and you could not revoke one token by key. *Illustrative;* it is Practice B2.
4. **Reports** — "revenue per category" is a two-hop join that fans out by design (a product in two categories counts in both), and "customers inactive this month" is an anti-join. Why `NOT EXISTS` and not `NOT IN`: the first nullable column in the subquery would empty the report (Idea 11). *Illustrative.*
5. **Your own code** — there is no SQL in the repo yet, so there is no example to cite honestly. The first one will be the Month 1 exit check, "Express + Postgres API with working JWT auth".

## Misconceptions & Traps

No SQL exists in the learner's files yet, so there are no findings from your own code; every entry below is a general trap, ordered by how often it ships.

**The belief** — "`LEFT JOIN` keeps every left row, whatever I put in `WHERE`."
*Why it seems right* — that is what `LEFT` is for, and with no `WHERE` it is exactly what happens.
*Why it is wrong* — the `WRONG` block under Level 2 → Joins, step by step: adding `WHERE o.placed_at >= '2026-10-05'` returned 2 rows instead of 3; Meera's `NULL` row failed the test like any other row.
*The correct model* — `WHERE` runs after the join on the finished rows, and keeps only the ones whose condition is *true*. A condition on the right-hand table belongs in `ON` if it defines a match, or in `WHERE` as `(cond OR right.id IS NULL)` if you really do want to filter afterwards.

**The belief** — "Joining one more table only adds columns."
*Why it seems right* — in the one-to-one case it does, and in the many-to-one case (orders → customers) the row count never changes.
*Why it is wrong* — the Sneakers block in Level 2: two order lines joined with two categories gave 4 rows, and `SUM(quantity)` reported 4 sold instead of 2.
*The correct model* — a join emits one row per matching *pair*. A one-to-many join multiplies the parent by its match count, and two one-to-many joins multiply each other. Before any `SUM` or `COUNT`, bring each child set down to one row per parent.

**The belief** — "`NOT IN (subquery)` and `NOT EXISTS` are the same thing."
*Why it seems right* — on a `NOT NULL` column they return identical rows, and `NOT IN` reads more naturally.
*Why it is wrong* — the referrals block in Level 3: `id NOT IN (SELECT referred_by FROM customers)` returned 0 rows, because one `referred_by` is `NULL`.
*The correct model* — `x NOT IN (a, b, NULL)` is `x <> a AND x <> b AND x <> NULL`; the last term is unknown, so the whole thing is unknown, and `WHERE` drops it. `NOT EXISTS` asks whether a matching row exists, and `NULL` never matches, so it is unaffected.

**The belief** — "`WHERE phone <> '9000000001'` means everyone except that number."
*Why it seems right* — in JavaScript, `null !== "9000000001"` is `true`, so the `NULL` rows would stay.
*Why it is wrong* —

```sql
SELECT name, phone FROM customers WHERE phone <> '9000000001';
```

```text
 name | phone
------+-------
(0 rows)
```

*The correct model* — SQL comparisons with `NULL` are unknown, not false, and `WHERE` keeps only true. Ask the null-safe question: `phone IS DISTINCT FROM '9000000001'`, or spell out `phone <> '…' OR phone IS NULL`.

**The belief** — "`float` is fine for prices; the rounding is tiny."
*Why it seems right* — JavaScript numbers are doubles and most carts seem to work.
*Why it is wrong* — the `0.1 + 0.2` block in Level 2: `0.30000000000000004`, and `= 0.3` is `f`. Sum a thousand such lines and the invoice total does not match the sum of the receipts.
*The correct model* — `numeric(10, 2)` stores decimal digits exactly and is the manual's own recommendation for money. Use `float` for measurements, never for amounts that will be added, compared or reconciled.

**The belief** — "Copying the customer's email onto `orders` is a harmless shortcut — the order emails need it anyway."
*Why it seems right* — it saves a join on the hot path, and it looks just like the `unit_price` snapshot.
*Why it is wrong* —

```sql
BEGIN;
ALTER TABLE orders ADD COLUMN customer_email text;
UPDATE orders o SET customer_email = c.email FROM customers c WHERE c.id = o.customer_id;

UPDATE customers SET email = 'asha.k@example.com' WHERE name = 'Asha';

SELECT o.id AS order_id, o.customer_email AS on_order, c.email AS on_customer
FROM orders o
JOIN customers c ON c.id = o.customer_id
WHERE c.name = 'Asha'
ORDER BY o.id;
ROLLBACK;
```

```text
 order_id |     on_order     |    on_customer
----------+------------------+--------------------
        1 | asha@example.com | asha.k@example.com
        2 | asha@example.com | asha.k@example.com
(2 rows)
```

*The correct model* — apply the snapshot test: if the two values disagree, is one of them wrong? For the price, no — the order *should* show what was paid. For the email, yes — the shipping notification for order 1 must go to Asha's current address. Different answers, so one is a snapshot and the other is the update anomaly from Why It Exists, rebuilt by hand (Ideas 5, 7).

**The belief** — "`UNIQUE` means no two rows can have the same value — and `NULL` is a value."
*Why it seems right* — a `UNIQUE` index refuses the second `'9000000001'`, so surely it refuses the second missing phone too.
*Why it is wrong* — the two phone blocks in Level 3: a third `NULL` phone was accepted; a second real number was rejected.
*The correct model* — uniqueness is tested with `=`, and `NULL = NULL` is unknown, so two `NULL`s are never duplicates. That is usually what you want for an optional field. When it is not, PostgreSQL 15's `UNIQUE NULLS NOT DISTINCT` says so explicitly.

**The belief** — "`ON DELETE CASCADE` everywhere — it keeps the database clean."
*Why it seems right* — orphans are bad (Idea 3), and cascade guarantees there are none.
*Why it is wrong* —

```sql
BEGIN;
ALTER TABLE orders
  DROP CONSTRAINT orders_customer_id_fkey,
  ADD  FOREIGN KEY (customer_id) REFERENCES customers (id) ON DELETE CASCADE;

DELETE FROM customers WHERE name = 'Ravi';

SELECT (SELECT COUNT(*) FROM orders)      AS orders_left,
       (SELECT COUNT(*) FROM order_items) AS items_left;
ROLLBACK;
```

```text
 orders_left | items_left
-------------+------------
           2 |          3
(1 row)
```

*The correct model* — one `DELETE` on a customer removed an order and its lines: paid-for history, gone without an error. The default `NO ACTION` would have refused (Level 2). Cascade only where the child is meaningless without the parent — `order_items`, `product_categories`, refresh tokens. Where the child is money or history, refuse, and make "delete a customer" a soft delete or an explicit workflow instead.

## Limits & Trade-offs

Normalization's cost is paid on every read: each fact you stored once must be joined back in. The product list for the Week 9 endpoint needs products, their categories and their sales — three tables — and there are two ways to pay:

| Approach                                                      | Round trips for 20 products | What it costs                                              |
| ------------------------------------------------------------- | --------------------------- | ---------------------------------------------------------- |
| one aggregate-first query (Level 2)                           | 1                           | a query with two subqueries; the planner needs indexes on `order_items.product_id` and `product_categories.product_id` to stay fast — Thursday |
| fetch products, then loop and fetch each one's categories in app code | `1 + 20 = 21`         | the N+1 pattern: a round trip per row, growing with page size |

Both return the same rows; only the second one gets slower per product.

When something else is the better choice, with the condition for each:

- **A maintained copy (cache) wins** when the same join result is read far more often than its inputs change, and a stale answer for a bounded time is acceptable — the Week 10 Redis cache-aside on the products endpoint is exactly this, with "invalidate on write" as the price. A snapshot needs no invalidation; a cache always does.
- **A flexible document column wins** when attributes genuinely differ per kind of product (shoe size, sleeve length, screen size) and are never joined on: Postgres stores such a bag as `jsonb` in a normal row. That is a Week 16 HLD conversation, not this one.
- **A document store wins** at the HLD level when the data is naturally one tree read whole and written whole, with no shared facts — the "SQL vs NoSQL with the reason" step of the backend skeleton in [how-to-study.md](../../../prep/how-to-study.md).
- **A derived column wins** when recomputing is measurably too slow and the inputs are append-only — an order's `total` after the order is final, for instance. Store it then, and only then.

Verdict: for a transactional app at this scale, normalize to 3NF by default, snapshot the values that are legally allowed to diverge, and add a cache or a derived column only when a measured read path demands it. The schema is the cheapest place to prevent a wrong answer and the most expensive place to repair one.

## Interview Notes

The question opens one of three ways at the target companies: "design the tables for X" (a cart, an order flow, a feed), "what is normalization — give an example", or a query to write on the spot ("customers with no orders this month", "top products per category"). The schema version is the Week 19 DBMS day's own phrasing: "explain each topic against your own project's schema". Open with the entities as nouns from the prompt, give each a surrogate key, place every foreign key on the many side, and name the one many-to-many and its junction table before you are asked.

Follow-ups, each with the idea the reply stands on:

- "Why not one table?" — the three anomalies, with the email example (Idea 5).
- "Where would you denormalize?" — the price snapshot, and the one-question test for whether a copy is allowed to differ (Idea 7).
- "What happens when a customer is deleted?" — `NO ACTION` by default; cascade only for children that are meaningless alone; soft delete for history (Idea 3).
- "Write customers who never ordered" — `NOT EXISTS`; then "and why not `NOT IN`?" — the `NULL` trap (Ideas 11, 12).
- "This report is double counting" — fan-out; aggregate each child set first (Idea 10).
- "`LEFT JOIN` but some rows are missing" — the condition is in `WHERE`; move it to `ON` (Idea 9).
- "How would you make this query fast?" — the hand-off to indexes; say "an index on the foreign key column" and that you would look at `EXPLAIN` (Thursday).

At 2 YOE the round checks that the shape is right — keys, directions, the junction table, 3NF with a reason — and that you can write the four joins without a reference. It does not expect partitioning, BCNF or planner internals.

Out loud, in under two minutes: "Each kind of thing gets a table with a numeric primary key. A one-to-many puts the parent's key on the child; a many-to-many gets a table of pairs. I normalize to 3NF so every fact is stored once — that is what prevents update, insert and delete anomalies — and I keep a deliberate copy only for values that must be allowed to differ, like the price on an order line. Constraints and types enforce the rules the API would otherwise have to repeat: not null, unique, check, numeric for money, timestamptz for time. For reads: an inner join keeps matched pairs, a left join keeps unmatched parents with nulls, and a filter on the child side goes in `ON`, not `WHERE`. Two one-to-many joins multiply, so I aggregate each side to one row per parent before joining. For 'rows with no match' I write `NOT EXISTS`, never `NOT IN`, because one null in the subquery makes `NOT IN` return nothing."
