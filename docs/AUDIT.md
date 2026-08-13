# Notebook Audit — full read of every file

Date: 2026-08-12. Scope: all 118 files under `notebook/` (106 `.cpp`, 12 extensionless), plus
`export_notebook.js`, `README.md`, `toc.yaml`, `notebook.tex`.

Totals: 5,429 lines of `.cpp` + ~1,330 lines in extensionless files. 10 live categories
(`10 - Game Theory` is listed in the README but does not exist on disk).

---

## Part 1 — Infrastructure problems

### 1.1 Twelve files have no `.cpp` extension → invisible in the PDF
`export_notebook.js:116` and `:123` filter on `name.endsWith('.cpp')`. Everything below is in
git but is silently dropped from `notebook.tex`, `toc.yaml`, and `notebook.pdf`. No warning is
printed. Renaming them is the single highest-value fix in the repo.

| File | Contains |
|---|---|
| `02 - Data Structures (DS)/dynamic_median_finding` | two-multiset median + cost-to-median |
| `02 - .../01 - Segment Tree/fast_merge_sort_tree` | merge sort tree w/ fractional cascading |
| `02 - .../01 - Segment Tree/getElementsWithPossiblity_of_having_frequency_greaterORequalTO_N_overK` | Misra–Gries / range majority candidates |
| `02 - .../06 - Square Root Decomposition/sqrtDecomposition` | plain block decomposition (snippet, not a struct) |
| `03 - Graph Theory/02 - Shortest Paths/DominatorTree` | Lengauer–Tarjan dominator tree |
| `03 - Graph Theory/02 - Shortest Paths/SPwithEdgeModify` | shortest path under single-edge-weight query (**newest commit**) |
| `03 - Graph Theory/04 - Satisfiability/2_sat2` | 3 × 2-SAT: classic, prefix at-most-one, segment-tree 2-SAT |
| `07 - Strings/05 - Palindromes/palindromic tree` | eertree + series links + min partition + count_string |
| `07 - Strings/05 - Palindromes/palindromicTreeWithRollBack` | eertree with push_back/pop_back |
| `08 - DP/01 - DP Optimizations/alien's trick` | worked WQS example (ABC218H) |
| `08 - DP/01 - DP Optimizations/bitsetONfrequencies` | bounded knapsack via binary grouping + bitset |
| `08 - DP/01 - DP Optimizations/MAX_OR_MIN_AVG_of_2points` | max/min average subarray via dynamic CHT |

### 1.2 `10 - Game Theory` does not exist
`README.md:20` advertises it ("MEX"). The directory is gone from disk. The committed
`notebook.tex` still contains a dangling empty `\section{Game Theory}`. See MISSING.md §9 —
this is a whole missing chapter, not just a stale line.

### 1.3 Committed build artifacts are stale
Re-running `node export_notebook.js` changes `notebook.tex` — `grayCode.cpp`'s `rev_g()` is
absent from the committed `.tex`, and the dead Game Theory section is still in it. So
`notebook.pdf` (1.6 MB, committed) does not match the sources. `notebook.toc` is also committed
even though the script deletes `.toc` as an aux file (`export_notebook.js:224`) — leftover from
a failed compile. There is no `.gitignore`.

### 1.4 README inaccuracies
- Claims "2-column **landscape** (A4)". The generated preamble is
  `\documentclass[9pt,a4paper,twocolumn]{extarticle}` — portrait.
- Claims `10 - Game Theory` exists.

### 1.5 Encoding
`\lstset{... extendedchars=false}` (`export_notebook.js:73`) with non-ASCII characters in
`05 - Combinatorics/02 - BurnsideLemma.cpp` (`ó` in "Pólya") and
`06 - Math/02 - Multiplicative Functions/02 - MobiusFunction.cpp` (`μ`). These render wrong or
vanish in the PDF. Replace with ASCII in source, or set `extendedchars=true` + `\usepackage[utf8]{inputenc}`
consistently.

### 1.6 Exporter depth cap
The script handles exactly two levels (`section/ → subsection/ → files`). A third level of
nesting is silently ignored. No such directory exists today, but the README's "Adding a New
Category" instructions don't mention the limit.

---

## Part 2 — Correctness bugs

Every file was read in full. The claims marked **[verified]** were additionally reproduced by
compiling the code against a brute-force oracle with randomized stress tests — see
[EDIT-PROPOSALS.md](EDIT-PROPOSALS.md) Part 0 for the list of structures that passed.

### P0 — will produce wrong answers

**`02 - DS/01 - Segment Tree/02 - LazySegmentTree.cpp` — build range ≠ query range.**
`build()` recurses on `[0, n-1]` but `update()` and `query()` recurse on `[0, sz-1]` where
`sz` is `n` rounded up to a power of two. When `n` is not a power of two the two recursions
produce *different tree shapes*, so built values land in nodes the queries never visit.
Fix: use `sz - 1` in `build()` too (and guard `lx >= v.size()` at leaves).

**`09 - Trees/01 - LCA/01 - LCA.cpp` — out-of-bounds read (UB), plus a C++20-only signature.**
`tour` has exactly `2n - 1` entries, but after `n *= 2` the init loop runs
`for (int i = 0; i < n; ++i) ... T[i][0] = tour[i];` — index `2n-1` is past the end.
*Verified by stress test:* the returned LCAs are **correct** (queries never read that cell), so this
is UB and an ASAN failure rather than a wrong-answer bug — downgraded from P0 to P1. Fix: loop to
`tour.size()`.
Separately, `LCA(int n, auto& adj)` is a C++20 abbreviated function template; **it does not compile
under C++17**, which some judges still default to. Make it `template<class G>`.

**`07 - Strings/02 - Hashing/01 - StringHashing.cpp` — will not compile as written.**
Uses `.st` / `.nd`, which are **not defined** in `01 - Template.cpp` (that file defines `fi` / `se`).
Separately: characters map to `1..26` (lower), `30..55` (upper), `60..69` (digits), but the first
base is `p.st = 67` — smaller than the largest symbol value (69). A base below the alphabet size
makes collisions *constructible* rather than merely improbable; the second base (97) is fine, so
the pair still holds up in practice, but the first component is weaker than it looks. Both bases
are also fixed, so this is anti-hashable on Codeforces. Fix: align the macros, and use random
bases in `[130, MOD)`.

**`03 - Graph Theory/04 - Satisfiability/01 - 2SAT.cpp` — does not compile against this repo's SCC.**
It calls `scc.init()`, `scc.addEdge()`, `scc.gen()`, `scc.compOf`. The repo's
`03 - Graph Connectivity/03 - SCC.cpp` exposes `add_edge()`, `solve()`, `comp`. Also uses `.st`/`.nd`.
The working 2-SAT is the extensionless `2_sat2` (self-contained Kosaraju). Either fix the API or
delete `01 - 2SAT.cpp` and promote `2_sat2`.

**`02 - DS/01 - Segment Tree/04 - SegmentTreeBeats.cpp` — `int sum` overflows.**
`Node::sum` is `int`. With `n = 2·10^5` and values up to `10^9` the root sum reaches `2·10^14`.
Change `sum` (and the `doPush*` arithmetic) to `long long`.

**`05 - Combinatorics/02 - BurnsideLemma.cpp` — no modular arithmetic.**
`fast_pow(k, gcd(i,n))` is plain `ll` exponentiation, so `k^n` overflows for any realistic `n`.
Usable only for toy inputs. Needs a `MOD` version, and the final `/ n` must become
multiplication by `inv(n)`.

**`01 - Miscellaneous/04 - random.cpp` — `random(int n)` returns all zeros.**
`vector<int> v(n);` is never filled before shuffling. Add `iota(v.begin(), v.end(), 0)` (or `1`).

### P1 — silent limitations that bite in contest

**`03 - Graph/03 - Graph Connectivity/01 - Bridges.cpp` and `02 - CutPoints.cpp` — no multi-edge support.**
Both skip the parent by *vertex* (`if (v == p) continue;`). With parallel edges, a doubled edge is
reported as a bridge. Fix: pass the incoming **edge id** and skip that instead.
`CutPoints` also uses `p != 0` as the "is root" test, so node `0` cannot exist (1-indexed only).

**`06 - Math/05 - Convolution/04 - GeneratingFunctions.cpp` — advertised O(N log N), actually O(N²).**
The private `multiply()` is a naive double loop, so `inv()` and `log()` are quadratic despite the
header comment. Wire it to a real NTT.

**`06 - Math/05 - Convolution/01 - FFT.cpp` — won't compile standalone.**
Uses `vi` (never typedef'd in `01 - Template.cpp`) and the `1i` imaginary literal (needs
`using namespace std::complex_literals`). Also only ships `convMod`; there is no plain
`conv(vector<double>)`.

**`06 - Math/05 - Convolution/02 - NTT.cpp` — transform only, no `multiply()` wrapper**, and it
redefines `MOD = 998244353` which collides with the global `MOD = 1e9+7` in the template.

**`06 - Math/01 - Modular Arithmetic/*` — `build()` is O(N log MOD).**
`invFact[i] = modDiv(invFact[i-1], i)` runs a full `modPow` per index. Use the standard
`invFact[N-1] = inv(fact[N-1])` then a downward loop → O(N). Same issue in `02 - ModularIntClass.cpp`.

**`04 - Number Theory/02 - .../04 - Sieve1e9.cpp` — 62 MB `bitset` as a struct member.**
`bitset<5e8>` inside the struct means a local instance blows the stack. Must be declared global/static.
Add that as a comment in the file.

**`04 - Number Theory/02 - .../05 - SegmentedSieve.cpp` — `ll limit = sqrt(R)` uses `double`.**
For `R` near `10^12` the rounding can under-shoot by 1 and leave a composite marked prime. Use
`sqrtl` then `while ((limit+1)*(limit+1) <= R) limit++;`. Also `L == 0` isn't handled (only `L == 1`).

**`06 - Math/04 - Linear Algebra/01 - LinearXorBasis.cpp` — member init order.**
Declared `vector<ll> basis; int log_bits, sz;` but initialized `: log_bits(...), sz(0), basis(...)`.
It happens to work (the ctor parameter shadows), but `-Wreorder` fires. Reorder the list.

**`02 - DS/04 - Trie/01 - StringTrie.cpp` — `alpha` is configurable but `c - 'a'` is hardcoded.**

**`03 - Graph/05 - Eulerian Path/01 - EulerianPath.cpp` — computes `in_deg`/`out_deg` but never
checks them.** No degree-parity or connectivity validation; it only verifies all edges were used
after the fact, and it can't tell you *which* start vertex is legal.

**`03 - Graph/06 - Flow and Min Cut/02 - MCMF.cpp` — `int` costs and caps.** Total cost overflows
`int` on large graphs; make cost/flow accumulators `long long`. Returns `{cost, flow}` (name order
is reversed vs. `minCostMaxFlow`).

**`04 - Number Theory/03 - Modular Arithmetic/01 - CRT.cpp` — `res += k * lcm` can overflow `ll`**
when the product of moduli approaches `10^18`. Needs `__int128` for the general case.

**`07 - Strings/01 - String Matching/02 - RabinKarp.cpp` — single fixed mod/base (31, 1e9+9).**
Trivially anti-hashed on Codeforces. Randomize.

**`07 - Strings/04 - Suffix Structures/01 - SuffixArray.cpp` — `queryLcp(i, j)` takes rank
positions, not string positions.** The conversion line is commented out. Easy to misuse under time
pressure; add a second wrapper that takes string indices.

**`09 - Trees/03 - HLD.cpp` — `path()` returns unordered ranges.** Fine for commutative merges,
silently wrong for non-commutative ones (e.g. matrix products along a path). No edge-weight mode.
`dfs1`/`dfs2` are recursive → stack risk at `n = 2·10^5` on a path graph (true of
`SCC`, `Bridges`, `CentroidDecomposition`, `DSUOnTree` too).

### P2 — notes, not bugs

- `01 - Template.cpp` reads `tc` unconditionally; remember to delete `cin >> tc` for single-test problems.
- `02 - fastIO.cpp`: `write_int` always appends `'\n'` (no space-separated output); `read_int` has no
  EOF handling. `Input` and `Output` are both `inline namespace`s that each declare `buf`/`pos`, so
  those names are ambiguous at global scope.
- `08 - DP/.../02 - CHT.cpp` is the classic `multiset` + `succ()` hull. Known to misbehave with
  duplicate slopes; prefer Li Chao when in doubt.
- `08 - DP/.../04 - LiChaoTree.cpp` uses `double` in `inter()` → precision loss at large coordinates;
  `start_x`/`end_x` are `int` while queries take `ll`.
- `11 - Geometry/01 - Geometry 2D/11 - HalfPlaneIntersection.cpp` hardcodes `Point(inf, inf)` instead
  of the template parameter `T`; relies on CTAD.
- Geometry is written double-first. There is no integer-exact `sgn`/orientation path, which is what
  you actually want for hull/2-SAT-style integer problems.
- `PST` (`03 - PersistentSegmentTree.cpp`) mutates old versions via `addChild()` during traversal.
  Values stay correct, but it allocates into "immutable" history.

---

## Part 3 — What is genuinely strong here

Worth saying, because the gap list is long: the segment-tree family (9 variants incl. beats,
persistent, dynamic, 2D, iterative), the Mo's family (array / subtree / path), the treap pair
(key-based + implicit with O(n) build), the DP-optimization set (D&C, Knuth, Li Chao + persistent,
rollback CHT, Alien's), and the palindromic tree with series links + rollback are all above the
level of a typical team notebook. The half-plane intersection and MEC are solid. The gaps are
concentrated in **combinatorics, number theory primitives, linear algebra, game theory, and
geometry predicates** — see MISSING.md.
