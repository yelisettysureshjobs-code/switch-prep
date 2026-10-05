# Product-Company Prep — 28 Week Plan

**Target roles:** Frontend / Full-stack, 2 YOE
**Target companies:** Myntra, Meesho, Blinkit, MakeMyTrip (and similar unicorns)
**Budget:** 4 hrs × 5 weekdays + one 4-hr weekend day roughly every other week (~22 hrs/week)
**Start applying:** Week 17. **Target companies:** Week 25+.

---

## The weekly rhythm

**Mon / Wed / Fri — DSA-heavy**

| Time | Block |
|---|---|
| 2.5 hr | DSA — 3 problems |
| 1.0 hr | JS internals / React depth |
| 0.5 hr | Revision — re-solve one old problem cold, no notes |

**Tue / Thu — Build-heavy**

| Time | Block |
|---|---|
| 2.0 hr | Machine coding, timed |
| 1.0 hr | DSA — 1–2 problems |
| 1.0 hr | System design / backend — on Thursdays in W11–16 this slot is LLD |

**Weekend (the ~1 day in 2 you get)** — one unbroken 4-hr block. Mock interview, or project integration. Never problem-grinding; that fits in weekday slices.

**Throughput:** ~12 DSA problems/week → ~200 by Week 20, covering ~35 patterns. That is the right number for these companies. Do not chase 400.

---

## Non-negotiables

1. **The 30-min revision block is the last thing you cut.** On a collapsed day, do revision + one problem and call it a win. Retention is what breaks on sparse schedules and it is expensive to rebuild.
2. **One line of notes per problem** in `prep/notes.md`: `problem → pattern → trigger`. Re-solve from the trigger, never re-read the solution.
3. **From Week 9, machine coding builds feed the project.** Stop building orphan widgets — every component lands in the one app.
4. **Commit daily.** The green graph is not the point; the streak is.

---

## Explicitly out of scope

Cut deliberately, not by accident:

- Advanced DP (digit DP, bitmask DP), segment trees, Fenwick, MST, SCC, and tries beyond the two basics in W11–12
- Striver A2Z's advanced-maths and string-algorithm sections (KMP, Z-function, Rabin-Karp)
- GraphQL, microservices, ORM comparisons
- OS beyond a one-day skim. DBMS and networks are **in** — Week 19, not optional for full-stack loops
- Three portfolio projects. You are building **one**.

---

## Where the problems come from

Everything in `daily/` is drawn from **Striver's A2Z sheet** (~455 problems on takeuforward.org), not the SDE sheet (~191). The topic ordering and problem names follow A2Z. What's here is a **~200-problem curated subset** — the A2Z spine with the advanced tail cut, since these companies don't ask it at 2 YOE.

Two honest caveats:

- The list was written from memory of the sheet, not scraped from it. A few names may differ slightly or sit in a different section than I've placed them. **Open the actual sheet alongside this plan** — treat these files as the filter, not the replacement.
- A2Z gets updated. If a topic has grown since, prefer the sheet's ordering over mine.

If you'd rather work from a smaller list: Striver's **SDE sheet (~191)** covers roughly the same ground more tersely, and **Blind 75 / NeetCode 150** are the usual alternatives. Pick one and finish it. Switching sheets halfway is the most common way people end up with 300 problems solved and no pattern recall.

## How to actually study

**[how-to-study.md](how-to-study.md)** — the method for each track, because they're different. DSA: learn the pattern, then struggle, with a 25-minute rule. Machine coding: code-first against a known rubric and a 90-minute budget. System design: learn-first with the RADIO framework, answered out loud — plus a separate skeleton for backend HLD. LLD: entities first, one core flow in code, 60 minutes. Read this before Week 1.

## Patterns, not problems

**[patterns.md](patterns.md) is the actual curriculum.** ~35 patterns, each with the trigger that tells you it applies. Every problem in `daily/` is tagged with its pattern in `code` formatting.

You are not memorising 200 problems. You are learning 35 patterns and training yourself to recognise the trigger inside 60 seconds. Name the pattern out loud before coding in interviews — it's scored, and it buys you a correction if you're wrong.

## Week-by-week checklist

### Month 1 — Arrays & Binary Search · [day-wise](daily/month-01.md)

- [ ] **W1** — Arrays easy+medium (~12) · JS: execution context, scope, closures · DOM: events + delegation · Backend: Node + Express setup, REST basics · MC: counter, star rating, accordion
- [ ] **W2** — Arrays medium+hard, Kadane/Dutch flag/intervals (~12) · JS: `this`, call/apply/bind, prototypes · Backend: Express routing, middleware, error handling · MC: todo with filters + localStorage
- [ ] **W3** — Binary search on arrays (~12) · JS: event loop, micro/macrotask, promises + async/await · CSS I: box model, cascade, positioning, stacking · Backend: Postgres — schema, joins, indexes · MC: tabs in **vanilla JS**, modal, tooltip (a11y-correct)
- [ ] **W4** — Binary search on answers + 2D (~12) · JS: polyfills I — map/filter/reduce, call/apply/bind, debounce, throttle · Backend: auth with JWT, bcrypt, refresh tokens · MC: autocomplete — debounce, cancel, keyboard nav
- [ ] **Milestone:** ~48 problems · polyfill set I written from memory · a running Express+Postgres API with auth

### Month 2 — Strings, Linked List, Recursion, Stacks · [day-wise](daily/month-02.md)

- [ ] **W5** — Strings (~12) · JS: polyfills II — Promise.all/race/allSettled/any, deep clone, EventEmitter, curry, memoize · SD: rendering strategies CSR/SSR/SSG/ISR · MC: infinite scroll + intersection observer
- [ ] **W6** — Linked list + doubly LL (~12) · React: reconciliation, keys, render cycle, StrictMode · CSS II: flexbox, grid, responsive · SD: browser rendering pipeline, CRP, reflow/repaint · MC: virtualized list (windowing, from scratch)
- [ ] **W7** — Recursion + backtracking (~12) · React: hooks deep — useEffect timing/cleanup, useRef, custom hooks · HTML: semantics, forms, script loading · SD: HTTP caching, CORS, cookies · MC: nested comments (recursive render)
- [ ] **W8** — Stacks, queues, monotonic stack (~12) · React: state — Context, Redux Toolkit, Zustand, server state · SD: XSS, CSRF, CSP, auth on the frontend · MC: kanban board with drag-drop
- [ ] **Milestone:** ~96 problems · all polyfills cold · React model articulate out loud

### Month 3 — Trees, Heaps, Greedy, Sliding Window · **project starts** · [day-wise](daily/month-03.md)

- [ ] **W9** — Binary trees: traversals, views, LCA (~12) · React: perf — memo, useMemo/useCallback, lazy, Suspense, Profiler · SD: **design an autocomplete** · **Project:** scaffold Vite+TS+React, routing, design tokens · **BE:** tested products API
- [ ] **W10** — BST + tree hard (~12) · React: forms, error boundaries, portals · SD: **design a product listing page at scale** · **Project:** product grid + virtualized list (reuse W6) · **BE:** Redis cache on the products API
- [ ] **W11** — Heaps **+ Greedy**, merged, + Implement Trie (~15) · TypeScript in React · SD: **design an image-heavy feed** · **LLD:** the method + parking lot · **Project:** filters, sort, URL-synced state · **BE:** indexed filter queries
- [ ] **W12** — **Sliding window, two pointers, bit manipulation**, + one more trie (~15) · Testing: RTL, Jest, MSW · SD: **design a component library** · **LLD:** Splitwise · **Project:** cart + optimistic UI with rollback · **BE:** versioned cart API
- [ ] **Milestone:** ~146 problems · the `window-variable` template from memory · project running on your own API · 2 LLD problems coded

### Month 4 — Graphs & DP · [day-wise](daily/month-04.md)

- [ ] **W13** — Graphs I: BFS, DFS, cycle detection, grid problems (~12) · Build tooling: Vite/Webpack, code splitting, tree shaking · SD: **design a chat app (frontend)** — WebSocket, ordering, reconnect · **LLD:** snake & ladder · **Project:** WebSocket order tracking · **BE:** queue + worker feeding it
- [ ] **W14** — Graphs II: topo sort, Dijkstra, shortest paths (~12) · Core Web Vitals — LCP, INP, CLS and the fix for each · SD: **caching strategies** — HTTP, SW, in-memory, stale-while-revalidate · **LLD:** in-memory cache with pluggable eviction · **Project:** offline cart with IndexedDB + sync on reconnect · **BE:** idempotent sync endpoint
- [ ] **W15** — DP I: 1D, 2D grid, subsequences (~12) · Bundle analysis + a real perf pass · SD: **design a checkout flow** — idempotency, failure states · **LLD:** movie seat booking · **Project:** checkout + error/edge states · **BE:** checkout in one transaction, last-item race
- [ ] **W16** — DP II: knapsack, LIS, stocks, partition (~12) · Accessibility: keyboard, ARIA, screen reader pass · SD: **HLD basics** — LB, cache, DB scaling, CDN, queues · **LLD:** rate limiter · **Project:** perf pass — Lighthouse before/after, measured · **BE:** Docker Compose
- [ ] **Milestone:** ~194 problems · project deployed · **perf numbers written down** · 6 LLD problems coded

### Month 5 — Design, Polish, First Applications · [day-wise](daily/month-05.md)

- [ ] **W17** — Resume rewrite (action → metric → tech) · project README as a design doc · LinkedIn/GitHub cleanup · **start applying: mid-tier, 15/week** · DSA: revision only
- [ ] **W18** — System design drills, 1/day out loud, timed — alternating frontend and backend HLD · behavioural stories in STAR (5 of them) · mine your day job for ownership numbers
- [ ] **W19** — CS fundamentals — DBMS (3 days, incl. SQL by hand), networks, one-day OS skim · backend HLD drills: URL shortener, rate limiter
- [ ] **W20** — Weak-pattern revision from `notes.md` · 2 full mock loops · first real interviews landing
- [ ] **Milestone:** applying actively · resume converting · ~200 problems, all revised at least twice

### Month 6 — Interview Mode · [day-wise](daily/month-06.md)

- [ ] **W21** — 2 mocks · post-mortem every real interview into `notes.md`
- [ ] **W22** — 2 mocks · patch the specific gaps interviews exposed, nothing else
- [ ] **W23** — 2 mocks · machine coding under strict 90 min, no lookups
- [ ] **W24** — Negotiation prep · offer-stage behavioural · keep DSA warm at 1 hr/day
- [ ] **Milestone:** interviewing weekly, feedback loop running

### Month 7 — Target Companies · [day-wise](daily/month-07.md)

- [ ] **W25–W28** — Myntra, Meesho, Blinkit, MakeMyTrip · referrals only, not portals · maintenance mode: 1 hr DSA + 1 hr revision + interview-specific prep
- [ ] **Milestone:** offers in hand

---

## Progress log

Keep a dated line here each week — what actually got done, not what was planned.

| Week | Problems | MC builds | Notes |
|---|---|---|---|
| W1 | | | |
