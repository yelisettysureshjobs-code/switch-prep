# Month 2 — Strings, Linked List, Recursion, Stacks (Weeks 5–8)

Pattern tags in `code` — see [patterns.md](../patterns.md).

---

## Week 5 — Strings

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Reverse words `string-simulation` · Largest odd number in string `string-simulation` · Longest common prefix `string-simulation`<br>**JS:** polyfills — `Promise.all`, `race`, `allSettled`, `any`<br>**Rev:** Book allocation `bs-on-answer` |
| **Tue** | **MC:** infinite scroll feed — IntersectionObserver, skeletons, error retry<br>**DSA:** Isomorphic strings `hashing` · Rotate string check `string-simulation`<br>**SD:** rendering strategies — CSR / SSR / SSG / ISR, when each wins, what each costs |
| **Wed** | **DSA:** Anagram check `freq-map` · Sort characters by frequency `freq-map` + `top-k-heap` · Max nesting depth `stack-simulation`<br>**JS:** polyfills — deep clone (cycles, Dates, Maps), `EventEmitter`<br>**Rev:** Painter's partition `bs-on-answer` |
| **Thu** | **MC:** finish infinite scroll — cleanup, scroll restoration<br>**DSA:** Roman ↔ Integer `string-simulation` · `atoi` `string-simulation`<br>**SD:** hydration, streaming SSR, islands — and why Myntra/Meesho care |
| **Fri** | **DSA:** Longest palindromic substring `expand-center` · Sum of beauty `freq-map` · Count with K distinct `window-hashmap` *(preview of Week 12 — solve as `exactly K = atMost(K) − atMost(K−1)`)*<br>**JS:** polyfills — `curry`, `memoize`, `once`, `pipe`/`compose`<br>**Rev:** 3 string problems from this week |
| **Wknd** | *(if available)* 4h: every polyfill from Weeks 4–5 rewritten cold in one sitting — a common round-1 filter |

## Week 6 — Linked List

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Reverse LL, iterative + recursive `pointer-reversal` · Middle of LL `slow-fast` · Detect cycle `slow-fast`<br>**React:** reconciliation, the diffing algorithm, why keys matter, list reorder bugs<br>**Rev:** Longest palindromic substring `expand-center` |
| **Tue** | **MC:** virtualized list from scratch — fixed height, then variable height (no library)<br>**DSA:** Start of cycle `slow-fast` *(know why the reset-to-head step works)* · Length of loop `slow-fast`<br>**SD:** browser rendering pipeline — parse, style, layout, paint, composite; reflow vs repaint |
| **Wed** | **DSA:** Remove Nth from end `slow-fast` + `dummy-node` · Merge two sorted LL `ll-merge` · Add two numbers `dummy-node`<br>**React:** render phase vs commit phase, batching, StrictMode double-render<br>**Rev:** Reverse LL recursive `pointer-reversal` |
| **Thu** | **MC:** finish virtualized list — overscan, scroll-to-index, resize handling<br>**DSA:** Palindrome LL `slow-fast` + `pointer-reversal` · Odd-even LL `dummy-node`<br>**SD:** what actually makes a page slow — measure, don't guess |
| **Fri** | **DSA:** Sort LL `ll-merge` + `slow-fast` *(merge sort on a list)* · Reverse in K groups `pointer-reversal` · Rotate LL `pointer-reversal`<br>**React:** controlled vs uncontrolled, lifting state, composition over prop drilling<br>**Rev:** Merge two sorted LL `ll-merge` |
| **Wknd** | *(if available)* 4h: mock interview #1 — 1 DSA + 1 JS round. Expect it to go badly; that's the point. |

## Week 7 — Recursion & Backtracking

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Subsets I & II `pick-nonpick` · Subset sum `pick-nonpick`<br>**React:** `useEffect` — dependency array, cleanup timing, the stale closure trap<br>**Rev:** Reverse in K groups `pointer-reversal` |
| **Tue** | **MC:** nested comments — recursive rendering, collapse, reply, depth limits<br>**DSA:** Combination sum I & II `backtracking-build` *(I reuses, II doesn't — the difference is one index)*<br>**SD:** HTTP caching — Cache-Control, ETag, stale-while-revalidate |
| **Wed** | **DSA:** Permutations I & II `permutation-swap` · Letter combinations `backtracking-build`<br>**React:** `useMemo` / `useCallback` — when they help, when they're noise, how to prove it<br>**Rev:** Combination sum I `backtracking-build` |
| **Thu** | **MC:** finish nested comments — optimistic add/delete, edit in place<br>**DSA:** Palindrome partitioning `backtracking-build` + `expand-center`<br>**SD:** cookies, SameSite, CORS preflight, credentials |
| **Fri** | **DSA:** N-Queens `board-backtracking` · Rat in a maze `board-backtracking` · Word search `board-backtracking`<br>**React:** `useRef`, `useLayoutEffect`, custom hooks — extract 3 from your MC builds<br>**Rev:** Permutations `permutation-swap` |
| **Wknd** | *(if available)* 4h: refactor all MC builds into a shared component folder — this becomes the project's UI kit |

## Week 8 — Stacks & Monotonic Stack

> `monotonic-stack` is the pattern of the week, and one of the highest-yield in the whole sheet. Trigger: *next/previous greater/smaller*. Trapping rainwater, histogram and subarray minimums all reduce to it — solve them in that order and you'll see the reduction.

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Implement stack/queue, array + LL `stack-simulation` · Valid parentheses `stack-simulation` · Min stack `stack-aux`<br>**React:** Context — cost, re-render behaviour, when it's the wrong tool<br>**Rev:** N-Queens `board-backtracking` |
| **Tue** | **MC:** kanban board — drag and drop (HTML5 DnD, no library), column reorder<br>**DSA:** Next greater element I & II `monotonic-stack` *(II: circular — iterate 2n)*<br>**SD:** XSS — stored/reflected/DOM, exactly how React protects you, and where it doesn't |
| **Wed** | **DSA:** Previous smaller `monotonic-stack` · Trapping rainwater `two-pointers-opposite` *(also solvable `monotonic-stack` — do both)* · Largest rectangle in histogram `monotonic-stack`<br>**React:** Redux Toolkit vs Zustand — pick one, build a slice properly<br>**Rev:** Next greater element II `monotonic-stack` |
| **Thu** | **MC:** finish kanban — persistence, keyboard-accessible fallback for DnD<br>**DSA:** Sum of subarray minimums `monotonic-stack` · Asteroid collision `stack-simulation`<br>**SD:** CSRF, CSP, secure headers, dependency risk |
| **Fri** | **DSA:** LRU cache `hashmap-dll` · LFU cache `hashmap-dll` *(stretch)* · Stock span `monotonic-stack`<br>**React:** server state — react-query patterns: caching, invalidation, optimistic updates<br>**Rev:** Trapping rainwater — both solutions |
| **Wknd** | *(if available)* 4h: **Month 2 audit** — 8 cold re-solves, plus write the project's product spec (see Month 3) |

---

**Month 2 exit check**
- [ ] ~96 problems committed
- [ ] You reach for `monotonic-stack` the moment you hear "next greater"
- [ ] Every polyfill written cold, no lookups
- [ ] Can explain reconciliation, effect timing and memoization out loud
- [ ] One mock survived · project spec written
