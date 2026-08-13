# Baskotian Template

*Husam Zaid · Ahmed Alaa · Yousef Osama* — ICPC team reference.

Team reference for ICPC-style contests (ECPC / ACPC). Every template is written to be **short
enough to retype from paper**, and the non-trivial ones have been **stress-tested against a
brute-force or reference implementation** — see [docs/](docs/).

## Docs

| File | What it is |
|---|---|
| [docs/PLAYBOOK.md](docs/PLAYBOOK.md) | every technique, what it solves, what else it's good for, priority |
| [docs/CONTEST-INTEL.md](docs/CONTEST-INTEL.md) | measured Codeforces tag trends + a problem-by-problem breakdown of ECPC 2024 |
| [docs/GEOMETRY.md](docs/GEOMETRY.md) | full geometry reference: routines, theorems, techniques, overflow rules |
| [docs/AUDIT.md](docs/AUDIT.md) | bugs found in the notebook and how they were verified |
| [docs/EDIT-PROPOSALS.md](docs/EDIT-PROPOSALS.md) | the refactor plan and its line-count accounting |
| [docs/STATUS.md](docs/STATUS.md) | what is verified, by which suite, and what is still missing |
| [docs/MISSING.md](docs/MISSING.md) | flat gap checklist with YouKnowWho cross-references |
| [docs/FROM-OLD-TEMPLATES.md](docs/FROM-OLD-TEMPLATES.md) | audit of the old `temps/` libraries: what they still have that this notebook does not |

## Directory structure

```
notebook/
├── 01 - Miscellaneous            # template (+ dbg), fast IO, grid, random, hashing guard, int128,
│                                 #   ternary/binary search idioms, bit tricks, stress harness,
│                                 #   Frac, inversions, expression parser, cycle finding, parallel
│                                 #   binary search, meet-in-the-middle, scheduling, Josephus, BigInt
├── 02 - Data Structures (DS)     # segment trees (lazy, beats, dynamic, persistent, 2D, iterative,
│                                 #   descent, merging), BIT (+ range-update), sparse & disjoint
│                                 #   sparse table, tries, DSU (+ weighted, rollback), Mo's (plain,
│                                 #   subtree, path, updates, rollback), block decomposition, treaps,
│                                 #   PBDS, monotonic structures, ODT, wavelet tree, offline dynamic
│                                 #   connectivity, Cartesian tree, bitset tricks, Venice + SWAG
├── 03 - Graph Theory             # traversal & 0-1 BFS, shortest paths (+ dominator tree, edge
│                                 #   modify), LowLink, SCC, 2-SAT, Euler path, Dinic, MCMF with
│                                 #   potentials, lower bounds, Stoer-Wagner, matching (+ Konig,
│                                 #   Hungarian), MST & KRT, directed MST, second-best MST,
│                                 #   Boruvka, block-cut tree, segment-tree graph
├── 04 - Number Theory            # Euclid, sieves (+ multiplicative, segmented, 1e9), Miller-Rabin
│                                 #   & Pollard rho, CRT & Garner, discrete log / sqrt / roots,
│                                 #   Lucas & binomial mod anything, power towers, formula sheet,
│                                 #   Mobius inversion, Du sieve, Lucy_Hedgehog, representations
├── 05 - Combinatorics            # formula sheet, Burnside/Polya, Catalan/Stirling/Bell/partitions,
│                                 #   inclusion-exclusion, tree counting, subset convolution, special
│                                 #   sequences, Euler transform & symbolic method, permanent,
│                                 #   Bernoulli/Faulhaber, cycle lemma & Fuss-Catalan, LGV & Polya
├── 06 - Math                     # Mint, totient/Mobius, matrices, XOR basis, Gaussian elimination,
│                                 #   FFT/NTT/FWHT, formal power series, Lagrange & Faulhaber,
│                                 #   Berlekamp-Massey, simplex-free LP notes
├── 07 - Strings                  # KMP, Z, Rabin-Karp, wildcard matching (FFT), hashing (+ 2D),
│                                 #   Aho-Corasick (+ DP over the automaton), suffix array (+ apps),
│                                 #   suffix automaton, generalized SAM, Manacher, palindromic tree,
│                                 #   Duval/Booth
├── 08 - Dynamic Programming (DP) # CHT/Li Chao/D&C/Knuth/Alien's, min-plus convolution, slope trick,
│                                 #   SOS, knapsack family, digit DP, LIS, bitmask & broken-profile,
│                                 #   interval DP, expected-value DP
├── 09 - Trees                    # LCA, binary lifting, DSU on tree, HLD, centroid, diameter,
│                                 #   Euler tour, rerooting, virtual tree, isomorphism (AHU)
├── 10 - Game Theory              # Sprague-Grundy, nim variants, retrograde analysis, minimax,
│                                 #   named games + periodicity theorems
└── 11 - Geometry                 # core/lines/polygon/circles/sweep/transforms/KD-tree/3D,
                                  #   half-plane intersection, polygon & circle unions, triangulation
```

Each `.cpp` holds one self-contained template with its complexity in the header comment.
Files whose content is a reference page (formula sheets) are comment-only by design.

## Exporting to PDF

```bash
./build.sh          # points at the local TeX under ~/texlive, then runs the exporter
node export_notebook.js            # same thing if pdflatex is already on your PATH
node export_notebook.js --no-pdf   # regenerate notebook.tex and toc.yaml only
```

Produces `notebook.tex`, `notebook.pdf` (A4, **portrait**, two columns, 9pt) and `toc.yaml`.
The script **warns** about any file it skips — anything not ending in `.cpp` is excluded from
the PDF, so watch that output.

The build is three `pdflatex` passes with `makeindex` between the first and second, and that
count is not padding: pass 1 writes the `.aux`/`.toc`/`.idx`, pass 2 sets the contents and the
index, pass 3 settles the `\pageref` in every per-section topic map.

Requires Node.js 14+ and a LaTeX distribution providing `pdflatex` **and** `makeindex` (MiKTeX on
Windows, TeX Live elsewhere). Packages used: `extarticle`, `listings`, `xcolor`, `titlesec`,
`fancyhdr`, `multicol`, `makeidx`, `hyperref`, and — optionally, each guarded by `\IfFileExists`
so a minimal install still compiles — `inconsolata`/`zi4`, `microtype`, `needspace`, `bookmark`.
Inconsolata is the one worth installing: it is ~15% narrower than Latin Modern Typewriter, so
noticeably fewer source lines wrap.

**Overleaf:** upload `notebook.tex` alone and compile. Everything above is in the default image.
The only thing you lose is the per-letter headings in the back index, because Overleaf runs
`makeindex` without `-s notebook.ist`; the index itself still builds.

## Conventions

- Directories and files carry a 2-digit ordering prefix: `01 - Name`.
- Vertices and array positions are **0-based** unless the header says otherwise.
- Anything that needs `Mint`, `DSU`, `MultSieve`, … says so in its header comment.
- Geometry: use `P<ll>` for predicates, `P<ld>` only when the answer is a real number.
- Avoid `__gcd` (libstdc++ only) — use `gcd` from `<numeric>`.

## Adding a template

1. Drop a `.cpp` into the right `notebook/XX - Category/` directory.
2. First line: one comment saying what it does and its complexity.
3. Re-run `node export_notebook.js`.
4. Stress-test it before the contest, not during one.
