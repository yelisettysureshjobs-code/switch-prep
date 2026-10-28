# Auth — Practice

> **Track:** BE · **Type:** concept · **Scheduled:** Week 4 · **Runtime:** Node v22.14.0 · **Generated:** 2026-10-27

## How to Use This File

- Every answer is collapsed. Predict first, then open.
- Warm-Up, Spot It and Self-Check are from memory with no notes. Build It is from a blank file; open a hint only after a real attempt. Explain It Back is out loud and alone.
- Your study method holds the rules for cold attempts and for logging. Write your own log line in your own notes file, in your own words.
- Outputs shown were run on the runtime above. Blocks marked `Not run` need Postgres or a browser.

## Warm-Up

### W1 — Whose secret?

```js
const jwt = require("jsonwebtoken");

const token = jwt.sign({ sub: "1", iat: 1800000000, exp: 1800000600 }, "secret-a");

const attempt = (secret) => {
  try {
    return jwt.verify(token, secret, {
      algorithms: ["HS256"],
      clockTimestamp: 1800000100,
    }).sub;
  } catch (err) {
    return err.message;
  }
};

console.log(attempt("secret-a"));
console.log(attempt("secret-b"));
```

<details>
<summary>Output and why</summary>

```text
1
invalid signature
```

The signature is recomputed with the secret you pass. With `secret-b` it does not match the one on the token, so `verify` throws (ideas #5, #6).

</details>

### W2 — The edge of `exp`

```js
const jwt = require("jsonwebtoken");

const token = jwt.sign({ sub: "1", iat: 1800000000, exp: 1800000600 }, "k");

for (const now of [1800000599, 1800000600, 1800000601]) {
  try {
    jwt.verify(token, "k", { algorithms: ["HS256"], clockTimestamp: now });
    console.log(now, "accepted");
  } catch (err) {
    console.log(now, err.message);
  }
}
```

<details>
<summary>Output and why</summary>

```text
1800000599 accepted
1800000600 jwt expired
1800000601 jwt expired
```

A token is valid only while `now < exp`, so at exactly `exp` it is already expired (idea #6).

</details>

### W3 — Compare against what?

```js
const bcrypt = require("bcrypt");

(async () => {
  const stored = await bcrypt.hash("Passw0rd", 4);
  console.log(await bcrypt.compare("Passw0rd", stored));
  console.log(await bcrypt.compare("passw0rd", stored));
  console.log(await bcrypt.compare("Passw0rd", "Passw0rd"));
  console.log(stored.startsWith("$2b$04$"));
})();
```

<details>
<summary>Output and why</summary>

```text
true
false
false
true
```

Passwords are case-sensitive. The third line is a classic bug: if the database holds the plain password instead of a hash, `compare` returns `false` even for the right password. The fourth shows the cost (`04`) written into the hash (idea #4).

</details>

### W4 — Pinning the algorithm

```js
const jwt = require("jsonwebtoken");

const token = jwt.sign({ sub: "1", iat: 1800000000, exp: 1800000600 }, "k", {
  algorithm: "HS256",
});

for (const algorithms of [["HS256"], ["HS512"]]) {
  try {
    jwt.verify(token, "k", { algorithms, clockTimestamp: 1800000100 });
    console.log(algorithms[0], "accepted");
  } catch (err) {
    console.log(algorithms[0], err.message);
  }
}
```

<details>
<summary>Output and why</summary>

```text
HS256 accepted
HS512 invalid algorithm
```

The token's header says `HS256`. When the server allows only `HS512` the token is refused. The server, not the token, decides the allowed algorithms (idea #6).

</details>

### W5 — A receipt that works once

```js
const valid = new Set(["t1"]);

const refresh = (token, next) => {
  if (!valid.delete(token)) return "rejected";
  valid.add(next);
  return `issued ${next}`;
};

console.log(refresh("t1", "t2"));
console.log(refresh("t1", "t3"));
console.log(refresh("t2", "t4"));
console.log([...valid].join(","));
```

<details>
<summary>Output and why</summary>

```text
issued t2
rejected
issued t4
t4
```

`delete` returns `true` only the first time, so each token works once (rotation, idea #9). Note what is missing: when `t1` is replayed, `t2` stays valid. If a thief used `t1` first, the thief now holds `t2`, and the real user is the one locked out. Reuse detection (idea #10) closes this by also revoking `t2`.

</details>

## Spot It

### S1 — Two different "no"s

Asha logs in and calls `DELETE /products/9`; the response is `403`. The same call with no `Authorization` header returns `401`. Name what each code says, within a minute, without writing code.

<details>
<summary>Answer</summary>

`401` means the server does not know who you are (missing or invalid token): authentication failed. `403` means it knows who you are and refuses: authorization failed, for example the role check. Ideas #6.

</details>

### S2 — The ban that did not bite

An admin bans a user at 10:00. At 10:05 the user's phone still loads private data. The API issues 15-minute JWT access tokens. Bug or by design? What would you change if bans had to be instant?

<details>
<summary>Answer</summary>

By design: the token is verified without asking the server, so it works until `exp`. For instant bans, either use server-side sessions, or keep the access lifetime very short and revoke the refresh token on ban (accepting a delay up to the lifetime), or add a per-request deny-list lookup, which gives up some of the statelessness. Ideas #3, #7, #8.

</details>

### S3 — Passwords by email

A teammate encrypts passwords with AES so support can "email forgotten passwords". What is wrong, and what should login store?

<details>
<summary>Answer</summary>

Encryption is reversible, so anyone with the key (or a leak of both) recovers every password, and "email the password" should never exist. Store only a slow, salted hash (bcrypt) and offer a reset link instead. Idea #4.

</details>

### S4 — The comment box

A comment box accidentally lets users post markup that runs as script for other visitors. Your single-page app keeps both tokens in `localStorage`. What can the attacker do, and which storage choice changes that?

<details>
<summary>Answer</summary>

That is XSS. Any script on the page can read `localStorage`, so it can copy both tokens and use them from anywhere. Keeping the access token in memory and the refresh token in an `HttpOnly` cookie stops the copy of the refresh token, though XSS can still act as the user while the tab is open. Idea #11.

</details>

### S5 — Valid token, blocked call (decoy)

Your API answers correctly from Postman with a valid token, but the browser console says the request was blocked for a missing `Access-Control-Allow-Origin` header. Is this an auth problem?

<details>
<summary>Answer</summary>

No. That is CORS, a browser rule about cross-origin requests, decided by headers the server sends. The token is fine. Your W7 slot covers it. This is the decoy for CORS.

</details>

### S6 — Random logouts

Users report being logged out at random when two tabs are open. The server rotates refresh tokens and revokes the whole family on reuse. Why?

<details>
<summary>Answer</summary>

Both tabs refreshed with the same cookie at once. The second request looks like a replay, so the family is revoked and the user must log in again. Fix it on the client with one shared in-flight refresh. Ideas #9, #10.

</details>

## Build It

### B1 — Register and login

*Scheduled: Week 4 · "BE: auth with JWT, bcrypt, refresh tokens" · Mode: from memory*

**Task.** Write `register(email, password)` and `login(email, password)` over an in-memory `Map`. Registering a taken email must fail. `login` must return a 15-minute JWT whose `sub` is the user id and must fail the same way for a wrong password and an unknown email.

<details>
<summary>Hint 1 — where to look</summary>

What belongs in the stored user record, and what must never be in it?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

`bcrypt.compare` needs the stored hash; an unknown email and a wrong password should produce the same error.

</details>

<details>
<summary>Solution</summary>

**Approach.** Hash on register, compare on login, sign a short-lived token.

```js
const bcrypt = require("bcrypt");
const jwt = require("jsonwebtoken");

const SECRET = "dev-secret-change-me";
const users = new Map(); // email -> { id, passwordHash, role }

const register = async (email, password) => {
  if (users.has(email)) throw new Error("email taken");
  const passwordHash = await bcrypt.hash(password, 10);
  users.set(email, { id: String(users.size + 1), passwordHash, role: "user" });
};

const login = async (email, password) => {
  const user = users.get(email);
  const ok = user && (await bcrypt.compare(password, user.passwordHash));
  if (!ok) throw new Error("bad credentials");
  return jwt.sign({ sub: user.id, role: user.role }, SECRET, { expiresIn: "15m" });
};

(async () => {
  await register("asha@example.com", "pw-123");
  const token = await login("asha@example.com", "pw-123");
  console.log(jwt.verify(token, SECRET, { algorithms: ["HS256"] }).sub);

  await login("asha@example.com", "nope").catch((err) => console.log(err.message));
  await login("ghost@example.com", "pw-123").catch((err) => console.log(err.message));
  await register("asha@example.com", "again").catch((err) => console.log(err.message));
})();
```

```text
1
bad credentials
bad credentials
email taken
```

**Edge cases handled.** Taken email, wrong password and unknown email (same message). Not handled and not run: an unknown email returns faster than a wrong password because bcrypt is skipped, a timing difference an attacker can use to find valid emails.

**Uses ideas:** #3, #4, #5

</details>

### B2 — A route guard with roles

*Scheduled: Week 4 · Mode: from memory*

**Task.** Write `requireAuth(role)` middleware: no or invalid token gives `401`, a valid token with the wrong role gives `403`. Prove it with three calls: no token, a user token, and a user token on an admin-only route.

<details>
<summary>Hint 1 — where to look</summary>

Which check must run first, and which status does each failure deserve?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Return a function from a function so the role is remembered, and pin the algorithm in `verify`.

</details>

<details>
<summary>Solution</summary>

**Approach.** The middleware and a run of exactly these calls are in [deep-dive.md](deep-dive.md) → Level 2 → Step 2 (`requireAuth`); its output there is `401`, `200` and `403` for the three calls.

**Edge cases handled.** Missing header, malformed header, expired or forged token, and wrong role (all shown in that output).

**Uses ideas:** #6

</details>

### B3 — Refresh with rotation, reuse detection and logout

*Scheduled: Week 4 · Mode: from memory*

**Task.** Extend your server with `/auth/login` (sets an `HttpOnly` refresh cookie and returns the access token), `/auth/refresh` (rotates), and `/auth/logout`. Prove: a normal refresh works; replaying the old refresh token fails and kills the family; after logout the token no longer refreshes. Keep refresh tokens hashed in your store.

<details>
<summary>Hint 1 — where to look</summary>

What does a row need to remember so you can tell "already used" from "never seen", and so you can find every token from one login?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Give each login a family id; mark a row `used` on refresh; a `used` row seen again deletes the whole family.

</details>

<details>
<summary>Solution</summary>

**Approach.** In-memory `Map` instead of Postgres, so it runs anywhere. The driver calls the routes with `fetch` and passes the cookie by hand.

```js
const crypto = require("crypto");
const express = require("express");
const bcrypt = require("bcrypt");
const jwt = require("jsonwebtoken");

const ACCESS_SECRET = "dev-access-secret";
const sha256 = (value) => crypto.createHash("sha256").update(value).digest("hex");

const users = new Map(); // email -> { id, passwordHash }
const refreshRows = new Map(); // tokenHash -> { userId, familyId, used }

const cookieOptions = {
  httpOnly: true,
  secure: true,
  sameSite: "strict",
  path: "/auth",
  maxAge: 7 * 24 * 3600 * 1000,
};

const issue = (userId, familyId) => {
  const refreshToken = crypto.randomBytes(32).toString("hex");
  refreshRows.set(sha256(refreshToken), { userId, familyId, used: false });
  const accessToken = jwt.sign({ sub: userId }, ACCESS_SECRET, { expiresIn: "15m" });
  return { accessToken, refreshToken };
};

const revokeFamily = (familyId) => {
  for (const [hash, row] of refreshRows) {
    if (row.familyId === familyId) refreshRows.delete(hash);
  }
};

const readCookie = (req, name) =>
  (req.headers.cookie ?? "")
    .split("; ")
    .map((pair) => pair.split("="))
    .find(([key]) => key === name)?.[1];

const app = express();
app.use(express.json());

const requireAuth = (req, res, next) => {
  const [scheme, token] = (req.headers.authorization ?? "").split(" ");
  try {
    if (scheme !== "Bearer") throw new Error("not a bearer token");
    req.user = jwt.verify(token, ACCESS_SECRET, { algorithms: ["HS256"] });
    next();
  } catch {
    res.status(401).json({ error: "unauthenticated" });
  }
};

app.post("/auth/login", async (req, res) => {
  const user = users.get(req.body.email);
  const ok = user && (await bcrypt.compare(req.body.password, user.passwordHash));
  if (!ok) return res.status(401).json({ error: "bad credentials" });
  const { accessToken, refreshToken } = issue(user.id, crypto.randomUUID());
  res.cookie("refreshToken", refreshToken, cookieOptions).json({ accessToken });
});

app.post("/auth/refresh", (req, res) => {
  const row = refreshRows.get(sha256(readCookie(req, "refreshToken") ?? ""));
  if (!row) return res.status(401).json({ error: "unknown token" });
  if (row.used) {
    revokeFamily(row.familyId);
    return res.status(401).json({ error: "reuse detected" });
  }
  row.used = true;
  const { accessToken, refreshToken } = issue(row.userId, row.familyId);
  res.cookie("refreshToken", refreshToken, cookieOptions).json({ accessToken });
});

app.post("/auth/logout", (req, res) => {
  const row = refreshRows.get(sha256(readCookie(req, "refreshToken") ?? ""));
  if (row) revokeFamily(row.familyId);
  res.clearCookie("refreshToken", { path: "/auth" }).status(204).end();
});

app.get("/notes", requireAuth, (req, res) => {
  res.json({ userId: req.user.sub, notes: [] });
});

const main = async () => {
  users.set("asha@example.com", {
    id: "42",
    passwordHash: await bcrypt.hash("pw-123", 4),
  });
  const server = await new Promise((resolve) => {
    const s = app.listen(0, () => resolve(s));
  });
  const base = `http://127.0.0.1:${server.address().port}`;

  const call = async (path, { method = "GET", token, cookie, body } = {}) => {
    const res = await fetch(base + path, {
      method,
      headers: {
        "Content-Type": "application/json",
        ...(token && { Authorization: `Bearer ${token}` }),
        ...(cookie && { Cookie: `refreshToken=${cookie}` }),
      },
      body: body && JSON.stringify(body),
    });
    return {
      status: res.status,
      json: res.status === 204 ? null : await res.json(),
      refreshToken: res.headers.get("set-cookie")?.match(/refreshToken=([^;]+)/)?.[1],
    };
  };

  const credentials = { email: "asha@example.com", password: "pw-123" };
  const login = await call("/auth/login", { method: "POST", body: credentials });
  console.log("login        :", login.status);

  const notes = await call("/notes", { token: login.json.accessToken });
  console.log("GET /notes   :", notes.status);

  const second = await call("/auth/refresh", { method: "POST", cookie: login.refreshToken });
  console.log("refresh      :", second.status);

  const replay = await call("/auth/refresh", { method: "POST", cookie: login.refreshToken });
  console.log("replay old   :", replay.status, replay.json.error);

  const newest = await call("/auth/refresh", { method: "POST", cookie: second.refreshToken });
  console.log("newest token :", newest.status, newest.json.error);

  const again = await call("/auth/login", { method: "POST", body: credentials });
  await call("/auth/logout", { method: "POST", cookie: again.refreshToken });
  const afterLogout = await call("/auth/refresh", { method: "POST", cookie: again.refreshToken });
  console.log("after logout :", afterLogout.status, afterLogout.json.error);

  server.close();
};

main();
```

```text
login        : 200
GET /notes   : 200
refresh      : 200
replay old   : 401 reuse detected
newest token : 401 unknown token
after logout : 401 unknown token
```

**Edge cases handled.** Rotation, replay of a used token (family revoked, so the newest token also fails), and logout. Not handled: the two-tabs race (needs the client-side shared refresh from the Level 3 fragment), and cleanup of expired rows.

**Uses ideas:** #8, #9, #10, #12

</details>

### B4 — Move the refresh table to Postgres

*Scheduled: Week 4 (with your W3 Postgres work) · Mode: from memory*

**Task.** Replace the `Map` in B3 with the `refresh_tokens` table from the deep dive. Make the rotation safe when two requests arrive in the same millisecond with the same token: exactly one may succeed.

<details>
<summary>Hint 1 — where to look</summary>

Where could the "read the row, then mark it used" pair go wrong if two requests interleave?

</details>

<details>
<summary>Hint 2 — the key idea</summary>

Do the check and the update in one statement, and use the number of rows it returns to decide.

</details>

<details>
<summary>Solution</summary>

**Approach.** One atomic `UPDATE ... RETURNING` claims the token. Zero rows means "not valid"; then you look up whether the hash exists and was already used, which is a replay.

```sql
-- Not run — paste into psql on a scratch database
-- 1) claim the token: only one concurrent request gets a row back
UPDATE refresh_tokens
SET used_at = now()
WHERE token_hash = $1 AND used_at IS NULL AND expires_at > now()
RETURNING user_id, family_id;

-- 2) if step 1 returned no row: was it a replay of a used token?
SELECT family_id FROM refresh_tokens WHERE token_hash = $1 AND used_at IS NOT NULL;

-- 3) if so, delete the whole family
DELETE FROM refresh_tokens WHERE family_id = $2;
```

**Edge cases handled.** Not run, so none are verified. By reasoning: concurrent use of the same token gives one winner (the second `UPDATE` matches no row), and an unknown hash returns no row in either query.

**Uses ideas:** #9, #10

</details>

## Self-Check Questions

### Q1 — Why not remember the connection?

Why can't a server simply remember who is on a connection and skip all this?

<details>
<summary>Answer</summary>

HTTP requests are independent: connections close, are reused and shared, and behind a load balancer the next request can reach a different server. So identity must travel with each request (idea #1).

</details>

### Q2 — What does each side hold in a session?

In a session design, what does the browser hold and what does the server hold?

<details>
<summary>Answer</summary>

The browser holds only an opaque random ID, usually in a cookie. The server holds the data behind it (who the user is) in a store. Deleting the server entry ends the session instantly (idea #2).

</details>

### Q3 — Who can read a JWT?

Someone copies your token but does not know the secret. What can they see and what can't they do?

<details>
<summary>Answer</summary>

They can read the header and payload (base64url is not encryption). They cannot change either without breaking the signature, and cannot mint a valid token without the secret (ideas #5, #6).

</details>

### Q4 — Why pin `algorithms`?

Why pass `algorithms: ["HS256"]` to `verify`?

<details>
<summary>Answer</summary>

The token's header says how to verify it, and an attacker controls the header, for example `alg: none` for "no signature needed". Pinning makes the server, not the token, decide what is acceptable (idea #6).

</details>

### Q5 — Make a JWT revocable

Give two ways to make JWT logins revocable and what each costs.

<details>
<summary>Answer</summary>

(1) Short access lifetime plus a revocable refresh token: costs a refresh flow and a delay up to the lifetime. (2) A server-side deny-list of token IDs checked on every request: instant, but it brings back a per-request lookup. Ideas #7, #8.

</details>

### Q6 — Why not one long-lived token?

Why not issue one 7-day token and skip refresh tokens?

<details>
<summary>Answer</summary>

A stolen token would work for `7 × 86,400 = 604,800 s` with no way to cancel it. A 15-minute access token bounds the damage to `900 s`, and the refresh token, being server-side, can be revoked (idea #8).

</details>

### Q7 — Why store the refresh token hashed?

Why does the table store `sha256(token)` and not the token itself?

<details>
<summary>Answer</summary>

If the table leaks, hashes cannot be turned back into working tokens. A fast hash is fine because the token is 32 random bytes, so guessing is infeasible, unlike a human password (idea #9).

</details>

### Q8 — Why punish the real user?

When an old refresh token is replayed, why revoke the whole family, even if it may log out the real user?

<details>
<summary>Answer</summary>

The server cannot tell whether the thief or the owner presented the used token. If the thief got in first, the thief holds the newest token, so revoking only the replayed one would leave the thief logged in. Forcing a fresh login is the safe side of the trade (idea #10).

</details>

### Q9 — Name the attack for each storage choice

For `localStorage`, memory and an `HttpOnly` cookie, name the attack that wins against each. What do `HttpOnly` and `SameSite=Strict` each stop?

<details>
<summary>Answer</summary>

`localStorage`: XSS copies it. Memory: XSS can act as the user while the tab is open, but a stolen copy is harder to obtain. `HttpOnly` cookie: CSRF, since the browser attaches it automatically. `HttpOnly` stops scripts reading the cookie; `SameSite=Strict` stops it being sent on cross-site requests. Ideas #11, #12.

</details>

### Q10 — Pick one

A server-rendered dashboard on a single server for 500 users needs instant bans. Sessions or JWT? Is "Sign in with Google" a JWT-vs-session question?

<details>
<summary>Answer</summary>

Sessions: instant revocation and a trivial store at this size. "Sign in with Google" is OAuth/OpenID Connect, a protocol for delegated login; JWT is only a token format that such a protocol may use. Idea #13.

</details>

### Q11 — What does the bcrypt cost do?

What does raising the bcrypt cost from 10 to 12 do, and why is that useful?

<details>
<summary>Answer</summary>

Each +1 doubles the work, so cost 12 is about four times slower (we measured about 68 ms and 274 ms). That slows an attacker guessing offline by the same factor, at the price of a slower login. Idea #4.

</details>

## Explain It Back

### E1 — Two minutes from the analogy

Explain "sessions vs tokens" to a new teammate in two minutes, starting from the theme-park story.

<details>
<summary>Rubric</summary>

- HTTP is stateless, so login must give the client something to attach (#1).
- Park A: ticket number, state on the server, instant cancel (#2).
- Park B: signed wristband, state on the client, no lookup (#3).
- The catch: no cancelling before expiry (#7).
- The fix: short wristbands plus a receipt (#8).
- Close with when you would choose each (#13).

</details>

### E2 — "Why not just use localStorage?"

An interviewer asks why you don't keep the JWT in `localStorage`. Answer.

<details>
<summary>Rubric</summary>

- Any script on the page can read `localStorage`: XSS steals the token (#11).
- The alternative: access token in memory, refresh token in an `HttpOnly`, `Secure`, `SameSite` cookie with a narrow `Path` (#12).
- Name what you trade: the cookie brings CSRF, which `SameSite` and the header-based access token reduce.
- Admit it is a trade, not a solved problem.

</details>

### E3 — Walk the refresh flow

Walk through what happens from an expired access token to a new one, and what happens if two tabs refresh together.

<details>
<summary>Rubric</summary>

- API returns `401` on the expired token (#8).
- The browser calls refresh with the cookie; the server finds the hash, marks it used, issues a new pair (#9).
- Old token is dead; the new one is in the same family (#10).
- Two tabs: the second looks like a replay, the family is revoked, the user logs in again (#10).
- The fix: one shared in-flight refresh on the client.

</details>