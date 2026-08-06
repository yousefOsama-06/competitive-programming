#define sz(aa) (int)aa.size()

template <class T>
struct sparseTable2D {
    int n, m;
    // jmp[kr][kc][i][j] stores the max of a 2^kr by 2^kc rectangle starting at (i, j)
    vector<vector<vector<vector<T>>>> jmp;

    void build(const vector<vector<T>>& V) {
        n = sz(V);
        if (n == 0) return;
        m = sz(V[0]);
        if (m == 0) return;

        int log_n = 32 - __builtin_clz(n);
        int log_m = 32 - __builtin_clz(m);

        jmp.assign(log_n, vector<vector<vector<T>>>(log_m, vector<vector<T>>(n, vector<T>(m))));

        // Base case: 1x1 rectangles (kr = 0, kc = 0)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                jmp[0][0][i][j] = V[i][j];
            }
        }

        // Build 1D sparse tables for each row (kr = 0, increasing kc)
        for (int kc = 1; kc < log_m; ++kc) {
            for (int i = 0; i < n; ++i) {
                for (int j = 0; j + (1 << kc) <= m; ++j) {
                    jmp[0][kc][i][j] = max(jmp[0][kc - 1][i][j], 
                                           jmp[0][kc - 1][i][j + (1 << (kc - 1))]);
                }
            }
        }

        // Build the 2D tables by merging rows (increasing kr, for all kc)
        for (int kr = 1; kr < log_n; ++kr) {
            for (int kc = 0; kc < log_m; ++kc) {
                for (int i = 0; i + (1 << kr) <= n; ++i) {
                    for (int j = 0; j + (1 << kc) <= m; ++j) {
                        jmp[kr][kc][i][j] = max(jmp[kr - 1][kc][i][j], 
                                                jmp[kr - 1][kc][i + (1 << (kr - 1))][j]);
                    }
                }
            }
        }
    }

    // Queries the maximum in the subgrid from (r1, c1) to (r2, c2) inclusive
    T query(int r1, int c1, int r2, int c2) {
        assert(r1 <= r2 && c1 <= c2);
        int kr = 31 - __builtin_clz(r2 - r1 + 1);
        int kc = 31 - __builtin_clz(c2 - c1 + 1);

        // Get the 4 overlapping sub-rectangles
        T top_left = jmp[kr][kc][r1][c1];
        T top_right = jmp[kr][kc][r1][c2 - (1 << kc) + 1];
        T bottom_left = jmp[kr][kc][r2 - (1 << kr) + 1][c1];
        T bottom_right = jmp[kr][kc][r2 - (1 << kr) + 1][c2 - (1 << kc) + 1];

        return max({top_left, top_right, bottom_left, bottom_right});
    }
};
