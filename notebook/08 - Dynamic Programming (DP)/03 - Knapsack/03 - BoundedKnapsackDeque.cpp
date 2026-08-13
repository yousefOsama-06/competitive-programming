// BOUNDED KNAPSACK IN O(n * W) - item i may be used at most cnt[i] times. The binary-grouping
// version in 02 - KnapsackFamily.cpp is O(W * sum log cnt), which is the one to write by default;
// reach for this when sum log cnt is too big (huge counts, or W already at the limit).
// THE REWRITE: fix item i and split the capacities by RESIDUE mod w[i]. Along one residue class,
//     dp'[t] = max over k in [0, cnt] of dp[t - k] + k * val
//            = ( max over j in [t - cnt, t] of dp[j] - j * val ) + t * val
// which is a SLIDING WINDOW MAXIMUM of width cnt + 1 - a monotone deque, O(1) amortised per cell.
// PRECONDITION: none beyond w[i] >= 1; this is an exact rewrite, not an approximation. (The
// subtracted term j * val is what makes the window comparable - do not forget to add it back.)
ll boundedKnapsackDeque(const vector<int>& w, const vector<ll>& val,
                       const vector<int>& cnt, int W) {
    int n = w.size();
    vector<ll> dp(W + 1, 0), nd(W + 1, 0);
    for (int i = 0; i < n; i++) {
        int wi = w[i], m = cnt[i];
        ll v = val[i];
        if (wi <= 0 || m <= 0) continue;                       // weight 0 items: add m * v to all
        if (m > W / wi) m = W / wi;                            // more copies than fit
        for (int s = 0; s < wi && s <= W; s++) {
            deque<pair<int, ll>> dq;                           // {t, dp[s + t*wi] - t*v}
            for (int t = 0; s + (ll)t * wi <= W; t++) {
                ll cur = dp[s + t * wi] - (ll)t * v;
                while (!dq.empty() && dq.back().second <= cur) dq.pop_back();
                dq.push_back({t, cur});
                while (dq.front().first < t - m) dq.pop_front();
                nd[s + t * wi] = dq.front().second + (ll)t * v;
            }
        }
        dp.swap(nd);
    }
    return dp[W];                                              // dp[c] = best with weight <= c
}
/* THE FAMILY, so you pick the right one under time pressure
 cnt[i] = 1                      -> 0/1 knapsack, O(n W)                (02 - KnapsackFamily)
 cnt[i] = infinity               -> unbounded, O(n W), forward loop     (02 - KnapsackFamily)
 general cnt[i], small sum log   -> binary grouping, O(W sum log cnt)   (02 - KnapsackFamily)
 general cnt[i], W large         -> THIS FILE, O(n W)
 FEASIBILITY only (no values)    -> bitset, O(n W / 64)                 (02 - KnapsackFamily)
 feasibility, many equal weights -> O(S sqrt S) by distinct value       (04 - SubsetSumSqrt)
 COUNTING the number of ways with bounded multiplicity: the deque does NOT apply (sums, not
   maxima) - use prefix sums along the same residue classes instead, dp'[t] = P[t] - P[t-cnt-1],
   which is the same O(n W) with the same residue decomposition.
 RECONSTRUCTION: store which t the deque front came from, or re-run the DP layer by layer keeping
   all n rows (O(n W) memory) and walk back.
 IF THE VALUES ARE THE WEIGHTS (subset sum), the reachability bitset beats everything by a factor
   of 64 - check that first, it is one line. */
