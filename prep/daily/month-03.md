# Month 3 — Trees, Heaps, Greedy, Sliding Window · Project Starts (Weeks 9–12)

From here the Tue/Thu machine-coding block **is** the project. No more orphan widgets.

**The project:** a commerce app shaped like the companies you're targeting — product listing → cart → checkout → order tracking, backed by your own Node/Postgres API. Non-negotiable features: virtualized list over 10k+ items, optimistic cart, offline cart via IndexedDB, WebSocket order tracking, and a measured performance pass.

**The backend is half the project, not a stub.** Tue/Thu project blocks from here carry a **BE** item: tested endpoints, a Redis cache, a queue-driven order pipeline, checkout in one transaction, Docker. If a block overruns, cut frontend polish, never the BE item.

**LLD starts Week 11.** The Thursday SD slot becomes one low-level design problem a week through Week 16 — method in [how-to-study.md](../how-to-study.md).

Pattern tags in `code` — see [patterns.md](../patterns.md).

---

## Week 9 — Binary Trees

> **RADIO gets learned this week, Tuesday, in 30 minutes** — then applied to every design question from here to Month 5. Don't spend longer on the framework itself; it's five buckets. The difficulty is what fills them, and that only comes from the eight design questions in Weeks 9–16.

| Day | Plan |
|---|---|
| **Mon** | **DSA:** All 3 traversals, recursive + iterative `dfs-recursive` · Level order `bfs-level` · Zigzag `bfs-level`<br>**React:** perf — `React.memo`, `lazy`, `Suspense`, Profiler on your own MC builds<br>**Rev:** LRU cache `hashmap-dll` |
| **Tue** | **Project:** scaffold — Vite + React + TS, routing, design tokens, layout shell<br>**DSA:** Height `dfs-recursive` · Balanced tree check `dfs-bottom-up` *(return height AND validity — the whole point of bottom-up)*<br>**SD (first half, 30 min):** learn **RADIO** — Requirements → Architecture → Data model → Interface → Optimizations (see [how-to-study.md](../how-to-study.md)). Write it on a card and keep it visible for every design answer from here on.<br>**SD (second half, 30 min):** **design an autocomplete** — your first attempt, out loud, RADIO in order. It will be bad. That's the point — Thursday is where it gets fixed. |
| **Wed** | **DSA:** Diameter `dfs-bottom-up` · Max path sum `dfs-bottom-up` *(return best-downward, record best-through — the key distinction)* · Identical trees `dfs-recursive`<br>**React:** `useMemo` / `useCallback` — when they help, why memo usually does nothing, and how to find the render actually costing you<br>**Rev:** Level order traversal `bfs-level` |
| **Thu** | **Project:** API — products endpoint with pagination + filtering, seeded 10k rows · **BE:** integration tests for it (supertest against a test database)<br>**DSA:** Boundary traversal `path-tracking` · Top view `bfs-level` *(BFS + horizontal distance map)*<br>**SD:** critique Tuesday's autocomplete answer against a reference. Score yourself per RADIO bucket — **which letter did you skip?** Almost everyone skips R and jumps to A. Write the gap down; it's the same gap next time. |
| **Fri** | **DSA:** Bottom view `bfs-level` · Right/left view `bfs-level` · Symmetric tree `dfs-recursive`<br>**React:** code splitting by route, `Suspense` boundaries, loading UX<br>**Rev:** Diameter `dfs-bottom-up` |
| **Wknd** | *(if available)* 4h: project integration — grid rendering real API data end to end |

## Week 10 — BST + Tree Hard

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Search/insert/delete in BST `bst-property` · Validate BST `bst-property` *(pass min/max bounds down)* · Kth smallest `bst-property` *(inorder is sorted)*<br>**React:** forms — controlled inputs at scale, validation, react-hook-form patterns<br>**Rev:** Max path sum `dfs-bottom-up` |
| **Tue** | **Project:** product grid + drop in your virtualized list from Week 6<br>**DSA:** LCA in BST `bst-property` · Inorder successor `bst-property`<br>**SD:** **design a product listing page at scale** — pagination vs infinite, filters, SEO, images |
| **Wed** | **DSA:** Root-to-node path `path-tracking` · LCA in binary tree `dfs-bottom-up` · Max width `bfs-level` *(index the nodes)*<br>**React:** error boundaries, portals, suspense fallbacks, graceful degradation<br>**Rev:** Validate BST `bst-property` |
| **Thu** | **Project:** skeleton, empty and error states — all of them, properly · **BE:** Redis cache-aside on the products endpoint — TTL, invalidate on write, measure the hit<br>**DSA:** Nodes at distance K `tree-to-graph` · Time to burn tree `tree-to-graph` *(same trick: parent pointers, then BFS)*<br>**SD:** SEO for SPAs — meta, structured data, why SSR matters commercially |
| **Fri** | **DSA:** Count nodes in complete tree `bst-property` · Construct tree from traversals `tree-construction` · Serialize/deserialize `bfs-level`<br>**React:** compound components, render props, headless patterns<br>**Rev:** LCA in binary tree `dfs-bottom-up` |
| **Wknd** | *(if available)* 4h: mock interview #2 — machine coding round, strict 90 min |

## Week 11 — Heaps + Greedy

> Merged deliberately: both are small topics and they overlap — half of "greedy" is really *greedy plus a heap*.

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Implement min/max heap + heapify `greedy-heap` · Kth largest element `top-k-heap` *(size-K heap, not a sort)* · K most frequent `top-k-heap` + `freq-map`<br>**TS:** generics, discriminated unions, typing props and hooks properly<br>**Rev:** Construct tree from traversals `tree-construction` |
| **Tue** | **Project:** filters + sort, state synced to URL, shareable links · **BE:** the filter and sort queries hit indexes — prove it with EXPLAIN<br>**DSA:** Merge K sorted lists `k-way-merge` · Task scheduler `greedy-heap`<br>**SD:** **design an image-heavy feed** — CDN, srcset, lazy load, LQIP, layout stability · last 15 min: critique it against a reference, write the tradeoffs down |
| **Wed** | **DSA:** Median from data stream `two-heaps` · Fractional knapsack `sort-then-greedy` *(sort by value/weight ratio)* · N meetings in a room `interval-scheduling` · Implement Trie — insert / search / startsWith `trie`<br>**TS:** utility types, `satisfies`, narrowing, typing your API layer<br>**Rev:** Kth largest element `top-k-heap` |
| **Thu** | **Project:** image pipeline — responsive images, lazy loading, zero CLS<br>**DSA:** Job sequencing `sort-then-greedy` · Non-overlapping intervals `interval-scheduling` *(sort by **end**, not start — this is the whole trick)*<br>**LLD:** learn the method (30 min — [how-to-study.md](../how-to-study.md)), then **parking lot** with a reference open. Entities and interfaces first, code second |
| **Fri** | **DSA:** Candy `sort-then-greedy` *(two passes, left then right)* · Jump game I & II `sort-then-greedy` · Gas station `sort-then-greedy`<br>**TS:** convert one earlier MC build fully to TS, no `any`<br>**Rev:** Merge K sorted lists `k-way-merge` |
| **Wknd** | *(if available)* 4h: project — search across products, wired to the autocomplete you already built |

## Week 12 — Sliding Window, Two Pointers, Bit Manipulation

> The week I nearly left out. `window-variable` is asked constantly at 2 YOE and it's one template: **expand right, shrink left while the window is invalid, record the answer.** Write that template once and 10 of these collapse into it.

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Max sum subarray of size K `window-fixed` · Longest substring without repeating characters `window-hashmap` · Longest with at most K distinct `window-hashmap` · Longest word with all prefixes `trie` *(last week's trie again — this is what an autocomplete index is)*<br>**Testing:** RTL basics — query priorities, user-event, testing behaviour not implementation<br>**Rev:** Median from data stream `two-heaps` |
| **Tue** | **Project:** cart — add/remove/quantity, **optimistic UI with rollback on failure**<br>**DSA:** Fruit into baskets `window-hashmap` *(it's "at most 2 distinct" in a costume)* · Longest repeating character replacement `window-hashmap`<br>**SD:** **design a component library** — theming, tokens, a11y contract, versioning |
| **Wed** | **DSA:** Binary subarrays with sum `window-variable` · Count nice subarrays `window-variable` · Number of substrings containing all three characters `window-variable`<br>**Testing:** MSW for API mocking, testing async UI, testing custom hooks<br>**Rev:** Longest substring without repeating `window-hashmap` |
| **Thu** | **Project:** cart persistence + server sync, conflict handling · **BE:** cart endpoints with a version column — reject stale writes with 409<br>**DSA:** Max points from cards `window-fixed` *(fixed window on the complement)* · Sliding window maximum `window-deque`<br>**LLD:** **Splitwise** — users, groups, expenses, split strategies (equal / exact / percent), simplify debts |
| **Fri** | **DSA:** Minimum window substring `window-hashmap` *(the hard one — worth a full hour)* · Single number I / II / III `xor-trick` · Count set bits, power of two `bit-basics` · Subsets via bitmask `bitmask-subsets`<br>**Testing:** write tests for the cart's optimistic path **and** its rollback path<br>**Rev:** Sliding window maximum `window-deque` |
| **Wknd** | *(if available)* 4h: **Month 3 audit** — 8 cold re-solves + deploy the project publicly for the first time |

---

**Month 3 exit check**
- [ ] ~146 problems committed, including a trie written from memory
- [ ] The `window-variable` template written from memory, and you can name which of the 10 problems it solves
- [ ] Project live: listing → filters → cart, on your own API
- [ ] Optimistic UI with a rollback path you can demo failing
- [ ] Products API tested and cached; cart API rejects stale writes
- [ ] 2 LLD problems coded — parking lot, Splitwise
- [ ] RADIO written from memory, and used in all 4 design answers
- [ ] 4 frontend system design problems answered out loud and critiqued
