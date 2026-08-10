// 2D Sparse Table - O(N * M * log N * log M) build, O(1) query
template<typename T = ll>
struct SparseTable2D {
    int n, m;
    // jmp[kr][kc][i][j] = max in 2^kr x 2^kc subgrid starting at (i, j)
    vector<vector<vector<vector<T>>>> jmp;

    SparseTable2D(const vector<vector<T>>& grid) {
        n = grid.size();
        if (!n) return;
        m = grid[0].size();
        if (!m) return;

        int log_n = __lg(n) + 1, log_m = __lg(m) + 1;
        jmp.assign(log_n, vector<vector<vector<T>>>(log_m, vector<vector<T>>(n, vector<T>(m))));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                jmp[0][0][i][j] = grid[i][j];
            }
        }

        for (int kc = 1; kc < log_m; kc++) {
            for (int i = 0; i < n; i++) {
                for (int j = 0; j + (1 << kc) <= m; j++) {
                    jmp[0][kc][i][j] = max(jmp[0][kc - 1][i][j], jmp[0][kc - 1][i][j + (1 << (kc - 1))]);
                }
            }
        }

        for (int kr = 1; kr < log_n; kr++) {
            for (int kc = 0; kc < log_m; kc++) {
                for (int i = 0; i + (1 << kr) <= n; i++) {
                    for (int j = 0; j + (1 << kc) <= m; j++) {
                        jmp[kr][kc][i][j] = max(jmp[kr - 1][kc][i][j], jmp[kr - 1][kc][i + (1 << (kr - 1))][j]);
                    }
                }
            }
        }
    }

    T query(int r1, int c1, int r2, int c2) const {
        int kr = __lg(r2 - r1 + 1), kc = __lg(c2 - c1 + 1);
        return max({jmp[kr][kc][r1][c1],
                    jmp[kr][kc][r1][c2 - (1 << kc) + 1],
                    jmp[kr][kc][r2 - (1 << kr) + 1][c1],
                    jmp[kr][kc][r2 - (1 << kr) + 1][c2 - (1 << kc) + 1]});
    }
};
