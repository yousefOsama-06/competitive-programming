// BOUNDED KNAPSACK, FEASIBILITY ONLY, with a bitset: O(n W / 64). Binary-group the counts
// (1,2,4,...) so "cnt copies of w" becomes O(log cnt) shift-ors. Use when you only need which sums
// are reachable, not the best value.
const int MAX_W = 100005;
bitset<MAX_W> dp;

void multiset_bitset_dp() {
    // Example: We have 13 items, each with weight 5
    int count = 13;
    int weight = 5;
    
    vector<int> bundled_weights;
    
    // Binary grouping logic
    int current_power = 1;
    while (count >= current_power) {
        bundled_weights.push_back(current_power * weight);
        count -= current_power;
        current_power *= 2;
    }
    // Add whatever is left over
    if (count > 0) {
        bundled_weights.push_back(count * weight);
    }
    
    // Now run the ultra-fast Bitset DP on the bundles
    dp[0] = 1;
    for (int bundle_w : bundled_weights) {
        dp |= (dp << bundle_w);
    }
}
