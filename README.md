# Competitive Programming Notebook

A curated collection of competitive programming templates and algorithms, organized by topic and ready to export as a printable PDF notebook.

## 📁 Directory Structure

All templates live inside the `notebook/` directory, organized by category:

```
notebook/
├── 01 - Miscellaneous            # Fast I/O & Utilities, grid, custom hash
├── 02 - Data Structures (DS)     # Segment Tree, BIT, Sparse Table, Trie, DSU, Mo's, Treap, PBDS
├── 03 - Graph Theory             # Traversal, Shortest Paths, Connectivity, 2-SAT, Eulerian Path, Flow & Matching
├── 04 - Number Theory            # GCD/LCM, Extended Euclid, Sieve, CRT
├── 05 - Combinatorics            # Combinatorics & counting templates
├── 06 - Math                     # Modular arithmetic, Matrices, FFT, NTT, XOR Basis
├── 07 - Strings                  # KMP, Z-function, Rabin-Karp, Hashing, Aho-Corasick, Suffix Array/Automaton, Manacher
├── 08 - Dynamic Programming (DP) # CHT, Li Chao tree, Divide & Conquer DP, SOS DP
├── 09 - Trees                    # DSU on Tree (Sack), HLD, Centroid Decomposition
├── 10 - Game Theory              # MEX
└── 11 - Geometry                 # Point 2D/3D, Convex Hull, Half-plane intersection
```

Each `.cpp` file contains a single self-contained template/algorithm.

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

> **Note:** On first run, MiKTeX may prompt you to install required LaTeX packages (`extarticle`, `listings`, `fancyhdr`, etc.). Approve the installations to proceed.

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
