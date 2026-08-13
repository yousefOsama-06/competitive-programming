// THE SEQUENCE (a*k + b) mod m, FOR k = 0..n-1, AS O(sqrt m) ARITHMETIC PROGRESSIONS.
// Each returned block {start, diff, len} lists consecutive terms start, start+diff, ... that do
// NOT wrap past m - so a value-sorted / value-structured query over the whole sequence becomes
// O(sqrt m) range operations instead of n individual ones. This is the constructive form of the
// THREE-DISTANCE THEOREM: the points k*a mod m take at most three distinct gaps.
// USE IT FOR: sum of x^((a k + b) mod m), sum over k of f((a k + b) mod m) for an f with prefix
// sums, min/max of (a k + b) mod m over a range of k, counting terms in a value window when the
// generic O(log) count (01 - Euclidean Algorithm/06) does not fit the shape of the problem.
// PRECONDITIONS for decomposeModAP: gcd(a, m) = 1 and n <= m. The wrapper solveModAP below
// removes both restrictions: gcd is peeled off by rewriting the sequence, and n > m is split
// into floor(n/m) identical full cycles plus a remainder.
// COMPLEXITY: O(sqrt m log m) for the decomposition (the sort dominates).
struct APBlock { ll start, diff, len; };                // start + i*diff for 0 <= i < len
vector<APBlock> decomposeModAP(ll a, ll b, ll m, ll n) {
    vector<APBlock> res;
    ll w = (ll)sqrtl((ld)m) + 1;
    if (n <= w) {                                       // too short to be worth the machinery
        for (ll i = 0; i < n; i++) res.push_back({(a * i + b) % m, 0, 1});
        return res;
    }
    vector<pair<ll, ll>> v(w + 1);                      // (residue, index) for the first w+1 terms
    for (ll i = 0; i <= w; i++) v[i] = {a * i % m, i};
    sort(all(v));
    ll d = 1, h = m;                                    // a*d = +-h (mod m) with h <= m/w, |d| <= w
    for (ll i = 0; i < w; i++) {                        // pigeonhole: two residues within m/w
        ll diff = v[i + 1].first - v[i].first;
        if (diff < h) h = diff, d = v[i + 1].second - v[i].second;
    }
    if (d > 0) {                                        // stepping k by d increases the value by h
        for (ll r = 0; r < d; r++)
            for (ll j = r; j < n;) {
                ll val = (a * j + b) % m;
                ll step = min((m - val - 1) / h + 1, (n - j - 1) / d + 1);
                res.push_back({val, h, step});
                j += step * d;
            }
    } else {                                            // stepping k by -d decreases it by h
        for (ll r = n - 1; r >= n + d; r--)
            for (ll j = r; j >= 0;) {
                ll val = (a * j + b) % m;
                ll step = min((m - val - 1) / h + 1, j / -d + 1);
                res.push_back({val, h, step});
                j += step * d;
            }
    }
    return res;
}
// FULL WRAPPER for arbitrary a, b, m, n: reduce gcd, then full cycles, then one decomposition.
//   g = gcd(a, m) > 1: write b = b1*g + b2 with b2 < g; then (a k + b) mod m
//                      = b2 + g * ((a/g * k + b1) mod (m/g))  - recurse on the smaller modulus.
//   n >= m (after gcd = 1): each residue in [0, m) appears exactly once per cycle, so
//                      floor(n/m) full cycles contribute a constant, and n mod m is left over.
// EXAMPLE (sum of x^((a k + b) mod m) mod p): per block the answer is the geometric-like sum
// x^start * (1 + x^diff + ... + x^(diff*(len-1))), computable in O(log len) by the standard
// "return (x^n, 1 + x + ... + x^(n-1)) together" doubling recursion.
