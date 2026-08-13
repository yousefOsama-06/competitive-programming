// KNUTH OPTIMIZATION - interval DP  dp[i][j] = min_{i<=k<j}(dp[i][k] + dp[k+1][j]) + cost(i,j)
// from O(n^3) to O(n^2), by restricting k to [opt[i][j-1], opt[i+1][j]].
// THE HYPOTHESIS IS ON cost, NOT ON opt. `opt[i][j-1] <= opt[i][j] <= opt[i+1][j]` is the
// CONCLUSION - you cannot test it before running. What you must check is BOTH of:
//   (1) QUADRANGLE INEQUALITY (Monge):  cost(a,c) + cost(b,d) <= cost(a,d) + cost(b,c)
//       for all a <= b <= c <= d.  Equivalent local form, and the only one checkable by hand:
//       cost(a,c) + cost(a+1,c+1) <= cost(a+1,c) + cost(a,c+1).
//   (2) MONOTONE ON INTERVALS:  a <= b <= c <= d  =>  cost(b,c) <= cost(a,d).
// Both hold for: sum of a non-negative array over [i,j]; (S_j - S_i)^2; f(S_j - S_i) with f convex
// and a[] >= 0; "number of equal pairs inside [i,j]". They FAIL for anything built from max, and
// monotonicity fails as soon as a[] can be negative (QI can still hold - D&C needs only QI,
// Knuth needs both). Applied without them there is no crash and no TLE, just a wrong answer.
// Classic uses: optimal BST, merging n piles of stones, CF 1101F-style interval costs.
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
