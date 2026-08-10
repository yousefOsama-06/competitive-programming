# Competitive Programming Notebook

A curated collection of competitive programming templates and algorithms, organized by topic and ready to export as a printable PDF notebook.

## 📁 Directory Structure

All templates live inside the `notebook/` directory, organized by category:

```
notebook/
├── 01 - Miscellaneous            # Fast I/O, template, utilities, random, custom hash, int128
├── 02 - Data Structures (DS)     # Segment Tree, BIT, Sparse Table, Trie, DSU, Mo's, Treap, PBDS, Monotonic
├── 03 - Graph Theory             # Traversal, Shortest Paths, Connectivity, 2-SAT, Eulerian Path, Flow & Matching
├── 04 - Number Theory            # Extended Euclid, Linear Diophantine, Sieve, Segmented Sieve, Factorization, Divisors, CRT, Floor Sum
├── 05 - Combinatorics            # Stars & Bars, Burnside's Lemma / Pólya
├── 06 - Math                     # Modular Arithmetic, Euler Totient, Möbius, Matrices, FFT, NTT, FWHT, Generating Functions, XOR Basis
├── 07 - Strings                  # KMP, Z-function, Rabin-Karp, Hashing, Aho-Corasick, Suffix Array/Automaton, Manacher
├── 08 - Dynamic Programming (DP) # CHT, Rollback CHT, Li Chao Tree, Persistent Li Chao, Divide & Conquer DP, SOS DP, Knuth, Alien's Trick
├── 09 - Trees                    # LCA, DSU on Tree (Sack), HLD, Centroid Decomposition, Tree Diameter, Euler Tour
├── 10 - Game Theory              # MEX
└── 11 - Geometry                 # Point 2D/3D, Lines, Convex Hull, Rotating Calipers, Half-plane Intersection, MEC
```

Each `.cpp` file contains a single self-contained template/algorithm encapsulated in a `struct` (where applicable), with complexity annotations in the header comment.

## 🖨️ Exporting to PDF

The `export_notebook.js` script generates a **print-ready LaTeX PDF** with:
- 2-column landscape layout (A4)
- Syntax-highlighted C++ code
- Table of contents
- Automatic section/subsection hierarchy from the directory structure

### Prerequisites

- [Node.js](https://nodejs.org/) (v14+)
- [MiKTeX](https://miktex.org/) (or any LaTeX distribution with `pdflatex`)

### Usage

```bash
node export_notebook.js
```

This generates:
- `notebook.tex` — the LaTeX source
- `notebook.pdf` — the compiled, ready-to-print PDF
- `toc.yaml` — structured table of contents in YAML format

> **Note:** On first run, MiKTeX may prompt you to install required LaTeX packages (`extarticle`, `listings`, `fancyhdr`, etc.). Approve the installations to proceed. If MiKTeX blocks compilation with an update nag, run `mpm --update` once from the command line to resolve it.

## ➕ Adding New Templates

1. Create a `.cpp` file in the appropriate `notebook/XX - Category/` directory.
2. The filename becomes the subsection title in the PDF (e.g., `BIT.cpp` → **BIT**).
3. Re-run `node export_notebook.js` to regenerate the PDF.

### Adding a New Category

1. Create a new directory in `notebook/` with the next index: `notebook/XX - Category Name/`
2. Add `.cpp` files inside it.
3. Re-run the export script.

## 📝 Naming Conventions

- **Directories** are prefixed with a 2-digit index for ordering: `01 - Name`
- **Subdirectories** follow the same convention within their parent
- **Files** are named after the algorithm/data structure they implement
- **Structs** are encapsulated with matching names (e.g., `struct FenwickTree`, `struct Sieve`)
- **Complexity** is documented in the header comment of each file
