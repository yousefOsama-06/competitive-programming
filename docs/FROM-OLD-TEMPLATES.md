# What `temps/` still has that the notebook does not

A full audit of the 879 files in `temps/` (nine separate reference libraries) against the live
notebook. `temps/` itself is gitignored — it is a local reference pile, not part of the notebook.

Verdict: the notebook already covers everything in `temps/array templates/`, `temps/dp optimizations
temps/`, `temps/code-library/Basics/`, and all of `temps/Problem-Solving/` except two files.
`temps/Templates-main/` is byte-identical to `temps/Templates/Templates-main/`. What follows is the
genuine remainder, ordered by value.

## HIGH — real techniques the notebook lacks

### Graph theory (the biggest hole)
| From | What it gives you | Cost |
|---|---|---|
| `Graph Theory/Blossom Algorithm.cpp` | **Maximum matching in a GENERAL graph.** Everything we ship is bipartite, and `GraphTheorems.cpp` lists this under "polynomial despite appearances" with no code behind it. | O(V³) |
| `Graph Theory/Blossom Algorithm Weighted.cpp` | Max-**cost** general matching (dual labels). Also the engine for Chinese postman. | O(n³) |
| `Graph Theory/Gomory Hu Tree.cpp` | All-pairs min cut from n−1 max flows. Only *mentioned* in `MinCutModelling.cpp`. | (n−1)×Dinic |
| `Graph Theory/Minimum Mean Weight Cycle.cpp` | Karp's min mean cycle — the subroutine for every "minimise average cost around a cycle" and min-ratio problem. | O(VE log(1/eps)) |
| `Graph Theory/Chinese Postman Problem.cpp` | Undirected CPP = APSP + max-cost general matching on the odd-degree vertices. | O(n³) |
| `Graph Theory/Edge Coloring Simple Graph.cpp` | **Constructive Vizing Δ+1 colouring.** Our theorem sheet states the bound and then says "do not look for an algorithm" — the Δ+1 *construction* is polynomial and this is it. | O(nm) |
| `Graph Theory/Edge Coloring Bipartite Graph{,Faster}.cpp` | König Δ-edge-colouring: the timetabling / "split into Δ rounds" primitive our theorem sheet promises. | O(nm) / O(M log M) |
| `Graph Theory/Eppsteins Algorithm.cpp` | k shortest **walks**. `MergeableHeap.cpp` names Eppstein as its use case and ships no algorithm. | O(m log m + k log k) |
| `Graph Theory/Min Cost Max Flow with Negative Cycles.cpp` | Cost-scaling push-relabel MCMF — handles the negative-cost-cycle case ours explicitly refuses. | O(V³ log(VC)) |
| `Graph Theory/Three Edge Connectivity.cpp` | We stop at 2-edge (LowLink) and biconnected (BlockCutTree). | O(n) |
| `Graph Theory/Online Articulation Bridges.cpp` | Incremental bridge maintenance — the online counterpart to LowLink. | O(α) amortised |
| `Graph Theory/Chordal Graph.cpp` | Max-cardinality search / perfect elimination ordering. Our theorem sheet lists chordal graphs as a polynomial case with no code. | O(n+m) |
| `Graph Theory/Cactus Graph.cpp` | Cactus → tree of cycles, so path queries become tree queries. | O(n+m) |
| `Graph Theory/Chromatic Polynoimial.cpp` | The whole polynomial (i.e. "count proper k-colourings"), not just the chromatic number. | O(n²2ⁿ) |
| `Graph Theory/System Of Difference Constraints.cpp` | xᵢ − xⱼ ≤ w as shortest paths. We mention it only in the Problem Solving chapter. | O(VE) |
| `Graph Theory/Maximum Density Subgraph.cpp` | Binary search + parametric min cut. | O(log(1/eps)·flow) |
| `Graph Theory/Stable Marriage Problem.cpp` | Gale–Shapley. Nothing in the notebook. | O(n²) |
| `Graph Theory/Inverse Graph.cpp` | Traverse the **complement** in O(n+m) without materialising O(n²) edges. | O(n+mα) |
| `Graph Theory/Long Path Decomposition.cpp` | O(1) k-th ancestor after O(n log n), and O(n) depth-indexed tree DP. | O(n log n) |
| `Graph Theory/Minimum Diameter Spanning Tree.cpp` | MDST via absolute 1-centre. | O(V³) |
| `Graph Theory/Number of Paths of Each Length in a Tree.cpp` | Centroid decomposition + FFT. | O(n log² n) |
| `Graph Theory/Hafnian of a Matrix.cpp` | #perfect matchings of a general graph — the non-bipartite Ryser. | O(2^{n/2}n³) |

### Data structures / trees
`Data Structures/Link Cut Tree.cpp` (**the single biggest hole** — path *and* virtual-subtree
aggregates) · `Reachability Tree.cpp` (Kruskal reconstruction tree; `MST.cpp` only mentions it) ·
`Queue Undo Trick.cpp` · `Permutation Tree.cpp` (PQ-tree / common-interval decomposition) ·
`Static to Dynamic Trick.cpp` (logarithmic method) · `Dynamic Diameter Online.cpp` ·
`Persistent UnionFind.cpp` + `Persistent Array.cpp` · `Binarizing a Tree.cpp` (the precondition that
makes merge-DPs O(n log n)) · `SQRT Tree.cpp` (O(1) query for any associative op **with** updates).

### Number theory
`Min_25 Sieve.cpp` · `Powerful Number Sieve.cpp` · `Prime Basis.cpp` (coprime basis without ever
factoring) · `Generalized Floor Sum of Arithmetic Progressions.cpp` (strict generalisation of our
`floorSum`) · `Sum of nCi over a Fixed Congruence Class.cpp` (roots-of-unity filter — we have none) ·
`Number of ax%p in a Range.cpp` + `Smallest Nonnegative x s.t. l <= ax%p <= r.cpp` ·
`Pisano Period.cpp` + `Fibonacci Number Faster.cpp` (**we have no Fibonacci routine at all**) ·
`Sqrt Decomposition of Mod of Arithmetic Progression.cpp` · `Prefix Sum Queries of nCi.cpp` ·
`Sum of The Number of Divisors in cbrt(n).cpp` · the three nonnegative-solution Diophantine files ·
`K Divisors.cpp` (with a bundled Meissel–Lehmer).

### Math / polynomials
`Math/Polynomial.cpp` (**strict superset** of ours: multipoint evaluation, fast interpolation,
Taylor shift, composition, chirp-z, poly gcd, resultant, k-th root) · `Linear Recurrence
Fastest.cpp` (Bostan–Mori, O(k log k log n) vs our Kitamasa O(k² log n)) · `Linear Recurrence With
Polynomial Coefficients.cpp` (P-recursive guessing — sees n!, central binomials, Motzkin, which
Berlekamp–Massey cannot) · `Simplex Algorithm.cpp` (**we have no LP at all**) · `Determinant of
Sparse Matrix.cpp` (Wiedemann) · `Characteristic Polynomial Faster.cpp` · `Reeds Sloane
Algorithm.cpp` (BM mod a composite) · `Determinant under Composite Modulo.cpp` · `NTT Online.cpp`
(relaxed convolution for self-referential DPs) · `Thomas Algorithm.cpp` (our `ExpectedValueDP.cpp`
names it and ships nothing) · the four **Stirling row/column** files (O(n log n) vs our O(n²)) ·
`Basis Vector ft Weighted Linearly Independent Vectors.cpp` · `FWHT in {Any,Ternary} Base.cpp`.

### Strings
`De Bruijn Sequence.cpp` · `All Substring Longest Common Subsequence.cpp` · `Cyclic LCS.cpp` ·
`String Hashing With Updates and Reverse.cpp` (palindrome checks **under updates**) · `String
Matching using Bitsets.cpp` (bitap) · `Aho Corasick Dynamic.cpp` · `Suffix Automaton Distinct
Substring Queries in Range.cpp` (SAM + LCT `access`-as-painting — a beautiful reusable pattern) ·
`Number of Palindromes in Range.cpp` (bundles a wavelet tree with **`sum`**, which ours lacks) ·
`Suffix Array Isomorphic.cpp`.

### DP
`1D1D DP.cpp` (**the one major DP optimisation we have no code for**) · `Connected Component
DP.cpp` · `Hirschbergs Algorithm.cpp` · `Subset Sum in SQRT.cpp` · `Dynamic Submask Count.cpp`.

### Geometry
`Delaunay Triangulation.cpp` · `Voronoi Diagram.cpp` · `Point Location.cpp` · **3D convex hull**
(`Geometry 3D.cpp` lines ~367–540) · `Convex Hull Dynamic.cpp` · `Half Plane Intersection
Dynamic.cpp` · the 3D `coords` frame (project coplanar 3D points into 2D so every 2D routine
applies), line–line distance and closest points, solid angle / spherical triangle area.

### Miscellaneous
`Matroid Intersection` (+ graphic and linear oracles) · `Schreier–Sims` (permutation groups) ·
`Logarithmic Subarray Aggregator` (O(log A) distinct gcd/or values per right endpoint) ·
`MEX of all Subarrays` · `XOR Equation Arbitrary Range` · `Bitset Custom` (range set/reset/flip) ·
`Trygub Num` · `K-th Root of a Permutation` · `Fraction Binary Search` (Stern–Brocot — our
`ContinuedFractions.cpp` describes it and ships nothing) · `Dates.cpp` (Julian day number) ·
`Factoradic Number System` (permutation rank/unrank) · `Subset Union of Bitsets` ·
`Maximum of Sum of All Pair Distances of K nodes in a Tree` (convex tree DP by treap Minkowski
merge — far more general than its title).

## Where `temps/` beats what we ship

1. **Prefix XOR basis** — max-XOR over a suffix `[l,n]` in O(B) with one array, versus our
   segment-tree-of-bases at O(log n · B²). Also a variant that reports *which indices* produce x.
2. **MCMF with negative cycles** — ours refuses the case; theirs is also faster on dense graphs.
3. **BigInt** — theirs is signed, has true big/big `divmod`, `gcd`, stream I/O, and auto-switches to
   FFT multiplication. Ours is unsigned with a note where the sign handling should be.
4. **FPS** — theirs is our file plus multipoint evaluation, interpolation, Taylor shift,
   composition, chirp-z, gcd, resultant.
5. **Linear recurrences** — Bostan–Mori instead of Kitamasa; and Reeds–Sloane fixes a
   *precondition* (our BM divides, so it dies mod a composite).
6. **Maximum clique** — ours is capped at 32 vertices by an `unsigned` mask; theirs is a bitset and
   runs n = 155 in about a second.
7. **Matching games** — ours needs Kuhn, so bipartite only; theirs uses Blossom.
8. **Factorisation / prime counting** — Montgomery reduction (2–3× on 64-bit `factor`), and
   Meissel–Lehmer at n^(2/3). Keep Lucy too: it also gives prime *sums*.
9. **Divisor summatory** — Stern–Brocot hyperbola walk at ~n^(1/3), about 100 ms at n = 1e18,
   versus our O(√n) blocks.
10. **Determinant** — division-free, so it works mod a composite; ours needs a prime.
11. **Suffix array** — SA-IS (linear) versus our O(n log n) doubling.
12. **Onion peeling** — decremental hull, O(n log² n) versus our O(n² log n).
13. **Wavelet tree** — theirs adds `sum(l, r, x)`, which is what makes "sum of the k smallest in a
    range" possible.

## Known bugs in `temps/` — do not copy verbatim

* `Math/Determinant.cpp` — returns a `double` determinant through an `int` return type.
* `Strings/String Matching using Bitsets.cpp` — pushes a bogus `N` when there are no occurrences.
* `Strings/Palindromic Tree Persistent.cpp` — `static const int A = 2`; bump for real alphabets.
