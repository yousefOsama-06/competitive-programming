// Alien's Trick (WNS / Penalty Binary Search) - Time: O(N log(PENALTY_RANGE))
// Solves exact K-element selection / partition DP when cost function f(k) is strictly convex
// Adds a penalty lambda per selected item to drop the k constraint
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
