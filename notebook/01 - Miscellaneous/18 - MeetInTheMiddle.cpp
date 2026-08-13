// MEET IN THE MIDDLE - n <= 40. Split into halves, enumerate 2^(n/2) subsets each side,
// then recombine by sorting + binary search / two pointers. O(2^(n/2) * n).

// All subset sums of a half, as a sorted vector.
vector<ll> subsetSums(const vector<ll>& a) {
    int n = a.size();
    vector<ll> s(1 << n, 0);
    for (int m = 1; m < (1 << n); m++) {
        int b = __builtin_ctz(m);
        s[m] = s[m ^ (1 << b)] + a[b];
    }
    return s;
}
// #subsets with sum <= S. Works with NEGATIVE values too (no pre-filter on the left half).
ll countAtMost(vector<ll> a, ll S) {
    int n = a.size(), h = n / 2;
    vector<ll> L(a.begin(), a.begin() + h), R(a.begin() + h, a.end());
    auto ls = subsetSums(L), rs = subsetSums(R);
    sort(all(rs));
    ll c = 0;
    for (ll x : ls) c += upper_bound(all(rs), S - x) - rs.begin();
    return c;
}
// Largest achievable sum <= S (the classic "closest subset sum from below").
ll bestAtMost(vector<ll> a, ll S) {
    int n = a.size(), h = n / 2;
    vector<ll> L(a.begin(), a.begin() + h), R(a.begin() + h, a.end());
    auto ls = subsetSums(L), rs = subsetSums(R);
    sort(all(rs));
    ll best = LLONG_MIN;
    for (ll x : ls) {                                  // no `x <= S` filter: x may be > S
        auto it = upper_bound(all(rs), S - x);         // and still pair with a negative right sum
        if (it != rs.begin()) best = max(best, x + *prev(it));
    }
    return best;
}
// OTHER SHAPES
//  - EXACT target: put one half in a hash map, look up (target - x) from the other.
//  - 4-SUM: split the four arrays 2+2, hash all pairwise sums.
//  - "choose exactly k items": bucket each half's subsets BY POPCOUNT, then pair bucket i with
//    bucket k-i (sort each bucket once).
//  - Optimising two objectives: sort one half by objective A and take a prefix maximum of
//    objective B, so each query is one binary search.
// If n <= 20 a plain 2^n loop is simpler; meet in the middle earns its keep from n = 30 upward.
