# Month 4 — DSU, DP, Tries, Strings, Bits · Project Finishes (Weeks 13–16)

Sheet: **[Striver's 180 – Master DSA Patterns](https://takeuforward.org/prep-hub/strivers-180-master-dsa-patterns?page=sheet)**, taken in sheet order — sections **Graphs (DSU / MST) → Mathematics** this month. The tag in `code` beside each problem is the pattern name the sheet gives it; [patterns.md](../patterns.md) maps each one to its trigger.

---

## Week 13 — DSU / MST + DP I

> Monday and Tuesday close out graphs with `Disjoint Set Union`. DP starts Wednesday, and every DP problem is the same three steps: **recursion → memoise → tabulate.** Never start at tabulation in an interview; state the recursion out loud, then optimise. Interviewers score that progression.

### Mon

- **DSA:**
  - Disjoint Set `Disjoint Set Union` + `Greedy Choice` *(union by size + path compression, from scratch)*
  - Find the MST weight `Disjoint Set Union` + `Greedy Choice` *(Kruskal: sort edges, union if not already connected — know Prim's too)*
  - Number of operations to make network connected `Disjoint Set Union`
- **Tooling:** Vite/Webpack — what a bundler does, dev vs prod, source maps
- **Rev:** Bridges in graph `Graph DFS / Connected Components`

### Tue

- **Project:** WebSocket order tracking — connection, message handling, UI updates
- **BE:** order status changes go through a queue (BullMQ on Redis) → worker → WebSocket push
- **DSA:**
  - Number of islands II `Disjoint Set Union` *(online queries — union with the four neighbours)*
  - Making a large island `Disjoint Set Union`
- **SD:** **design a chat app (frontend)** — polling vs SSE vs WebSocket (and when polling is correct), ordering, dedupe, reconnect, offline queue

### Wed

- **DSA:**
  - Frog jump with K distances `Linear DP`
  - House robber `Linear DP`
  - Ninja's training `Multi-State DP` *(the previous day's choice is part of the state)*
- **Tooling:** code splitting, dynamic import, tree shaking — verify with the bundle analyzer
- **Rev:** Disjoint Set from scratch `Disjoint Set Union`

### Thu

- **Project:** reconnect with backoff, message ordering, stale-state recovery
- **DSA:** Unique paths II `Grid DP`
- **LLD:** **snake & ladder** — board, players, dice, turn loop; keep the rules swappable

### Fri

- **DSA:**
  - Best time to buy and sell stock with transaction fees `Stock Trading DP`
  - Best time to buy and sell stock IV `Stock Trading DP` *(one state machine — holding / not holding / transactions left. Draw the states)*
- **Tooling:** analyze your project's bundle, write down the 3 biggest offenders
- **Rev:** House robber `Linear DP`

### Wknd

- *(if available)* 4h: project — order tracking end to end, live, demoable

## Week 14 — DP II: knapsack & sequences

### Mon

- **DSA:**
  - 0 and 1 Knapsack `Knapsack / Subset DP`
  - Partition a set into two subsets with minimum absolute sum difference `Knapsack / Subset DP` *(read the answer off the last row of the subset-sum table)*
  - Target sum `Knapsack / Subset DP` *(count subsets with sum `(total − target) / 2`)*
- **Perf:** Core Web Vitals — LCP, INP, CLS: what each measures, what actually fixes each
- **Rev:** Ninja's training `Multi-State DP`

### Tue

- **Project:** offline cart — IndexedDB persistence, queue mutations while offline
- **DSA:**
  - Coin change II `Knapsack / Subset DP` *(unbounded — stay on the same index after picking)*
  - Longest common subsequence `String DP`
- **SD:** **caching strategies** — HTTP, service worker, in-memory, SWR; cache invalidation; the service worker update flow and its pitfalls

### Wed

- **DSA:**
  - Longest common substring `String DP` *(the LCS table, reset to 0 on mismatch)*
  - Longest palindromic subsequence `String DP` *(LCS of the string and its reverse)*
  - Edit distance `String DP`
- **Perf:** measure your project — Lighthouse, WebPageTest, throttled real device. **Record the baseline numbers.**
- **Rev:** Target sum `Knapsack / Subset DP`

### Thu

- **Project:** sync on reconnect, conflict resolution, an honest "offline" UI state
- **BE:** idempotent sync endpoint — replaying the queued mutations twice changes nothing
- **DSA:** Wildcard matching `String DP` *(`*` matches empty, or one more character — two branches)*
- **LLD:** **in-memory cache** with pluggable eviction (LRU / LFU / TTL) — the strategy pattern on top of the LRU you wrote in W6

### Fri

- **DSA:**
  - Longest Increasing Subsequence `LIS / Sequence DP` + `Lower Bound / Upper Bound` *(n² DP, then n log n with lower bound)*
  - Number of Longest Increasing Subsequences `LIS / Sequence DP` *(carry a count array beside the length array)*
- **Perf:** fix offender #1 from the bundle analysis, re-measure
- **Rev:** Edit distance `String DP`

### Wknd

- *(if available)* 4h: mock interview #3 — frontend system design round

## Week 15 — Partition DP, Tries, String matching

> Three short, hard sections. Partition DP: *try every split point, solve both sides*. Tries: a children map plus an end-of-word flag — and the two XOR problems are the same structure over bits. String matching: Rabin-Karp, Z and KMP answer one question three ways — learn what each one precomputes.

### Mon

- **DSA:**
  - Matrix chain multiplication `Interval / Partition DP` *(try every split point, solve both sides)*
  - Palindrome partitioning II `Interval / Partition DP` *(front partition — min cuts starting from index i)*
- **Perf:** image optimization — formats, sizing, priority hints, preload
- **Rev:** Longest Increasing Subsequence `LIS / Sequence DP`

### Tue

- **Project:** checkout — multi-step form, address, payment stub, idempotency
- **BE:** order + items + stock decrement in **one Postgres transaction**, idempotency-key table
- **DSA:** Trie Implementation and Advanced Operations `Prefix Matching` *(insert / search / startsWith first, then count-words, count-prefix, erase)*
- **SD:** **design a checkout flow** — failure states, retries, double-submit, trust

### Wed

- **DSA:**
  - Longest Word with All Prefixes `Prefix Matching` *(this is what an autocomplete index is)*
  - Number of distinct substrings in a string `Prefix Matching` *(insert every suffix, count the new nodes)*
  - Maximum XOR of two numbers in an array `Bitwise XOR` *(binary trie — at each bit, walk to the opposite child if it exists)*
- **Perf:** font loading, critical CSS, render-blocking resources
- **Rev:** Matrix chain multiplication `Interval / Partition DP`

### Thu

- **Project:** every failure state in checkout — network, validation, decline, timeout — each one reported (error boundary + logging)
- **BE:** two checkouts racing for the last item — make one lose cleanly (`SELECT … FOR UPDATE`)
- **DSA:** Maximum Xor with an element from an array `Bitwise XOR` *(offline: sort the numbers and the queries, insert as the limit grows)*
- **LLD:** **movie seat booking** — shows, seats, holds that expire, two users picking the same seat

### Fri

- **DSA:**
  - Rabin Karp Algorithm `String Matching / Rolling Hash` *(rolling hash — and what you do on a hash collision)*
  - Z function `String Matching / Rolling Hash`
- **Perf:** fix offenders #2 and #3, re-measure, write the delta down
- **Rev:** Trie Implementation, from memory `Prefix Matching`

### Wknd

- *(if available)* 4h: project — full happy path demoable in 3 minutes, no excuses

## Week 16 — Strings, Bits, Maths + Project Polish

> The sheet ends Wednesday. Thursday and Friday are the first days of revision, not a break.

### Mon

- **DSA:**
  - KMP Algorithm or LPS array `String Matching / Rolling Hash`
  - Shortest Palindrome `String Matching / Rolling Hash` *(LPS of `s + '#' + reverse(s)`)*
- **a11y:** keyboard navigation audit of the whole project, fix what fails
- **Rev:** Wildcard matching `String DP`

### Tue

- **Project:** final perf pass — code splitting, prefetch, caching headers. **Lighthouse after-numbers.**
- **BE:** Dockerfile + Compose for API, Postgres and Redis — `docker compose up` runs the whole thing
- **DSA:**
  - Power Set Bit Manipulation `Bitmask Enumeration` *(iterate `0 … 2ⁿ − 1`; bit i set means take element i)*
  - Single Number - II `Basic Traversal / Scan` *(count each bit mod 3)*
- **SD:** **HLD basics** — load balancers, caching layers, DB replication/sharding, CDN, queues

### Wed

- **DSA:**
  - Single Number - III `Bitwise XOR` *(XOR everything, then split on the lowest set bit)*
  - Find the repeating and missing number `Sort and Scan` *(sum + sum-of-squares equations, or the XOR split you just wrote)*
  - Print all primes till N `Number Theory` *(Sieve of Eratosthenes)*
- **a11y:** ARIA, focus management, screen reader pass on cart + checkout
- **Rev:** KMP Algorithm or LPS array `String Matching / Rolling Hash`

### Thu

- **Project:** README as a **design doc** — architecture, decisions, rejected alternatives, perf before/after table
- **DSA:** **sheet complete — 179 / 179.** Cold re-solve 2 problems marked `!!` in `notes.md`
- **LLD:** **rate limiter** — token bucket and sliding window behind one interface *(the HLD versions of this and the URL shortener are in Week 19)*

### Fri

- **DSA:** full-sheet sweep — 3 cold re-solves picked at random: one graph, one DP, one binary-search-on-answer
- **a11y + polish:** dark mode, responsive audit, final visual pass
- **Rev:** Maximum Xor with an element from an array `Bitwise XOR`

### Wknd

- *(if available)* 4h: **Month 4 audit** — deploy final, verify the perf numbers are real and reproducible

---

**Month 4 exit check**
- [ ] **All 179 sheet problems committed**, including a trie and a DSU written from memory
- [ ] You state recursion → memo → tabulation as a progression, out loud, without prompting
- [ ] Project deployed, demo path rehearsed
- [ ] Backend demoable: a cache hit, a queue-driven order update, the last-item race, `docker compose up`
- [ ] 6 LLD problems coded, each with its class diagram kept
- [ ] **Perf numbers written down** — e.g. "LCP 4.1s → 1.3s via X, Y, Z"
- [ ] README reads like a design doc, not a feature list
