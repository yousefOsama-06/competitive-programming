#define sz(aa) (int)aa.size()

template <class T>
struct squareSparseTable {
    int n, m;
    // jmp[k][i][j] stores the max of a 2^k by 2^k square starting at (i, j)
    vector<vector<vector<T>>> jmp;

    void build(const vector<vector<T>>& V) {
        n = sz(V);
        if (n == 0) return;
        m = sz(V[0]);
        if (m == 0) return;

        // The maximum possible square side length is min(n, m)
        int max_len = min(n, m);
        int log_max = 32 - __builtin_clz(max_len);

        // Only 3 dimensions: log(min(N,M)) x N x M
        jmp.assign(log_max, vector<vector<T>>(n, vector<T>(m)));

        // Base case: 1x1 squares (k = 0)
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < m; ++j) {
                jmp[0][i][j] = V[i][j];
            }
        }

        // Build larger squares by combining 4 smaller squares
        for (int k = 1; k < log_max; ++k) {
            int pw = 1 << (k - 1); // Length of the smaller squares (2^(k-1))
            for (int i = 0; i + (pw * 2) <= n; ++i) {
                for (int j = 0; j + (pw * 2) <= m; ++j) {
                    jmp[k][i][j] = max({
                        jmp[k - 1][i][j],               // Top-Left
                        jmp[k - 1][i][j + pw],          // Top-Right
                        jmp[k - 1][i + pw][j],          // Bottom-Left
                        jmp[k - 1][i + pw][j + pw]      // Bottom-Right
                    });
                }
            }
        }
    }

    // Queries the maximum in a square from (r1, c1) to (r2, c2)
    T query(int r1, int c1, int r2, int c2) {
        assert(r1 <= r2 && c1 <= c2);
        assert((r2 - r1) == (c2 - c1)); // Ensure it is actually a square

        int len = r2 - r1 + 1;
        int k = 31 - __builtin_clz(len);
        int pw = 1 << k;

        // Overlap four 2^k x 2^k squares to cover the arbitrary S x S square
        return max({
            jmp[k][r1][c1],
            jmp[k][r1][c2 - pw + 1],
            jmp[k][r2 - pw + 1][c1],
            jmp[k][r2 - pw + 1][c2 - pw + 1]
        });
    }
};
