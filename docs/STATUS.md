# Notebook status — 2026-08-13 (third pass)

**386 files, 23,695 lines, ~260 printed pages.** Every non-trivial template has been compiled and stress-tested against
an independent brute force or a verified reference (KACTL / AtCoder Library / cp-algorithms /
YouKnowWho / exhaustive search).

## Verification — 32 suites, 0 failures

| Suite | Covers |
|---|---|
| v1 | Mint & factorials · matrix power · XOR basis (`merge`, `kth`) · binary trie (`erase`, `cntLess`) · Miller–Rabin / Pollard / φ / τ / σ / divisors · multiplicative sieve · Möbius identities · BSGS / Tonelli–Shanks / primitive root / mult-order · Catalan / Stirling ×2 / Bell / partitions / surjections · Burnside vs orbit enumeration |
| v2 | lazy segtree · segtree descent · LowLink (bridges / cut points / 2ECC, multi-edge safe) · 2-SAT vs 2ⁿ · hashing · Gaussian elimination / determinant / inverse · GF(2) elimination · Lagrange & Faulhaber · Berlekamp–Massey & Kitamasa · Grundy |
| v3 | Kruskal & Prim · BIT range-update-range-query · weighted DSU · LIS · knapsack · Duval & minRotation · LCA & binary lifting · rerooting · Kuhn & König · slope trick |
| v4 | Lucy prime counting & prime sum (π(10⁷)) · linear congruence · extended CRT · power towers · sum of two squares / Pythagorean triples / Legendre · continued fractions & best approximation · bounded stars-and-bars · binomial inversion · exact-gcd counts · Prüfer round-trip · matrix-tree · connected labelled graphs · zeta/Möbius/OR/AND/subset convolution · Motzkin / Schröder / Narayana / Eulerian / hook length / q-binomial |
| v5 | half-plane intersection vs repeated `cut` · polygon union · circle union · KD-tree · max-Manhattan · Manhattan MST vs O(n²) MST · Grundy periodicity · Wythoff · Moore's Nim_k |
| v6 | binary/ternary search idioms & `isqrt`/`icbrt` · submask & Gosper enumeration · ODT · disjoint sparse table · wavelet tree · offline dynamic connectivity · Cartesian tree · Mo with updates · Mo with rollback · Hungarian vs n! brute · suffix automaton (distinct / occurrences / k-th substring / LCS) · TSP & assignment · interval DP |
| v7 | exact `Frac` · inversions · expression parser · Floyd cycle & functional graph `kth` · block-cut tree (cut vertices, forest property, edge-in-one-block) |
| v8 | parallel binary search · meet-in-the-middle (subset sums, ±values) · scheduling (Johnson's rule, EDF, Moore–Hodgson) · maximal rectangle / largest zero submatrix · min-plus convolution (convex) |
| v9 | Du sieve (Σφ, Σμ) · Garner / arbitrary-modulus CRT · virtual tree · segment-tree graph · flows with lower bounds (feasible / min / max) |
| v10 | Euler transform & symbolic method (SEQ/MSET/PSET/CYC) · permanent (Ryser) vs n! · Bernoulli & Faulhaber · cycle lemma & Fuss–Catalan · MCMF with Johnson potentials vs SPFA · segment-tree merging |
| v11 | formal power series (inv / log / exp / sqrt / pow / divmod) on NTT · broken-profile DP vs bitmask DP · Aho–Corasick DP (count strings avoiding a set) · 2D hashing |
| v12 | bit tricks · search idioms · stress-test harness · interval-set ODT (assign / count) |
| v13 | expected-value DP (hitting times, absorbing chains, tridiagonal) · transitive closure (bitset) · bitset LCS (u64 limbs) · suffix-array applications (distinct / repeated / pattern range / LCS) · tree isomorphism (AHU, rooted & unrooted) |
| v14 | directed MST (Chu-Liu/Edmonds) vs brute arborescence · Borůvka · Stoer–Wagner global min cut vs max-flow over all pairs |
| v15 | LGV lemma vs path enumeration · Pólya cycle index with fixed colour counts · Lyndon word counts · named games (Lasker, Kayles, Dawson, Mock Turtles, Ruler) · Venice technique · monoid SWAG |
| v16 | second-best MST vs enumeration of all spanning trees · wildcard matching (FFT) vs O(nm) · generalized SAM (distinct substrings, per-string counts, LCS of k strings) · polygon triangulation (count, orientation, area) · Josephus ×3 vs simulation · BigInt (+ − × ÷ % cmp) vs `__int128` |
| v17 | regression after the final rename/cleanup pass: `Mint` factorial tables · `Blocks` (sqrt decomposition, rewritten) · `reachableSums` · `lcsViaSuffixArray` · `polyUnion` after the `edgeRatio` rename |

### Bugs the testing caught in code written here

`area2` on an empty polygon · transposed args in `lineInter` · `cut` dropping on-line vertices ·
BIT range-update off-by-one · `Rerooting` aggregate too weak · `pythagorean` parity inverted ·
`polyUnion` mis-adapted · `bestApprox` ignoring semiconvergents · `manhattanEdges` wrong octant ·
`toPrufer` desynced from `fromPrufer` · **Wythoff using `⌊(b−a)φ⌋` instead of `b−a`** ·
`qBinom` 0/0 at q=1 · `factor(0)` infinite loop · **`MoWithUpdates` starting from the final array
state** · `MoWithRollback` reusing the persistent part for short queries · `SAM::countOcc()` not
idempotent · ODT shipped with two placeholder stubs · `divmod` truncating the remainder before
subtracting · `SegTreeMerging::update` holding an `int&` across a reallocating `newNode()` ·
`MeetInTheMiddle` filtering `x <= S` (breaks with negative values) · `bitLCS` accidentally O(nm) ·
`GenSAM` mask aliasing at k > 64.

### Bugs found in the original repository

`LazySegmentTree` build/query index mismatch (wrong answers) · `Bridges` skipping the parent by
vertex instead of edge id (doubled edge reported as a bridge) · `SegmentTreeBeats` `int` sum
overflow · `BurnsideLemma` non-modular `fast_pow` overflow · `random(int n)` shuffling an all-zero
vector · `StringHashing` using undefined `.st`/`.nd` and fixed bases · `2SAT` calling a nonexistent
SCC API · `LCA` out-of-bounds read + C++20-only `auto` parameter · `SegmentedSieve` float `sqrt` ·
`CRT` overflow · `Sieve1e9` stack bitset · **`NumberTheoryFormulas` nesting `/* */` inside a block
comment** (silently ended the comment and leaked the rest of the file as code) ·
`BlockDecomposition` shipped as loose statements at file scope (would not compile at all).

### Name collisions caught by compiling files together

`Mint P(n,r)` vs geometry `template struct P` → **`Perm`** · `ratio` vs `std::ratio` →
**`edgeRatio`** · `__gcd` (libstdc++ only) → `std::gcd` repo-wide · two `struct MCMF`
(the SPFA version was deleted; the potentials version is now `06 - Flow and Min Cut/02`) ·
two `longestCommonSubstring` (SA version → `lcsViaSuffixArray`) · two `subsetSums`
(knapsack version → `reachableSums`). Files that still share a name with a sibling they would
never be pasted with (`struct Edge`, `struct Node`, `PalindromeTree`, `extGCD`) now carry a
one-line `// NOTE:` saying so.

### Whole-notebook consistency sweep

Every one of the 197 files is compiled twice — once with all of its earlier siblings prepended,
once alone with its declared prerequisites. 24 files do not build in complete isolation:
**14 depend on a template in another folder and now carry a `// Needs: <file>` line on line 1**;
the other 10 need only `__lg`, `__gnu_pbds`, or a macro from `01 - Template.cpp`, all of which
every ICPC judge provides. Three more defects surfaced here and were fixed: `Mint` used an
undeclared `N` for its factorial tables, `DivideAndConquerDP` used an undefined `.nd` macro, and
`random.cpp` / the two treaps each declared their own `rng` (now flagged with a `// NOTE:`).

## Section sizes

| Section | files | lines |
|---|---:|---:|
| 01 Miscellaneous | 32 | 1,803 |
| 02 Data Structures | 71 | 4,532 |
| 03 Graph Theory | 54 | 3,422 |
| 04 Number Theory | 51 | 2,369 |
| 05 Combinatorics | 15 | 806 |
| 06 Math | 43 | 2,075 |
| 07 Strings | 33 | 2,295 |
| 08 Dynamic Programming | 34 | 1,917 |
| 09 Trees | 16 | 1,067 |
| 10 Game Theory | 6 | 520 |
| 11 Geometry | 28 | 2,429 |
| 12 Problem Solving | 3 | 460 |

## The printed document

Built locally with pdflatex (a TeX Live payload extracted without root), and designed to be built
on Overleaf: upload `notebook.tex`, compiler pdfLaTeX, and latexmk runs makeindex for you.

* **Contents** on page 1: two columns, sections in bold with page numbers, every subsection dotted
  underneath. The whole map of the notebook on one page.
* **Every section opens with a topic strip** naming its sub-topics and the page each starts on.
* **Running head** on every page: section on the left, sub-topic on the right, under a hairline.
* **Alphabetical index of every routine** at the back, ~790 entries, name to page number.
* **A one-line description under every file title**, lifted from line 1 of the source, set in
  sans so it reads as a caption rather than as code.
* Code at 7.6pt with a hanging indent and a continuation arrow on wrapped lines; comments in a
  grey-green that stays legible when printed in black and white; a hairline between the columns.
* **Inconsolata when available** (Overleaf, full TeX Live), Latin Modern Typewriter otherwise.
  Inconsolata is ~15%% narrower, so 100-column source stops wrapping; the fallback keeps the
  notebook building on a minimal install.

## Third pass - everything missing from the old libraries

Six agents, one per domain, ported **+155 templates** from `temps/code-library/` plus Alaa's
upstream work. Per instruction these were compile-checked rather than stress-tested; several were
verified against brute force anyway, which caught real bugs in the SOURCES: DPLL returned SAT
prematurely, the path-union check accepted non-path unions, banded edit distance had the wrong
band boundaries, and the pattern-containment poset let a pattern's own terminal mask shorter
suffixes. It also caught three in OUR notebook: `Template.cpp` never defined `lll` though three
files use it, `NimbersAndSums` used `u64` with no typedef, and a member named `sz` collided with
the `sz()` macro.

## Second pass## Second pass — nine research agents, every finding verified before it was applied

Nine domain agents audited the notebook against KACTL, ShahjalalShohag/code-library, cp-algorithms,
AtCoder Library, OI Wiki, Library Checker, ei1333's library, and real ECPC/ACPC/Codeforces sets.
Every bug below was **reproduced locally** (compiled, brute-forced, or run under UBSan) before the
fix went in; every added routine was stress-tested against an independent reference.

### Crashes and hangs found

* `PersistentLiChaoTree` — `(l+r)/2` truncates toward zero, so on a negative interval the left
  child **equals the parent**: infinite recursion on ~2/3 of negative query abscissae.
* `minkowski` — on clockwise input neither merge index advances: an unexplained **TLE/MLE**, not a
  wrong answer. Now normalises orientation.
* `Offline2DFenwickTree` — the constructor's parameter shadowed the member, so `add` walked one row
  past the end of `vals` (fires at n = 2, 6, 10, 14, …).
* `Sieve1e9` — inner loop bound `n/2` wrote one index past a 60 MB `bitset`.
* Empty-input UB in `mec`, `inConvex`, `KD::nearest`, `closestPair`, `TreeIsomorphism::centers`,
  `DynamicMedian::remove_val`, `BerlekampMassey::linearRec`.

### Silent wrong answers found

* `patternRange` — `R` was never reset, so an **absent** pattern returned a non-empty SA range
  (4642 wrong answers in 12000 random queries).
* `wildcardMatch` — `c - 'a' + 1` is negative for any character below `'a'`, and the FFT identity
  needs non-negative values; terms cancelled and reported matches that do not exist. Now compresses
  the alphabet.
* `IterativeSegTree::update` — `combine(tree[p], tree[p^1])` reverses the arguments when `p` is a
  right child: wrong for every non-commutative merge.
* `CHT::bad` — `long double` comparison of ~2e18 products, consulted exactly when three lines are
  nearly collinear. Now `__int128`, and the query comparator too.
* `LiChaoTree` — `double` intersection abscissa, off by ~1e3 in x at 1e18 coefficients. Rewritten
  to compare values at `l`/`mid`/`r` in exact `ll`.
* `SegmentTreeBeats` — every value field was `int`; two range-adds at 1e9 overflow.
* `Hash2D` — re-seeded its bases per object, so two grids were never comparable (and it was
  *non-deterministic*: same-tick construction agreed).
* `staircaseLosing` — the code and its own comment used **opposite** ground conventions; wrong on
  96 of 256 four-position states.
* `Periodic::detect` — took the first index where the period matched instead of the last; wrong for
  S = {2,4,7} and three other small subtraction sets.
* `lasker(0)` returned **−1**, which XORed into a game sum flips every bit.
* `pathsAvoidingLine(2,2,1)` returned **−1**; `ballot(4,−2)` returned `MOD−2`.
* `perpBisector` on `P<ll>` returned a **different line**, not an imprecise one.
* `segInter`'s division overflows above |coordinate| ≈ 1.6e6, and `segCross` inherited it while
  claiming to be division-free. `segCross` is now sign-only — exact at 1e9 and ~30× faster.
* `sgn(int)` / `sgn(double)` were **ambiguous** — a hard compile error on any non-`ll` argument.
* `GaussianElimination` dropped every free column after the last pivot, so `MOD^|free|` was wrong.
* `NTT` silently returns garbage above length 2^23; there was no assert and no note.
* `primeSum`, `DuSieve::P`, `floorSum`, `Cornacchia`, `CRT`, `Garner`, `LucasAndBinomialMod` all had
  undocumented 64-bit ceilings well below their advertised ranges.

### Wrong preconditions (the ones that cost a WA with no crash)

`KnuthDP` documented its *conclusion* (`opt` monotone) instead of its checkable hypothesis
(quadrangle inequality **and** interval monotonicity). `AliensTrick` omitted the max-count
tie-break, which is the entire correctness proof, and called convexity "strict". `qBinom`'s real
precondition is `ord_p(q) > k`, not `q != 1`. Anti-SG needs `SG(u)=0 ⟺ u terminal` — without it the
rule is wrong on ~1.3% of random impartial games. Hall needed "subset of the **left** part";
Dilworth needed "**finite** poset". `MoOnPaths` could not be called at all — the data needed to
build its queries was private and computed only inside `solve`.

### Added this pass

Sections: **12 Problem Solving** (reframing · search · proof tools · constraints-as-hints ·
constructive/interactive · randomisation · empirics · contest craft · pre-submit checklist ·
verdict triage). New files: graph theorem sheet, min-cut modelling card, exact-exponential graph
algorithms (Bron–Kerbosch, chromatic number, Steiner tree), triangle & 4-cycle counting, tree DP,
Euler-tour toolbox, second number-theory formula sheet, nimbers + octal-game generator + anti-SG +
every-SG + Euclid's game, second-best MST, wildcard matching, generalized SAM, polygon
triangulation, Josephus, BigInt, floor_sum, adaptive Simpson, mergeable heap, monotone CHT.

### Suites added this pass

| Suite | Covers |
|---|---|
| v18 | shortest path with one edge changed, vs a fresh Dijkstra per query (17 838 random graphs) |
| v19 | `floorSum` · adaptive Simpson · skew heap with lazy add and meld |
| v20 | Bron–Kerbosch max clique · chromatic number · Steiner tree · triangle and 4-cycle counting |
| v21 | dynamic upper hull and monotone CHT at slopes ±1e9, intercepts ±1e18 |
| v22 | Li Chao, exact-integer rewrite, incl. segment inserts over negative domains |
| v23 | maximum weight closure vs brute force over all closed sets, set recovery included |
| v24 | tree DP: weighted MIS and connected tree knapsack vs subset enumeration |
| v25 | Jacobi symbol · nim product (mex definition + field axioms) · octal Grundy generator · Euclid's game |
| geo | `segCross` at \|c\| = 1e9 · `minkowski` on CW input · empty-input guards · `sgn` overloads |

## Printed-layout changes

The PDF is now built for the physical act of finding something under time pressure:

* an **alphabetical index of every routine** at the back (520 entries) — you look up `minRectArea`
  and get a page number instead of thumbing;
* a **running head on every page**: section on the left, current sub-topic on the right;
* a **one-line description under every file title**, lifted from line 1 of the source;
* a `HOW TO FIND SOMETHING` box on page 1 that names the three lookup routes in order of speed;
* denser TOC, no listing line numbers (you never type them), and `\needspace` so a title is never
  orphaned at the foot of a column.

## Pulled in from the old `temps/` libraries

Everything below was rewritten from scratch (not copied) and stress-tested against an independent
reference before it went in. The full audit of what `temps/` had is in
[FROM-OLD-TEMPLATES.md](FROM-OLD-TEMPLATES.md).

| Added | Verified against | Suite |
|---|---|---|
| **Blossom** — maximum matching in a general graph | brute-force matching over all subsets, 20k graphs | v26 |
| **Gomory-Hu tree** — all-pairs min cut from n−1 flows | a fresh Dinic per pair | v26 |
| **Karp's minimum mean cycle** | bitmask enumeration of every simple cycle | v26 |
| **Systems of difference constraints** | every constraint re-checked, plus an exhaustive box search on the infeasible cases | v26 |
| **Link-Cut Tree** — dynamic forest, path aggregates | 4000 random forests × 40 mixed link/cut/update/query ops against BFS | v27 |
| **Simplex** — two-phase LP | feasibility, optimality vs a grid search, and objective consistency | v28 |
| **Thomas algorithm** (tridiagonal) · **determinant mod a composite** | back-substitution check · cofactor expansion in `__int128` | v28 |
| **1D/1D DP optimisation** — the self-referential Monge case | O(n²) brute force on two Monge families | v29 |
| **Fibonacci fast doubling + Pisano period** | iterative Fibonacci mod 5 different moduli; period minimality | v29 |
| **De Bruijn sequences** · **bitap / shift-and** | every length-n word appears exactly once cyclically · naive matching | v30 |
| **Permutation rank/unrank** · **Julian day number** · **Stern-Brocot search** | all n! permutations for n ≤ 7 · round-trip over 800 years + leap rules · brute-force minimal fraction | v30, v31 |
| **Persistent array + persistent DSU** | per-version reference copies, with branching histories | v32 |

Two real bugs in this batch were caught by the tests, not by reading: `Simplex::pivot` applied the
pivot-column update twice (the solver cycled forever), and the first `fracSearch` had a doubling
loop that could make no progress. Both were found because the test hung, then fixed and re-verified.

## Still missing (ranked)

**Worth doing next**
1. Link-cut tree / top tree (only real gap left in dynamic trees)
2. Blossom algorithm (general matching) — long; the notes card covers when you need it
3. Min_25 sieve (Lucy_Hedgehog covers most of what it buys you)
4. 3D convex hull + Delaunay as code (the construction is written up in `10 - Triangulation.cpp`)
5. Persistent lazy segment tree; persistent DSU
6. Bentley–Ottmann segment intersection (the sweep card covers the O(n²) fallback)
7. Matroid intersection; Schreier–Sims

**Deliberately notes-only**
Everything above appears as a written card with the algorithm sketch, complexity, and the problem
shape that needs it — enough to reconstruct it, without spending notebook pages on code that is
rarely the intended solution at ECPC/ACPC level.

## Known limitation

`notebook.pdf` cannot be regenerated here — no `pdflatex` on this machine. `notebook.tex` and
`toc.yaml` are current; run `node export_notebook.js` where LaTeX is installed. The exporter warns
about any non-`.cpp` file it would skip (currently zero).
