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

**Throughput:** ~11 DSA problems/week → the whole of Striver's 180 sheet (179 problems, ~40 patterns) by Week 16, then revision only. That is the right number for these companies. Do not chase 400.

---

## Non-negotiables

1. **The 30-min revision block is the last thing you cut.** On a collapsed day, do revision + one problem and call it a win. Retention is what breaks on sparse schedules and it is expensive to rebuild.
2. **One line of notes per problem** in `prep/notes.md`: `problem → pattern → trigger`. Re-solve from the trigger, never re-read the solution.
3. **From Week 9, machine coding builds feed the project.** Stop building orphan widgets — every component lands in the one app.
4. **Commit daily.** The green graph is not the point; the streak is.

---

## Explicitly out of scope

Cut deliberately, not by accident:

- Advanced DP (digit DP, bitmask DP), segment trees, Fenwick — anything that isn't on the 180 sheet. *(MST, SCC, bridges and the trie-XOR problems used to be on this list; the sheet includes them, so they're in — Weeks 12, 13 and 15.)*
- Advanced maths beyond the sieve. *(KMP, Z-function and Rabin-Karp are on the sheet — Weeks 15–16.)*
- GraphQL, microservices, ORM comparisons
- OS beyond a one-day skim. DBMS and networks are **in** — Week 19, not optional for full-stack loops
- Three portfolio projects. You are building **one**.

---

## Where the problems come from

Everything in `daily/` is **[Striver's 180 – Master DSA Patterns](https://takeuforward.org/prep-hub/strivers-180-master-dsa-patterns?page=sheet)** — 179 problems across 17 topics, each one tagged by the sheet with a pattern. The plan takes them in sheet order, every problem exactly once, under the sheet's own names, with the sheet's pattern name beside each.

Three honest caveats:

- The list was pulled from the sheet page on 2026-10-06. The sheet gets updated — if a problem here is missing there or the other way round, the sheet wins.
- The sheet assumes basics it doesn't list: the binary search template, tree traversals, BFS/DFS skeletons. Where that matters the daily plan adds a 15-minute warm-up.
- This plan originally followed Striver's A2Z sheet. Anything you solved from that version that isn't on the 180 sheet (Pascal's Triangle, Set Matrix Zeros) stays in `notes.md` as extra reps.

This is the list now. Finish it — switching sheets halfway is the most common way people end up with 300 problems solved and no pattern recall.

## How to actually study

**[how-to-study.md](how-to-study.md)** — the method for each track, because they're different. DSA: learn the pattern, then struggle, with a 25-minute rule. Machine coding: code-first against a known rubric and a 90-minute budget. System design: learn-first with the RADIO framework, answered out loud — plus a separate skeleton for backend HLD. LLD: entities first, one core flow in code, 60 minutes. Read this before Week 1.

## Patterns, not problems

**[patterns.md](patterns.md) is the actual curriculum.** Every problem in `daily/` has the sheet's own pattern name beside it in `code` formatting; patterns.md maps each of those ~40 names to the trigger that tells you it applies.

You are not memorising 179 problems. You are learning ~40 patterns and training yourself to recognise the trigger inside 60 seconds. Name the pattern out loud before coding in interviews — it's scored, and it buys you a correction if you're wrong.

## Week-by-week checklist

### Month 1 — Arrays & Binary Search · [day-wise](daily/month-01.md)

- [ ] **W1** — Arrays & hashing (13) · JS: execution context, scope, closures · DOM: events + delegation · Backend: Node + Express setup, REST basics · MC: counter, star rating, accordion
- [ ] **W2** — Arrays hard + binary search on arrays (10) · JS: `this`, call/apply/bind, prototypes · Backend: Express routing, middleware, error handling · MC: todo with filters + localStorage
- [ ] **W3** — Binary search on the answer + sliding window (12) · JS: event loop, micro/macrotask, promises + async/await · CSS I: box model, cascade, positioning, stacking · Backend: Postgres — schema, joins, indexes · MC: tabs in **vanilla JS**, modal, tooltip (a11y-correct)
- [ ] **W4** — Counting windows + recursion & backtracking (13) · JS: polyfills I — map/filter/reduce, call/apply/bind, debounce, throttle · Backend: auth with JWT, bcrypt, refresh tokens · MC: autocomplete — debounce, cancel, keyboard nav
- [ ] **Milestone:** 49 problems · sliding-window template from memory · polyfill set I written from memory · a running Express+Postgres API with auth

### Month 2 — Strings, Linked List, Recursion, Stacks · [day-wise](daily/month-02.md)

- [ ] **W5** — Linked list (13) · JS: polyfills II — Promise.all/race/allSettled/any, deep clone, EventEmitter, curry, memoize · SD: rendering strategies CSR/SSR/SSG/ISR · MC: infinite scroll + intersection observer
- [ ] **W6** — Linked list hard + stack/queue design, LRU/LFU (11) · React: reconciliation, keys, render cycle, StrictMode · CSS II: flexbox, grid, responsive · SD: browser rendering pipeline, CRP, reflow/repaint · MC: virtualized list (windowing, from scratch)
- [ ] **W7** — Monotonic stack + greedy (11) · React: hooks deep — useEffect timing/cleanup, useRef, custom hooks · HTML: semantics, forms, script loading · SD: HTTP caching, CORS, cookies · MC: nested comments (recursive render)
- [ ] **W8** — Intervals + heaps (8) · React: state — Context, Redux Toolkit, Zustand, server state · SD: XSS, CSRF, CSP, auth on the frontend · MC: kanban board with drag-drop
- [ ] **Milestone:** 92 problems · all polyfills cold · React model articulate out loud

### Month 3 — Trees, Heaps, Greedy, Sliding Window · **project starts** · [day-wise](daily/month-03.md)

- [ ] **W9** — Binary trees: level-order views, DFS, LCA (13) · React: perf — memo, useMemo/useCallback, lazy, Suspense, Profiler · SD: **design an autocomplete** · **Project:** scaffold Vite+TS+React, routing, design tokens · **BE:** tested products API
- [ ] **W10** — Tree hard + BST (12) · React: forms, error boundaries, portals · SD: **design a product listing page at scale** · **Project:** product grid + virtualized list (reuse W6) · **BE:** Redis cache on the products API
- [ ] **W11** — Graphs I: BFS, DFS, cycle detection, topo sort (12) · TypeScript in React · SD: **design an image-heavy feed** · **LLD:** the method + parking lot · **Project:** filters, sort, URL-synced state · **BE:** indexed filter queries
- [ ] **W12** — Graphs II: shortest paths, SCC, bridges (12) · Testing: RTL, Jest, MSW · SD: **design a component library** · **LLD:** Splitwise · **Project:** cart + optimistic UI with rollback · **BE:** versioned cart API
- [ ] **Milestone:** 141 problems · you can pick BFS / DFS / topo / Dijkstra / Bellman-Ford in the first minute · project running on your own API · 2 LLD problems coded

### Month 4 — Graphs & DP · [day-wise](daily/month-04.md)

- [ ] **W13** — DSU / MST + DP I: linear, grid, stocks (11) · Build tooling: Vite/Webpack, code splitting, tree shaking · SD: **design a chat app (frontend)** — WebSocket, ordering, reconnect · **LLD:** snake & ladder · **Project:** WebSocket order tracking · **BE:** queue + worker feeding it
- [ ] **W14** — DP II: knapsack, string DP, LIS (11) · Core Web Vitals — LCP, INP, CLS and the fix for each · SD: **caching strategies** — HTTP, SW, in-memory, stale-while-revalidate · **LLD:** in-memory cache with pluggable eviction · **Project:** offline cart with IndexedDB + sync on reconnect · **BE:** idempotent sync endpoint
- [ ] **W15** — Partition DP, tries, string matching (9) · Bundle analysis + a real perf pass · SD: **design a checkout flow** — idempotency, failure states · **LLD:** movie seat booking · **Project:** checkout + error/edge states · **BE:** checkout in one transaction, last-item race
- [ ] **W16** — KMP, bit manipulation, maths — **sheet complete** (7) · Accessibility: keyboard, ARIA, screen reader pass · SD: **HLD basics** — LB, cache, DB scaling, CDN, queues · **LLD:** rate limiter · **Project:** perf pass — Lighthouse before/after, measured · **BE:** Docker Compose
- [ ] **Milestone:** all 179 sheet problems · project deployed · **perf numbers written down** · 6 LLD problems coded

### Month 5 — Design, Polish, First Applications · [day-wise](daily/month-05.md)

- [ ] **W17** — Resume rewrite (action → metric → tech) · project README as a design doc · LinkedIn/GitHub cleanup · **start applying: mid-tier, 15/week** · DSA: revision only
- [ ] **W18** — System design drills, 1/day out loud, timed — alternating frontend and backend HLD · behavioural stories in STAR (5 of them) · mine your day job for ownership numbers
- [ ] **W19** — CS fundamentals — DBMS (3 days, incl. SQL by hand), networks, one-day OS skim · backend HLD drills: URL shortener, rate limiter
- [ ] **W20** — Weak-pattern revision from `notes.md` · 2 full mock loops · first real interviews landing
- [ ] **Milestone:** applying actively · resume converting · all 179 sheet problems revised at least twice

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
