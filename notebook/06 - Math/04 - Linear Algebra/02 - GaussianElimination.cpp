// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp) for the modular routines.
// Gaussian elimination mod a PRIME. Returns rank; solves A x = b; also det and inverse.
// O(n^2 m). Pass the augmented matrix (n rows, m+1 cols) to solve().
int gaussMod(vector<vector<Mint>>& a, int m) {           // reduce first m columns to RREF
    int n = a.size(), rank = 0;
    for (int col = 0; col < m && rank < n; col++) {
        int piv = -1;
        for (int i = rank; i < n; i++) if (a[i][col].v) { piv = i; break; }
        if (piv < 0) continue;
        swap(a[rank], a[piv]);
        Mint iv = a[rank][col].inv();
        for (auto& x : a[rank]) x *= iv;
        for (int i = 0; i < n; i++) if (i != rank && a[i][col].v) {
            Mint f = a[i][col];
            for (size_t j = 0; j < a[i].size(); j++) a[i][j] -= f * a[rank][j];
        }
        rank++;
    }
    return rank;
}
// Solve A x = b. Returns {} if inconsistent, else one solution (free vars = 0).
// `free` receives the indices of the free variables (solution space has MOD^|free| points).
vector<Mint> solveLinear(vector<vector<Mint>> a, vector<Mint> b, vector<int>* freeIdx = nullptr) {
    int n = a.size(), m = a[0].size();
    for (int i = 0; i < n; i++) a[i].push_back(b[i]);
    int rank = gaussMod(a, m);
    for (int i = rank; i < n; i++) if (a[i][m].v) return {};      // 0 = nonzero
    vector<Mint> x(m, 0);
    int r = 0;
    for (int c = 0; c < m; c++) {                   // must run over EVERY column: stopping at
        if (r < rank && a[r][c].v) x[c] = a[r][m], r++;   // r == rank drops the free columns
        else if (freeIdx) freeIdx->push_back(c);          // that come after the last pivot
    }
    return x;
}
Mint determinant(vector<vector<Mint>> a) {               // O(n^3), destroys a
    int n = a.size();
    Mint det = 1;
    for (int col = 0; col < n; col++) {
        int piv = -1;
        for (int i = col; i < n; i++) if (a[i][col].v) { piv = i; break; }
        if (piv < 0) return 0;
        if (piv != col) swap(a[col], a[piv]), det = Mint(0) - det;
        det *= a[col][col];
        Mint iv = a[col][col].inv();
        for (int i = col + 1; i < n; i++) if (a[i][col].v) {
            Mint f = a[i][col] * iv;
            for (int j = col; j < n; j++) a[i][j] -= f * a[col][j];
        }
    }
    return det;
}
vector<vector<Mint>> inverse(vector<vector<Mint>> a) {   // {} if singular
    int n = a.size();
    for (int i = 0; i < n; i++) { a[i].resize(2 * n, 0); a[i][n + i] = 1; }
    if (gaussMod(a, n) < n) return {};
    vector<vector<Mint>> r(n, vector<Mint>(n));
    for (int i = 0; i < n; i++) for (int j = 0; j < n; j++) r[i][j] = a[i][n + j];
    return r;
}
// MATRIX-TREE: #spanning trees = determinant of the Laplacian (deg on diagonal, -1 per edge)
// with any one row AND the matching column deleted.
