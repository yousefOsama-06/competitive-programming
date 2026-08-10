// 2D Monotonic Queue (Sliding Window 2D Min/Max) - Time: O(N * M), Space: O(N * M)
// Computes min in every K x L subgrid of an N x M matrix
template<typename T = ll>
struct MonotonicQueue2D {
    int n, m;

    // Returns a matrix res where res[i][j] is the min in subgrid [i..i+K-1][j..j+L-1]
    vector<vector<T>> sliding_window_2d(const vector<vector<T>>& grid, int K, int L) {
        n = grid.size();
        m = grid[0].size();

        // 1. Sliding window min horizontally along each row
        vector<vector<T>> row_min(n, vector<T>(m - L + 1));
        for (int i = 0; i < n; i++) {
            deque<int> dq;
            for (int j = 0; j < m; j++) {
                while (!dq.empty() && grid[i][dq.back()] >= grid[i][j]) dq.pop_back();
                dq.pb(j);
                if (dq.front() <= j - L) dq.pop_front();
                if (j >= L - 1) row_min[i][j - L + 1] = grid[i][dq.front()];
            }
        }

        // 2. Sliding window min vertically down each column of row_min
        vector<vector<T>> res(n - K + 1, vector<T>(m - L + 1));
        for (int j = 0; j < m - L + 1; j++) {
            deque<int> dq;
            for (int i = 0; i < n; i++) {
                while (!dq.empty() && row_min[dq.back()][j] >= row_min[i][j]) dq.pop_back();
                dq.pb(i);
                if (dq.front() <= i - K) dq.pop_front();
                if (i >= K - 1) res[i - K + 1][j] = row_min[dq.front()][j];
            }
        }

        return res;
    }
};
