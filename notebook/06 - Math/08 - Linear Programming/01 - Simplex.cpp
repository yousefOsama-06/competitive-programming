// TWO-PHASE SIMPLEX.  maximise c.x  subject to  A x <= b,  x >= 0.
// Returns the optimum, or +inf if unbounded, or -inf if infeasible; x is filled with a solution.
// Exponential in theory, O(n*m) per pivot and instant at contest sizes (n, m up to a few hundred).
// REACH FOR IT when the problem is "optimise a linear objective under linear constraints" and no
// combinatorial structure presents itself - resource allocation, blending, fractional covering,
// zero-sum matrix games, and any min-cost-flow-with-side-constraints that flow cannot express.
typedef vector<ld> vd;
struct Simplex {
    int m, n;
    vector<int> B, N;
    vector<vd> D;
    Simplex(const vector<vd>& A, const vd& b, const vd& c)
        : m(b.size()), n(c.size()), B(m), N(n + 1), D(m + 2, vd(n + 2, 0)) {
        for (int i = 0; i < m; i++) for (int j = 0; j < n; j++) D[i][j] = A[i][j];
        for (int i = 0; i < m; i++) B[i] = n + i, D[i][n] = -1, D[i][n + 1] = b[i];
        for (int j = 0; j < n; j++) N[j] = j, D[m][j] = -c[j];
        N[n] = -1, D[m + 1][n] = 1;
    }
    void pivot(int r, int s) {
        ld inv = 1 / D[r][s];
        for (int i = 0; i < m + 2; i++) if (i != r && fabsl(D[i][s]) > 1e-9) {
            ld f = D[i][s] * inv;                              // the pivot COLUMN is fixed up by
            for (int j = 0; j < n + 2; j++) if (j != s) D[i][j] -= f * D[r][j];   // the loop below
        }
        for (int j = 0; j < n + 2; j++) if (j != s) D[r][j] *= inv;
        for (int i = 0; i < m + 2; i++) if (i != r) D[i][s] *= -inv;
        D[r][s] = inv;
        swap(B[r], N[s]);
    }
    bool simplex(int phase) {
        int x = m + phase - 1;
        for (;;) {
            int s = -1;
            for (int j = 0; j <= n; j++)
                if (N[j] != -phase && (s < 0 || make_pair(D[x][j], N[j]) < make_pair(D[x][s], N[s]))) s = j;
            if (D[x][s] >= -1e-9) return true;
            int r = -1;
            for (int i = 0; i < m; i++) {
                if (D[i][s] <= 1e-9) continue;
                if (r < 0 || make_pair(D[i][n + 1] / D[i][s], B[i]) < make_pair(D[r][n + 1] / D[r][s], B[r])) r = i;
            }
            if (r < 0) return false;                           // unbounded
            pivot(r, s);
        }
    }
    ld solve(vd& x) {
        int r = 0;
        for (int i = 1; i < m; i++) if (D[i][n + 1] < D[r][n + 1]) r = i;
        if (D[r][n + 1] < -1e-9) {                             // phase 1: find any feasible point
            pivot(r, n);
            if (!simplex(2) || D[m + 1][n + 1] < -1e-9) return -1.0L / 0.0L;   // infeasible
            for (int i = 0; i < m; i++) if (B[i] == -1) {
                int s = 0;
                for (int j = 1; j <= n; j++)
                    if (make_pair(D[i][j], N[j]) < make_pair(D[i][s], N[s])) s = j;
                pivot(i, s);
            }
        }
        bool ok = simplex(1);
        x.assign(n, 0);
        for (int i = 0; i < m; i++) if (B[i] < n) x[B[i]] = D[i][n + 1];
        return ok ? D[m][n + 1] : 1.0L / 0.0L;                 // +inf = unbounded
    }
};
/* MODELLING NOTES - the part that actually costs you time
 MINIMISE instead: negate c and negate the answer.
 >= CONSTRAINT: multiply the row by -1.   EQUALITY: add it twice, as <= and as >=.
 FREE VARIABLE (no x >= 0): substitute x = u - v with u, v >= 0. Doubles that column.
 UPPER BOUND x_i <= U: just another row; simplex has no special handling and does not need any.
 LINEAR-FRACTIONAL objective (c.x + a) / (d.x + b): CHARNES-COOPER - substitute y = x/(d.x+b),
   t = 1/(d.x+b) and maximise c.y + a t subject to A y <= b t, d.y + b t = 1, t >= 0. Linear again.
 INTEGER answers are NOT guaranteed. If the constraint matrix is TOTALLY UNIMODULAR (network
   matrices, interval matrices, bipartite incidence) the optimum IS integral - that is exactly why
   flow problems have integral optima. Otherwise you need ILP, which is NP-hard.
 ZERO-SUM MATRIX GAME: shift A so every entry is > 0, then min sum(y) s.t. A y >= 1, y >= 0 gives
   value v = 1/sum(y) and the column player's mixed strategy y*v; the DUAL gives the row player's.
   Von Neumann: max_x min_y x^T A y = min_y max_x x^T A y.
 DUALITY, which is often the actual solution: max c.x, Ax <= b, x >= 0  is dual to
   min b.y, A^T y >= c, y >= 0; the two optima are equal. COMPLEMENTARY SLACKNESS at the optimum:
   x_j > 0 => the j-th dual constraint is tight, and y_i > 0 => the i-th primal row is tight.
   The combinatorial specialisations you already know are max-flow/min-cut and Konig.
 PRECISION: this is floating point. If the data is integral and small, and you need an exact
   answer, prefer a combinatorial model (flow, matching) over simplex. */
