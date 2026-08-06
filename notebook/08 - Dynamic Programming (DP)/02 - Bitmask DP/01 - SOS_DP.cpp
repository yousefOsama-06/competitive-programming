// Computes the sum of all subsets for every bitmask in O(N * 2^N)
vector<long long> SOS_DP(int n_bits, const vector<long long>& a) {
    int max_mask = 1 << n_bits;
    vector<long long> dp = a; // dp[mask] initially holds exactly the value for mask
    
    for (int i = 0; i < n_bits; ++i) {
        for (int mask = 0; mask < max_mask; ++mask) {
            if (mask & (1 << i)) {
                dp[mask] += dp[mask ^ (1 << i)];
            }
        }
    }
    return dp; // dp[mask] now contains sum of all submasks of mask
}
