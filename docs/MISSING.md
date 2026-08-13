# Missing Topics — raw gap list (appendix)

> **Superseded by [PLAYBOOK.md](PLAYBOOK.md).** Read that first — it organizes the same material by
> technique, says what each one *solves*, cross-links multi-use techniques, and prioritizes from
> measured ECPC/ACPC/Codeforces evidence ([CONTEST-INTEL.md](CONTEST-INTEL.md)).
> This file is kept only as a flat checklist with `[YKW: path]` pointers to reference
> implementations in `ShahjalalShohag/code-library`.


Benchmarked against the current 106-file notebook, YouKnowWho's `ShahjalalShohag/code-library`
(387 files), KACTL, and the standard ICPC regional/WF topic set.

**Priority key**
- **P0** — you will lose a problem without it. Add before the next contest.
- **P1** — shows up regularly; add once P0 is clear.
- **P2** — rare / speculative; add if there's page budget left.

**`[YKW]`** = a reference implementation exists in `ShahjalalShohag/code-library` under the named
file, so it can be lifted and reformatted rather than written from scratch.

**347 numbered items**, grouped by notebook section and ordered P0 → P1 → P2 within each.

---

## 0. Miscellaneous / contest infrastructure

Have: template, fastIO, grid deltas, random, coordinate compression, gray code, custom hash,
pragmas, `__int128` I/O.

### P0
1. **Stress-testing harness** — generator + brute + diff loop as a shell/bash snippet. `[YKW: Miscellaneous/Stress Testing.cpp]`
2. **Debug macro** — variadic `dbg(...)` printing containers/pairs/tuples, disabled by `#ifdef LOCAL`.
3. **Binary search idioms** — `firstTrue(lo, hi, pred)` / `lastTrue(lo, hi, pred)` on integers, and the real-valued variant with fixed iteration count.
4. **Ternary search** — integer (unimodal, `while (r - l > 2)`) and real versions.
5. **Bit tricks reference page** — submask enumeration `for (int s = m; s; s = (s-1) & m)`, `__builtin_*`, next-permutation-of-bits, iterate subsets of size k, lowbit decomposition.
6. **0-1 BFS / multi-source BFS on grids** — deque version, plus the `dx/dy` loop you already have.
7. **128-bit `mulmod`** — `(__int128)a * b % m`, plus a `u64` fallback for when `__int128` is unavailable.
8. **Interval set / "Chtholly" (ODT)** — `set<Interval>` with `split`/`assign`, for range-assign workloads. `[YKW: Data Structures/Interval Set.cpp]`
9. **Parallel binary search** — offline, over a shared structure. `[YKW: Miscellaneous/Parallel Binary Search.cpp]`

### P1
10. **BigInt** — add/sub/mul (schoolbook + FFT), divmod by small, comparison, I/O. `[YKW: Miscellaneous/BigInt.cpp]`
11. **Fraction / rational** — exact `<`, normalization by gcd, `__int128` intermediate.
12. **Inversion counting** — BIT and merge-sort versions. `[YKW: Miscellaneous/Inversions.cpp]`
13. **Expression parser** — shunting-yard / recursive descent with unary minus and parentheses. `[YKW: Miscellaneous/Expression Parsing.cpp]`
14. **Date arithmetic** — Julian day number, weekday, leap year, date diff. `[YKW: Miscellaneous/Dates.cpp]`
15. **Fraction binary search / Stern–Brocot** — best rational approximation with bounded denominator. `[YKW: Miscellaneous/Fraction Binary Search.cpp]`
16. **Permutation cycles** — decomposition, order, applying `p^k`. `[YKW: Miscellaneous/Permutation Cycles.cpp]`
17. **Josephus problem** — recursive O(n) and O(k log n). `[YKW: Miscellaneous/Josephus Problem.cpp]`
18. **Custom bitset** — with `_Find_first`/`_Find_next` and shift helpers when `std::bitset` isn't enough. `[YKW: Miscellaneous/Bitset Custom.cpp]`
19. **Two pointers template** — the sliding-window shape with add/remove hooks.
20. **Ordered multiset** — pbds `tree` keyed on `pair<T,int>`, with the counter trick documented.
21. **Coordinate compression for 2D / offline query batching**.
22. **`MEX` of a set with updates** — and MEX of all subarrays. `[YKW: Miscellaneous/MEX of all Subarrays.cpp]`

### P2
23. Matroid intersection (colorful / graphic / linear). `[YKW: Miscellaneous/Matroid Intersection *.cpp]`
24. Schreier–Sims (permutation group order / membership). `[YKW: Miscellaneous/Schreier–Sims algorithm.cpp]`
25. Trygub number (lazy-carry arbitrary base accumulator). `[YKW: Miscellaneous/Trygub Num.cpp]`
26. Knight moves on an infinite grid (closed form). `[YKW: Miscellaneous/Knight Moves in Infinity Grid.cpp]`
27. Negative-base representation. `[YKW: Miscellaneous/Negative Base (Binary).cpp]`
28. Codebook space remover (shrink source for the printed PDF). `[YKW: Miscellaneous/Space Remover For Codebook.cpp]`
29. Logarithmic subarray aggregator (all distinct `f(l..r)` values per `r`, e.g. gcd/or/and). `[YKW: Miscellaneous/Logarithmic Subarray Aggregator.cpp]`

---

## 1. Data Structures

Have: segment trees (basic, lazy, persistent ×2, beats, dynamic, merge-sort, iterative, 2D),
BIT (1D, 2D, offline-2D), sparse table (1D, 2D, square), tries (string, binary, persistent),
DSU (plain, rollback), Mo's (array, subtree, path), sqrt decomposition, treap (key + implicit),
pbds ordered set, monotonic stack/queue/2D, dynamic median.

### P0
30. **BIT with range update + range query** — the two-BIT trick. You only have point-update. `[YKW: Data Structures/BIT with Range Update and Range Query.cpp]`
31. **Segment tree descent** — `walk`/`find_first(l, pred)` in O(log n), not O(log²n). This is the single most-used missing segtree operation.
32. **Binary lifting as a standalone struct** — `kth_ancestor`, `lca`, jump-while-predicate. Currently it only exists buried inside `MoOnPaths.cpp`.
33. **DSU with potentials / weighted DSU** — parity, difference constraints, bipartiteness check. `[YKW: Data Structures/Augmented DSU.cpp]`
34. **Kruskal reconstruction tree (reachability tree)** — bottleneck queries, "reachable within weight w". `[YKW: Data Structures/Reachability Tree.cpp]`
35. **Offline dynamic connectivity** — segment tree on time + DSU with rollback. `[YKW: Data Structures/Dynamic Connectivity Problem.cpp]`
36. **Mo's with updates (3D Mo)** — block size `n^(2/3)`. `[YKW: Data Structures/MOs with Update.cpp]`
37. **Mo's with rollback** — for add-only structures where remove is expensive. `[YKW: Data Structures/MOs with DSU.cpp]`
38. **Disjoint sparse table** — O(1) query for any associative op (not just idempotent). `[YKW: Data Structures/Disjoint Sparse Table.cpp]`
39. **Segment tree merging** — small-to-large merge of dynamic segment trees on a tree. `[YKW: Data Structures/Segment Tree Merging.cpp]`
40. **Sliding window aggregation (SWAG)** — monotonic queue generalized to any semigroup.

### P1
41. **Wavelet tree** — k-th smallest in range, rank/count, without offline. `[YKW: Data Structures/Wavelet Tree.cpp]`
42. **Persistent lazy segment tree**. `[YKW: Data Structures/Segment Tree Persistent Lazy.cpp]`
43. **Persistent DSU / partially persistent DSU**. `[YKW: Data Structures/DSU Partially Persistent.cpp, Persistent UnionFind.cpp]`
44. **Persistent array / queue / meldable heap**. `[YKW: Data Structures/Persistent Array.cpp, Persistent Queue.cpp, Persistent Meldable Heap.cpp]`
45. **Link-Cut Tree** — dynamic tree connectivity, path aggregates. `[YKW: Data Structures/Link Cut Tree.cpp]`
46. **Cartesian tree** — build in O(n) from an array, for RMQ-as-tree problems. `[YKW: Data Structures/Cartesian Tree.cpp]`
47. **Iterative lazy segment tree** — for when the recursive one TLEs.
48. **Segment tree with arithmetic-progression range update**. `[YKW: Data Structures/Segment Tree with Arithmetic Progression.cpp]`
49. **Dynamic 2D segment tree** (sparse). `[YKW: Data Structures/Segment Tree 2D Dynamic.cpp]`
50. **KD-tree** — nearest neighbour, orthogonal range counting. `[YKW: Data Structures/KD Tree.cpp]`
51. **Queue undo trick / static-to-dynamic transformation**. `[YKW: Data Structures/Queue Undo Trick.cpp, Static to Dynamic Trick.cpp]`
52. **Venice technique** — global add with per-element offsets. `[YKW: Data Structures/Venice Technique.cpp]`
53. **SQRT tree** — O(1) query, O(n log log n) build. `[YKW: Data Structures/SQRT Tree.cpp]`
54. **Sqrt decomposition with periodic rebuild** — the "buffer + rebuild every √q ops" pattern.
55. **Persistent treap**. `[YKW: Data Structures/Treap persistent.cpp]`
56. **Long-path decomposition** — O(1) k-th ancestor after O(n log n). `[YKW: Graph Theory/Long Path Decomposition.cpp]`
57. **XOR segment tree** (range xor-assign / xor-query). `[YKW: Data Structures/XOR Segment Tree.cpp]`

### P2
58. Top tree / Euler tour tree. `[YKW: Data Structures/Top Tree.cpp]`
59. Permutation tree (PQ-tree for subarray-is-permutation queries). `[YKW: Data Structures/Permutation Tree.cpp]`
60. Balanced bracket tree; binarizing a tree. `[YKW: Data Structures/Balanced Bracket Tree.cpp, Binarizing a Tree.cpp]`
61. 4D Mo; Mo's online. `[YKW: Data Structures/4D MO.cpp, MOs Online.cpp]`
62. Dynamic tree diameter (online). `[YKW: Data Structures/Dynamic Diameter Online.cpp]`
63. Kinetic segment tree.
64. Generic fractional cascading.
65. Splay tree / RBST.

---

## 2. Graph Theory

Have: topological sort, Dijkstra, Floyd–Warshall, Bellman–Ford, dominator tree, bridges, cut
points, SCC, 2-SAT (×2 + prefix + segment-tree variants), Eulerian path, Dinic, MCMF, Hopcroft–Karp,
shortest-path-under-edge-modification.

### P0 — the big holes
66. **Minimum spanning tree** — Kruskal **and** Prim. *There is no MST in this notebook at all.* `[YKW: Graph Theory/Krushkal's MST.cpp, Prim's MST.cpp]`
67. **Plain BFS / DFS reference** — with the standard bookkeeping (parent, color, entry/exit, path reconstruction). `[YKW: Graph Theory/BFS.cpp, DFS.cpp]`
68. **Cycle detection + extraction** — directed (color DFS) and undirected (DSU / DFS), returning the actual cycle. `[YKW: Graph Theory/Cycle Detection.cpp]`
69. **Kuhn's algorithm** — simple O(VE) bipartite matching. Hopcroft–Karp alone is overkill and harder to modify under pressure. `[YKW: Graph Theory/Kuhns Algorithm.cpp]`
70. **König's theorem toolkit** — min vertex cover, max independent set, min path cover on a DAG, all recovered from a bipartite matching.
71. **Hungarian algorithm** — min-cost perfect matching on a dense bipartite graph, O(n³). `[YKW: Graph Theory/Hungarian Algorithm.cpp]`
72. **Block-cut tree / biconnected components** — both vertex-BCC and edge-BCC (bridge tree). `[YKW: Graph Theory/Block Cut Tree.cpp]`
73. **Bridge tree (2-edge-connected component condensation)**.
74. **Functional graph handling** — `i → f(i)`, rho shape, cycle detection, distance-to-cycle, k-th successor via binary lifting.
75. **Dijkstra over a segment tree of range edges** — "connect u to all of [l,r]" in O(n log n). `[YKW: Graph Theory/Dijkstra on Segment Tree.cpp]`
76. **Maximum closure / project selection** (min-cut modelling). `[YKW: Graph Theory/Maximum Closure Problem.cpp]`
77. **Flows with lower bounds (L-R flow)** — feasible flow, min/max flow with bounds. `[YKW: Graph Theory/L R Flow with Dinic.cpp, L R Flow with MCMF.cpp]`
78. **Matrix-tree theorem (Kirchhoff)** — count spanning trees. `[YKW: Graph Theory/Kirchoffs Theorem.cpp]`
79. **Virtual tree / auxiliary tree** — for multi-query tree problems. `[YKW: Graph Theory/Virtual Tree.cpp]`
80. **Tree isomorphism / canonical subtree hashing (AHU)**. `[YKW: Graph Theory/Tree Isomorphism.cpp]`
81. **System of difference constraints** → Bellman–Ford. `[YKW: Graph Theory/System Of Difference Constraints.cpp]`

### P1
82. **SPFA with negative-cycle extraction** (not just detection). `[YKW: Graph Theory/SPFA.cpp]`
83. **Johnson's algorithm** — all-pairs shortest path with negative edges. `[YKW: Graph Theory/Johnson's Algorithm.cpp]`
84. **MCMF with potentials (Dijkstra-based)** — much faster than the SPFA version you have. `[YKW: Graph Theory/Min Cost Max Flow.cpp]`
85. **MCMF with negative cycles**. `[YKW: Graph Theory/Min Cost Max Flow with Negative Cycles.cpp]`
86. **Stoer–Wagner** — global min cut, no source/sink. `[YKW: Graph Theory/Stoer Wagner Algorithm.cpp]`
87. **Gomory–Hu tree** — all-pairs min cut. `[YKW: Graph Theory/Gomory Hu Tree.cpp]`
88. **Blossom algorithm** — general (non-bipartite) matching, unweighted and weighted. `[YKW: Graph Theory/Blossom Algorithm.cpp, Blossom Algorithm Weighted.cpp]`
89. **Directed MST (Chu–Liu/Edmonds)**. `[YKW: Graph Theory/Directed MST.cpp]`
90. **Manhattan MST** — O(n log n) via 8-sector sweeps. `[YKW: Graph Theory/Manhattan MST.cpp]`
91. **Borůvka** — needed for MST-with-xor / MST-on-implicit-graph problems. `[YKW: Graph Theory/Boruvka's Algorithm.cpp]`
92. **Minimum mean weight cycle** (Karp). `[YKW: Graph Theory/Minimum Mean Weight Cycle.cpp]`
93. **Girth / minimum weight cycle** through each vertex or edge. `[YKW: Graph Theory/Minimum Weight Cycle For Each Vertex.cpp]`
94. **Maximum clique / maximum independent set** — Bron–Kerbosch + the meet-in-the-middle fast version. `[YKW: Graph Theory/Maximum Clique.cpp, Maximum Clique and Maximum Independent Set Super Fast.cpp]`
95. **Prüfer code** ↔ labeled tree; Cayley's formula. `[YKW: Graph Theory/Prufer Code.cpp]`
96. **Steiner tree** — DP over subsets of terminals. `[YKW: Graph Theory/Steiner Tree Problem.cpp]`
97. **Stable marriage (Gale–Shapley)**. `[YKW: Graph Theory/Stable Marriage Problem.cpp]`
98. **Eulerian path with full validity checks** — degree conditions, connectivity, and correct start vertex, directed and undirected separately. `[YKW: Graph Theory/Euler Path Directed.cpp, Euler Path Undirected.cpp]`
99. **Lexicographically smallest topological order** (priority queue) and counting topological orders.
100. **Maximum density subgraph** (binary search + min cut). `[YKW: Graph Theory/Maximum Density Subgraph.cpp]`
101. **Online bridges / online 2-edge connectivity**. `[YKW: Graph Theory/Online Articulation Bridges.cpp]`
102. **k shortest paths (Eppstein / Yen)**. `[YKW: Graph Theory/Eppsteins Algorithm.cpp]`
103. **Minimum diameter spanning tree**. `[YKW: Graph Theory/Minimum Diameter Spanning Tree.cpp]`
104. **Dynamic MST (offline)**. `[YKW: Graph Theory/Dynamic MST Offline.cpp]`
105. **Chromatic number / chromatic polynomial**. `[YKW: Graph Theory/Chromatic Number.cpp, Chromatic Polynoimial.cpp]`
106. **Edge colouring** — bipartite (Δ colours) and Vizing for simple graphs. `[YKW: Graph Theory/Edge Coloring Bipartite Graph.cpp, Edge Coloring Simple Graph.cpp]`

### P2
107. Three-edge connectivity. `[YKW: Graph Theory/Three Edge Connectivity.cpp]`
108. Cactus graph algorithms; cycle basis. `[YKW: Graph Theory/Cactus Graph.cpp]`
109. Chordal graphs, perfect elimination ordering. `[YKW: Graph Theory/Chordal Graph.cpp]`
110. Chinese postman problem. `[YKW: Graph Theory/Chinese Postman Problem.cpp]`
111. Min cut / Gomory–Hu on planar graphs. `[YKW: Graph Theory/Min Cut in a Planar Graph.cpp, Gomory Hu Tree of Planar Graph.cpp]`
112. Counting: labeled graphs, DAGs, arborescences, cliques. `[YKW: Graph Theory/Counting Labeled Graphs.cpp, Number of DAG.cpp, Number of Arborescence.cpp, Number of Different Cliques.cpp]`
113. Tutte matrix / randomized general matching. `[YKW: Graph Theory/Tuttes Theorem.cpp, Randomized Matching Unweighted.cpp]`
114. Hafnian / permanent of a matrix. `[YKW: Graph Theory/Hafnian of a Matrix.cpp]`
115. Path union / path intersection on a tree. `[YKW: Graph Theory/Path Union.cpp, Path Intersection.cpp]`
116. ST-numbering; tree orientation. `[YKW: Graph Theory/ST Numbering.cpp, Tree Orientation.cpp]`
117. Unique min cut check. `[YKW: Graph Theory/Unique Min Cut.cpp]`
118. Number of paths of each length in a tree (centroid + FFT). `[YKW: Graph Theory/Number of Paths of Each Length in a Tree.cpp]`
119. 3-cycle / 4-cycle counting. `[YKW: Graph Theory/3 CYCLE and 4 CYCLE.cpp]`
120. 3-SAT (randomized / Schöning). `[YKW: Graph Theory/3 SAT.cpp]`
121. Inverse (complement) graph BFS in O(V+E). `[YKW: Graph Theory/Inverse Graph.cpp]`
122. Min-cost vertex cover on a general graph. `[YKW: Graph Theory/Minimum Cost Vertex Cover General Graph.cpp]`
123. Shortest path avoiding each edge. `[YKW: Graph Theory/Shortest Path That Doesnt Pass Through Each Edge.cpp]`
124. Dynamic DAG reachability. `[YKW: Graph Theory/DAG Reachability Dynamic.cpp]`

---

## 3. Trees

Have: LCA (Euler + sparse table), DSU on tree, HLD, centroid decomposition, tree diameter, Euler tour.

### P0
125. **Binary-lifting LCA** — the up-table version, with `kth_ancestor` and `jump(u, v, k)`. The Euler-tour LCA you have can't do k-th ancestor.
126. **Rerooting DP template** — compute f(root = v) for all v in O(n), with a pluggable merge.
127. **Tree flattening for path updates** — Euler tour with ±1 + BIT (subtree add / path sum, and the reverse).
128. **HLD with edge weights** and a documented non-commutative path merge (left half reversed).
129. **Small-to-large merging** — generic `map`/`set` merge helper (distinct from your sack).
130. **Tree hashing** — canonical rooted and unrooted forms for isomorphism/dedup.
131. **Virtual tree** (also listed under Graph, needs to live next to HLD).

### P1
132. **LCA in O(1)** — ±1 RMQ / sparse table on the Euler tour with block decomposition. `[YKW: Graph Theory/LCA in O(1).cpp]`
133. **Centroid decomposition with a persistent per-centroid structure** — the distance-array pattern for "count pairs at distance ≤ k" and updates.
134. **Tree knapsack** — the O(n²) subtree-size-bounded merge, with the proof-of-complexity note.
135. **Counting paths with a property** — centroid + BIT/sorted-merge boilerplate.
136. **Level ancestor / jump pointer structure** — including the ladder decomposition variant.
137. **AHU canonical form** — group isomorphic subtrees.
138. **Auxiliary structures on the centroid tree** — parent chains, `dist_to_ancestor` cache.
139. **k-th vertex on a path** (LCA + k-th ancestor combined helper).

### P2
140. Dynamic tree diameter under edge updates.
141. Long-path decomposition for depth-indexed DP (`dsu on tree` for depth arrays in O(n)).
142. Maximum sum of pairwise distances of k nodes. `[YKW: Miscellaneous/Maximum of Sum of All Pair Distances of K nodes in a Tree.cpp]`

---

## 4. Number Theory

Have: extended Euclid, linear Diophantine (2 and n vars), linear sieve + SPF, trial-division
factorization, divisors, bitset sieve to 1e9, segmented sieve, CRT, floor sum.

### P0 — this section is missing its most-used primitives
143. **Miller–Rabin** — deterministic for 64-bit with the standard 7-base witness set. `[YKW: Number Theory/Miller Rabin.cpp]`
144. **Pollard's rho** — factor up to 10^18 in ~O(n^(1/4)). Without this and Miller–Rabin, any problem with `a_i ≤ 10^18` is unsolvable from this notebook. `[YKW: Number Theory/Pollard Rho.cpp]`
145. **Euler's totient for a single value** (via factorization). You only have the sieve.
146. **Divisor count τ(n) and divisor sum σ(n)** — from factorization, and via linear sieve for a range. `[YKW: Number Theory/Linear Sieve for Multiplicative Functions.cpp]`
147. **Baby-step giant-step (discrete log)** — plus the extended version for non-coprime bases. `[YKW: Number Theory/Discrete Log.cpp]`
148. **Tonelli–Shanks** — square root mod p. `[YKW: Number Theory/Tonelli Shanks Algorithm.cpp]`
149. **Primitive root** mod p / mod n. `[YKW: Number Theory/Primitive Root.cpp]`
150. **Lucas' theorem** and **generalized Lucas** (nCr mod prime power, then CRT for arbitrary mod). `[YKW: Number Theory/Lucas Theorem.cpp, nCr Modulo Any Mod.cpp]`
151. **Linear congruence solver** `ax ≡ b (mod m)` returning all solutions. `[YKW: Number Theory/Linear Congruence Equation.cpp]`
152. **Fast Fibonacci (fast doubling)** + **Pisano period**. `[YKW: Number Theory/Fibonacci Number Faster.cpp, Pisano Period.cpp]`

### P1
153. **Multiplicative order** of a mod n. `[YKW: Number Theory/Multiplicative Order.cpp]`
154. **Discrete k-th root** mod p. `[YKW: Number Theory/Discrete Root.cpp]`
155. **Prime counting π(n)** — Lucy_Hedgehog / Meissel. `[YKW: Number Theory/Prime Counting Function.cpp]`
156. **Min_25 sieve** — prefix sums of multiplicative functions to 10^11. `[YKW: Number Theory/Min_25 Sieve.cpp]`
157. **Dirichlet convolution / divisor-sum inversion** toolkit. `[YKW: Number Theory/Dirichlet Convolution.cpp]`
158. **Generalized floor sum** — `Σ floor((a·i+b)/m) · i^k`, and the ACL `floor_sum` extension. `[YKW: Number Theory/Generalized Floor Sum of Arithmetic Progressions.cpp]`
159. **Sum of divisors over a range** / hyperbola method (`Σ_{i≤n} τ(i)`, `Σ σ(i)`). `[YKW: Number Theory/Sum of Floors.cpp]`
160. **Power tower** `a^a^…^a mod m` via the generalized Euler theorem. `[YKW: Number Theory/Power Tower.cpp]`
161. **Continued fractions / rational approximation**. `[YKW: Number Theory/Continued Fractions.cpp, Rational Approximation.cpp]`
162. **Pythagorean triples** generation (primitive + all). `[YKW: Number Theory/Pythagorean  Triplets.cpp]`
163. **Sum of two squares (Fermat)** — representation + count. `[YKW: Number Theory/Fermats Theorem on Sum of Two Squares.cpp]`
164. **Number of solutions to `x² ≡ 1 (mod m)`**, and to `ax + by ≤ c`. `[YKW: Number Theory/Number of Solutions to x^2 = 1 mod m.cpp]`
165. **Smallest x with `l ≤ ax mod p ≤ r`**. `[YKW: Number Theory/Smallest Nonnegative Integer x s.t. l <= ax % p <= r.cpp]`
166. **Linear Diophantine with non-negative-solution counting** (you have the solver, not the count). `[YKW: Number Theory/Linear Diophantine Equation with Nonnegative Solutions.cpp]`
167. **Prime basis** — factor a set of numbers over a common basis without full factorization. `[YKW: Number Theory/Prime Basis.cpp]`
168. **Segmented linear sieve** for multiplicative functions over `[L, R]`.

### P2
169. Powerful number sieve. `[YKW: Number Theory/Powerful Number Sieve.cpp]`
170. Pell's equation. `[YKW: Number Theory/Pells Equation.py]`
171. Factoradic / mixed-radix / prime number system. `[YKW: Number Theory/Factoradic Number System.cpp, Prime Number System.cpp]`
172. Smallest number with exactly k divisors; numbers with exactly k divisors. `[YKW: Number Theory/Smallest Number Having Exactly K Divisors.cpp, K Divisors.cpp]`
173. LCM of Fibonacci numbers; maximum coprime product. `[YKW: Number Theory/LCM of Fibonacci Numbers.cpp, Maximum Coprime Product.cpp]`
174. Number of distinct k-th powers mod n. `[YKW: Number Theory/Number of Distinct Kth Powers Modulo n.cpp]`
175. `Φ` field / quadratic-integer arithmetic. `[YKW: Number Theory/Phi Field.cpp]`
176. Intersection of arithmetic progressions. `[YKW: Number Theory/Intersection of Arithmetic Progressions.cpp]`

---

## 5. Combinatorics

Have: **two files** — stars & bars, Burnside/Pólya (non-modular, overflowing). This is the
weakest section relative to how often it is needed.

### P0
177. **Factorials + inverse factorials mod p, O(n) build** — move/duplicate from `06 - Math` into this section, with `nCr`, `nPr`, `multinomial`, and `C(n, k)` for `n` up to 10^18 with small `k`.
178. **Catalan numbers** — closed form, recurrence, ballot problem, cycle lemma, and the "paths not crossing a diagonal" reflection argument.
179. **Stirling numbers** — first and second kind: single value, whole row (via NTT), whole column. `[YKW: Number Theory/Stirling Number of the First Kind for Fixed n.cpp` and 3 siblings`]`
180. **Bell numbers** — Bell triangle O(n²) and the EGF/NTT method. `[YKW: Number Theory/Bell Number.cpp]`
181. **Derangements** `!n` and partial derangements (exactly k fixed points). `[YKW: Number Theory/Derangement.cpp]`
182. **Inclusion–exclusion recipe page** — surjections, "at least k" → "exactly k" via binomial inversion, coprime counting with Möbius, union of divisibility classes.
183. **Binomial identity cheat sheet** — hockey stick, Vandermonde, `Σ C(n,k)·k`, `Σ C(n,k)²`, upper/lower negation, absorption. One printed page saves derivation time.
184. **Burnside with a modulus** — plus cycle indices for cyclic, dihedral, symmetric groups; necklace and bracelet counts mod p.
185. **Subset-sum convolution / SOS-based counting** — ranked zeta/Möbius transform.
186. **Twelvefold way table** — balls/boxes distinguishable or not, with/without surjectivity, mapped to formulas.

### P1
187. **Integer partitions** — `p(n)` via pentagonal number theorem O(n√n), partitions into k parts, distinct parts. `[YKW: Number Theory/Partition Function.cpp]`
188. **Lindström–Gessel–Viennot** — non-intersecting lattice paths as a determinant.
189. **Matrix-tree theorem** (also under Graph) — as a *counting* tool.
190. **Prüfer / Cayley** — count labeled forests with given degrees or components.
191. **Prefix sums of `C(n, i)` over `i`** and sums over a fixed congruence class of `i`. `[YKW: Number Theory/Prefix Sum Queries of nCi.cpp, Sum of nCi over a Fixed Congruence Class.cpp]`
192. **`Σ C(a_i, k)` for all k** (via NTT). `[YKW: Number Theory/Sum of C(a(i), k) for each k from 1 to n.cpp]`
193. **Sum of `nCi` for fixed large n**. `[YKW: Number Theory/Sum of nCi for a Fixed Large n.cpp]`
194. **Motzkin / Narayana / Schröder / super-Catalan** — with the problems each maps to.
195. **Hook length formula / Young tableaux counting**.
196. **q-binomials (Gaussian binomial coefficients)**. `[YKW: Math/q Binomial.cpp]`
197. **Generating-function recipe page** — OGF/EGF for common sequences, product/composition rules, "coefficient extraction" patterns.

### P2
198. Combinatorics on necklaces with fixed content (Pólya with a variable per colour).
199. Counting labeled structures (graphs, DAGs, connected graphs) via exponential formula.
200. Permanent / hafnian as counting devices.
201. Bijections page — Catalan bijections, RSK correspondence sketch.

---

## 6. Math / Linear algebra / Polynomials

Have: modular helpers, `Mint`, φ sieve, Möbius sieve, matrix mult + power (two variants), XOR
basis, FFT (`convMod` only, doesn't compile standalone), NTT (transform only), FWHT, FPS with a
naive O(n²) multiply.

### P0 — linear algebra is entirely absent
202. **Gaussian elimination over ℝ** — with partial pivoting, rank, free variables, and solution reconstruction. `[YKW: Math/Gaussian Elimination.cpp]`
203. **Gaussian elimination mod p**. `[YKW: Math/Gaussian Elimination Modular.cpp]`
204. **Gaussian elimination mod 2 with `bitset`** — O(n³/64), for xor-equation systems. `[YKW: Math/Gaussian Elimination Modulo 2.cpp]`
205. **Matrix determinant** — mod prime, and mod composite (Euclidean-style row reduction). `[YKW: Math/Determinant.cpp, Determinant under Composite Modulo.cpp]`
206. **Matrix inverse** — mod p and mod 2. `[YKW: Math/Inverse of A Matrix.cpp]`
207. **Matrix rank**.
208. **Working `conv()`** for real/integer FFT convolution, plus a compile-clean `FFT.cpp` (fix `vi`, `1i`).
209. **NTT `multiply()` wrapper** + **arbitrary-mod NTT** (three-prime CRT or the split trick). `[YKW: Math/NTT.cpp, NTT With Any Prime MOD.cpp]`
210. **Polynomial class** — `inv`, `exp`, `log`, `sqrt`, `pow`, division/mod, shift, all on a real NTT. Your FPS is a stub. `[YKW: Math/Polynomial.cpp]`
211. **Berlekamp–Massey** + **Kitamasa / linear-recurrence k-th term**. Solves "find the pattern" and huge-n DP. `[YKW: Math/BerleKamp Massey.cpp, Linear Recurrence.cpp]`
212. **Lagrange interpolation** — general, and the O(k) consecutive-points version for polynomial prefix sums. `[YKW: Math/Lagrange Interpolation.cpp]`
213. **Faulhaber / `Σ i^k`** in O(k) or O(k log k). `[YKW: Math/Faulhaber Formula Fastest.cpp]`
214. **XOR basis, extended** — reduced row echelon form, k-th smallest representable value, count of distinct values, basis merge, basis of a range (prefix-basis trick). Your current `Basis` only does insert + max. `[YKW: Math/Basis Vector Reduced Row Echelon Form.cpp]`
215. **Subset sum with `bitset`** and **bounded knapsack via binary grouping** — promote `bitsetONfrequencies` to a proper file. `[YKW: Math/Subset Sum Problem.cpp]`

### P1
216. **Min-plus / max-plus convolution** for convex sequences (SMAWK or the merge trick). `[YKW: Math/Max Convolution between Convex Funtions.cpp, Miscellaneous/Min Plus Convolution (Convex and Convex).cpp]`
217. **Subset (zeta/Möbius) convolution** — ranked SOS, O(2ⁿn²). `[YKW: Dynamic Programming Optimizations/SOS convolutions.cpp]`
218. **Online / relaxed convolution** (divide & conquer NTT) for self-referential DP. `[YKW: Math/NTT Online (D&C).cpp]`
219. **2D NTT / 2D convolution**. `[YKW: Math/NTT 2D.cpp]`
220. **Simplex / linear programming**. `[YKW: Math/Simplex Algorithm.cpp]`
221. **Numerical integration** — Simpson with adaptive subdivision, Romberg. `[YKW: Math/Integration (Simpsons).cpp, Integration (Romberg).cpp]`
222. **Root finding** — bisection and Newton on `double`, plus real ternary search with an iteration-count note.
223. **`Mint` with runtime mod / Montgomery multiplication** for speed. `[YKW: Math/Modint.cpp]`
224. **Matrix exponentiation wrapper for DP** — build transition matrix from a recurrence, with the "add a constant row" trick.
225. **Characteristic polynomial**; **determinant of a sparse matrix**. `[YKW: Math/Characteristic Polynomial Faster.cpp, Determinant of Sparse Matrix.cpp]`
226. **Linear recurrence with polynomial coefficients (P-recursive)**. `[YKW: Math/Linear Recurrence With Polynomial Coefficients.cpp]`
227. **Multipoint evaluation & polynomial interpolation** (O(n log²n)).
228. **Polynomial factorization** over F_p. `[YKW: Math/Polynomial Factorization.cpp]`
229. **Freivalds' algorithm** — randomized matrix-product verification. `[YKW: Math/Freivalds Algorithm.cpp]`
230. **FWHT in bases other than 2** (ternary/any). `[YKW: Math/FWHT in Any Base.cpp]`
231. **Cyclic convolution** (arbitrary length, Bluestein). `[YKW: Math/Cyclic Convolution.cpp]`
232. **Thomas algorithm** (tridiagonal systems). `[YKW: Math/Thomas Algorithm.cpp]`

### P2
233. Reed–Solomon / Reeds–Sloane (linear recurrence mod prime power). `[YKW: Math/Reeds Sloane Algorithm.cpp]`
234. Permanent / hafnian of a matrix. `[YKW: Math/Permanent of a Matrix.cpp, Hafnian of a Matrix.cpp]`
235. Determinants of structured matrices (cyclic, permanent, product). `[YKW: Math/Determinant of Cyclic Matrix.cpp` and siblings`]`
236. Vandermonde solving; generating function of a linear recurrence. `[YKW: Math/Vandermonde Matrix.cpp, Generating Function of a Linear Recurrence.cpp]`
237. Finite field GF(2^k) arithmetic. `[YKW: Math/Finite Field Arithmetic Binary.cpp]`
238. Lagrange multipliers (continuous optimization). `[YKW: Math/Lagrange Multiplier.cpp]`
239. Polynomial sum `Σ f(i) r^i` for polynomial f. `[YKW: Math/Polynomial Sum.cpp]`
240. Number of solutions of an equation by meet-in-the-middle. `[YKW: Math/Number of Solutions of A Equation MITM.cpp]`
241. "Slime trick" (mergeable multiset DP). `[YKW: Math/The Slime Trick.cpp]`

---

## 7. Strings

Have: KMP (+ automaton), Rabin–Karp, Z-function, hashing (buggy base), Aho–Corasick, suffix array
(+ LCP sparse table), minimal suffix automaton, Manacher, palindromic tree (+ rollback).

### P0
242. **Rewrite the hash struct** — random bases ≥ alphabet size, double mod, `get(l, r)` without modular inverses (store prefix powers and compare `h[r]·p^{n-l}`), plus `concat` and `equal(l1,r1,l2,r2)`. `[YKW: Strings/String Hashing.cpp]`
243. **Suffix automaton, fully equipped** — endpos counts (`cnt` via link-tree topological order), number of distinct substrings, total occurrences, first occurrence position, k-th lexicographic substring, longest common substring of two strings, link tree. Yours is the bare `extend()`. `[YKW: Strings/Suffix Automaton.cpp]`
244. **Generalized suffix automaton** — multiple strings in one machine.
245. **Suffix array applications** — count distinct substrings, compare two substrings in O(1), longest repeated substring, LCS via SA+LCP with a separator, and the "k-th smallest substring" walk.
246. **Duval's algorithm (Lyndon factorization)** and **Booth's algorithm** — minimal cyclic rotation. Extremely common and completely absent.
247. **LCS / edit distance** — the plain O(nm) DP with reconstruction, plus **Hirschberg** for O(min(n,m)) memory. `[YKW: Strings/All Substring Longest Common Subsequence.cpp, Dynamic Programming Optimizations/Hirschbergs Algorithm.cpp]`
248. **Borders / periods from the prefix function** — all borders, smallest period, "is a repetition of".

### P1
249. **2D hashing** — for grid pattern matching. `[YKW: Strings/String Hashing 2D.cpp]`
250. **Hashing with point updates** — hash in a BIT / segment tree, and reversible hashing for palindrome checks. `[YKW: Strings/String Hashing With Updates and Reverse.cpp]`
251. **Aho–Corasick + DP over the automaton** — "count strings of length L avoiding all patterns" boilerplate.
252. **Dynamic Aho–Corasick** — the log-buckets-of-tries trick for online insertion. `[YKW: Strings/Aho Corasick Dynamic.cpp]`
253. **String matching with bitsets** — Shift-Or, O(nm/64), and mismatch-tolerant variants. `[YKW: Strings/String Matching using Bitsets.cpp]`
254. **String matching with FFT** — wildcards and mismatch counting. `[YKW: Strings/String Matching With FFT.cpp]`
255. **Bit-parallel LCS** — O(nm/64). `[YKW: Strings/Bit LCS.cpp]`
256. **SA-IS** — O(n) suffix array for n ≥ 10^6.
257. **De Bruijn sequence** construction. `[YKW: Strings/De Bruijn Sequence.cpp]`
258. **Number of palindromic substrings in a range**; **minimum palindromic factorization** as its own file (currently buried inside the eertree). `[YKW: Strings/Number of Palindromes in Range.cpp, Minimum Palindrome Factorization.cpp]`
259. **Z-function applications page** — string periodicity, "is t a substring of s+s", compressed string equality.
260. **Prefix automaton / KMP automaton for multiple patterns**. `[YKW: Strings/Prefix Automaton.cpp]`

### P2
261. Suffix tree (Ukkonen), or suffix tree derived from the suffix automaton of the reversed string.
262. Cyclic LCS. `[YKW: Strings/Cyclic LCS.cpp]`
263. Isomorphic suffix array (matching under alphabet permutation). `[YKW: Strings/Suffix Array Isomorphic.cpp]`
264. Palindromic tree, persistent. `[YKW: Strings/Palindromic Tree Persistent.cpp]`
265. Runs / Main–Lorentz (all maximal repetitions).
266. Suffix automaton of a trie.
267. Distinct-substring queries over a range. `[YKW: Strings/Suffix Automaton Distinct Substring Queries in Range.cpp]`
268. Aho–Corasick all-pair occurrence relations. `[YKW: Strings/Aho Corasick All Pair Occurrence Relation.cpp]`

---

## 8. Dynamic Programming

Have: divide & conquer optimization, CHT (multiset), rollback CHT, Li Chao, persistent Li Chao,
Knuth, Alien's trick (×2), SOS DP, bitset bounded knapsack (extensionless), max/min-average CHT.

### P0 — the classics are missing while the exotic optimizations are present
269. **Digit DP** — the `(pos, tight, started, state)` template, for counting numbers in `[L, R]` with a digit property. `[YKW: Dynamic Programming Optimizations/Digit DP.cpp]`
270. **LIS in O(n log n)** — length *and* reconstruction, plus non-decreasing / longest-non-increasing variants.
271. **Knapsack family** — 0/1, unbounded, bounded via binary grouping, bounded via monotonic deque, plus reconstruction and the "count ways" version.
272. **Bitmask DP** — TSP, minimum set cover, assignment, and the submask-enumeration O(3ⁿ) partition DP.
273. **Broken profile / plug DP** — grid tilings and connected-region counting. `[YKW: Dynamic Programming Optimizations/Connected Component DP.cpp]`
274. **Monotonic (deque) CHT** — O(1) amortized when both slopes and queries are sorted. You only have the O(log n) versions.
275. **Slope trick** — two heaps + lazy shifts, for convex piecewise-linear DP. Completely absent, and it wins whole problems.
276. **Interval DP** — matrix chain / optimal BST / palindrome partition skeleton (Knuth is the optimization, but the base pattern isn't written down).
277. **Tree DP skeletons** — subtree DP, rerooting, tree knapsack (cross-link to §3).
278. **Grundy / game DP** — cross-link to §9.

### P1
279. **SOS DP, complete** — subset sums, superset sums, inverse transform, and subset convolution. `[YKW: Dynamic Programming Optimizations/SOS DP.cpp, SOS convolutions.cpp]`
280. **DP over divisors** — the `for (d) for (multiples of d)` harmonic pattern. `[YKW: Dynamic Programming Optimizations/DP Over Divisors.cpp]`
281. **1D1D DP** — the online monotone-stack / "Knuth without a matrix" pattern. `[YKW: Dynamic Programming Optimizations/1D1D DP.cpp]`
282. **Dynamic CHT (fully dynamic, arbitrary insert order)** — Li Chao covers it, but add the balanced-BST hull too. `[YKW: Dynamic Programming Optimizations/Dynamic Convex Hull Trick.cpp]`
283. **Persistent CHT**. `[YKW: Dynamic Programming Optimizations/Persistent CHT.cpp]`
284. **Alien's trick, complete** — real-valued λ, tie-handling for collinear hull points, and *reconstruction* of the k-item solution.
285. **Expected value / probability DP** — the "solve a linear system on states" pattern for cyclic dependencies (needs Gaussian elimination, §6).
286. **Bitset DP** — reachability, subset-sum, transitive closure in O(n³/64).
287. **Matrix exponentiation DP** — long-transition recurrences (cross-link to §6).
288. **Subset sum in O(n√n / 64)** with the distinct-values trick. `[YKW: Dynamic Programming Optimizations/Subset Sum in SQRT.cpp]`
289. **Subset union of bitsets** — the classic offline trick. `[YKW: Dynamic Programming Optimizations/Subset Union of Bitsets.cpp]`
290. **DP on convex hulls / DP where the transition is a Minkowski sum**. `[YKW: Dynamic Programming Optimizations/DP on Convex Hulls.cpp]`
291. **`x2 + 1` trick** (double-and-add DP over prefixes). `[YKW: Dynamic Programming Optimizations/x2 +1 trick.cpp]`
292. **XOR equation counting DP**. `[YKW: Dynamic Programming Optimizations/XOR Equation.cpp]`
293. **Dynamic submask counting**. `[YKW: Dynamic Programming Optimizations/Dynamic Submask Count.cpp]`

### P2
294. SMAWK / totally monotone matrix search (the O(n) alternative to D&C opt).
295. Number of subsequences with product ≥ K. `[YKW: Dynamic Programming Optimizations/Number of Subsequences Having Product at least K.cpp]`
296. Lagrangian relaxation beyond Alien's (multi-constraint).
297. Memoization helpers — hashed state maps, `array`-of-`array` allocators, and iterative-deepening for DP with huge state spaces.

---

## 9. Game Theory — **entire section absent**

The README advertises this section; the directory does not exist. Recreate it as `notebook/10 - Game Theory/`.

### P0
298. **Sprague–Grundy** — `mex`, grundy of a position, XOR of independent games, memoized computation over a DAG.
299. **Nim** — normal play, and the misère rule.
300. **Nim variants** — staircase nim, Moore's Nim_k, subtraction games, "take-away with a set S", Wythoff's game, Fibonacci nim.
301. **Grundy for common games** — Kayles, Dawson's chess, coin-turning games, Mock Turtles — with the periodicity note (compute a prefix, detect the period, extrapolate).
302. **Game DP on explicit states** — win/lose BFS from terminal positions (retrograde analysis) on a game graph with cycles.

### P1
303. **Matching game on a graph** — first player wins iff the start vertex is in every maximum matching. `[YKW: Game Theory/Matching Game On A Graph.cpp]`
304. **Green Hackenbush** — grundy of trees/graphs by fusion. `[YKW: Game Theory/Green Hackenbush.cpp]`
305. **Blue–Red Hackenbush** — surreal-number values. `[YKW: Game Theory/Blue Red Hackenbush.cpp]`
306. **Alpha–beta pruning / minimax** with memo. `[YKW: Game Theory/Alpha Beta Pruning.cpp]`

### P2
307. Partisan game theory basics — surreal numbers, temperature, cooling.
308. Combinatorial game "sum of games" arithmetic beyond XOR.

---

## 10. Geometry

Have: Point2D, lines/segments/rays, angles, polygon area, circumcircle, circle–line intersection,
circle–circle *area*, convex hull, rotating calipers (diameter + width), minimum enclosing circle,
half-plane intersection, Point3D.

### P0 — the predicates are missing, only the exotica is present
309. **Point in polygon** — O(n) ray casting / winding for arbitrary polygons, **and** O(log n) for convex. Absent, and it's in half the geometry problems.
310. **Integer-exact geometry mode** — `long long` points with `sgn`/`orient` returning `{-1,0,1}`, and a documented "don't use `double` here" rule. Everything is currently double-first.
311. **Angular sort** — exact comparator using half-plane + cross product, no `atan2`.
312. **Polygon basics** — perimeter, centroid, is-convex, orientation (CW/CCW), normalization.
313. **Convex polygon queries** — extreme point in a direction, tangents from an external point, point-in-convex, all in O(log n).
314. **Closest pair of points** — divide & conquer or sweep. `[YKW: Geometry/Closest Pair of Points.cpp]`
315. **Minkowski sum of convex polygons** — plus the "max/min distance between two convex hulls" application.
316. **Polygon clipping by a half-plane (Sutherland–Hodgman)** and convex-polygon intersection.
317. **Rectangle union area / perimeter** — sweep line + segment tree. `[YKW: Geometry/Rectangle Union.cpp]`
318. **Circle tangents** — point→circle, circle↔circle internal/external common tangents.
319. **Pick's theorem + lattice points** — on a segment (`gcd`), in a polygon.
320. **Segment intersection, exact** — proper/improper, collinear overlap, using integer orientation (yours goes through `lineInter` + `double`).

### P1
321. **Circle–polygon intersection area** — the standard "sum of signed triangle-circle areas".
322. **Area of union of circles**.
323. **Dynamic convex hull** — insert points, query extreme. `[YKW: Geometry/Convex Hull Dynamic.cpp]`
324. **All-pairs segment intersection (Bentley–Ottmann / sweep)**. `[YKW: Geometry/All Pair Segment Intersection.cpp]`
325. **Minimum-area / minimum-perimeter enclosing rectangle** (rotating calipers).
326. **Convex layers (onion decomposition)**. `[YKW: Geometry/Onion Decomposition.cpp]`
327. **3D geometry** — plane representation, line–plane and plane–plane intersection, distance point–plane, tetrahedron volume, 3D convex hull. You only have `Point3D`. `[YKW: Geometry/Geometry 3D.cpp]`
328. **Half-plane intersection, dynamic**. `[YKW: Geometry/Half Plane Intersection Dynamic.cpp]`
329. **Triangle centers** — incenter, circumcenter (have), orthocenter, centroid; incircle and excircle radii.
330. **Maximum points on a line / collinear triples**.
331. **Delaunay triangulation** and **Voronoi diagram**. `[YKW: Geometry/Delaunay Triangulation.cpp, Voronoi Diagram.cpp]`
332. **Point location in a planar subdivision**. `[YKW: Geometry/Point Location.cpp]`
333. **Maximum triangle area given side lengths / from a point set**. `[YKW: Geometry/Maximum Area of Triangle, Given are Lengths.cpp]`

### P2
334. Spherical geometry — great-circle distance, spherical polygon area.
335. Smallest enclosing sphere (3D Welzl).
336. Line arrangement / duality transforms.
337. Rotating calipers on two hulls (min distance between convex polygons).
338. Integer circle lattice points (`x² + y² = n` counting — cross-link to §4 sum-of-two-squares).

---

## 11. Repo hygiene (do these first, they're 20 minutes total)

339. **P0** — rename the 12 extensionless files to `.cpp` (AUDIT.md §1.1). Twelve algorithms currently do not exist as far as the printed notebook is concerned.
340. **P0** — fix the bugs in AUDIT.md Part 2 (lazy segtree build range, LCA OOB, hashing base, 2SAT API, beats overflow, Burnside overflow, `random()` returning zeros).
341. **P0** — recreate `notebook/10 - Game Theory/`.
342. **P1** — make `export_notebook.js` **warn** on any non-`.cpp` file it skips, instead of dropping it silently.
343. **P1** — regenerate `notebook.tex` / `notebook.pdf`; add a `.gitignore` for `*.aux *.log *.toc *.out` and decide whether the PDF should be committed at all.
344. **P1** — fix the README (portrait not landscape; Game Theory).
345. **P1** — replace `ó` / `μ` with ASCII, or turn on `extendedchars`.
346. **P2** — add a one-page index at the front of the PDF: "problem smell → template name", which is what you actually navigate by during a contest.
347. **P2** — make every file self-contained about its dependencies (a `// needs: Mint, NTT` header line). Several files silently depend on `Template.cpp` macros (`pb`, `all`, `st`, `nd`) that aren't all defined there.
