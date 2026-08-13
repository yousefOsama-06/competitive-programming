// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp). MOD must be PRIME.
// LINEAR RECURRENCE WITH POLYNOMIAL COEFFICIENTS (a "P-recursive" or holonomic sequence):
//   sum_{i=0..order} P_i(m - i) * a[m - i] = 0   for all large m,   deg P_i <= deg.
// This is the strictly stronger sibling of Berlekamp-Massey. BM only finds CONSTANT-coefficient
// recurrences, so it fails on n!, C(2n, n), Motzkin, derangements, Hertzsprung, and essentially
// every sequence built from binomials or factorials. Those are all P-recursive.
// REACH FOR IT in the classic contest workflow: brute-force the first ~60-100 terms, guess the
// recurrence here, then extend it to n = 1e7 in O(n * order * deg). Also tells you the answer IS
// holonomic, which is itself a strong hint about the intended solution.
// COST: guessing is O(n^3 / deg^2)-ish Gaussian elimination on an R x C matrix; extending is
// linear. You must GUESS deg (try 0, 1, 2, ... until it succeeds) - order comes out automatically.
// You need roughly (deg + 2) * (order + 1) terms; give it more and the verification is meaningful.

// coeffs[i][d] = coefficient of m^d in P_i. Returns {} if no such recurrence fits the data.
vector<vector<Mint>> findPRec(const vector<Mint>& t, int deg) {
    int n = t.size(), B = (n + 2) / (deg + 2), C = B * (deg + 1), R = n - (B - 1);
    if (B < 2 || R < C - 1) return {};
    vector<vector<Mint>> mat(R, vector<Mint>(C));
    for (int y = 0; y < R; y++)
        for (int b = 0; b < B; b++) {
            Mint v = t[y + b];
            for (int d = 0; d <= deg; d++) mat[y][b * (deg + 1) + d] = v, v *= Mint(y + b);
        }
    int rank = 0;
    for (int x = 0; x < C && rank < R; x++) {                    // RREF
        int piv = -1;
        for (int y = rank; y < R; y++) if (mat[y][x].v) { piv = y; break; }
        if (piv < 0) continue;
        swap(mat[rank], mat[piv]);
        Mint iv = Mint(1) / mat[rank][x];
        for (int j = x; j < C; j++) mat[rank][j] *= iv;
        for (int y = 0; y < R; y++) if (y != rank && mat[y][x].v) {
            Mint c = mat[y][x];
            for (int j = x; j < C; j++) mat[y][j] -= c * mat[rank][j];
        }
        rank++;
    }
    if (rank == C) return {};                                    // no non-trivial kernel vector
    // column `rank` is free: set it to 1 and read the pivot columns off (the matrix is in RREF)
    int order = rank / (deg + 1);
    vector<vector<Mint>> r(order + 1, vector<Mint>(deg + 1, 0));
    r[0][rank % (deg + 1)] = 1;
    for (int y = rank - 1; y >= 0; y--)
        r[order - y / (deg + 1)][y % (deg + 1)] = Mint(0) - mat[y][rank];
    return r;
}
// Extend the sequence to index n. `terms` must hold at least `order` correct initial values.
// PRECONDITION: the leading polynomial P_0(m) must be non-zero mod MOD at every m you generate -
// if it vanishes the recurrence genuinely does not determine a[m] (a "singularity" of the
// recurrence); shift the start or pick a different MOD.
vector<Mint> extendPRec(int n, const vector<vector<Mint>>& c, vector<Mint> terms) {
    int order = c.size() - 1, deg = c[0].size() - 1;
    vector<Mint> a = terms;
    a.resize(max<int>(n + 1, terms.size()), 0);
    for (int m = terms.size(); m <= n; m++) {
        Mint s = 0;
        for (int i = 1; i <= order; i++) {
            Mint p = 1, k = Mint(m - i);
            for (int d = 0; d <= deg; d++) s += a[m - i] * c[i][d] * p, p *= k;
        }
        Mint den = 0, p = 1;
        for (int d = 0; d <= deg; d++) den += c[0][d] * p, p *= Mint(m);
        a[m] = (Mint(0) - s) / den;
    }
    return a;
}
// EXAMPLES that come out immediately (deg, order):
//   n!  -> (1, 1):  a[m] - m * a[m-1] = 0.        Catalan -> (1, 1).      Motzkin -> (1, 2).
//   derangements -> (1, 2).  C(2n, n) -> (1, 1).  Hertzsprung -> (1, 4).  sum C(n,k)^3 -> (2, 2).
// ALWAYS SANITY-CHECK: re-run extendPRec on the initial terms and compare against the input; a
// recurrence that fits by accident is the main failure mode when you feed too few terms.
// THEORY: holonomic sequences are closed under +, Cauchy product, and Hadamard product, and the
// generating function satisfies a linear ODE with polynomial coefficients. That is why "sum of
// products of binomials" is essentially always P-recursive (Zeilberger's algorithm proves it).
// If you also need the k-th term for k = 1e18, P-recursive sequences admit an O(sqrt(k) log k)
// evaluation by the factorial-style baby-step/giant-step product of matrices - far more code.
