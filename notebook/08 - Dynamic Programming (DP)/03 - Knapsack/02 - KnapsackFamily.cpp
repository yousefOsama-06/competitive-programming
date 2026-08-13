// KNAPSACK FAMILY. W = capacity. All O(n*W) unless noted.
// 0/1: iterate capacity DOWNWARD so each item is used once.
ll knap01(const vector<int>& w, const vector<ll>& v, int W) {
    vector<ll> dp(W + 1, 0);
    for (size_t i = 0; i < w.size(); i++)
        for (int c = W; c >= w[i]; c--) dp[c] = max(dp[c], dp[c - w[i]] + v[i]);
    return dp[W];
}
// UNBOUNDED: iterate capacity UPWARD.
ll knapInf(const vector<int>& w, const vector<ll>& v, int W) {
    vector<ll> dp(W + 1, 0);
    for (size_t i = 0; i < w.size(); i++)
        for (int c = w[i]; c <= W; c++) dp[c] = max(dp[c], dp[c - w[i]] + v[i]);
    return dp[W];
}
// BOUNDED (item i available cnt[i] times) via BINARY GROUPING: split cnt into 1,2,4,...,rest
// and run 0/1 on the groups. O(W * sum log cnt).
ll knapBounded(const vector<int>& w, const vector<ll>& v, const vector<int>& cnt, int W) {
    vector<int> W2; vector<ll> V2;
    for (size_t i = 0; i < w.size(); i++) {
        int c = cnt[i];
        for (int k = 1; c > 0; k <<= 1) {
            int t = min(k, c); c -= t;
            W2.push_back(w[i] * t), V2.push_back(v[i] * t);
        }
    }
    return knap01(W2, V2, W);
}
// FEASIBILITY ONLY (which sums are reachable) -> bitset, O(n*W/64). Massively faster.
bitset<100001> reachableSums(const vector<int>& w) {
    bitset<100001> dp;
    dp[0] = 1;
    for (int x : w) dp |= dp << x;
    return dp;
}
// COUNTING ways (mod p): same loops, dp[c] += dp[c - w[i]].
// Order matters: item-outer/capacity-inner counts COMBINATIONS,
//                capacity-outer/item-inner counts PERMUTATIONS (ordered sequences).
