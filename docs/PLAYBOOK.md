# Team Notebook Playbook

Every technique the notebook should carry, what each one *does*, and what else you can point it at.
Priorities come from measured data — see [CONTEST-INTEL.md](CONTEST-INTEL.md). Bugs in what already
exists are in [AUDIT.md](AUDIT.md).

**Tier** — T0 build first (ECPC-2024-proven or top rising CF tag or unlocks a family) ·
T1 build second (regular regional material) · T2 last / notes-only.
**Status** — ✅ in the notebook · ⚠️ partial or buggy · ❌ missing.

---

# Part I — Problem smell → technique

The index you actually use at minute 40 of a contest. Read the constraint, jump to the row.

## By constraint

| You see | It's almost certainly |
|---|---|
| `n ≤ 20` | bitmask DP over subsets, meet-in-the-middle |
| `n ≤ 40` | meet-in-the-middle (2^20 halves) |
| `n ≤ 100` | O(n³) — Floyd, Gaussian elimination, matrix ops, interval DP |
| `n ≤ 500` | O(n³) or O(n² log n) — Hungarian, DP on pairs |
| `n ≤ 5000` | O(n²) — LCS/edit distance, tree knapsack, D&C opt |
| `n ≤ 2·10⁵`, queries | segment tree / BIT / offline sorting / Mo's |
| `n ≤ 10⁶`, no queries | O(n) or O(n log n) — prefix sums, monotonic stack, two pointers, Kadane |
| `n ≤ 10⁹` (a *value*, not a count) | binary search on answer, digit DP, dynamic segtree, math formula |
| `aᵢ ≤ 10¹⁸` | Miller–Rabin + Pollard rho, `__int128` |
| `Σn ≤ 10⁶` over test cases | per-test O(n); never allocate global arrays per test |
| answer `mod 1e9+7` | counting → combinatorics / DP / matrix power / expected value |
| `mod 998244353` | NTT is intended |
| ≤ 20 queries to an oracle | interactive binary search (`2^20 > 10⁶`) |
| output real number, `1e-6` | geometry, ternary search, or binary search on reals |

## By phrasing

| Phrase in the statement | Reach for |
|---|---|
| "minimum number of X to cover [L,R]" | greedy farthest-reach + **binary lifting on reach** (ECPC24-D) |
| "for every prefix / for every i" | incremental DS, or offline sweep with a BIT |
| "maximum XOR of a subset" | **XOR basis** (+ segment tree of bases if it's a range query) |
| "minimum XOR with an ancestor" | **binary trie with erase**, maintained on the DFS stack (ECPC24-O) |
| "count subarrays such that ..." | two pointers, divide & conquer, or contribution per element via monotonic stack |
| "count subarrays where all adjacent diffs ≥ x", many x | **offline: sort queries by x**, maintain runs with DSU |
| "is it possible to ... within k operations" | binary search on the answer + greedy check |
| "expected value / probability" | linearity of expectation first; then modular inverse; then linear system if cyclic |
| "the array is a permutation" | cycle decomposition, functional graph |
| "each node has exactly one outgoing edge" | **functional graph** — rho shape, binary lifting |
| "distance between every pair" | Floyd (n≤500), or centroid decomposition, or BFS from each node |
| "after each update, output ..." | offline segment-tree-on-time + rollback DSU, or a real dynamic DS |
| "lexicographically smallest" | greedy + a "can I still finish?" feasibility check |
| "OR of range equals SUM of range" | the values are **bit-disjoint** → at most 30 non-zero per window → two pointers |
| "choose exactly k" | Alien's/WQS trick, or DP with a k dimension, or exchange argument |
| "sum over all pairs of gcd/lcm" | **divisor-sum / Möbius / Euler-φ identity** (ECPC24-F) |
| "grid, move to adjacent cell" | BFS / 0-1 BFS / Dijkstra on the grid |
| "tree, queries on paths" | LCA + prefix-on-Euler-tour, or HLD |
| "tree, queries on subtrees" | Euler tour → contiguous range → any array DS |
| "first/last position where predicate holds" | **segment tree descent** (ECPC24-N) |
| "two players play optimally" | Sprague–Grundy / mex, or win-lose retrograde BFS |

---

# Part II — Technique catalog

---

## 0. Core & contest infrastructure

### 0.1 Solution template · T0 · ⚠️
Header, macros, multi-test loop, precision setup.
- **Fix:** the current one always does `cin >> tc`; add a commented single-test path, and add
  `typedef vector<int> vi` (the FFT file needs it and it isn't there).

### 0.2 Debug macro `dbg(...)` · T0 · ❌
Variadic printer for containers, pairs, tuples, guarded by `#ifdef LOCAL`.
- **Solves:** the actual bottleneck in a 5-hour contest — seeing state without rewriting `cout` lines.
- **Also:** pairs with the stress tester below; print the failing case directly.

### 0.3 Stress tester · T0 · ❌
`gen.cpp` + `brute.cpp` + a bash loop that diffs until mismatch.
- **Solves:** every WA on a problem you *believe* is right. Single highest-ROI 20 lines in a notebook.
- **Also:** validates your own new templates before the contest; finds edge cases in geometry and DP.

### 0.4 Binary search idioms · T0 · ❌
`firstTrue(lo,hi,pred)` / `lastTrue(lo,hi,pred)` on integers; fixed-iteration version on reals.
- **Solves:** "minimum k such that feasible", scheduling, capacity, "minimize the maximum".
- **Also:** the outer loop of parallel binary search; binary search on the answer inside a DP check;
  interactive query budgeting (ECPC24-A).

### 0.5 Ternary search · T1 · ❌
Integer (`while (r-l > 2)`) and real (fixed ~200 iterations) versions for unimodal functions.
- **Solves:** minimize a convex cost over a parameter — placement problems, "best meeting point".
- **Also:** inner search of Alien's trick; geometry (closest point on a curve); convex DP checks.

### 0.6 Integer sqrt / cbrt with correction · T0 · ❌
`sqrtl` then `while (r*r > n) r--; while ((r+1)*(r+1) <= n) r++;`
- **Solves:** solving `x·y=B, x+y=S`; perfect-square tests; segmented sieve bounds.
- **Also:** ECPC 2025 qualification used exactly this shape. Fixes the `sqrt(R)` bug in
  `SegmentedSieve.cpp`. Also needed for Pell / sum-of-two-squares.

### 0.7 Bit tricks page · T0 · ❌
Submask enumeration `for (s=m; s; s=(s-1)&m)`, `__builtin_popcount/ctz/clz`, next-permutation-of-bits,
iterate k-subsets, lowbit.
- **Solves:** bitmask DP transitions, SOS DP, subset convolution, "split into two groups".
- **Also:** the O(3ⁿ) subset-partition DP; XOR basis reasoning; bitset optimizations.

### 0.8 `__int128` and safe modmul · T0 · ⚠️
Have the I/O; need `mulmod(a,b,m) = (__int128)a*b%m`.
- **Solves:** Miller–Rabin and Pollard rho (which are unusable without it), CRT with big moduli
  (fixes the overflow noted in `CRT.cpp`), overflow-safe comparisons in CHT and geometry.

### 0.9 Interval set / ODT ("Chtholly") · T0 · ❌
`set<{l,r,val}>` with `split(pos)` and `assign(l,r,v)`; amortized near-linear when there are many
range-assigns.
- **Solves:** range-assign + range-query; painting problems; "how many distinct values remain".
- **Also:** the standard trick for **range-AND / range-OR / range-mod** updates (ECPC24-H) because
  those operations rapidly make ranges constant.

### 0.10 Parallel binary search · T1 · ❌
Binary search the answer for all queries simultaneously, sweeping the shared structure once per level.
- **Solves:** "after which update does query q become satisfiable"; k-th smallest offline.
- **Also:** pairs with rollback DSU and BIT; a common alternative to persistence.

### 0.11 Fast I/O · T1 · ⚠️
- **Fix:** `write_int` always appends `\n`, so you cannot print space-separated output. Add a
  separator parameter. Needed for problems with `n ≤ 10⁶` (ECPC24-B, ECPC24-J).

### 0.12 Ordered set / multiset (pbds) · T1 · ⚠️
`find_by_order`, `order_of_key`.
- **Solves:** k-th smallest with insert/erase, rank queries, inversion counting online.
- **Also:** document the `pair<T,int>` trick to get a multiset; the raw `tree` alone can't.

### 0.13 BigInt · T1 · ❌ · 0.14 Fraction/rational · T1 · ❌ · 0.15 Expression parser · T1 · ❌
- BigInt for exact huge arithmetic (factorials, Fibonacci without mod). Fraction for exact
  comparison of ratios — also the right tool for "maximum average subarray" and geometry slopes.
  Parser for calculator/formula problems, which show up in ad-hoc slots.

### 0.16 Inversion counting · T1 · ❌
BIT or merge-sort.
- **Solves:** "minimum adjacent swaps to sort" and every disguise of it (bubble sort distance,
  permutation distance, "make the array equal to b").

### 0.17 Misc utilities · T2 · ❌
Dates/Julian day · Josephus · permutation cycles & `p^k` · Stern–Brocot / best rational approximation ·
Gray code (✅ have) · negative-base conversion · knight moves on infinite grid ·
matroid intersection · Schreier–Sims.

---

## 1. Data structures

### 1.1 Segment tree (point update, range query) · T0 · ✅
- **Also:** the merge function is the whole game — carry a struct (sum, max, count-of-max,
  best-prefix/suffix/subarray) and most "range" problems collapse into it. Max-subarray-in-range
  (ECPC24-J shape) is a 4-field merge.

### 1.2 Lazy segment tree · T0 · ⚠️ **buggy**
- **Fix first:** build recurses on `[0,n-1]`, query/update on `[0,sz-1]` → wrong shape when `n`
  isn't a power of two. See AUDIT §2.
- **Solves:** range add + range sum/min/max, range assign, range affine.

### 1.3 **Segment tree descent (walk)** · T0 · ❌
Descend the tree guided by node aggregates to find the first/last index satisfying a predicate, in
**O(log n)** instead of O(log² n).
- **Solves:** first index ≥ l with `a[i] ≥ x`; leftmost i where a prefix sum crosses S; "how far
  right can I extend before the condition breaks".
- **Also:** **ECPC 2024 N is exactly this**. Same idea gives Fenwick `lower_bound` (✅ already in
  `FenwickTree.cpp`), k-th element of a frequency tree, and the greedy step of interval covering.

### 1.4 BIT / Fenwick · T0 · ⚠️
Have point-update range-query 1D/2D/offline-2D.
- **Missing (T0):** **range-update range-query** via two BITs. Needed constantly for
  "add v to [l,r], ask sum of [l,r]" when a lazy segtree is overkill.
- **Also:** BIT over an Euler tour = subtree add / path sum on a tree (see 3.5).

### 1.5 Sparse table · T0 · ✅ · **Disjoint sparse table** · T1 · ❌
- Sparse table is O(1) only for idempotent ops (min/max/gcd/or/and). **Disjoint sparse table**
  extends O(1) to *any* associative op (sum, product, matrix, hash).
- **Also:** sparse table on `OR` is what makes ECPC24-I tractable; sparse table on LCP powers
  suffix-array substring comparison.

### 1.6 XOR basis (linear basis over GF(2)) · T0 · ⚠️ **incomplete**
Have insert + max. **Missing: reduced row echelon form, k-th smallest representable value, count of
distinct representables, and — critically — `merge(basisA, basisB)`.**
- **Solves:** max/min/k-th XOR of a subset; "can x be formed"; number of distinct subset-XORs
  (= 2^rank); XOR-linear-independence.
- **Also:** without `merge` you cannot put a basis in a segment tree node, which is exactly what
  **ECPC 2024 H** requires. Also used in: XOR-MST (Borůvka on a trie), cycle space of a graph
  (XOR of fundamental cycles), and solving xor-equation systems (see 6.4).

### 1.7 Binary trie · T0 · ⚠️ **needs erase**
Have insert + max-xor. **Missing: `erase`, `min_xor`, `count(x ^ y < k)`, k-th.**
- **Solves:** max/min XOR pair; count pairs with XOR in a range; XOR-closest.
- **Also:** **ECPC 2024 O** needs insert *and erase* so the trie tracks the current root→node path
  during DFS — a pattern worth writing down on its own ("DFS with a rollbackable global structure").
  Also the inner loop of XOR-MST and of "minimum XOR in a subtree".

### 1.8 DSU · T0 · ✅ · **weighted / potential DSU** · T0 · ❌
Plain + rollback exist. Missing the version storing `pot[v] = value(v) − value(parent)`.
- **Solves:** bipartiteness under edge additions, "is this system of `a−b=c` constraints consistent",
  parity/mod-k relations, "are these two in the same group with known offset".
- **Also:** rollback DSU (✅) + segment-tree-on-time = **offline dynamic connectivity** (1.13).

### 1.9 Kruskal reconstruction tree · T0 · ❌
Build a binary tree while running Kruskal; internal node = the merging edge's weight.
- **Solves:** "minimum possible maximum edge weight on a path u→v" (bottleneck) in O(1) after LCA;
  "all nodes reachable using edges ≤ w" = a subtree; offline reachability under a weight threshold.
- **Also:** turns many "binary search the threshold + DSU" problems into a single tree query;
  combines with Euler tour + BIT for counting.

### 1.10 Mo's algorithm · T0 · ✅ (array, subtree, path)
- **Missing (T0):** **Mo with updates** (3D Mo, block `n^{2/3}`) and **Mo with rollback**
  (add-only structures where remove is hard — e.g. maintaining a max, or a DSU).
- **Also:** Mo + XOR basis, Mo + DSU, Mo + BIT are all standard combos.

### 1.11 Monotonic stack / queue · T0 · ✅
- **Also:** the "contribution technique" — for each element find the range where it is the
  minimum/maximum, then sum `a[i] × count`. That single pattern answers a whole family of
  "sum over all subarrays of min/max/..." problems. Write the pattern down next to the code.

### 1.12 Sliding window aggregation (SWAG) · T1 · ❌
Two-stack queue generalized to any semigroup (not just min/max).
- **Solves:** sliding-window gcd, matrix product, hash, "any associative op over a moving window".
- **Also:** your `TwoStackQ` is the min/max special case — generalize it and delete the duplicate.

### 1.13 Offline dynamic connectivity · T1 · ❌
Segment tree over the timeline + rollback DSU.
- **Solves:** "edges appear and disappear, answer connectivity/component-count at each time".
- **Also:** the same skeleton answers offline "is there a path with all weights ≤ w at time t",
  and dynamic MST-ish problems.

### 1.14 Segment tree merging · T1 · ❌
Merge two dynamic segment trees in amortized O(n log n) total.
- **Solves:** "for each subtree, the most frequent colour / k-th value" without DSU-on-tree.
- **Also:** an alternative to small-to-large that also supports queries mid-merge.

### 1.15 Wavelet tree · T1 · ❌
- **Solves:** k-th smallest in a range, count of values ≤ x in a range, range-rank — online, without
  persistence and without Mo's.
- **Also:** replaces merge-sort tree with a better constant and more query types.

### 1.16 Persistent structures · T1 · ⚠️
Have persistent segtree, persistent Li Chao, persistent trie. Missing persistent **lazy** segtree,
persistent DSU, persistent array/queue.
- **Solves:** "answer as of version k", k-th smallest in a range (prefix roots), rollback in
  functional style.

### 1.17 Li Chao tree / CHT · T1 · ✅ (plus rollback, persistent)
- **Missing (T0):** the **monotonic deque CHT** — O(1) amortized when slopes and queries are both
  sorted. That's the version you want inside an O(n) DP.
- **Also:** Li Chao also answers "max over a set of lines/segments at x" for non-DP geometry.

### 1.18 Treap · T1 · ✅ (key + implicit)
- **Also:** implicit treap = array with O(log n) insert/erase/reverse/split — use it for
  "cut and paste a subarray", rope-like text problems, and k-th order statistics with structure.

### 1.19 Cartesian tree · T1 · ❌
Build in O(n) from an array (min-heap on value, BST on index).
- **Solves:** turns "range minimum" structure into a tree — largest rectangle in histogram,
  "for each element the range where it's minimal", divide-and-conquer on RMQ.

### 1.20 KD-tree · T1 · ❌ · Interval/segment-tree 2D dynamic · T1 · ❌
Nearest neighbour and orthogonal range counting when coordinates are large and sparse.

### 1.21 Advanced / read-only · T2
Link-Cut Tree (dynamic tree paths) · Top tree / ETT · SQRT tree · Venice technique
(global add with offsets) · queue-undo & static-to-dynamic transforms · permutation tree ·
4D Mo · Mo online · dynamic diameter.

---

## 2. Graphs

### 2.1 BFS / DFS / 0-1 BFS · T0 · ❌ (!)
There is no plain BFS or DFS reference in the notebook.
- **Solves:** shortest path on unweighted graphs and grids; connected components; bipartite check;
  cycle detection; topological order by DFS.
- **Also:** **0-1 BFS** (deque) for edges of weight 0/1 — grid problems where "turning costs 1",
  "breaking a wall costs 1"; **multi-source BFS** for "distance to nearest X"; **BFS on states**
  (position + mask + parity) is the workhorse of ad-hoc graph problems.

### 2.2 Dijkstra · T0 · ✅
- **Also:** Dijkstra on a *state graph* (node × fuel, node × parity, node × k-edges-used) is far more
  common than plain Dijkstra. Note the pattern next to the code.
- **Missing (T1):** **Dijkstra over a segment tree of range edges** — "connect u to every node in
  [l,r] with cost w" in O(n log n) instead of O(n²).

### 2.3 MST — Kruskal & Prim · T0 · ❌ (!)
**There is no MST anywhere in this notebook.** That is the most surprising single gap.
- **Solves:** minimum connection cost; second-best MST; "minimize the maximum edge" (also solvable
  by the Kruskal reconstruction tree, 1.9).
- **Also:** Kruskal's sorted-edge sweep is the base of KRT, of offline bottleneck queries, and of
  Borůvka; Prim is the one you want on dense/implicit graphs (e.g. complete graph on points).
- **Borůvka (T1):** the version that works when edges are implicit — **XOR-MST** (Borůvka + binary
  trie) and **Manhattan MST** (Borůvka + 8 sectors) are both real regional problems.

### 2.4 Cycle detection & extraction · T0 · ❌
Directed (colour DFS) and undirected (DFS parent / DSU), returning the *actual* cycle vertices.
- **Solves:** "find any cycle", "is it a DAG", "detect a contradiction in constraints".
- **Also:** negative-cycle extraction from Bellman–Ford; cycle in a functional graph (2.5).

### 2.5 Functional graph toolkit · T0 · ❌
Each node has exactly one out-edge → components are "rho" shaped: a tree hanging off a cycle.
- **Solves:** "apply the permutation k times", "where does the ball end up after k steps",
  "how many nodes eventually reach a cycle", "smallest k with f^k(x)=x".
- **Also:** combines with binary lifting (3.1) for `k ≤ 10¹⁸`; permutation problems are functional
  graphs in disguise; "each person points at one other person" statements.

### 2.6 Topological sort · T0 · ✅
- **Also:** lexicographically smallest topo order (priority queue), longest path on a DAG,
  counting paths on a DAG, and DP-over-DAG all sit on this. Add the priority-queue variant (T1).

### 2.7 SCC + condensation · T0 · ✅ · 2-SAT · T0 · ⚠️
- **Fix:** `01 - 2SAT.cpp` doesn't compile against this repo's SCC (AUDIT §2). The working one is
  the extensionless `2_sat2`, which also carries the **prefix at-most-one** and **segment-tree
  2-SAT** variants — both are genuinely strong; make them visible.
- **Solves:** any "choose one of two per item with pairwise constraints"; scheduling with conflicts;
  "place each item left or right".

### 2.8 Bridges, articulation points, block-cut tree · T0 · ⚠️
Bridges & cut points exist but **break on multi-edges** (AUDIT §2) — fix by skipping the incoming
*edge id*, not the parent vertex.
- **Missing (T0):** **bridge tree** (2-edge-connected condensation) and **block-cut tree**.
- **Solves:** "which edges, if removed, disconnect the graph"; "is there a path avoiding vertex v";
  the bridge tree turns "how many bridges on the path u→v" into a tree path query.

### 2.9 Bipartite matching — Kuhn · T0 · ❌ (Hopcroft–Karp ✅)
Kuhn is O(VE) but 15 lines and trivially modifiable; HK is neither.
- **Solves:** assignment, "maximum non-conflicting pairs", chess-board domino/rook placement.
- **Also (T0):** the **König toolkit** — min vertex cover, max independent set, min path cover on a
  DAG — are all recovered from one matching. Those three phrasings are far more common in statements
  than the word "matching".

### 2.10 Max flow (Dinic) · T1 · ✅ · min cut modelling · T0 · ❌
The code exists; the *modelling page* does not, and that's the part you need.
- **Solves:** max flow = min cut; **project selection / maximum closure** ("take profits, pay
  prerequisites"); bipartite vertex cover; "partition items into two sides with pairwise penalties".
- **Missing (T1):** **flows with lower bounds (L-R flow)**, MCMF with Johnson potentials (the
  current SPFA MCMF is slow and uses `int` cost — overflow risk).

### 2.11 Difference constraints · T1 · ❌
`x_j − x_i ≤ w` → edge, then Bellman–Ford/SPFA.
- **Solves:** scheduling with "task B starts ≥ 5 after A", feasibility of inequality systems,
  "assign values with pairwise gap constraints".

### 2.12 Eulerian path · T1 · ⚠️
Hierholzer exists but never checks degrees or connectivity, and can't pick a legal start vertex.
- **Solves:** "use every edge exactly once", de Bruijn sequences, reconstructing a word from k-mers.

### 2.13 Matrix-tree theorem (Kirchhoff) · T1 · ❌
Count spanning trees = any cofactor of the Laplacian determinant.
- **Needs:** determinant mod p (see 6.2) — which the notebook also lacks.
- **Also:** counts spanning forests, arborescences (directed version), and is a common
  "combinatorics wearing a graph costume" problem.

### 2.14 Virtual (auxiliary) tree · T1 · ❌
Compress k marked nodes + their pairwise LCAs into an O(k) tree.
- **Solves:** "Σ over queries of a tree DP restricted to k nodes" when Σk is bounded — the standard
  way to answer many tree queries without re-DPing the whole tree.

### 2.15 Tree isomorphism / canonical hashing · T1 · ❌
AHU canonical form, or randomized subtree hashing.
- **Solves:** "how many distinct subtree shapes", "are these two trees the same", dedup of states.

### 2.16 Other graph · T1
Johnson (APSP with negative edges) · Stoer–Wagner (global min cut, no s/t) · Gomory–Hu (all-pairs
min cut) · Hungarian (dense assignment, O(n³)) · directed MST (Chu–Liu) · minimum mean cycle (Karp) ·
Prüfer code ↔ labeled trees · Steiner tree DP · maximum clique (Bron–Kerbosch) ·
stable marriage · SPFA with negative-cycle extraction.

### 2.17 Read-only · T2
Blossom (general matching) · edge colouring · chromatic number/polynomial · planar min cut ·
three-edge connectivity · cactus · chordal graphs · Chinese postman · Eppstein k-shortest ·
counting labeled graphs / DAGs / cliques · Tutte matrix · hafnian.

---

## 3. Trees

### 3.1 Binary lifting · T0 · ❌ (buried)
The up-table currently exists only *inside* `MoOnPaths.cpp`. Extract it.
- **Solves:** k-th ancestor; LCA; "jump while a predicate holds" (first ancestor with weight > x);
  k-th vertex on a path.
- **Also:** binary lifting is not a tree technique — it's a **"jump 2^k steps" technique**. Same
  table answers: functional graphs (2.5), sparse jump pointers over intervals
  (**ECPC 2024 D**: "min lamps to cover [L,R]" = jump by farthest-reach), doubling on strings,
  and "apply the operation 10¹⁸ times".

### 3.2 LCA · T0 · ⚠️
Euler-tour + sparse table exists but **reads out of bounds** (AUDIT §2) and can't do k-th ancestor.
Keep it (it's O(1) query) *and* add binary lifting (3.1) for the jump queries.

### 3.3 Euler tour flattening · T0 · ✅
Subtree = contiguous range.
- **Also:** the bridge from *every* array data structure to trees. Subtree add/sum with a BIT;
  subtree k-th with a wavelet tree/merge-sort tree; **subtree min over a value computed during DFS**
  — which is exactly ECPC 2024 O.

### 3.4 DFS with a rollbackable global structure · T0 · ❌ (pattern)
Enter node → add its value to a global structure; leave → remove it. The structure then always
represents the current root→node path.
- **Solves:** "min XOR with any ancestor" (**ECPC 2024 O**, with a binary trie that supports erase),
  "is there an ancestor with value v", "max value on the path from the root".
- **Also:** the same skeleton with a `map` is small-to-large; with a BIT it's "count ancestors less
  than x"; with a stack it's "nearest ancestor satisfying P".

### 3.5 Path updates via Euler tour ±1 · T0 · ❌
`+v` at u, `+v` at w, `−v` at lca, `−v` at parent(lca)`, then subtree sums give path values.
- **Solves:** "add v to every node on path u–w, then query a node/subtree" without HLD.
- **Also:** the dual (point update, path query) is subtree-sum on the flipped formulation.

### 3.6 Rerooting DP · T0 · ❌
Compute `f(v as root)` for all v in O(n) with a down-pass and an up-pass.
- **Solves:** "for each node, the sum/max of distances to all others"; "for each root, the answer";
  any tree DP asked for every possible root.
- **Also:** the technique generalizes to "recompute an aggregate excluding one child" — prefix/suffix
  products over children — which is itself a reusable idiom.

### 3.7 HLD · T1 · ⚠️
Exists. Add: **edge weights mode** and a documented non-commutative merge (the left half of the path
comes out reversed).

### 3.8 Small-to-large / DSU on tree · T1 · ✅
- **Also:** the generic `map`-merge form (merge the smaller map into the larger) solves "count
  distinct colours in every subtree" with no sack bookkeeping — worth having both.

### 3.9 Centroid decomposition · T1 · ✅
- **Also:** the standard companion is "distance from each node to each of its O(log n) centroid
  ancestors" — cache it; that array answers "nearest marked node" and "count pairs at distance ≤ k".

### 3.10 Tree knapsack · T1 · ❌
The O(n²) subtree-size-bounded merge (with the pairing-argument complexity note).
- **Solves:** "choose k nodes from a tree maximizing X"; tree colouring with a budget.

### 3.11 Others · T1/T2
LCA in O(1) via ±1 RMQ · long-path decomposition (O(1) k-th ancestor) · AHU subtree grouping ·
counting paths with a property (centroid + BIT) · dynamic diameter.

---

## 4. Number theory

### 4.1 Miller–Rabin · T0 · ❌ (!)
Deterministic for 64-bit with the 7-witness set.
- **Solves:** primality for `n ≤ 10¹⁸`. Needs `mulmod` (0.8).
- **Also:** the gate for Pollard rho; also used to test candidate divisors quickly.

### 4.2 Pollard's rho · T0 · ❌ (!)
Factor `n ≤ 10¹⁸` in ~O(n^{1/4}).
- **Solves:** factor huge numbers → divisor count/sum, φ(n), "how many divisors of n are …".
- **Also:** with 4.1 these two are the **precondition for any problem with `aᵢ ≤ 10¹⁸`**. Without
  them, that entire class is unreachable from this notebook.

### 4.3 Sieves · T0 · ✅ (linear+SPF, bitset to 1e9, segmented)
- **Fix:** `Sieve1e9`'s 62 MB bitset must be global (stack overflow otherwise); `SegmentedSieve`
  needs the corrected `isqrt` (0.6).

### 4.4 Divisor-sum / Dirichlet machinery · T0 · ❌
`Σ_{d|n}`, Möbius inversion, `Σ_{i,j} f(gcd(i,j))` rewritten as `Σ_d f'(d)·(n/d)²`, φ-identities.
- **Solves:** "sum of gcd over all pairs", "count coprime pairs", "sum of lcm over all pairs",
  and **ECPC 2024 F** (`ΣΣ lcm(aᵢ,aⱼ)·lcm(i,j)`).
- **Also:** the same rewriting powers "count multiples", "Σ φ(i)", "Σ τ(i)" and DP over divisors
  (8.11). The notebook has a Möbius *sieve* but none of the *identities* — write the identity page.

### 4.5 Multiplicative function sieve · T0 · ❌
Linear sieve computing τ, σ, φ, μ, or any multiplicative f, in O(n).
- **Solves:** any "for all i ≤ n compute f(i)" where f is multiplicative — extremely common setup.

### 4.6 Single-value φ, τ, σ · T0 · ❌
From a factorization, not a sieve (needed when `n` is huge — pairs with 4.2).

### 4.7 Modular toolkit · T0 · ⚠️
`Mint`, factorials, inverse factorials.
- **Fix:** `build()` is O(N log MOD) — use `invFact[N-1] = inv(fact[N-1])` then descend (the ECPC
  2025 solutions do exactly this). O(N) matters when N = 10⁶.
- **Missing:** `inv[i]` linear recurrence `inv[i] = -(MOD/i)*inv[MOD%i] % MOD`.

### 4.8 BSGS (discrete log) · T0 · ❌ · Tonelli–Shanks (sqrt mod p) · T0 · ❌ · primitive root · T1 · ❌
- **Solves:** `a^x ≡ b`, `x² ≡ a`, generator of the multiplicative group.
- **Also:** discrete root, NTT with a custom modulus, and "cycle length of a modular map" all sit on
  these three.

### 4.9 Lucas & generalized Lucas · T0 · ❌
`C(n,r) mod p` for huge n; prime-power version + CRT for arbitrary modulus.
- **Solves:** counting problems with `n ≤ 10¹⁸` and a small modulus; grid paths mod small primes.

### 4.10 Linear congruence & CRT · T0 · ⚠️
CRT exists (fix the `__int128` overflow). Missing `ax ≡ b (mod m)` returning the full solution set.
- **Solves:** "find the smallest t with t ≡ rᵢ mod mᵢ"; meeting-time problems; calendar puzzles.

### 4.11 Fast Fibonacci + Pisano period · T1 · ❌
Fast doubling for F(n) mod m; Pisano for the period.
- **Also:** any 2-term linear recurrence; and Pisano is the "cycle length mod m" pattern in general.

### 4.12 Floor sums · T1 · ⚠️
`floor_sum` exists. Add the **generalized** version (`Σ i^k · floor((ai+b)/m)`) and the
`Σ_{i} floor(n/i)` hyperbola/blocking trick (`for (l=1; l<=n; l=r+1)`), which is the real workhorse.
- **Solves:** `Σ τ(i)`, `Σ σ(i)`, counting lattice points under a line, `Σ floor(n/i)·f(i)`.

### 4.13 Prime counting / Min_25 / powerful-number sieve · T2 · ❌
For `Σ f(i)` up to 10¹¹. Rare, but nothing else works when it appears.

### 4.14 Others · T1/T2
Multiplicative order · discrete k-th root · continued fractions & rational approximation ·
Pythagorean triples · sum of two squares · power tower (`a^a^a mod m`) · `x² ≡ 1 mod m` ·
prime basis · Pell.

---

## 5. Combinatorics

**The notebook has 2 files here and this is the fastest-rising Codeforces tag (+7.6).**
Treat this section as the top build priority after the T0 data-structure fixes.

### 5.1 nCr / nPr / multinomial mod p · T0 · ⚠️ (lives in the Math section)
Move it here and add: `C(n,k)` for huge n with small k; Pascal for tiny n without a modulus.

### 5.2 Inclusion–exclusion page · T0 · ❌
Recipes, not just theory: surjections `Σ(-1)^i C(k,i)(k-i)^n`; "at least k" → "exactly k" by binomial
inversion; coprime counting via μ; union of divisibility classes; derangements.
- **Also:** IE is the bridge between counting and number theory (4.4) and between counting and
  bitmask DP (8.4) — it's the most cross-cutting idea in the section.

### 5.3 Binomial identity cheat sheet · T0 · ❌
Hockey stick, Vandermonde, `ΣC(n,k)k`, `ΣC(n,k)²`, upper/lower negation, absorption.
- **Why code isn't enough:** the win is recognizing the closed form in 30 seconds. One printed page.

### 5.4 Catalan numbers · T0 · ❌
Closed form + recurrence + the reflection/cycle-lemma argument.
- **Solves:** balanced brackets, non-crossing structures, monotone lattice paths under a diagonal,
  binary tree shapes, stack-sortable permutations, triangulations.

### 5.5 Stirling numbers (1st & 2nd kind) · T0 · ❌
Single value, whole row (via NTT), whole column.
- **Solves:** "partition n labeled items into k non-empty groups" (2nd kind); permutations with k
  cycles (1st kind); converting `Σ i^k` into falling factorials.
- **Also:** the standard tool for "sum of k-th powers" problems and for surjection counting.

### 5.6 Bell numbers · T1 · ❌ · Derangements · T1 · ❌ · Partitions p(n) · T1 · ❌
Set partitions; no-fixed-point permutations; integer partitions via the pentagonal number theorem.

### 5.7 Burnside / Pólya, with a modulus · T0 · ⚠️ **overflows today**
- **Fix:** the current file uses non-modular `fast_pow`, so it overflows on any real input, and
  divides by `|G|` instead of multiplying by its inverse.
- **Solves:** count colourings up to rotation/reflection — necklaces, bracelets, cube faces,
  grid symmetries.

### 5.8 Twelvefold way table · T1 · ❌
Balls/boxes × distinguishable-or-not × injective/surjective/any → the formula for each of the 12.
- **Why:** most "how many ways to distribute" statements are one of these twelve, and picking the
  wrong cell is the usual error.

### 5.9 Lindström–Gessel–Viennot · T2 · ❌
Non-intersecting lattice paths = a determinant. Needs 6.2.

### 5.10 Others · T1/T2
Matrix-tree as counting (2.13) · Prüfer/Cayley · prefix sums of `C(n,i)` · `Σ C(aᵢ,k)` via NTT ·
Motzkin/Narayana/Schröder · hook length formula · q-binomials · generating-function recipe page.

---

## 6. Math, linear algebra, polynomials

### 6.1 Gaussian elimination · T0 · ❌ (!)
Three versions: over ℝ (partial pivoting), mod p, and mod 2 with `bitset` (O(n³/64)).
- **Solves:** linear systems; rank; free-variable enumeration; **expected-value DP with cyclic
  dependencies** (states → equations) which is the common modern use.
- **Also:** the mod-2 version solves XOR-equation systems, "light switch" puzzles, and computes the
  rank of a set of vectors — the dense sibling of the XOR basis (1.6).

### 6.2 Determinant & matrix inverse & rank · T0 · ❌
Mod prime, mod composite, and real.
- **Solves:** Matrix-tree spanning-tree counting (2.13), LGV (5.9), solving systems, testing linear
  dependence.

### 6.3 Matrix exponentiation · T0 · ✅
- **Also:** write down the *modelling* patterns next to it: linear recurrences; counting walks of
  length k; DP with a small state and a huge time axis; adding a constant row to handle `+c` terms;
  min-plus matrix product for "shortest path with exactly k edges".

### 6.4 Linear recurrences — Berlekamp–Massey + Kitamasa · T1 · ❌
Given the first ~2k terms, recover the recurrence; then jump to term n in O(k log k log n).
- **Solves:** "the answer looks like a pattern" — brute-force small n, feed BM, extrapolate.
  A genuinely general-purpose weapon for hard counting problems.

### 6.5 Lagrange interpolation · T0 · ❌
General form + the O(k) consecutive-points form.
- **Solves:** "the answer is a polynomial of degree ≤ k in n" — prefix sums of polynomials,
  `Σ i^k` (Faulhaber), DP answers that turn out polynomial.
- **Also:** pairs with 6.4: BM for linear recurrences, interpolation for polynomials.

### 6.6 FFT / NTT · T1 · ⚠️ **neither is usable as shipped**
`FFT.cpp` doesn't compile standalone (`vi`, `1i`); `NTT.cpp` has only the transform, no `multiply()`,
and redefines `MOD`.
- **Solves:** polynomial multiplication; counting pairs/triples with a given sum; string matching
  with wildcards; convolution of distributions.
- **Missing (T1):** arbitrary-mod NTT (3-prime CRT), online/relaxed convolution.

### 6.7 Polynomial class (FPS) · T1 · ⚠️ **claims O(n log n), is O(n²)**
`inv`, `log`, `exp`, `sqrt`, `pow`, division — all currently on a naive multiply.

### 6.8 Bitwise convolutions · T1 · ✅ FWHT · ❌ subset convolution
XOR/AND/OR convolution exists. Missing **subset (ranked zeta/Möbius) convolution**.
- **Solves:** "for each mask, combine over all splits into two disjoint submasks" — set-cover
  counting, partition-into-groups DP.

### 6.9 Subset sum with bitset / bounded knapsack · T0 · ⚠️
`bitsetONfrequencies` (binary grouping + `dp |= dp << w`) is correct but is one of the twelve
extension-less files, so it doesn't exist in the printed notebook. Promote it.

### 6.10 Numerical · T1
Simpson/Romberg integration · Newton & bisection root-finding · Freivalds' randomized product check.

### 6.11 Read-only · T2
Simplex/LP · characteristic polynomial · sparse determinant · polynomial factorization ·
multipoint evaluation · permanent/hafnian · GF(2^k) arithmetic · Thomas algorithm.

---

## 7. Strings

Note: `strings` is one of the *few declining* Codeforces tags (−1.1) and was not a theme of ECPC
2024. Fix what's broken, add the two cheap classics, and stop there unless you have time.

### 7.1 Hashing · T0 · ⚠️ **won't compile**
Uses `.st`/`.nd` which this template never defines; first base (67) is below the alphabet size;
both bases are fixed (anti-hashable).
- **Rewrite:** random bases, double mod, `get(l,r)` without modular inverses, plus `equal()` and
  `concat()`.
- **Also:** hashing is the general "compare two things in O(1)" hammer — substring equality,
  tree hashing (3.11), 2D grid pattern matching, dedup of states.

### 7.2 KMP / Z-function · T0 · ✅
- **Also:** write the *applications* down: all borders of a prefix; smallest period
  (`n − π[n]`); "is s a rotation of t" (`t in s+s`); string compression; and the prefix automaton
  for DP over a pattern.

### 7.3 Manacher · T0 · ✅
- **Also:** ECPC 2024 I pairs it with a bit-disjointness two-pointer. Note the combination pattern:
  "longest palindromic substring **subject to** another monotone predicate" → Manacher + binary
  search on radius, or two pointers.

### 7.4 Duval (Lyndon factorization) & Booth · T0 · ❌
Minimal cyclic rotation in O(n).
- **Solves:** "smallest rotation", canonical form of a cyclic string, comparing necklaces.
- **Also:** used for dedup under rotation, which links to Burnside (5.7).

### 7.5 Suffix automaton · T1 · ⚠️ (bare `extend()` only)
Add: endpos counts via the link tree, number of distinct substrings, total occurrences,
first-occurrence position, k-th lexicographic substring, **longest common substring of two strings**.
- **Also:** the link tree is a suffix tree of the reversed string — many "substring" questions become
  subtree aggregates on it (which then reuses 3.3).

### 7.6 Suffix array · T1 · ⚠️
Exists with LCP + sparse table, but `queryLcp` takes rank positions and the conversion is commented
out. Add the applications: count distinct substrings, O(1) substring comparison, longest repeated
substring, LCS via a separator.

### 7.7 Aho–Corasick · T1 · ✅
- **Also:** the high-value use isn't matching, it's **DP over the automaton**: "count strings of
  length L avoiding all patterns" — write that skeleton next to the code.

### 7.8 LCS / edit distance · T0 · ❌
Plain O(nm) with reconstruction; Hirschberg for O(min(n,m)) memory; bit-parallel LCS for O(nm/64).

### 7.9 Others · T1/T2
2D hashing · hashing with updates · string matching with bitsets (Shift-Or) or FFT (wildcards) ·
SA-IS · de Bruijn · palindromic tree ✅ (extension-less — rename) · Main–Lorentz runs.

---

## 8. Dynamic programming

The notebook has the exotic optimizations and is missing the classics.

### 8.1 Kadane family · T0 · ❌
Max subarray; max subarray with a constraint; max circular subarray; **max subarray as a segment-tree
merge** (sum, best-prefix, best-suffix, best).
- **Solves:** ECPC 2024 J ("max subarray of every prefix after ≤1 swap") is a Kadane variant.
- **Also:** the 4-field merge turns *any* range query about subarrays into a segment tree (1.1).

### 8.2 LIS in O(n log n) · T0 · ❌
Length + reconstruction; non-decreasing variant; LIS as patience sorting.
- **Also:** "longest chain of pairs", "maximum non-conflicting intervals ordered by one key",
  Dilworth (min chain cover = longest antichain).

### 8.3 Knapsack family · T0 · ❌
0/1, unbounded, bounded (binary grouping + monotonic-deque), counting version, reconstruction.
- **Also:** the deque-bounded version is the same monotonic-queue idea as 1.11.

### 8.4 Bitmask DP · T0 · ❌
TSP, assignment, minimum set cover, and the **O(3ⁿ) partition-into-groups** DP over submasks.
- **Also:** with IE (5.2) and SOS (8.10) these three form the whole `n ≤ 20` toolkit.

### 8.5 Digit DP · T0 · ❌
`(pos, tight, started, state)` memo for "count numbers in [L,R] with property P".
- **Solves:** digit-sum divisibility, "contains no 13", "digits non-decreasing", counting by base.
- **Also:** the same shape counts strings under a lexicographic bound, and works over any positional
  representation (binary → "count x ≤ N with popcount k").

### 8.6 Interval DP · T0 · ❌
`dp[l][r]` over lengths — matrix chain, optimal BST, palindrome partitioning, "burst balloons".
- **Also:** Knuth optimization (✅ have) is a speed-up *of this*, but the base pattern isn't written
  down anywhere in the notebook.

### 8.7 Tree DP skeletons · T0 · ❌ (see §3)
Subtree DP, rerooting (3.6), tree knapsack (3.10).

### 8.8 Slope trick · T0 · ❌
Maintain a convex piecewise-linear cost function as a pair of heaps with lazy shifts.
- **Solves:** "make the array non-decreasing with minimum total |change|"; scheduling with earliness/
  tardiness penalties; a whole family that looks unapproachable without it.

### 8.9 Monotonic-deque CHT · T0 · ❌
O(1) amortized when slopes and queries are both monotonic — see 1.17.

### 8.10 SOS DP · T0 · ⚠️
Subset sums exist; add superset sums, the inverse transform, and submask enumeration cost analysis.
- **Solves:** "for each mask, aggregate over all submasks"; counting pairs with `a & b == 0`;
  combined with IE for "count subsets with a given AND/OR".

### 8.11 DP over divisors · T1 · ❌
`for d: for multiples of d` — harmonic O(n log n).
- **Solves:** "count pairs with gcd = g", "for each d, how many are divisible by d" → Möbius (4.4).

### 8.12 Expected value / probability DP · T1 · ❌
Linearity of expectation first; modular inverse; and the **linear-system** form when states are
cyclic (needs 6.1).
- **Solves:** ECPC 2024 G; random-walk problems; "expected number of steps until".

### 8.13 Bitset DP · T1 · ❌
Reachability, subset-sum, transitive closure at O(n³/64) or O(n²/64).

### 8.14 Have already · ✅
D&C optimization · Knuth · Li Chao (+persistent) · rollback CHT · Alien's/WQS ·
1D max-average via CHT · binary-grouped bitset knapsack (extension-less).
- **Improve (T1):** Alien's needs real-valued λ, collinear-tie handling, and *reconstruction*.

---

## 9. Game theory — **the whole section is missing**

The README advertises `10 - Game Theory`; the directory does not exist. Meanwhile `games` is a
*rising* Codeforces tag (2.8% → 4.5%). This is cheap to build and currently zero.

### 9.1 Sprague–Grundy + mex · T0 · ❌
Grundy value of a position; XOR of independent games; memoized computation over a DAG.
- **Solves:** any impartial game; "split a pile", "move a token on a DAG", sums of independent games.

### 9.2 Nim and variants · T0 · ❌
Normal nim (XOR), misère rule, **staircase nim**, Moore's Nim_k, subtraction games, Wythoff,
Fibonacci nim.
- **Also:** staircase nim is the disguise used most often — "move coins leftward on a strip".

### 9.3 Retrograde analysis (win/lose BFS) · T0 · ❌
BFS backwards from terminal positions over a game graph *with cycles*, counting out-degrees.
- **Solves:** games where Grundy doesn't apply (partisan, or draws possible); "can the first player
  force a win from each state".

### 9.4 Grundy periodicity · T1 · ❌
Compute a prefix of Grundy values, detect the period, extrapolate.
- **Why:** the standard route for "n ≤ 10⁹" game problems; without it you can only brute force.

### 9.5 Others · T1/T2
Matching game on a graph (first player wins iff the start vertex is in every maximum matching) ·
Green / Blue-Red Hackenbush · alpha-beta minimax.

---

## 10. Geometry

Declining on Codeforces (−2.2) but still shows at regionals, and ECPC 2024 had one (E: angle
bisector + rotation). Build the **predicates**, skip the exotica.

### 10.1 Integer-exact core · T0 · ❌
`long long` points, `sgn(x)`, `orient(a,b,c) ∈ {−1,0,1}`, exact segment intersection.
- **Why:** the notebook is double-first, which is the single biggest source of geometry WAs.
  Use doubles only when the answer is real.

### 10.2 Point in polygon · T0 · ❌
O(n) ray casting / winding for arbitrary polygons; O(log n) for convex.
- **Solves:** appears in roughly half of all geometry problems.

### 10.3 Exact angular sort · T0 · ❌
Compare by half-plane (`y>0 or (y==0 and x>0)`) then cross product — no `atan2`.
- **Also:** the basis for radial sweeps, "count points in an angular range", visibility problems,
  and half-plane intersection ordering (which the notebook currently does with `atan2l`).

### 10.4 Angle/rotation helpers · T0 · ⚠️
Have `angleBetween`, `rotate`. Add: **signed** rotation angle (CW negative), angle bisector
direction, and normalization to (−π, π].
- **Solves:** ECPC 2024 E directly.

### 10.5 Polygon basics · T0 · ⚠️
Area ✅. Missing: perimeter, centroid, is-convex, orientation, and normalization to CCW.

### 10.6 Convex polygon queries · T0 · ❌
Extreme point in a direction, tangents from an outside point, point-in-convex — all O(log n).
- **Also:** these three are what make convex hull *useful* after you've built it (10.8 ✅).

### 10.7 Closest pair of points · T0 · ❌ · Minkowski sum · T1 · ❌ · polygon clipping · T1 · ❌
Closest pair (D&C or sweep); Minkowski sum for "min/max distance between two convex bodies";
Sutherland–Hodgman clipping for "area of a polygon inside a half-plane".

### 10.8 Have already · ✅
Convex hull · rotating calipers (diameter, width) · minimum enclosing circle · half-plane
intersection · circle–line and circle–circle area · circumcircle · Point3D.

### 10.9 Sweep-line geometry · T1 · ❌
Rectangle union area/perimeter (sweep + segment tree); all-pairs segment intersection.
- **Also:** the sweep + "segment tree over coordinates" skeleton is reusable for scheduling and
  interval-counting problems that aren't geometric at all.

### 10.10 Pick's theorem & lattice points · T1 · ❌
`A = I + B/2 − 1`; points on a segment = `gcd(|dx|,|dy|)`.
- **Also:** connects geometry to number theory (4.x) — counting lattice points under a line is a
  floor-sum (4.12).

### 10.11 Circles · T1 · ❌
Tangents (point→circle, circle↔circle), circle–polygon intersection area, union of circles.

### 10.12 3D · T1 · ⚠️
Only `Point3D` exists. Add plane representation, line–plane intersection, point–plane distance,
tetrahedron volume. 3D convex hull is T2.

### 10.13 Read-only · T2
Delaunay/Voronoi · point location · convex layers · dynamic hull · minimum-area enclosing rectangle ·
spherical geometry.

---

# Part III — Multi-use index

Techniques that pay for their page many times over. If build time is short, these first.

| Technique | Serves |
|---|---|
| **Binary lifting** (3.1) | tree k-th ancestor · LCA · functional graphs · interval cover by farthest-reach (ECPC24-D) · "apply op 10¹⁸ times" · doubling on strings |
| **Euler tour** (3.3) | subtree → range, so *every* array structure becomes a tree structure · path updates (3.5) · Mo on trees ✅ · suffix-automaton link tree (7.5) |
| **XOR basis** (1.6) | max/k-th subset XOR · segment tree of bases (ECPC24-H) · XOR-MST · graph cycle space · xor equations (with 6.1) |
| **Binary trie** (1.7) | max/min XOR pair · XOR with ancestors (ECPC24-O) · XOR-MST inner loop · count pairs with XOR < k |
| **Segment tree descent** (1.3) | first-index queries (ECPC24-N) · Fenwick lower_bound · k-th element · greedy extension steps |
| **Monotonic stack** (1.11) | next-greater · largest rectangle · **contribution technique** for "sum over all subarrays of min/max" · Cartesian tree (1.19) |
| **Divisor-sum / Möbius** (4.4) | gcd/lcm pair sums (ECPC24-F) · coprime counting · DP over divisors (8.11) · lattice-point counting |
| **Inclusion–exclusion** (5.2) | counting · number theory (coprimality) · bitmask DP · surjections · derangements |
| **Gaussian elimination** (6.1) | linear systems · rank · expected-value DP with cycles (8.12) · determinant → matrix-tree (2.13) · mod-2 xor systems |
| **Matrix exponentiation** (6.3) | linear recurrences · counting walks · huge-time DP · min-plus for k-edge shortest paths |
| **Hashing** (7.1) | substring equality · tree isomorphism (3.11) · 2D patterns · state dedup |
| **Binary search on answer** (0.4) | feasibility + greedy check · parallel BS (0.10) · Alien's trick · interactive budgets (ECPC24-A) |
| **Offline query sorting** | queries by threshold + DSU (ECPC24-M) · queries by right endpoint + BIT · Mo's · parallel binary search |
| **Rollback (DSU / trie / CHT)** | offline dynamic connectivity (1.13) · Mo with rollback (1.10) · DFS path structures (3.4) · divide-and-conquer over time |

---

# Part IV — Suggested build order

Each step is independently useful; stop anywhere.

**Step 0 — repo hygiene (≈30 min).** Rename the 12 extension-less files to `.cpp`; recreate
`notebook/10 - Game Theory/`; make the exporter warn on skipped files; fix the README.

**Step 1 — fix what's broken (≈2 h).** The seven P0 bugs in [AUDIT.md](AUDIT.md): lazy segtree build
range, LCA out-of-bounds, hashing macros, 2-SAT API, beats `int` overflow, Burnside modulus,
`random()` returning zeros.

**Step 2 — the ECPC-proven eight (≈1 day).** Segment tree descent · XOR basis with merge + basis in a
segment tree · binary trie with erase · standalone binary lifting · divisor-sum/Möbius identity page ·
interactive template · Kadane family · offline-query-sorting page. *These are the eight that would
have cost you problems at ECPC 2024.*

**Step 3 — the absent fundamentals (≈1 day).** MST (Kruskal + Prim) · BFS/DFS/0-1 BFS · Kuhn +
König · Miller–Rabin + Pollard rho · Gaussian elimination (ℝ / mod p / mod 2) · digit DP · LIS ·
knapsack family · point-in-polygon + exact orientation.

**Step 4 — combinatorics (≈half day).** It is the fastest-rising tag and the thinnest section:
inclusion–exclusion recipes · binomial identities · Catalan · Stirling · modular Burnside ·
twelvefold way.

**Step 5 — game theory (≈half day).** Grundy/mex · nim variants · retrograde analysis · periodicity.

**Step 6 — everything else,** by tier, as page budget allows.
