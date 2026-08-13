// ALIENS TRICK (Lagrangian / wqs binary search) - drops the "exactly k pieces" constraint by charging
// a penalty lambda per piece, solving the unconstrained DP, and binary searching lambda until the
// optimal solution uses k. REQUIRES the optimum as a function of k to be CONVEX; check that first.
// 1. DP State Structure
struct DPState {
    long long cost;
    long long count; // Number of items/segments picked

    // TIE-BREAKER LOGIC:
    // If costs are equal, we maximize the count to handle collinear points on the convex hull.
    // If you are writing a MINIMIZATION problem with min(), change this to:
    // if (cost != other.cost) return cost > other.cost; (since priority queue / max naturally takes the "biggest")
    // Actually, when using standard >, < operators for min/max DP:
    bool operator<(const DPState& other) const {
        if (cost != other.cost) return cost < other.cost;
        return count < other.count; // Always prefer taking MORE items if costs are tied
    }
};

void solve_aliens() {
    int n, k;
    cin >> n >> k;
    // Read ABC 218 H input: A array is of size N-1
    vector<long long> A(n);
    for(int i = 1; i < n; i++) cin >> A[i];
    // If K > N / 2, swap colors so K <= N / 2 (the score is symmetric)
    if (k > n / 2) k = n - k;
    // Construct 1-indexed values array 'a' where a[i] = A[i-1] + A[i]
    vector<long long> a(n + 1);
    for (int i = 1; i <= n; i++) {
        a[i] = A[i - 1] + (i < n ? A[i] : 0);
    }
    // 2. The Unconstrained DP
    // Returns the optimal {cost, count} for a given penalty.
    auto check_dp = [&](long long penalty) -> DPState {
        // dp[i][0]: Max score in prefix i, ending completely outside a segment
        // dp[i][1]: Max score in prefix i, ending inside an active segment
        vector<vector<DPState>> dp(n + 1, vector<DPState>(2, {0, 0}));

        for (int i = 1; i <= n; i++) {
            // Transition 0: We don't pick item i
            dp[i][0] = max(dp[i-1][0], dp[i-1][1]);

            // Transition 1: We pick item i
            // Rule: ONLY apply penalty when STARTING a new pick/segment!
            // Type B logic: To ensure non-adjacency, we must transition from dp[i-1][0].

            // --- IF MAXIMIZING PROFIT ---
            DPState start_new = {dp[i-1][0].cost + a[i] - penalty, dp[i-1][0].count + 1};

            // Replaced max(start_new, continue_old) with just start_new
            dp[i][1] = start_new;
        }
        return max(dp[n][0], dp[n][1]);
    };

    // 3. WQS Binary Search
    // The bounds must be large enough to cover the maximum possible penalty.
    // FIXED: Dropped bounds to 2e9 to prevent k * mid_penalty from overflowing long long
    long long low = -2e9 - 7, high = 2e9 + 7;
    long long best_ans = 0;

    while (low <= high) {
        long long mid_penalty = low + (high - low) / 2;

        DPState res = check_dp(mid_penalty);

        // We want EXACTLY k items.
        // If we picked >= k items, it means the penalty wasn't harsh enough to stop us.
        // Even if res.count > k, we save the answer because of collinear points on the hull.
        if (res.count >= k) {

            // --- IF MAXIMIZING PROFIT ---
            // We subtracted the penalty K times, so we add it back.
            best_ans = res.cost + (1LL * k * mid_penalty);

            // --- IF MINIMIZING COST ---
            // best_ans = res.cost - (1LL * k * mid_penalty);

            // Penalty was too weak, increase it to force picking fewer items next time
            low = mid_penalty + 1;
        } else {
            // We picked too few items. The penalty was too harsh.
            high = mid_penalty - 1;
        }
    }

    // Output the final answer
    cout << best_ans << "\n";
}
