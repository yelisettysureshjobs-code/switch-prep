# Month 1 — Arrays, Hashing, Binary Search, Sliding Window, Recursion (Weeks 1–4)

Legend: **DSA** 2.5h (Mon/Wed/Fri) or 1h (Tue/Thu) · **JS** 1h (four of these slots in Months 1–2 are DOM / CSS / HTML) · **MC** machine coding 2h · **SD/BE** 1h · **Rev** 30 min cold re-solve
Sheet: **[Striver's 180 – Master DSA Patterns](https://takeuforward.org/prep-hub/strivers-180-master-dsa-patterns?page=sheet)**, taken in sheet order — sections **Arrays → Recursion and Backtracking** this month. The tag in `code` beside each problem is the pattern name the sheet gives it; [patterns.md](../patterns.md) maps each one to its trigger. All 179 problems on the sheet are scheduled exactly once across Weeks 1–16, under the sheet's own names.

> Solved before this plan started: Kadane's Algorithm and Next Permutation are both on this sheet — Kadane's comes back on Wednesday (print the subarray this time), Next Permutation only as a **Rev**. Pascal's Triangle and Set Matrix Zeros aren't on the 180 sheet; they stay in `notes.md` as extra reps.

---

## Week 1 — Arrays & Hashing

### Mon

- **DSA:**
  - Longest subarray with sum K `Prefix / Precomputation` *(positives only → sliding window; once negatives appear → prefix-sum hashmap. Same problem, different pattern — the single best lesson in the sheet)*
  - Two Sum `Basic Traversal / Scan` *(one pass, hash map of complements)*
  - Sort an array of 0's 1's and 2's `Sort and Scan` *(Dutch national flag — three pointers, one pass)*
- **JS:** execution context, hoisting, TDZ, `var`/`let`/`const` — write examples that prove each
- **Rev:** Kadane's Algorithm cold `Basic Traversal / Scan`

### Tue

- **MC:** counter + star rating component (plain React, no libs)
- **DSA:**
  - Majority Element-I `Basic Traversal / Scan` *(hash map first, then Moore's voting)*
  - Majority Element-II `Frequency Counting` *(at most two candidates — Moore's with two counters)*
- **BE:** Node + Express setup, folder structure, first REST endpoints

### Wed

- **DSA:**
  - Kadane's Algorithm — and print the subarray `Basic Traversal / Scan`
  - Maximum Product Subarray in an Array `Basic Traversal / Scan` *(track the running min too — a negative can flip it)*
  - Longest Consecutive Sequence in an Array `Basic Traversal / Scan` *(hash set; only start counting from a number with no predecessor)*
- **JS:** scope chain, lexical scope, closures — counter factory, `once()`, private state, closures in loops, IIFE / module pattern
- **Rev:** Next Permutation cold `Two Pointer`

### Thu

- **MC:** accordion (single + multi open, keyboard accessible)
- **DSA:**
  - Count subarrays with given sum `Prefix / Precomputation` *(store prefix → count, look for `prefix − K`)*
  - Count subarrays with given xor K `Bitwise XOR` *(identical shape — XOR replaces subtraction)*
- **BE:** REST design — resources, verbs, status codes, versioning

### Fri

- **DSA:**
  - 3 Sum `Two Pointer` + `Sort and Scan`
  - 4 Sum `Two Pointer` + `Sort and Scan` *(3 Sum with one more outer loop — skip duplicates at every level)*
  - Merge two sorted arrays without extra space `Two Pointer` + `Sort and Scan` *(then the gap method)*
- **DOM:** events — capture vs bubble, delegation, `preventDefault` vs `stopPropagation`. Build a clickable list with one delegated listener, no framework
- **Rev:** Longest subarray with sum K — both versions `Prefix / Precomputation`

### Wknd

- *(if available)* 4h: finish unfinished MC builds, refactor properly, push

## Week 2 — Arrays hard + Binary search on arrays

### Mon

- **DSA:**
  - Trapping Rainwater `Monotonic Stack / Queue` *(do prefix-max / suffix-max, then two pointers; the stack version comes back in Week 7)*
  - Count Inversions `Divide and Conquer` *(the counting happens during the merge step)*
  - Reverse Pairs `Divide and Conquer` *(same merge sort — count before you merge)*
- **JS:** `this` in every context — global, method, arrow, class, callback
- **Rev:** Longest Consecutive Sequence in an Array `Basic Traversal / Scan`

### Tue

- **MC:** todo app — add/edit/delete, filters, localStorage (timed 90 min)
- **DSA:**
  - *warm-up, 15 min:* binary search template + lower / upper bound from scratch — not on the sheet, but every problem in this section stands on it
  - Find minimum in Rotated Sorted Array `Rotated Array Search` *(one half is always sorted — identify which)*
- **BE:** Express middleware, centralized error handling, request validation

### Wed

- **DSA:**
  - Search in rotated sorted array-II `Rotated Array Search` *(duplicates break the "which half is sorted" test — know why, and the fix)*
  - Single element in sorted array `Lower Bound / Upper Bound` *(pair parity flips after the single element)*
  - Find peak element `Lower Bound / Upper Bound`
- **JS:** call / apply / bind — behaviour, then hand-write all three
- **Rev:** 3 Sum `Two Pointer` + `Sort and Scan`

### Thu

- **MC:** finish + refactor the todo (extract hooks, memo where it matters)
- **DSA:** Search in 2D matrix - II `Lower Bound / Upper Bound` *(corner walk — start where one direction always decreases)*
- **BE:** logging, env config, CORS, project conventions

### Fri

- **DSA:**
  - Find Peak Element - II `Lower Bound / Upper Bound` *(binary search the columns, take each column's max row)*
  - Find Nth root of a number `Search on Answer` *(first taste of searching the answer, not the array)*
  - re-solve 2 weak ones from Week 1
- **JS:** prototypes, prototype chain, `class` desugared, inheritance
- **Rev:** Count Inversions `Divide and Conquer`

### Wknd

- *(if available)* 4h: write `notes.md` entries for every problem so far (24, counting Next Permutation) — pattern + trigger only

## Week 3 — Binary search on the answer + Sliding window

> **`Search on Answer`** is Monday and Tuesday. Trigger words in the question — *minimum maximum*, *maximum minimum*, *minimise the largest*. Write the `isFeasible(x)` helper first, every single time, and the rest is the Week 2 template.
>
> **`Sliding Window`** starts Wednesday and is asked constantly at 2 YOE. It's one template: **expand right, shrink left while the window is invalid, record the answer.** Write that template once and all nine window problems on the sheet collapse into it.

### Mon

- **DSA:**
  - Koko eating bananas `Search on Answer`
  - Aggressive Cows `Search on Answer` *(maximise the minimum — flip the feasibility check)*
  - Book Allocation Problem `Search on Answer` *(LeetCode's Split Array Largest Sum, and Painter's Partition, are this exact problem renamed — notice it)*
- **JS:** the event loop — call stack, task queue, microtask queue; predict output of 6 tricky snippets
- **Rev:** Search in rotated sorted array-II `Rotated Array Search`

### Tue

- **MC:** tabs in **vanilla JS, no React** — keyboard nav, ARIA roles, lazy panel content, one delegated listener
- **DSA:** Matrix Median `Lower Bound / Upper Bound` *(search the value range; count elements ≤ mid with upper bound on each row)*
- **BE:** Postgres — schema design, normalization, the joins you'll actually use

### Wed

- **DSA:**
  - Kth element of 2 sorted arrays `Lower Bound / Upper Bound`
  - Median of 2 sorted arrays `Lower Bound / Upper Bound` *(both: search the partition point, not the value)*
  - Longest Substring Without Repeating Characters `Sliding Window`
- **JS:**
  - promises — chaining, error propagation, `finally`, common footguns
  - async/await, sequential vs parallel
- **Rev:** Koko eating bananas `Search on Answer`

### Thu

- **MC:** modal + tooltip — portals, focus trap, escape, click-outside
- **DSA:**
  - Maximum Points You Can Obtain from Cards `Sliding Window` *(fixed window on the complement)*
  - Max Consecutive Ones III `Sliding Window` *(longest window with at most K zeros)*
- **BE:** indexes — what they cost, when they help, EXPLAIN on your own queries

### Fri

- **DSA:**
  - Longest Repeating Character Replacement `Sliding Window` *(window is valid while `len − maxFreq ≤ K`)*
  - Longest Substring With At Most K Distinct Characters `Sliding Window`
  - Minimum Window Substring `Sliding Window` *(the hard one — worth a full hour)*
- **CSS I:** box model, cascade + specificity, positioning, stacking context, BFC — predict the result of 6 tricky snippets
- **Rev:** Median of 2 sorted arrays `Lower Bound / Upper Bound`

### Wknd

- *(if available)* 4h: rebuild the todo app from scratch, timed 90 min — first taste of interview pressure

## Week 4 — Counting windows + Recursion & Backtracking

> Monday's three are one trick: **exactly K = atMost(K) − atMost(K−1)**. From Tuesday it's recursion, and every problem is either *pick / don't pick* or *build, recurse, undo*. Say which one out loud before you write a line.

### Mon

- **DSA:**
  - Number of Substrings Containing All Three Characters `Sliding Window`
  - Binary Subarrays With Sum `Sliding Window` *(exactly K = atMost(K) − atMost(K−1))*
  - Count number of Nice subarrays `Sliding Window` *(map odd → 1 and it's the previous problem)*
- **JS:** polyfills — `map`, `filter`, `reduce`, `forEach` (hand-written, no lookups)
- **Rev:** Minimum Window Substring `Sliding Window`

### Tue

- **MC:** autocomplete — debounce, cancellation, keyboard nav, highlight match (timed 90 min)
- **DSA:**
  - Power Set `Subsets` *(pick / don't pick at each index)*
  - Subsets II `Subsets` *(sort, then skip equal elements at the same depth)*
- **SD/BE:** auth — JWT vs sessions, refresh token flow, where tokens live

### Wed

- **DSA:**
  - Combination Sum `Permutations & Combinations`
  - Combination Sum II `Permutations & Combinations` *(I reuses an element, II doesn't — the difference is one index)*
  - Generate Parentheses `Permutations & Combinations` *(only add `)` while close < open)*
- **JS:** polyfills — `call`, `apply`, `bind`, `debounce`, `throttle`
- **Rev:** Book Allocation Problem `Search on Answer`

### Thu

- **MC:** finish autocomplete — caching, loading/empty/error states
- **DSA:**
  - Letter Combinations of a Phone Number `Permutations & Combinations`
  - Palindrome partitioning `Permutations & Combinations` *(cut wherever the prefix is a palindrome, recurse on the rest)*
- **SD/BE:** wire auth into your Express API end to end

### Fri

- **DSA:**
  - Word Search `Constraint Satisfaction`
  - N Queen `Constraint Satisfaction` *(place, validate, recurse, remove)*
  - Sudoku Solver `Constraint Satisfaction`
- **JS:** rewrite every Week-4 polyfill from memory, timed 45 min
- **Rev:** Combination Sum II `Permutations & Combinations`

### Wknd

- *(if available)* 4h: **Month 1 audit** — 5 random cold re-solves from `notes.md`. Failures go on the Week 5 revision list.

---

**Month 1 exit check**
- [ ] 49 problems committed — sheet sections Arrays → Recursion and Backtracking
- [ ] You can state the `Search on Answer` trigger without looking, and recognise that Book Allocation / Split Array Largest Sum / Painter's Partition are one problem
- [ ] The `Sliding Window` template written from memory, plus the `atMost(K) − atMost(K−1)` counting trick
- [ ] Polyfill set I written from memory in under 45 min
- [ ] Express + Postgres API with working JWT auth
- [ ] 5 machine coding components built and refactored — tabs without React
- [ ] Can explain event delegation, specificity and stacking context out loud
