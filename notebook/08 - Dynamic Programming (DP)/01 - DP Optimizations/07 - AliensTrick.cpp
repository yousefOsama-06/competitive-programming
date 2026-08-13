// ALIENS TRICK (Lagrangian relaxation / wqs binary search) - O(n log(range)).
// Drops an "exactly k pieces" constraint: charge lambda per piece, solve the UNCONSTRAINED DP,
// and binary search lambda until the optimum uses k pieces. Answer = cost(lambda) - k*lambda.
// PRECONDITION: f(k), the optimum with exactly k pieces, must be CONVEX in k (for a minimum;
// concave for a maximum). NOT "strictly" - strictness is never needed and rarely true.
// THREE THINGS THAT MAKE IT CORRECT, all easy to get wrong:
//  (1) dp_solver MUST return the LARGEST optimal count for that lambda. With f convex, the
//      difference d(k) = f(k) - f(k-1) is non-decreasing; the search below finds the largest
//      lambda with count >= k, and then d(k) <= -lambda <= d(k+1), so k really is optimal at
//      that lambda and cost - k*lambda = f(k). Any other tie-break makes the last line wrong.
//  (2) An integer lambda suffices ONLY if all costs are integers.
//  (3) [min_penalty, max_penalty] must straddle every slope of f; assert it.
// CONVEX families: "k pairwise non-adjacent items", "exactly k segments with a Monge cost",
// "k disjoint intervals", "min-cost flow of value k" (flow cost is always convex in the value),
// matroid-rank-constrained optima. TRAPS: two coupled constraints, or a cost rewarding a parity
// of k. CHECK IT: brute-force f(k) for n <= 60 and assert f(k+1)-f(k) >= f(k)-f(k-1).
struct AliensTrick {
    // Binary searches optimal penalty lambda to choose exactly K items
    // Returns minimum cost to choose exactly K items
    static ll solve(int n, int k, ll min_penalty, ll max_penalty, function<pair<ll, int>(ll)> dp_solver) {
        ll l = min_penalty, r = max_penalty, best_lambda = 0;

        while (l <= r) {
            ll mid = l + (r - l) / 2;
            auto [cost, count] = dp_solver(mid);
            if (count >= k) {
                best_lambda = mid;
                l = mid + 1; // Increase penalty if we used too many items
            } else {
                r = mid - 1; // Decrease penalty if we used too few items
            }
        }

        // True cost = cost_with_penalty - (k * best_lambda)
        auto [cost, count] = dp_solver(best_lambda);
        return cost - (ll)k * best_lambda;
    }
};
