# Month 1 — Arrays & Binary Search (Weeks 1–4)

Legend: **DSA** 2.5h (Mon/Wed/Fri) or 1h (Tue/Thu) · **JS** 1h · **MC** machine coding 2h · **SD/BE** 1h · **Rev** 30 min cold re-solve
Pattern tags in `code` — see [patterns.md](../patterns.md). Problems are from **Striver's A2Z sheet**; verify names against takeuforward.org as you go.

> You already have `pascalTriangle`, `setMatrixZeros`, `kadnesAlgo`, `nextPermutation` in `Arrays/`. Week 1 Day 1 starts *after* those.

---

## Week 1 — Arrays: easy → medium

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Longest subarray with sum K, positives `window-variable` → then with negatives `prefix-sum-hashmap` *(same problem, different pattern once negatives appear — this is the single best lesson in the sheet)* · Two Sum `hashing` · Sort 0s/1s/2s `dutch-flag`<br>**JS:** execution context, hoisting, TDZ, `var`/`let`/`const` — write examples that prove each<br>**Rev:** Kadane's cold `kadane` |
| **Tue** | **MC:** counter + star rating component (plain React, no libs)<br>**DSA:** Majority element n/2 `hashing` → Moore's voting · Majority element n/3 `hashing`<br>**BE:** Node + Express setup, folder structure, first REST endpoints |
| **Wed** | **DSA:** Maximum subarray, print the subarray `kadane` · Stock buy/sell I `kadane` *(running min, same skeleton)* · Rearrange by sign `two-pointers-same-dir`<br>**JS:** scope chain, lexical scope, closures — counter factory, `once()`, private state<br>**Rev:** Next Permutation cold |
| **Thu** | **MC:** accordion (single + multi open, keyboard accessible)<br>**DSA:** Leaders in array `sorting-then-scan` *(suffix max, right to left)* · Longest consecutive sequence `hashing`<br>**BE:** REST design — resources, verbs, status codes, versioning |
| **Fri** | **DSA:** Rotate matrix 90° `matrix-simulation` · Spiral traversal `matrix-simulation` · Count subarrays with sum K `prefix-sum-hashmap`<br>**JS:** closures in loops, IIFE, module pattern, memory implications<br>**Rev:** Set Matrix Zeros cold `matrix-simulation` |
| **Wknd** | *(if available)* 4h: finish unfinished MC builds, refactor properly, push |

## Week 2 — Arrays: medium → hard

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Pascal's Triangle, all 3 variants `matrix-simulation` · 3-Sum `two-pointers-opposite` · 4-Sum `two-pointers-opposite`<br>**JS:** `this` in every context — global, method, arrow, class, callback<br>**Rev:** Longest consecutive sequence `hashing` |
| **Tue** | **MC:** todo app — add/edit/delete, filters, localStorage (timed 90 min)<br>**DSA:** Largest subarray with 0 sum `prefix-sum-hashmap` · Count subarrays with XOR K `prefix-sum-hashmap` *(identical shape, XOR replaces subtraction)*<br>**BE:** Express middleware, centralized error handling, request validation |
| **Wed** | **DSA:** Merge intervals `intervals` · Merge two sorted arrays, no extra space `two-pointers-opposite` *(then the gap method)* · Find repeating + missing `cyclic-sort`<br>**JS:** call / apply / bind — behaviour, then hand-write all three<br>**Rev:** 3-Sum `two-pointers-opposite` |
| **Thu** | **MC:** finish + refactor the todo (extract hooks, memo where it matters)<br>**DSA:** Count inversions `merge-sort-variant` · Reverse pairs `merge-sort-variant`<br>**BE:** logging, env config, CORS, project conventions |
| **Fri** | **DSA:** Maximum product subarray `kadane` *(track min too — a negative can flip)* · Missing number variants `xor-trick` · re-solve 2 weak ones<br>**JS:** prototypes, prototype chain, `class` desugared, inheritance<br>**Rev:** Merge intervals `intervals` |
| **Wknd** | *(if available)* 4h: write `notes.md` entries for all 24 problems — pattern + trigger only |

## Week 3 — Binary search on arrays

| Day | Plan |
|---|---|
| **Mon** | **DSA:** BS template, get the invariant right `bs-index` · lower/upper bound `bs-bounds` · first & last occurrence `bs-bounds`<br>**JS:** the event loop — call stack, task queue, microtask queue; predict output of 6 tricky snippets<br>**Rev:** Count inversions `merge-sort-variant` |
| **Tue** | **MC:** tabs — keyboard nav, ARIA roles, lazy panel content<br>**DSA:** Search in rotated sorted array I & II `bs-rotated` *(II breaks on duplicates — know why)*<br>**BE:** Postgres — schema design, normalization, the joins you'll actually use |
| **Wed** | **DSA:** Min in rotated array `bs-rotated` · Single element in sorted array `bs-index` · Peak element `bs-index`<br>**JS:** promises — chaining, error propagation, `finally`, common footguns<br>**Rev:** lower/upper bound from scratch `bs-bounds` |
| **Thu** | **MC:** modal + tooltip — portals, focus trap, escape, click-outside<br>**DSA:** Kth element of two sorted arrays `bs-partition`<br>**BE:** indexes — what they cost, when they help, EXPLAIN on your own queries |
| **Fri** | **DSA:** Median of two sorted arrays `bs-partition` · Search 2D matrix I `bs-2d` *(flatten)* · II `bs-2d` *(corner walk)*<br>**JS:** async/await vs promises, sequential vs parallel, error handling<br>**Rev:** Search in rotated array II `bs-rotated` |
| **Wknd** | *(if available)* 4h: rebuild the autocomplete from scratch, timed — first taste of interview pressure |

## Week 4 — Binary search on the answer

> The whole week is one pattern: `bs-on-answer`. Trigger words in the question — *minimum maximum*, *maximum minimum*, *minimise the largest*. Write the `isFeasible(x)` helper first, every single time, and the rest is the Week 3 template.

| Day | Plan |
|---|---|
| **Mon** | **DSA:** Sqrt(x) `bs-on-answer` · Nth root `bs-on-answer` · Koko eating bananas `bs-on-answer`<br>**JS:** polyfills — `map`, `filter`, `reduce`, `forEach` (hand-written, no lookups)<br>**Rev:** Median of two sorted arrays `bs-partition` |
| **Tue** | **MC:** autocomplete — debounce, cancellation, keyboard nav, highlight match (timed 90 min)<br>**DSA:** Min days to make bouquets `bs-on-answer` · Smallest divisor `bs-on-answer`<br>**SD/BE:** auth — JWT vs sessions, refresh token flow, where tokens live |
| **Wed** | **DSA:** Capacity to ship packages `bs-on-answer` · Kth missing positive `bs-bounds` · Aggressive cows `bs-on-answer`<br>**JS:** polyfills — `call`, `apply`, `bind`, `debounce`, `throttle`<br>**Rev:** Koko eating bananas `bs-on-answer` |
| **Thu** | **MC:** finish autocomplete — caching, loading/empty/error states<br>**DSA:** Book allocation `bs-on-answer` · Split array largest sum `bs-on-answer` *(same problem, renamed — notice it)*<br>**SD/BE:** wire auth into your Express API end to end |
| **Fri** | **DSA:** Painter's partition `bs-on-answer` *(also the same problem)* · Min max of gas stations `bs-on-answer` · Row with max 1s `bs-bounds`<br>**JS:** rewrite every Week-4 polyfill from memory, timed 45 min<br>**Rev:** Aggressive cows `bs-on-answer` |
| **Wknd** | *(if available)* 4h: **Month 1 audit** — 5 random cold re-solves from `notes.md`. Failures go on the Week 5 revision list. |

---

**Month 1 exit check**
- [ ] ~48 problems committed
- [ ] You can state the `bs-on-answer` trigger without looking, and recognise that Book allocation / Split array / Painter's partition are one problem
- [ ] Polyfill set I written from memory in under 45 min
- [ ] Express + Postgres API with working JWT auth
- [ ] 5 machine coding components built and refactored
