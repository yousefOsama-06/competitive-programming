// 2D Square Sparse Table - O(N * M * log(min(N, M))) build, O(1) square query
template<typename T = ll>
struct SquareSparseTable {
    int n, m;
    // jmp[k][i][j] = max of 2^k x 2^k square starting at (i, j)
    vector<vector<vector<T>>> jmp;

    SquareSparseTable(const vector<vector<T>>& grid) {
        n = grid.size();
        if (!n) return;
        m = grid[0].size();
        if (!m) return;

        int log_max = __lg(min(n, m)) + 1;
        jmp.assign(log_max, vector<vector<T>>(n, vector<T>(m)));

        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                jmp[0][i][j] = grid[i][j];
            }
        }

        for (int k = 1; k < log_max; k++) {
            int pw = 1 << (k - 1);
            for (int i = 0; i + (pw * 2) <= n; i++) {
                for (int j = 0; j + (pw * 2) <= m; j++) {
                    jmp[k][i][j] = max({jmp[k - 1][i][j],
                                        jmp[k - 1][i][j + pw],
                                        jmp[k - 1][i + pw][j],
                                        jmp[k - 1][i + pw][j + pw]});
                }
            }
        }
    }

    // Queries max in square subgrid from (r1, c1) to (r2, c2)
    T query(int r1, int c1, int r2, int c2) const {
        int k = __lg(r2 - r1 + 1), pw = 1 << k;
        return max({jmp[k][r1][c1],
                    jmp[k][r1][c2 - pw + 1],
                    jmp[k][r2 - pw + 1][c1],
                    jmp[k][r2 - pw + 1][c2 - pw + 1]});
    }
};
