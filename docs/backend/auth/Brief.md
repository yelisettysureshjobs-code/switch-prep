# Auth — Brief

> **Track:** BE · **Type:** concept · **Scheduled:** Week 4 · **Runtime:** Node v22.14.0 · **Generated:** 2026-10-27

## Explain It Like I'm 5

A theme park gate guard forgets you the moment you walk away, so every ride needs proof you paid.

Park A: the front desk keeps your bag and hands you a numbered ticket. Each ride-guard phones the desk: "Is ticket 17 okay?" The desk can tear ticket 17 up any time.

Park B: the desk stamps a wristband: "Asha, adult, good until 6:00", with a sparkly sticker nobody can copy. Ride-guards just check the sparkle and the clock. No phone call, but nobody can snip a wristband off your arm from afar.

Park B's fix: wristbands last 15 minutes. Show your paper receipt at the desk for a new one. The desk can say no.

In real terms: Park A is a session, Park B is a JWT access token, the receipt is a refresh token, and the desk is your auth server.

## What It Is & Where It Shows Up

**What it is:** auth is the way a stateless server learns, on every request, who is calling and whether they are allowed.

HTTP keeps no memory between requests, so login must give the client something to attach to each later request. The two designs differ in where the state lives: on the server (a session) or in a signed token the client carries (a JWT). Without this knowledge people put secrets in tokens, trust unverified ones, or build logins that cannot be revoked.

**Where it shows up:** every protected route in an Express API (your W9 products API, W12 cart API) and the login flow of your frontend project (W8). The question is usually phrased "JWT vs sessions?", "how do you log a JWT user out?" or "where do you store the token?"

## Glossary

| Term | In plain words | Precise definition |
| --- | --- | --- |
| Stateless | the server forgets you between requests | HTTP keeps no per-client memory between requests |
| Authentication | proving who you are | the server establishes the caller's identity; failure is `401` |
| Authorization | being allowed to do it | a check of what an identified caller may do; failure is `403` |
| Session | the server keeps your details | server-side state looked up by an ID the client sends |
| Session ID | a meaningless number on your ticket | a random identifier, usually carried in a cookie |
| Cookie | a note the browser keeps and resends | a `name=value` the server sets and the browser attaches automatically to matching requests |
| Token | the details travel with you | a value the client sends that the server can check without a lookup |
| Hash | a one-way scramble | a function that cannot be reversed |
| Salt | extra random text mixed in | random data added so equal passwords hash differently |
| bcrypt | a deliberately slow scrambler for passwords | a salted, tunable-cost password hash |
| JWT | a signed wristband | `header.payload.signature`, each part base64url, carrying claims |
| Claim | one fact on the wristband | a name/value statement in the payload (`sub`, `exp`, `role`) |
| Signature | the sparkle nobody can copy | a value computed from the token and a secret; any edit breaks it |
| Secret | the stamp only the desk owns | the key used to compute and check the signature |
| Verify | check the sparkle and the clock | recompute the signature, check the allowed algorithm and `exp` |
| Bearer token | whoever holds it gets in | a token accepted from anyone who presents it |
| Access token | the 15-minute wristband | a short-lived token sent with each API call |
| Refresh token | the receipt for a new wristband | a long-lived, opaque, server-stored credential used only to get new access tokens |
| Rotation | a new receipt every time | issuing a new refresh token on each use and retiring the old |
| Token family | all receipts from one visit | the chain of refresh tokens descended from one login |
| Reuse detection | spotting a used receipt again | treating a retired refresh token's reappearance as theft and revoking the family |
| Revocation | cancelling early | making a credential stop working before its natural expiry |
| `HttpOnly` | the zipped pocket only the browser opens | a cookie flag forbidding scripts from reading the cookie |
| `SameSite` | only same-park requests carry it | a cookie flag limiting sending on cross-site requests |
| XSS | a stranger's script runs in your page | injected script executing on your site |
| CSRF | another site makes your browser ask | a cross-site request that rides on cookies the browser attaches |

## Core Ideas

| # | Idea | Level | In one line | Taught in | Tested by |
| --- | --- | --- | --- | --- | --- |
| 1 | HTTP is stateless | basic | every request must say who you are | [deep-dive.md](deep-dive.md) → Why It Exists | Q1, E1 |
| 2 | Session | basic | server keeps the state; client holds an opaque ID | [deep-dive.md](deep-dive.md) → Level 1 | Q2, E1 |
| 3 | Token | basic | client holds signed claims; no per-login server state | [deep-dive.md](deep-dive.md) → Level 1 | B1, S2, E1 |
| 4 | Password hashing | basic | slow, salted hash (bcrypt), never plaintext or a fast hash | [deep-dive.md](deep-dive.md) → Level 2 (Step 0) | W3, S3, B1, Q11 |
| 5 | JWT anatomy | intermediate | `header.payload.signature`: signed, not encrypted | [deep-dive.md](deep-dive.md) → Level 2 (Step 1) | Q3, B1 |
| 6 | Verification | intermediate | recompute the signature, pin the algorithm, check `exp`; never trust `decode` | [deep-dive.md](deep-dive.md) → Level 2 (Step 2) | W1, W2, W4, S1, B2, Q3, Q4 |
| 7 | Unrevocable until `exp` | intermediate | the core trade-off against sessions | [deep-dive.md](deep-dive.md) → Level 2 (Step 3) | S2, Q5, E1 |
| 8 | Access plus refresh | intermediate | short access token limits damage; refresh restores convenience | [deep-dive.md](deep-dive.md) → Level 1 | S2, Q5, Q6, B3, E3 |
| 9 | Refresh flow | intermediate | 401, refresh, retry; the refresh token is stored hashed on the server | [deep-dive.md](deep-dive.md) → Level 3 (3.2) | W5, S6, Q7, B3, B4, E3 |
| 10 | Rotation and reuse detection | advanced | a replayed old token revokes the whole family | [deep-dive.md](deep-dive.md) → Level 3 (3.3) | W5, S6, Q8, B3, B4, E3 |
| 11 | Where tokens live | intermediate | `localStorage` vs memory vs `HttpOnly` cookie trade XSS for CSRF | [deep-dive.md](deep-dive.md) → Level 3 (3.4) | S4, Q9, E2 |
| 12 | Cookie flags | advanced | `HttpOnly`, `Secure`, `SameSite`, narrow `Path` | [deep-dive.md](deep-dive.md) → Level 3 (3.5) | B3, Q9, E2 |
| 13 | Choosing | intermediate | when sessions win, when tokens win | [deep-dive.md](deep-dive.md) → Limits & Trade-offs | Q10, E1 |

Practice item IDs refer to [practice.md](practice.md).

## Know This Cold

**The rules**

1. Sessions: the server stores the state and the client holds an opaque ID. Tokens: the client holds signed claims.
2. A JWT is `header.payload.signature`. It is signed, not encrypted, so never put secrets in it.
3. Always `verify` with a pinned `algorithms` list; never trust `decode`. `exp` is checked at verify, and a token is valid only while `now < exp`.
4. Stateless means no revocation before `exp`, so keep access tokens short (15 minutes).
5. Refresh tokens are opaque and random, stored hashed on the server, rotated on every use; a replay revokes the family.
6. Storage: the access token in memory; the refresh token in an `HttpOnly`, `Secure`, `SameSite` cookie with a narrow `Path`.
7. Passwords: bcrypt, compared with `bcrypt.compare`; the same error for an unknown email and a wrong password.

**The canonical snippet** (from [deep-dive.md](deep-dive.md) → Level 2, Step 1)

```js
const jwt = require("jsonwebtoken");

const secret = "dev-secret-change-me";
const token = jwt.sign(
  { sub: "42", role: "user", iat: 1800000000, exp: 1800000900 },
  secret,
  { algorithm: "HS256" }
);

const [header, payload, signature] = token.split(".");
console.log("parts:", token.split(".").length);
console.log("header :", Buffer.from(header, "base64url").toString());
console.log("payload:", Buffer.from(payload, "base64url").toString());
console.log("signature bytes:", Buffer.from(signature, "base64url").length);

const claims = jwt.verify(token, secret, {
  algorithms: ["HS256"],
  clockTimestamp: 1800000100,
});
console.log("verified:", claims);
```

```text
parts: 3
header : {"alg":"HS256","typ":"JWT"}
payload: {"sub":"42","role":"user","iat":1800000000,"exp":1800000900}
signature bytes: 32
verified: { sub: '42', role: 'user', iat: 1800000000, exp: 1800000900 }
```

## Costs & Limits

| What it costs | When it bites |
| --- | --- |
| One store lookup per request (sessions) | every request; the store must be shared by all servers, and the default `MemoryStore` is for development only |
| No revocation before `exp` (tokens) | a banned or stolen access token works up to `900 s`; a stolen refresh token up to `7 × 86,400 = 604,800 s` unless revoked server-side |
| JWT size | 161 bytes for a four-claim token (measured), sent on every request; grows with every claim |
| bcrypt time | about 68 ms at cost 10 and 274 ms at cost 12 (measured here; each +1 doubles), so logins are CPU-heavy and worth rate-limiting |
| Refresh rows | `10⁴ users × 32 refreshes/day = 3.2 × 10⁵ rows/day` if used rows are kept, so you need a cleanup job |

Hard limits: the payload is readable by anyone; bcrypt only reads the first 72 bytes of a password (recalled — verify); a memory-held token is lost on reload; with `HS256` everything that can verify can also sign.

## Connections

**Prerequisites**

- Closures and promises: see your own notes on both.
- HTTP and REST basics (W1), Express routing and middleware (W2), Postgres tables (W3), cookies and CORS (W7): no notes yet; a short primer for each is in [deep-dive.md](deep-dive.md).

**Confusables**

- Authentication vs authorization: `401` means "who are you?", `403` means "I know you, and no".
- Hashing vs encryption vs signing: a hash cannot be reversed, encryption can, a signature only proves no edits.
- Session ID vs JWT: if the server must look the value up it is a session ID; if it explains itself and carries a signature it is a JWT.
- OAuth/OIDC vs JWT: protocol vs token format.
- Access vs refresh token: access opens doors, refresh only gets new access.

**Unlocks**

- W7: cookies and CORS.
- W8: XSS, CSRF, CSP and auth on the frontend.
- W9: the tested products API (protected routes).
- W12: the cart API.
- W13: authenticating the WebSocket handshake.

## Mastery Checklist

- [ ] You can explain why HTTP forces a login to hand the client something, without notes.
- [ ] You can write register, login and a `requireAuth(role)` guard from a blank file.
- [ ] You can predict the output of an unseen `verify` snippet (wrong secret, expired, wrong algorithm) and say why.
- [ ] You can draw the refresh flow with rotation and say what a replay causes.
- [ ] You can name the attack that wins against `localStorage`, memory and an `HttpOnly` cookie.
- [ ] You can name two situations where JWT bites and where sessions win.
- [ ] You can explain all of it to someone else in two minutes, starting from the theme-park analogy.

## 2-Minute Revision

- HTTP forgets you, so login hands you something to attach every time.
- Session: server keeps the state, you hold an opaque ID, cancel is instant.
- JWT: `header.payload.signature`; signed, not encrypted; verify with a pinned algorithm; `decode` is not trust.
- Cost of a token: no revocation before `exp`, so access tokens last 15 minutes.
- Refresh token: opaque, stored hashed, used once; a replay revokes the family.
- Storage: access in memory; refresh in an `HttpOnly` + `Secure` + `SameSite` cookie.
- Passwords: bcrypt, same error for bad email and bad password.
- Trap 1: a JWT payload is readable. Trap 2: two tabs refreshing at once can log the user out.