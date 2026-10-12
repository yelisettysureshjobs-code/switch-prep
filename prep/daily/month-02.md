# Month 2 — Linked List, Stacks & Queues, Greedy, Heaps (Weeks 5–8)

Sheet: **[Striver's 180 – Master DSA Patterns](https://takeuforward.org/prep-hub/strivers-180-master-dsa-patterns?page=sheet)**, taken in sheet order — sections **Linked List → Heaps** this month. The tag in `code` beside each problem is the pattern name the sheet gives it; [patterns.md](../patterns.md) maps each one to its trigger.

---

## Week 5 — Linked List

### Mon

- **DSA:**
  - Reverse a LL — iterative + recursive `In-place Reversal`
  - Find Middle of Linked List `Fast & Slow Pointers`
  - Detect a loop in LL `Fast & Slow Pointers`
- **JS:** polyfills — `Promise.all`, `race`, `allSettled`, `any`
- **Rev:** N Queen `Constraint Satisfaction`

### Tue

- **MC:** infinite scroll feed — IntersectionObserver, skeletons, error retry
- **DSA:**
  - Find the starting point in LL `Fast & Slow Pointers` *(know why the reset-to-head step works)*
  - Length of loop in LL `Fast & Slow Pointers`
- **SD:** rendering strategies — CSR / SSR / SSG / ISR, when each wins, what each costs

### Wed

- **DSA:**
  - Remove Nth node from the back of the LL `Two Pointer` *(a gap of N between two pointers; a dummy node removes the head edge case)*
  - Find the intersection point of Y LL `Two Pointer`
  - Merge two Sorted Lists `Two Pointer`
- **JS:** polyfills — deep clone (cycles, Dates, Maps), `EventEmitter`
- **Rev:** Reverse a LL, recursive `In-place Reversal`

### Thu

- **MC:** finish infinite scroll — cleanup, scroll restoration
- **DSA:**
  - Check if LL is palindrome or not `Fast & Slow Pointers` + `In-place Reversal` *(find middle, reverse the second half, compare)*
  - Sort a Linked List of 0's 1's and 2's `Frequency Counting` *(then the three-dummy-lists version, no counting)*
- **SD:** hydration, streaming SSR, islands — and why Myntra/Meesho care

### Fri

- **DSA:**
  - Add two numbers in Linked List `Simulation / Implementation`
  - Rotate a LL `Two Pointer` *(make it a ring, then cut)*
  - Sort LL `Divide and Conquer` *(merge sort on a list — middle + merge, both already written this week)*
- **JS:** polyfills — `curry`, `memoize`, `once`, `pipe`/`compose`
- **Rev:** Merge two Sorted Lists `Two Pointer`

### Wknd

- *(if available)* 4h: every polyfill from Weeks 4–5 rewritten cold in one sitting — a common round-1 filter

## Week 6 — Linked List hard + Stack & Queue design

### Mon

- **DSA:**
  - Reverse LL in group of given size K `In-place Reversal`
  - Clone a LL with random and next pointer `Simulation / Implementation` *(hashmap first, then the interleaving trick)*
  - Flattening of LL `K-way Merge` *(merge two sorted lists, repeatedly — LeetCode's Merge K Sorted Lists is the same idea with a heap)*
- **React:** reconciliation, the diffing algorithm, why keys matter, list reorder bugs
- **Rev:** Find the starting point in LL `Fast & Slow Pointers`

### Tue

- **MC:** virtualized list from scratch — fixed height, then variable height (no library)
- **DSA:**
  - Implement Min Stack `Simulation / Implementation` *(carry the running min alongside each value)*
  - Asteroid Collision `Simulation / Implementation`
- **SD:** browser rendering pipeline — parse, style, layout, paint, composite; reflow vs repaint

### Wed

- **DSA:**
  - Celebrity Problem `Simulation / Implementation` *(two pointers eliminate one candidate per comparison)*
  - LRU Cache `Simulation / Implementation` *(hashmap + doubly linked list — O(1) lookup plus O(1) reorder needs both)*
- **React:** render phase vs commit phase, batching, StrictMode double-render
- **Rev:** Reverse LL in group of given size K `In-place Reversal`

### Thu

- **MC:** finish virtualized list — overscan, scroll-to-index, resize handling
- **DSA:** LFU Cache `Simulation / Implementation` *(an LRU list per frequency + a `minFreq` pointer — budget the full hour)*
- **SD:** what actually makes a page slow — measure, don't guess

### Fri

- **DSA:**
  - Next Greater Element `Monotonic Stack / Queue`
  - Next Greater Element - 2 `Monotonic Stack / Queue` *(circular — iterate 2n)*
  - Stock span problem `Monotonic Stack / Queue` *(previous greater, in disguise)*
- **CSS II:** flexbox, grid, responsive (media + container queries) — build a holy-grail layout and a card grid, no framework
- **Rev:** LRU Cache `Simulation / Implementation`

### Wknd

- *(if available)* 4h: mock interview #1 — 1 DSA + 1 JS round. Expect it to go badly; that's the point.

## Week 7 — Monotonic Stack + Greedy

> `Monotonic Stack / Queue` is the pattern of the week (it started Friday with Next Greater Element), and one of the highest-yield in the whole sheet. Trigger: *next/previous greater/smaller*. Subarray minimums, histogram and maximal rectangle all reduce to it — solve them in that order and you'll see the reduction.

### Mon

- **DSA:**
  - Remove K Digits `Monotonic Stack / Queue`
  - Sum of Subarray Minimums `Monotonic Stack / Queue` *(count the subarrays where each element is the minimum — previous smaller × next smaller)*
  - Sum of Subarray Ranges `Monotonic Stack / Queue` *(sum of maximums − sum of minimums)*
- **React:** `useEffect` — dependency array, cleanup timing, the stale closure trap
- **Rev:** Next Greater Element - 2 `Monotonic Stack / Queue`

### Tue

- **MC:** nested comments — recursive rendering, collapse, reply, depth limits
- **DSA:** Largest rectangle in a histogram `Monotonic Stack / Queue`
- **SD:** HTTP caching — Cache-Control, ETag, stale-while-revalidate

### Wed

- **DSA:**
  - Maximum Rectangles `Monotonic Stack / Queue` *(the histogram problem, once per row)*
  - Sliding Window Maximum `Monotonic Stack / Queue` *(monotonic deque — evict from both ends)*
  - Jump Game - I `Greedy Choice` *(track the farthest reachable index)*
- **HTML:** semantic elements, forms + native validation, `<script>` async / defer / module, storage vs cookies *(`useMemo` / `useCallback` moved to W9 Wed)*
- **Rev:** Largest rectangle in a histogram `Monotonic Stack / Queue`

### Thu

- **MC:** finish nested comments — optimistic add/delete, edit in place
- **DSA:** Valid Paranthesis Checker `Greedy Choice` *(LeetCode: Valid Parenthesis String — keep a min / max range of open brackets)*
- **SD:** cookies, SameSite, CORS preflight, credentials

### Fri

- **DSA:**
  - Candy `Greedy Choice` *(two passes, left then right)*
  - N meetings in one room `Intervals / Scheduling` *(sort by **end** time)*
  - Insert Interval `Intervals / Scheduling`
- **React:** `useRef`, `useLayoutEffect`, custom hooks — extract 3 from your MC builds
- **Rev:** Trapping Rainwater — both solutions: two pointers, then `Monotonic Stack / Queue`

### Wknd

- *(if available)* 4h: refactor all MC builds into a shared component folder — this becomes the project's UI kit

## Week 8 — Intervals + Heaps

> Two small sections, back to back. For every interval problem the whole difficulty is **choosing the sort key** — say it out loud before coding. Heaps on this sheet start from the implementation: write heapify yourself before you ever reach for a library priority queue.

### Mon

- **DSA:**
  - Non-overlapping Intervals `Intervals / Scheduling` + `Sort and Scan` *(sort by **end**, not start — this is the whole trick)*
  - Minimum number of platforms required for a railway `Intervals / Scheduling` *(sort arrivals and departures separately, then sweep)*
  - Job sequencing Problem `Greedy Choice` *(sort by profit, take the latest free slot)*
- **React:** Context — cost, re-render behaviour, when it's the wrong tool
- **Rev:** Candy `Greedy Choice`

### Tue

- **MC:** kanban board — drag and drop (HTML5 DnD, no library), column reorder
- **DSA:**
  - Heapify Algorithm `Simulation / Implementation`
  - Build heap from a given Array `Simulation / Implementation` *(heapify down from the last parent — O(n), know why)*
- **SD:** XSS — stored/reflected/DOM, exactly how React protects you, and where it doesn't

### Wed

- **DSA:**
  - Implement Min Heap `Simulation / Implementation`
  - K-th Largest element in an array `Top K Elements` + `Priority Queue` *(size-K min-heap, not a sort)*
- **React:** Redux Toolkit vs Zustand — pick one, build a slice properly
- **Rev:** Non-overlapping Intervals `Intervals / Scheduling`

### Thu

- **MC:** finish kanban — persistence, keyboard-accessible fallback for DnD
- **DSA:** Find Median from Data Stream `Two Heaps` + `Priority Queue` *(max-heap for the low half, min-heap for the high half)*
- **SD:** CSRF, CSP, secure headers, dependency risk

### Fri

- **DSA:** no new problems — the sheet is done through Heaps. Cold re-solve the 3 problems with the most `!` marks in `notes.md`
- **React:** server state — react-query patterns: caching, invalidation, optimistic updates
- **Rev:** Sliding Window Maximum `Monotonic Stack / Queue`

### Wknd

- *(if available)* 4h: **Month 2 audit** — 8 cold re-solves, plus write the project's product spec (see Month 3)

---

**Month 2 exit check**
- [ ] 92 problems committed — sheet done through Heaps
- [ ] You reach for `Monotonic Stack / Queue` the moment you hear "next greater"
- [ ] Every polyfill written cold, no lookups
- [ ] Can explain reconciliation and effect timing out loud
- [ ] Can choose flex vs grid for a layout and say why
- [ ] One mock survived · project spec written
