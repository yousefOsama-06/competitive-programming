// XOR-equation systems over GF(2) with bitset - O(n*m/64). n equations, m variables,
// row[i][m] holds the RHS. Returns rank, or -1 if inconsistent.
template <int M>
int gaussXor(vector<bitset<M + 1>>& a, int m, bitset<M>* sol = nullptr) {
    int n = a.size(), rank = 0;
    vector<int> where(m, -1);
    for (int col = 0; col < m && rank < n; col++) {
        int piv = -1;
        for (int i = rank; i < n; i++) if (a[i][col]) { piv = i; break; }
        if (piv < 0) continue;
        swap(a[rank], a[piv]);
        for (int i = 0; i < n; i++) if (i != rank && a[i][col]) a[i] ^= a[rank];
        where[col] = rank++;
    }
    for (int i = rank; i < n; i++) if (a[i][m]) return -1;
    if (sol) { sol->reset(); for (int c = 0; c < m; c++) if (where[c] >= 0) (*sol)[c] = a[where[c]][m]; }
    return rank;                                        // #solutions = 2^(m - rank)
}
// USES: "light switch" puzzles, xor-equation counting, rank of a set of vectors over GF(2),
//       linear independence of xor values (dense sibling of the XOR basis).

// ---- Gaussian elimination over the reals, with partial pivoting ----
// Returns rank; a is n x (m+1) augmented. Solution in x (free vars = 0). eps-based.
int gaussReal(vector<vector<ld>> a, int m, vector<ld>& x) {
    int n = a.size(), rank = 0;
    vector<int> where(m, -1);
    for (int col = 0; col < m && rank < n; col++) {
        int piv = rank;
        for (int i = rank; i < n; i++) if (fabsl(a[i][col]) > fabsl(a[piv][col])) piv = i;
        if (fabsl(a[piv][col]) < 1e-12) continue;
        swap(a[rank], a[piv]);
        for (int i = 0; i < n; i++) if (i != rank) {
            ld f = a[i][col] / a[rank][col];
            for (int j = col; j <= m; j++) a[i][j] -= f * a[rank][j];
        }
        where[col] = rank++;
    }
    x.assign(m, 0);
    for (int c = 0; c < m; c++) if (where[c] >= 0) x[c] = a[where[c]][m] / a[where[c]][c];
    return rank;
}
// The classic use is EXPECTED-VALUE DP with cyclic state dependencies: write one equation per
// state, E[s] = 1 + sum p(s->t) E[t], and solve. n <= ~500 fits O(n^3).
