# Pattern Index

The point of the whole DSA block. You are not memorising 200 problems — you are learning ~35 patterns and the **trigger** that tells you which one applies. In an interview you recognise the trigger in the first 60 seconds, or you don't solve it.

Every problem in `daily/` is tagged with one of these.

---

## Arrays

| Pattern | Trigger — what in the question tells you |
|---|---|
| `hashing` | "has it appeared before", "count of", "find the duplicate" — anything needing O(1) lookup of seen state |
| `two-pointers-opposite` | Sorted array + looking for a pair/triplet meeting a sum condition |
| `two-pointers-same-dir` | In-place removal/partition where read and write indices move at different speeds |
| `prefix-sum` | Range sums, or subarray sums queried repeatedly |
| `prefix-sum-hashmap` | "count subarrays with sum/XOR = K" — store prefix→count, look for `prefix - K` |
| `kadane` | Max/min contiguous subarray — reset when the running value stops helping |
| `dutch-flag` | Three-way partition in one pass, O(1) space |
| `sorting-then-scan` | Order unlocks the problem — intervals, consecutive runs, greedy pairing |
| `intervals` | Sort by start, then merge/overlap-check against the last kept interval |
| `cyclic-sort` | Values are in range `1..n` — put each value at its own index, then scan for mismatches |
| `matrix-simulation` | Rotate, spiral, in-place marking — no algorithm, just careful index handling |
| `merge-sort-variant` | Count inversions / reverse pairs — the counting happens during the merge step |

## Binary Search

| Pattern | Trigger |
|---|---|
| `bs-index` | Sorted array, find a position — get the invariant right and the rest follows |
| `bs-bounds` | lower_bound / upper_bound / first & last occurrence — the two templates everything else is built from |
| `bs-rotated` | Sorted but rotated — one half is always sorted; identify which, then decide |
| `bs-on-answer` | **The big one.** Answer is a number in a range, and "is X feasible?" is monotonic — binary search the answer, not the array. Trigger words: *minimum maximum*, *maximum minimum*, *minimise the largest* |
| `bs-2d` | Sorted matrix — either treat as flat array, or start from a corner where one direction always decreases |
| `bs-partition` | Median of two sorted arrays — search the split point, not the value |

## Sliding Window & Two Pointers

| Pattern | Trigger |
|---|---|
| `window-fixed` | "subarray of size K" — window size given |
| `window-variable` | "longest/shortest subarray such that <condition>" — expand right, shrink left while invalid |
| `window-hashmap` | Window condition involves counts/distinct characters |
| `window-deque` | Max/min *within* the window — monotonic deque, evict from both ends |

## Bit Manipulation

| Pattern | Trigger |
|---|---|
| `xor-trick` | "everything appears twice except one" — XOR cancels pairs |
| `bit-basics` | Set/clear/check the i-th bit, count set bits, power-of-two checks |
| `bitmask-subsets` | Generate all subsets by iterating `0..2^n - 1` |

## Strings

| Pattern | Trigger |
|---|---|
| `freq-map` | Anagrams, character counts, sorting by frequency |
| `expand-center` | Palindromic substrings — expand around each of the 2n−1 centres |
| `string-simulation` | Parsing — roman numerals, atoi, word reversal. Precision, not cleverness |

## Linked List

| Pattern | Trigger |
|---|---|
| `slow-fast` | Middle, cycle detection, cycle start, Nth from end — two pointers at different speeds |
| `pointer-reversal` | Reverse in place — prev/curr/next dance, or recursion |
| `dummy-node` | Head might change — a dummy node removes every edge case |
| `ll-merge` | Two sorted lists, or K sorted lists (then it's a heap problem) |

## Recursion & Backtracking

| Pattern | Trigger |
|---|---|
| `pick-nonpick` | Subsets, subset sums — at each index, take it or don't. The base of most DP too |
| `backtracking-build` | Build a partial answer, recurse, **undo the change on the way out** |
| `permutation-swap` | Permutations via swapping in place rather than a used[] array |
| `board-backtracking` | N-Queens, Sudoku, rat in a maze — place, validate, recurse, remove |

## Stacks & Queues

| Pattern | Trigger |
|---|---|
| `monotonic-stack` | "next/previous greater/smaller element" — and every problem that reduces to it (histogram, rainwater, subarray minimums) |
| `stack-simulation` | Matching, nesting, undo semantics — parentheses, asteroid collision |
| `stack-aux` | Min stack — carry the extra state alongside the value |
| `hashmap-dll` | LRU cache — O(1) lookup plus O(1) reorder needs both structures |

## Trees

| Pattern | Trigger |
|---|---|
| `dfs-recursive` | Anything computable from left and right subtree results |
| `bfs-level` | "level", "depth-wise", "view from a side", shortest path in an unweighted tree |
| `dfs-bottom-up` | Return a tuple upward — height *and* balanced, sum *and* valid — one pass instead of n² |
| `path-tracking` | Carry the path down, record at leaves, pop on the way back |
| `bst-property` | Left < root < right — turns search into binary search, and inorder into a sorted array |
| `tree-construction` | Build from traversals — preorder gives the root, inorder gives the split |
| `tree-to-graph` | "distance K from a node", "time to burn" — add parent pointers, then it's a BFS |

## Heaps

| Pattern | Trigger |
|---|---|
| `top-k-heap` | "K largest/smallest/most frequent" — heap of size K, not a full sort |
| `k-way-merge` | Merge K sorted things — heap holding one head per list |
| `two-heaps` | Running median — max-heap for the low half, min-heap for the high half |
| `greedy-heap` | Repeatedly take the current best — task scheduler, connect ropes |

## Tries

| Pattern | Trigger |
|---|---|
| `trie` | Many strings queried by **prefix** — autocomplete, "starts with", "every prefix is also a word". A node is a children map plus an end-of-word flag |

## Greedy

| Pattern | Trigger |
|---|---|
| `sort-then-greedy` | Sort by the right key, then take locally-best choices. **The whole difficulty is choosing the sort key** |
| `interval-scheduling` | Max non-overlapping intervals — sort by *end* time, not start |
| `exchange-argument` | Prove greedy works by showing any optimal solution can be swapped toward yours |

## Graphs

| Pattern | Trigger |
|---|---|
| `bfs-grid` | Shortest path on an unweighted grid, or spreading level by level |
| `dfs-grid` | Connected components, flood fill, islands |
| `multi-source-bfs` | Several starting points at once — push them all before the loop starts |
| `cycle-undirected` | Track the parent; a visited neighbour that isn't the parent means a cycle |
| `cycle-directed` | Recursion stack, not just visited |
| `topo-sort` | Dependencies, ordering, prerequisites — Kahn's (indegree) or DFS finish order |
| `bipartite` | Two-colouring, "can these be split into two groups" |
| `dijkstra` | Weighted, non-negative edges, shortest path — PQ, not a plain queue |
| `bfs-0-1` | Weights are only 0 and 1 — deque instead of a PQ |
| `bellman-ford` | Negative edges, or "at most K stops" |
| `dsu` | Dynamic connectivity, merging groups, "are these connected" |

## Dynamic Programming

| Pattern | Trigger |
|---|---|
| `dp-1d` | State depends on the previous one or two indices — stairs, house robber |
| `dp-grid` | Move through a 2D grid, count paths or minimise cost |
| `dp-pick-nonpick` | Subset sums, target sums, partitions. **Recursion first, then memoise, then tabulate** |
| `dp-knapsack` | Capacity constraint + value to maximise. Unbounded = you may reuse an item |
| `dp-strings` | LCS, edit distance, distinct subsequences — 2D over two string indices |
| `dp-state-machine` | Stock problems — you're in a state (holding / not holding / cooldown) with transitions |
| `dp-lis` | Longest increasing subsequence — O(n²) DP, then the O(n log n) patience version |

---

## How to use this file

1. Solve the problem.
2. Write the line in `notes.md`: `problem → pattern → trigger`.
3. On a cold re-solve, read **only the trigger**. If you can't rebuild the code from it, the trigger is wrong — rewrite the trigger, not the code.
4. In interviews, name the pattern out loud before you start coding. Interviewers score the recognition, and it buys you a correction if you're wrong.
