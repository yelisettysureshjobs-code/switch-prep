# Month 4 — Graphs & DP · Project Finishes (Weeks 13–16)

Pattern tags in `code` — see [patterns.md](../patterns.md).

---

## Week 13 — Graphs I: traversal

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Representations (adj list vs matrix) · BFS `bfs-grid` · DFS `dfs-grid` · Number of provinces `dfs-grid` *(then redo with `dsu`)*<br>**Tooling:** Vite/Webpack — what a bundler does, dev vs prod, source maps<br>**Rev:** Candy `sort-then-greedy` |
| **Tue** | **Project:** WebSocket order tracking — connection, message handling, UI updates · **BE:** order status changes go through a queue (BullMQ on Redis) → worker → WebSocket push<br>**DSA:** Number of islands `dfs-grid` · Flood fill `dfs-grid`<br>**SD:** **design a chat app (frontend)** — polling vs SSE vs WebSocket (and when polling is correct), ordering, dedupe, reconnect, offline queue |
| **Wed** | **DSA:** Rotten oranges `multi-source-bfs` *(push all sources before the loop — that's the entire pattern)* · Detect cycle undirected `cycle-undirected` · directed `cycle-directed` · Surrounded regions `dfs-grid` *(invert: start from the boundary)*<br>**Tooling:** code splitting, dynamic import, tree shaking — verify with the bundle analyzer<br>**Rev:** Number of islands `dfs-grid` |
| **Thu** | **Project:** reconnect with backoff, message ordering, stale-state recovery<br>**DSA:** Distance of nearest 0 `multi-source-bfs` · Number of enclaves `dfs-grid`<br>**LLD:** **snake & ladder** — board, players, dice, turn loop; keep the rules swappable |
| **Fri** | **DSA:** Word ladder I & II `bfs-grid` *(the graph is implicit — neighbours are one-letter edits)* · Bipartite check `bipartite`<br>**Tooling:** analyze your project's bundle, write down the 3 biggest offenders<br>**Rev:** Rotten oranges `multi-source-bfs` |
| **Wknd** | *(if available)* 4h: project — order tracking end to end, live, demoable |

## Week 14 — Graphs II: ordering & shortest paths

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Topological sort, Kahn + DFS `topo-sort` · Cycle detection via topo `topo-sort` · Course schedule I & II `topo-sort`<br>**Perf:** Core Web Vitals — LCP, INP, CLS: what each measures, what actually fixes each<br>**Rev:** Word ladder I `bfs-grid` |
| **Tue** | **Project:** offline cart — IndexedDB persistence, queue mutations while offline<br>**DSA:** Alien dictionary `topo-sort` · Eventual safe states `cycle-directed`<br>**SD:** **caching strategies** — HTTP, service worker, in-memory, SWR; cache invalidation; the service worker update flow and its pitfalls |
| **Wed** | **DSA:** Shortest path in DAG `topo-sort` · unweighted graph `bfs-grid` · Dijkstra `dijkstra` *(PQ, and know why plain BFS breaks on weights)*<br>**Perf:** measure your project — Lighthouse, WebPageTest, throttled real device. **Record the baseline numbers.**<br>**Rev:** Course schedule II `topo-sort` |
| **Thu** | **Project:** sync on reconnect, conflict resolution, an honest "offline" UI state · **BE:** idempotent sync endpoint — replaying the queued mutations twice changes nothing<br>**DSA:** Shortest path in binary maze `bfs-0-1` · Path with minimum effort `dijkstra`<br>**LLD:** **in-memory cache** with pluggable eviction (LRU / LFU / TTL) — the strategy pattern on top of the LRU you wrote in W8 |
| **Fri** | **DSA:** Cheapest flights within K stops `bellman-ford` *(Dijkstra fails here — understand exactly why)* · Network delay time `dijkstra` · Number of ways to arrive `dijkstra` · Accounts merge `dsu`<br>**Perf:** fix offender #1 from the bundle analysis, re-measure<br>**Rev:** Dijkstra from scratch `dijkstra` |
| **Wknd** | *(if available)* 4h: mock interview #3 — frontend system design round |

## Week 15 — DP I

> Every DP problem, same three steps: **recursion → memoise → tabulate.** Never start at tabulation in an interview; state the recursion out loud, then optimise. Interviewers score that progression.

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Climbing stairs `dp-1d` · Frog jump + K distance `dp-1d` · House robber I & II `dp-1d`<br>**Perf:** image optimization — formats, sizing, priority hints, preload<br>**Rev:** Cheapest flights within K stops `bellman-ford` |
| **Tue** | **Project:** checkout — multi-step form, address, payment stub, idempotency · **BE:** order + items + stock decrement in **one Postgres transaction**, idempotency-key table<br>**DSA:** Ninja's training `dp-grid` · Grid unique paths I & II `dp-grid`<br>**SD:** **design a checkout flow** — failure states, retries, double-submit, trust |
| **Wed** | **DSA:** Min path sum `dp-grid` · Triangle `dp-grid` · Falling path sum `dp-grid` · Cherry pickup `dp-grid` *(two agents = two indices in the state)*<br>**Perf:** font loading, critical CSS, render-blocking resources<br>**Rev:** House robber II `dp-1d` |
| **Thu** | **Project:** every failure state in checkout — network, validation, decline, timeout — each one reported (error boundary + logging) · **BE:** two checkouts racing for the last item — make one lose cleanly (`SELECT … FOR UPDATE`)<br>**DSA:** Subset sum equal to K `dp-pick-nonpick` · Partition equal subset sum `dp-pick-nonpick`<br>**LLD:** **movie seat booking** — shows, seats, holds that expire, two users picking the same seat |
| **Fri** | **DSA:** Count subsets with sum K `dp-pick-nonpick` · Partition with min difference `dp-pick-nonpick` · Target sum `dp-pick-nonpick`<br>**Perf:** fix offenders #2 and #3, re-measure, write the delta down<br>**Rev:** Grid unique paths II `dp-grid` |
| **Wknd** | *(if available)* 4h: project — full happy path demoable in 3 minutes, no excuses |

## Week 16 — DP II + Project Polish

| Day | Plan |
|---|---|
| **Mon** | **DSA:** 0/1 knapsack `dp-knapsack` · Unbounded knapsack `dp-knapsack` · Coin change I & II `dp-knapsack` *(I minimises, II counts — same table, different recurrence)*<br>**a11y:** keyboard navigation audit of the whole project, fix what fails<br>**Rev:** Partition equal subset sum `dp-pick-nonpick` |
| **Tue** | **Project:** final perf pass — code splitting, prefetch, caching headers. **Lighthouse after-numbers.** · **BE:** Dockerfile + Compose for API, Postgres and Redis — `docker compose up` runs the whole thing<br>**DSA:** Rod cutting `dp-knapsack` · Longest common subsequence `dp-strings`<br>**SD:** **HLD basics** — load balancers, caching layers, DB replication/sharding, CDN, queues |
| **Wed** | **DSA:** Print LCS `dp-strings` · Longest common substring `dp-strings` · Longest palindromic subsequence `dp-strings` · Edit distance `dp-strings`<br>**a11y:** ARIA, focus management, screen reader pass on cart + checkout<br>**Rev:** Coin change II `dp-knapsack` |
| **Thu** | **Project:** README as a **design doc** — architecture, decisions, rejected alternatives, perf before/after table<br>**DSA:** Wildcard matching `dp-strings` · Distinct subsequences `dp-strings`<br>**LLD:** **rate limiter** — token bucket and sliding window behind one interface *(the HLD versions of this and the URL shortener are in Week 19)* |
| **Fri** | **DSA:** Stocks I–VI `dp-state-machine` *(one state machine, six variations — draw the states)* · LIS, n² then n log n `dp-lis` · Longest bitonic `dp-lis`<br>**a11y + polish:** dark mode, responsive audit, final visual pass<br>**Rev:** Edit distance `dp-strings` |
| **Wknd** | *(if available)* 4h: **Month 4 audit** — deploy final, verify the perf numbers are real and reproducible |

---

**Month 4 exit check**
- [ ] ~194 problems committed
- [ ] You state recursion → memo → tabulation as a progression, out loud, without prompting
- [ ] Project deployed, demo path rehearsed
- [ ] Backend demoable: a cache hit, a queue-driven order update, the last-item race, `docker compose up`
- [ ] 6 LLD problems coded, each with its class diagram kept
- [ ] **Perf numbers written down** — e.g. "LCP 4.1s → 1.3s via X, Y, Z"
- [ ] README reads like a design doc, not a feature list
