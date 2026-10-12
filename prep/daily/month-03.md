# Month 3 — Trees & Graphs · Project Starts (Weeks 9–12)

From here the Tue/Thu machine-coding block **is** the project. No more orphan widgets.

**The project:** a commerce app shaped like the companies you're targeting — product listing → cart → checkout → order tracking, backed by your own Node/Postgres API. Non-negotiable features: virtualized list over 10k+ items, optimistic cart, offline cart via IndexedDB, WebSocket order tracking, and a measured performance pass.

**The backend is half the project, not a stub.** Tue/Thu project blocks from here carry a **BE** item: tested endpoints, a Redis cache, a queue-driven order pipeline, checkout in one transaction, Docker. If a block overruns, cut frontend polish, never the BE item.

**LLD starts Week 11.** The Thursday SD slot becomes one low-level design problem a week through Week 16 — method in [how-to-study.md](../how-to-study.md).

Sheet: **[Striver's 180 – Master DSA Patterns](https://takeuforward.org/prep-hub/strivers-180-master-dsa-patterns?page=sheet)**, taken in sheet order — sections **Binary Trees → Graphs (Shortest Path)** this month. The tag in `code` beside each problem is the pattern name the sheet gives it; [patterns.md](../patterns.md) maps each one to its trigger.

---

## Week 9 — Binary Trees

> **RADIO gets learned this week, Tuesday, in 30 minutes** — then applied to every design question from here to Month 5. Don't spend longer on the framework itself; it's five buckets. The difficulty is what fills them, and that only comes from the eight design questions in Weeks 9–16.

### Mon

- **DSA:**
  - *warm-up, 15 min:* the three DFS traversals, recursive — the sheet assumes them
  - Level Order Traversal `BFS / Level Order`
  - Right/Left View of BT `BFS / Level Order`
  - Top View of BT `BFS / Level Order` *(BFS + horizontal-distance map)*
- **React:** perf — `React.memo`, `lazy`, `Suspense`, Profiler on your own MC builds
- **Rev:** LRU Cache `Simulation / Implementation`

### Tue

- **Project:** scaffold — Vite + React + TS, routing, design tokens, layout shell
- **DSA:**
  - Maximum Width of BT `BFS / Level Order` *(index the nodes like a heap)*
  - Vertical Order Traversal `BFS / Level Order`
- **SD (first half, 30 min):** learn **RADIO** — Requirements → Architecture → Data model → Interface → Optimizations (see [how-to-study.md](../how-to-study.md)). Write it on a card and keep it visible for every design answer from here on.
- **SD (second half, 30 min):** **design an autocomplete** — your first attempt, out loud, RADIO in order. It will be bad. That's the point — Thursday is where it gets fixed.

### Wed

- **DSA:**
  - Print root to leaf path in BT `Tree DFS / Recursive Decomposition` *(carry the path down, pop on the way back)*
  - Diameter of Binary Tree `Tree DFS / Recursive Decomposition` *(return height, record best-through — one pass)*
  - LCA in BT `Tree DFS / Recursive Decomposition`
- **React:** `useMemo` / `useCallback` — when they help, why memo usually does nothing, and how to find the render actually costing you
- **Rev:** Top View of BT `BFS / Level Order`

### Thu

- **Project:** API — products endpoint with pagination + filtering, seeded 10k rows
- **BE:** integration tests for it (supertest against a test database)
- **DSA:**
  - Print all nodes at a distance of K in BT `Tree DFS / Recursive Decomposition`
  - Minimum time taken to burn the BT from a given Node `Tree DFS / Recursive Decomposition` *(same trick for both: parent pointers, then it's a BFS)*
- **SD:** critique Tuesday's autocomplete answer against a reference. Score yourself per RADIO bucket — **which letter did you skip?** Almost everyone skips R and jumps to A. Write the gap down; it's the same gap next time.

### Fri

- **DSA:**
  - Boundary Traversal `Tree DFS / Recursive Decomposition`
  - Construct a BT from Preorder and Inorder `Tree DFS / Recursive Decomposition` *(preorder gives the root, inorder gives the split)*
  - Maximum path sum `Tree DFS / Recursive Decomposition` *(return best-downward, record best-through — the key distinction)*
- **React:** code splitting by route, `Suspense` boundaries, loading UX
- **Rev:** Diameter of Binary Tree `Tree DFS / Recursive Decomposition`

### Wknd

- *(if available)* 4h: project integration — grid rendering real API data end to end

## Week 10 — Tree Hard + BST

### Mon

- **DSA:**
  - Morris Inorder Traversal `Tree DFS / Recursive Decomposition` *(threaded tree — O(1) space)*
  - Serialize and De-serialize BT `Tree DFS / Recursive Decomposition`
  - Insert a given node in BST `Lower Bound / Upper Bound`
- **React:** forms — controlled inputs at scale, validation, react-hook-form patterns
- **Rev:** Maximum path sum `Tree DFS / Recursive Decomposition`

### Tue

- **Project:** product grid + drop in your virtualized list from Week 6
- **DSA:**
  - Delete a node in BST `Lower Bound / Upper Bound` *(three cases — the two-children one is the question)*
  - LCA in BST `Tree DFS / Recursive Decomposition` *(the split point)*
- **SD:** **design a product listing page at scale** — pagination vs infinite, filters, SEO, images

### Wed

- **DSA:**
  - Check if a tree is a BST or not `Tree DFS / Recursive Decomposition` *(pass min / max bounds down)*
  - Kth Smallest and Largest element in BST `Tree DFS / Recursive Decomposition` *(inorder is sorted)*
  - Inorder successor and predecessor in BST `Lower Bound / Upper Bound`
- **React:** error boundaries, portals, suspense fallbacks, graceful degradation
- **Rev:** LCA in BT `Tree DFS / Recursive Decomposition`

### Thu

- **Project:** skeleton, empty and error states — all of them, properly
- **BE:** Redis cache-aside on the products endpoint — TTL, invalidate on write, measure the hit
- **DSA:**
  - Construct a BST from a preorder traversal `Tree DFS / Recursive Decomposition` *(carry an upper bound — O(n))*
  - Two sum in BST `Two Pointer` *(two BST iterators: `next` and `before`)*
- **SD:** SEO for SPAs — meta, structured data, why SSR matters commercially

### Fri

- **DSA:**
  - Correct BST with two nodes swapped `Tree DFS / Recursive Decomposition` *(inorder: find the one or two inversions)*
  - Largest BST in Binary Tree `Tree DFS / Recursive Decomposition` *(return size, min and max upward)*
- **React:** compound components, render props, headless patterns
- **Rev:** Construct a BT from Preorder and Inorder `Tree DFS / Recursive Decomposition`

### Wknd

- *(if available)* 4h: mock interview #2 — machine coding round, strict 90 min

## Week 11 — Graphs I: traversal & topological order

> The sheet assumes you can already write BFS and DFS over an adjacency list. If you can't do both from memory in 10 minutes, do that first on Monday — everything in the next two weeks is one of those two skeletons with a twist.

### Mon

- **DSA:**
  - Flood fill algorithm `Graph DFS / Connected Components`
  - Number of islands `Graph DFS / Connected Components`
  - Rotten Oranges `BFS / Level Order` *(multi-source — push every source before the loop starts; that's the entire pattern)*
- **TS:** generics, discriminated unions, typing props and hooks properly
- **Rev:** Check if a tree is a BST or not `Tree DFS / Recursive Decomposition`

### Tue

- **Project:** filters + sort, state synced to URL, shareable links
- **BE:** the filter and sort queries hit indexes — prove it with EXPLAIN
- **DSA:**
  - Distance of nearest cell having one `BFS / Level Order` *(multi-source again)*
  - Surrounded Regions `Graph DFS / Connected Components` *(invert: start from the boundary)*
- **SD:**
  - **design an image-heavy feed** — CDN, srcset, lazy load, LQIP, layout stability
  - last 15 min: critique it against a reference, write the tradeoffs down

### Wed

- **DSA:**
  - Number of distinct islands `Graph DFS / Connected Components` *(store each island's shape relative to its start cell)*
  - Detect a cycle in an undirected graph `Graph DFS / Connected Components` *(a visited neighbour that isn't the parent)*
  - Bipartite graph `Graph DFS / Connected Components` *(two-colouring)*
- **TS:** utility types, `satisfies`, narrowing, typing your API layer
- **Rev:** Rotten Oranges `BFS / Level Order`

### Thu

- **Project:** image pipeline — responsive images, lazy loading, zero CLS
- **DSA:**
  - Detect a cycle in a directed graph `Topological Ordering` *(recursion stack, not just visited — or Kahn's leaves nodes unprocessed)*
  - Topological sort or Kahn's algorithm `Topological Ordering`
- **LLD:** learn the method (30 min — [how-to-study.md](../how-to-study.md)), then **parking lot** with a reference open. Entities and interfaces first, code second

### Fri

- **DSA:**
  - Course Schedule II `Topological Ordering`
  - Alien Dictionary `Topological Ordering` *(the first differing letter of two adjacent words is an edge)*
- **TS:** convert one earlier MC build fully to TS, no `any`
- **Rev:** Number of islands `Graph DFS / Connected Components`

### Wknd

- *(if available)* 4h: project — search across products, wired to the autocomplete you already built

## Week 12 — Graphs II: shortest paths, SCC & bridges

> One question picks the algorithm: **what do the edge weights look like?** Unit weights → BFS. DAG → relax in topo order. Non-negative → Dijkstra. Negative edges or "at most K edges" → Bellman-Ford. All pairs → Floyd-Warshall. Friday is the hard tail of the section — Kosaraju, bridges, articulation points.

### Mon

- **DSA:**
  - Shortest path in undirected graph with unit weights `Shortest Path`
  - Shortest path in DAG `Shortest Path` *(relax in topological order)*
  - Dijkstra's algorithm `Shortest Path` + `Priority Queue` *(and know why plain BFS breaks on weights)*
- **Testing:** RTL basics — query priorities, user-event, testing behaviour not implementation
- **Rev:** Topological sort or Kahn's algorithm `Topological Ordering`

### Tue

- **Project:** cart — add/remove/quantity, **optimistic UI with rollback on failure**
- **DSA:**
  - Path with minimum effort `Shortest Path` *(Dijkstra where the path cost is a max, not a sum)*
  - Cheapest flight within K stops `Shortest Path` *(Dijkstra by cost fails here — understand exactly why)*
- **SD:** **design a component library** — theming, tokens, a11y contract, versioning

### Wed

- **DSA:**
  - Bellman ford algorithm `Shortest Path`
  - Floyd warshall algorithm `Shortest Path`
  - Word ladder I `BFS / Level Order` *(the graph is implicit — neighbours are one-letter edits)*
- **Testing:** MSW for API mocking, testing async UI, testing custom hooks
- **Rev:** Dijkstra's algorithm from scratch `Shortest Path`

### Thu

- **Project:** cart persistence + server sync, conflict handling
- **BE:** cart endpoints with a version column — reject stale writes with 409
- **DSA:** Word ladder II `BFS / Level Order` *(BFS to record levels, then backtrack from the end word — the full hour)*
- **LLD:** **Splitwise** — users, groups, expenses, split strategies (equal / exact / percent), simplify debts

### Fri

- **DSA:**
  - Kosaraju's algorithm `Graph DFS / Connected Components` *(finish order, reverse the graph, DFS again)*
  - Bridges in graph `Graph DFS / Connected Components`
  - Articulation point in graph `Graph DFS / Connected Components` *(one `tin` / `low` template for both — only the comparison changes)*
- **Testing:** write tests for the cart's optimistic path **and** its rollback path
- **Rev:** Cheapest flight within K stops `Shortest Path`

### Wknd

- *(if available)* 4h: **Month 3 audit** — 8 cold re-solves + deploy the project publicly for the first time

---

**Month 3 exit check**
- [ ] 141 problems committed — sheet done through Graphs → Shortest Path
- [ ] Given a graph question, you name BFS / DFS / topo sort / Dijkstra / Bellman-Ford inside the first minute, and say why
- [ ] Project live: listing → filters → cart, on your own API
- [ ] Optimistic UI with a rollback path you can demo failing
- [ ] Products API tested and cached; cart API rejects stale writes
- [ ] 2 LLD problems coded — parking lot, Splitwise
- [ ] RADIO written from memory, and used in all 4 design answers
- [ ] 4 frontend system design problems answered out loud and critiqued
