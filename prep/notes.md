# Problem Notes — pattern & trigger only

One line per problem. **Never write the solution here.** Write the trigger — the observation that unlocks it — so you can re-derive the code cold.

Use the pattern tags from [patterns.md](patterns.md) in the Pattern column.

Mark `!` each time you fail a cold re-solve. Anything at `!!` goes on the Week 20 sweep list.

| #   | Problem                     | Pattern               | Trigger                                                                                                                                                                                                                     | Fails |
| --- | --------------------------- | --------------------- | --------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------- | ----- |
| 1   | Kadane's Algorithm          | `kadane`              | Reset sum to 0 the moment it goes negative — a negative prefix never helps                                                                                                                                                  |       |
| 2   | Next Permutation            | `sorting-then-scan`   | Find first `a[i] < a[i+1]` from the right, swap with next-larger in suffix, reverse suffix                                                                                                                                  |       |
| 3   | Set Matrix Zeros            | `matrix-simulation`   | Use row 0 and col 0 as the marker storage; handle col 0 separately                                                                                                                                                          |       |
| 4   | Pascal's Triangle           | `matrix-simulation`   | Row element = previous element × (row−col)/col — no need to build prior rows                                                                                                                                                |       |
| 5   | Two Sum                     | `hashing`             | Store elements in a hash map to find complements in one pass and check if we are taking the same element twice                                                                                                              |       |
| 6   | sort 0 1 2                  | `dutch-national-flag` | Use three pointers to partition the array into three sections , dont move the middle pointer for 2's                                                                                                                        |       |
| 7   | longest subarray with sum k | `sliding-window`      | Expand the window by moving the end pointer, contract from the start to find valid subarrays , add the value at the end pointer at the starting of the loop and use a loop to move the first pointer until the sum is valid |   !    |
| 8   | longest subarray with sum k with negatives | `prefix-sum-hashmap`      | Use a hashmap to store the prefix sums and their indices,if the current sum is x and the hashmap contains (x-k), we found a subarray with sum k                                                                           |    !   |
