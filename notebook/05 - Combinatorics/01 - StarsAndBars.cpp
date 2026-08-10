// Stars and Bars (Number of Solutions to x_1 + x_2 + ... + x_k = n) - O(1)
// 1. Non-negative integer solutions (x_i >= 0): C(n + k - 1, k - 1)
// 2. Positive integer solutions (x_i >= 1): C(n - 1, k - 1)
struct StarsAndBars {
    // Requires precomputed Combination C(n, k)
    static ll count_non_negative(ll n, ll k, function<ll(ll, ll)> nCr) {
        if (n < 0 || k <= 0) return 0;
        return nCr(n + k - 1, k - 1);
    }

    static ll count_positive(ll n, ll k, function<ll(ll, ll)> nCr) {
        if (n < k || k <= 0) return 0;
        return nCr(n - 1, k - 1);
    }
};
