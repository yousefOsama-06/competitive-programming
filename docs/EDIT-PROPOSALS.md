# Edit proposals — **APPLIED** (2026-08-12)

> Everything in Parts 1–5 below has been applied, plus ~20 new templates. All new and rewritten
> code was stress-tested against brute force / reference implementations — 3 suites, 0 failures.
> The one thing NOT done: `notebook.pdf` could not be regenerated (no `pdflatex` on this machine).
> Run `node export_notebook.js` where LaTeX is installed. `notebook.tex` and `toc.yaml` ARE current.


Every change I want to make to existing files, why, and what it costs or saves in **lines you have
to hand-type**. Approve individually or in blocks.

Current notebook: **5,429 lines** across 106 `.cpp` (+ 12 extension-less files, ~1,330 more lines).
If everything below is approved: **≈ 4,350 lines**, with more coverage and the bugs gone.

---

## Part 0 — What I verified as CORRECT (do not touch)

I compiled each of these against a brute-force oracle with randomized stress tests
(`g++ -std=c++20`, thousands of cases, small n so every branch is hit):

| File | Test | Result |
|---|---|---|
| `01 - SegmentTree.cpp` | point update + range sum vs brute, n = 1…9 | **ok** |
| `01 - FenwickTree.cpp` | range sum **and** `lower_bound` vs brute | **ok** |
| `01 - LCA.cpp` | LCA vs naive climbing, n = 1…60 | **ok** (but see E-2) |
| `01 - LinearXorBasis.cpp` | `getMax` vs brute over all 2ⁿ subsets | **ok** |
| `01 - Manacher.cpp` | `is_palindrome(l,r)` for every (l,r) | **ok** |
| `08 - ConvexHull.cpp` + `09 - RotatingCalipers.cpp` | hull CCW + `hullDiameter` vs O(n²) | **ok** |
| `02 - FloorSum.cpp` | vs brute, 20 000 random (n,m,a,b) | **ok** |
| `01 - CRT.cpp` | vs exhaustive search, 20 000 cases | **ok** |
| `02 - TwoStackQueue.cpp` | sliding min/max vs deque, 20 000 ops | **ok** |
| `02 - ImplicitTreap.cpp` | add/reverse/erase/query vs vector, n ≤ 40 | **ok** |
| `08 - IterativeSegTree.cpp` | vs brute | **ok** |
| `01 - Dinic.cpp` | max flow vs Ford–Fulkerson, 3 000 random graphs | **ok** |

That is the good news: the core of this notebook is sound.

---

## Part 1 — Bug fixes (please approve all of these)

| # | File | Change | Lines |
|---|---|---|---|
| **B-1** | `02 - LazySegmentTree.cpp` | **Confirmed wrong by test**: `build` recurses on `[0,n-1]`, `update`/`query` on `[0,sz-1]`. Reproduced: n=3, query `[2,2]` returns −4, correct is 0. Rewrite compact with a proper `apply`/`push` pair. | 92 → **55** |
| **B-2** | `01 - Bridges.cpp` | **Confirmed wrong by test**: on a graph with a doubled edge it reports 1 bridge; correct is 0. Skip by incoming **edge id**, not parent vertex. Merge with `02 - CutPoints.cpp` (same DFS) into one `LowLink` giving bridges + cut points + component ids. | 36+39 → **48** |
| **B-3** | `05 - SegmentTreeBeats.cpp` | `Node::sum` is `int`; overflows at n=2·10⁵ with values 10⁹. → `long long`. | +0 |
| **B-4** | `02 - BurnsideLemma.cpp` | `fast_pow` has no modulus → overflows on any real input; and it divides by `n` instead of multiplying by `inv(n)`. Rewrite modular. | 40 → **25** |
| **B-5** | `04 - random.cpp` | `random(int n)` shuffles an all-zero vector — add `iota`. | +1 |
| **B-6** | `01 - StringHashing.cpp` | Uses `.st`/`.nd`, which the template never defines → **does not compile**. Fixed bases (anti-hackable), first base below the alphabet size. Full rewrite: random double-mod bases, no modular inverses (compare `h[r]·p^{n−l}`). | 83 → **30** |
| **B-7** | `01 - 2SAT.cpp` | Calls `scc.init/addEdge/gen/compOf`; this repo's SCC exposes `add_edge/solve/comp` → **does not compile**. See D-2 for the replacement. | −79 |
| **B-8** | `05 - SegmentedSieve.cpp` | `ll limit = sqrt(R)` in `double` can undershoot at R≈10¹²; use `sqrtl` + correction loop. | +2 |
| **B-9** | `01 - CRT.cpp` | `res += k * lcm` overflows when moduli approach 10¹⁸ → `__int128` accumulator. | +2 |
| **B-10** | `04 - Sieve1e9.cpp` | 62 MB `bitset` as a struct member blows the stack if instantiated locally. Add a one-line comment: must be global/static. | +1 |

---

## Part 2 — Portability / usability (cheap, high value)

| # | File | Change | Lines |
|---|---|---|---|
| **E-1** | `01 - Template.cpp` | Add `typedef vector<int> vi;` (the FFT file needs it and it isn't there), a `dbg(...)` macro under `#ifdef LOCAL`, and a commented single-test path. | 58 → **72** |
| **E-2** | `01 - LCA.cpp` | Two issues. (a) the init loop runs to `2n` while `tour` has `2n−1` entries — an out-of-bounds **read**. It does not change any answer (queries never reach that cell) but it is UB and ASAN flags it; one-character fix. (b) `LCA(int n, auto& adj)` is a C++20 abbreviated template — **it does not compile under C++17**, which several judges still default to. Make it a real `template<class G>`. | 34 → **36** |
| **E-3** | `02 - fastIO.cpp` | `write_int` unconditionally appends `\n`, so you cannot print space-separated output. Add a separator argument. Also trim to reader-only — with `sync_with_stdio(false)` the writer rarely earns its 30 lines. | 57 → **28** |
| **E-4** | `dynamic_median_finding` (extension-less) | Uses `sz(...)`, which the template does not define → does not compile as shipped. Add the macro to the template or spell it `(int)L.size()`. | +0 |
| **E-5** | All extension-less files | Rename to `.cpp` so they appear in the PDF (12 files, ~1,330 lines currently invisible). | +0 |

---

## Part 3 — Deduplication (pure typing savings, no behaviour change)

These are the ones that matter most for a handwritten contest notebook.

| # | Merge | Why | Lines |
|---|---|---|---|
| **D-1** | `01 - ModularArithmetic.cpp` + `02 - ModularIntClass.cpp` | Same functionality twice (free functions vs a class). Keep only `Mint`. Also fixes the O(N log MOD) `build()` → O(N) with the descending-inverse trick. | 151 → **45** (−106) |
| **D-2** | `01 - 2SAT.cpp` + `2_sat2` | Three 2-SAT structs across two files, one of which doesn't compile. Keep one Kosaraju `TwoSat` (~55 lines) plus a 12-line note describing the prefix-`at_most_one` and segment-tree-range tricks — the tricks are *ideas*, not code you retype. | 366 → **67** (−299) |
| **D-3** | `03 - PersistentSegmentTree.cpp` + `04 - PersistentSegTreeVector.cpp` | Byte-for-byte the same except `ll val` vs `int val` and `update` (adds) vs `set` (assigns). One file with both methods. | 123 → **55** (−68) |
| **D-4** | `01 - MatrixExponentiation.cpp` + `02 - FastMatrixPower.cpp` | Same algorithm, `vector<vector<int>>` vs `array<array<int,SZ>,SZ>`. One templated version; note that fixing SZ at compile time is the speed trick. | 104 → **32** (−72) |
| **D-5** | Drop `03 - SquareSparseTable.cpp` | Strictly weaker than `02 - SparseTable2D.cpp` (square queries only). | −44 |
| **D-6** | `05 - SegmentTreeBeats.cpp` | 281 lines is not a hand-typeable template. Trim to the standard chmin + range-add + sum beats and drop the chmax/assign extension (a note explains how to add it back). | 281 → **110** (−171) |
| **D-7** | Delete `04 - GeneratingFunctions.cpp` | Advertises O(n log n) but its `multiply` is a naive O(n²) double loop, so `inv`/`log` are quadratic — it is slower than doing it by hand and gives false confidence. Re-add once NTT actually works (see N-2). | −73 |
| **D-8** *(optional)* | The three Mo files | 327 lines with a duplicated `Query` struct and block-sort in each. One core + three 20-line adapters. Riskier to retype under pressure, so this one is your call. | 327 → **185** (−142) |

**Dedup subtotal: −975 lines.**

---

## Part 4 — Files that must GROW (they're incomplete for what ECPC actually asks)

| # | File | Change | Lines |
|---|---|---|---|
| **N-1** | `01 - LinearXorBasis.cpp` | Add `merge(a,b)`, reduced row echelon form, `kth` smallest representable, `count` of distinct values. Without `merge` you cannot put a basis in a segment tree node — which is exactly ECPC 2024 H. | 30 → **48** |
| **N-2** | `02 - BinaryTrie.cpp` | Add `erase`, `min_xor`, `count(x ^ y < k)`. ECPC 2024 O needs insert **and** erase so the trie tracks the current DFS path. | 45 → **62** |
| **N-3** | `01 - FFT.cpp` | Fix `vi` / `1i` so it compiles standalone; add a plain `conv()` for doubles. | 46 → **44** |
| **N-4** | `02 - NTT.cpp` | Add a `multiply()` wrapper; rename its `MOD` so it stops colliding with the template's `1e9+7`. | 44 → **48** |
| **N-5** | `01 - FenwickTree.cpp` | Add the two-BIT range-update range-query variant as a second struct in the same file. | 40 → **62** |

**Growth subtotal: +59 lines.**

---

## Part 5 — Geometry rewrite

Detailed in [GEOMETRY.md](GEOMETRY.md); draft code in [`docs/proposed/geometry/`](proposed/geometry/).
Every routine there has been stress-tested against an independent brute force.

| Current | Proposed | Note |
|---|---|---|
| `01 - Point2D.cpp` (85) + `03 - AngleOperations.cpp` (16) | `01 - Core.cpp` (**49**) | adds exact polar-sort comparator and signed `angleTo`; drops unused members and the duplicated `dcmp` |
| `02 - Line2D.cpp` (116) | `02 - Lines.cpp` (**73**) | the four near-identical `line/seg/ray` × `Inter/Dist` families collapse into one set; adds perpendicular bisector, angle-bisector direction, `Ax+By=C` conversion |
| `04 - PolygonArea.cpp` (10) + `08 - ConvexHull.cpp` (19) + `09 - RotatingCalipers.cpp` (26) | `03 - Polygon.cpp` (**168**) | **adds point-in-polygon, point-in-convex O(log n), extreme-vertex O(log n), polygon cut, Minkowski sum, min enclosing rectangle, Pick's theorem** |
| `05 - Circle3Points.cpp` (14) + `06 - CircleLine.cpp` (22) + `07 - CircleCircle.cpp` (20) + `10 - MEC.cpp` (31) | `04 - Circles.cpp` (**115**) | **adds all four common tangents, tangents from a point, incenter/orthocenter, circle∩polygon area, power of a point, radical axis** |
| — | `05 - Sweep.cpp` (**93**) | **new**: closest pair, rectangle-union area, max points on a line, convex layers |
| `02 - Geometry 3D/01 - Point3D.cpp` (70) | `06 - Geometry3D.cpp` (**74**) | **adds planes, line∩plane, plane∩plane, exact tetrahedron orientation, sphere∩line, great-circle** |
| `11 - HalfPlaneIntersection.cpp` (103) | unchanged | works; note added that repeated `cut()` is shorter to retype when there are few half-planes |

**Geometry: 532 → 675 raw lines (403 → 539 code lines, +34%) for roughly 3× the coverage.**
This is the one section that gets *bigger*, deliberately — see GEOMETRY.md §11 for the
drop-in-this-order list if page budget runs out.

---

## Bottom line

| | lines |
|---|---:|
| now | 5,429 |
| bug fixes (Part 1) | −40 |
| portability (Part 2) | −41 |
| dedup (Part 3) | −975 |
| growth (Part 4) | +59 |
| geometry (Part 5) | +143 |
| **after** | **≈ 4,575** |

Plus the 12 extension-less files become visible, and the ~1,000 lines you no longer have to type
are freed for the missing T0 templates from [PLAYBOOK.md](PLAYBOOK.md).

---

## What I need from you

1. Approve Part 1 (bugs) — I'd do these regardless.
2. Say yes/no on each dedup in Part 3, especially **D-2** (2-SAT, −299) and **D-6** (beats, −171),
   since those delete working code.
3. Confirm the geometry rewrite direction before I move it into `notebook/`.
4. Tell me whether **D-8** (Mo's) is worth the risk to you.
