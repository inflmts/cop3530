<!-- Documentation [14.5 Points] -->

# Project 2: Simplified PageRank Algorithm

```
report.md
Author: Daniel Li
Date: 04/04/2025
```

- **Describe the data structure you used to implement the graph and why.**

  A set of pages (`std::set<Page>`) is used to implement the graph, keyed on the
  page URL, to prevent duplicate pages. Each page contains a set of pages it
  links to (`std::unordered_set<Page*>`) to prevent duplicate edges. Circular
  loops are not currently handled correctly.

- **What is the computational complexity of each method in your implementation
  in the worst case in terms of Big O notation?**

  Let `u` be the length of the longest URL, `v` be the number of pages, `e` be
  the number of edges, and `d` be the the highest outdegree of any page.

  **Insertion**

  These lines form the "insertion method" of the graph implementation:

  ```cpp
  Page& src = const_cast<Page&>(*pages.emplace(src_url).first);
  Page& dest = const_cast<Page&>(*pages.emplace(dest_url).first);
  src.links.emplace(&dest);
  ```

  The time complexity of this code depends on the underlying `std::set` and
  `std::unordered_set` implementation. Assuming `std::set` is implemented using
  a red-black tree and `std::unordered_set` is implemented using a closed hash
  table backed by a red-black tree:

  - Each `pages.emplace` (`std::set::emplace`) is `O(u log v)`.
  - In the worst case, the hash function is terrible and
    `std::unordered_set::emplace` degrades to a red-black tree insertion:
    `O(log d)`. `u` is not relevant because this insertion operates on `Page`
    pointers.

  The total time complexity is `O(u log v + log d)`.

  **pagerank**

  The `pagerank` function does the following:

  - Initializes each rank to zero: `O(v)`
  - For a constant `p`, which does not affect the time complexity:
    - For each page, copies the current rank to a temporary variable and
      initializes the new rank to zero: `O(v)`
    - For each edge, performs a constant-time computation: `O(e)`

  The total time complexity is `O(v + e)`.

- **What is the computational complexity of your main method in your
  implementation in the worst case in terms of Big O notation?**

  Let `u` be the length of the longest URL, `v` be the number of pages, `e` be
  the number of edges, and `d` be the the highest outdegree of any page.

  The main method (`pagerank_shell` function) does the following:

  - Performs `e` insertions: `O(e * (u log v + log d))`
  - Calls `pagerank`: `O(v + e)`
  - Prints each page with its final rank: `O(v)`

  The total time complexity is `O(v + e * (u log v + log d))`.

- **What did you learn from this assignment, and what would you do differently
  if you had to start over?**

  I learned that a simple is sometimes better. If I had to start over, I would
  have read the FAQ sheet first and realized that I didn't have to deal with
  parallel edges (I could've used a `std::vector` instead of a
  `std::unordered_set`, for example).

