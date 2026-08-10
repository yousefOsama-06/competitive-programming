// Knuth's DP Optimization - Time: O(N^2), Space: O(N^2)
// Optimizes interval DP: dp[i][j] = min_{i <= k < j} (dp[i][k] + dp[k+1][j]) + cost(i, j)
// Applicable when opt[i][j-1] <= opt[i][j] <= opt[i+1][j]
struct KnuthDP {
    int n;
    vector<vector<ll>> dp;
    vector<vector<int>> opt;

    KnuthDP(int n = 0) : n(n), dp(n + 2, vector<ll>(n + 2, 0)), opt(n + 2, vector<int>(n + 2, 0)) {}

    // User-defined cost function cost(i, j)
    ll solve(function<ll(int, int)> cost) {
        for (int i = 1; i <= n; i++) {
            opt[i][i] = i;
        }

        for (int len = 2; len <= n; len++) {
            for (int i = 1; i + len - 1 <= n; i++) {
                int j = i + len - 1;
                dp[i][j] = llinf;
                int L = opt[i][j - 1], R = opt[i + 1][j];
                for (int k = L; k <= min(R, j - 1); k++) {
                    ll cur = dp[i][k] + dp[k + 1][j] + cost(i, j);
                    if (cur < dp[i][j]) {
                        dp[i][j] = cur;
                        opt[i][j] = k;
                    }
                }
            }
        }
        return dp[1][n];
    }
};
