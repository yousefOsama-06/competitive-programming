// MIN-PLUS / MAX-PLUS CONVOLUTION:  c[k] = min_i ( a[i] + b[k-i] ).
// In general this is quadratic and CONJECTURED to stay that way (the MinConv hypothesis:
// MinConv, MaxConv, unbounded knapsack and 0/1 knapsack are all equivalent under subquadratic
// reductions). So the ONLY fast versions rely on convexity.

// BOTH CONVEX -> O(n + m). The difference sequence of the answer is the sorted MERGE of the two
// difference sequences: greedily always take the smaller next slope.
vector<ll> minPlusConvexConvex(const vector<ll>& a, const vector<ll>& b) {
    int n = a.size(), m = b.size();
    vector<ll> c(n + m - 1);
    c[0] = a[0] + b[0];
    int i = 0, j = 0;
    for (int k = 1; k < n + m - 1; k++) {
        ll da = (i + 1 < n) ? a[i + 1] - a[i] : LLONG_MAX;
        ll db = (j + 1 < m) ? b[j + 1] - b[j] : LLONG_MAX;
        if (da <= db) c[k] = c[k - 1] + da, i++;
        else c[k] = c[k - 1] + db, j++;
    }
    return c;
}
// MAX-PLUS with both CONCAVE: same code, take the LARGER slope first (or negate and reuse).
vector<ll> maxPlusConcaveConcave(vector<ll> a, vector<ll> b) {
    for (ll& x : a) x = -x;
    for (ll& x : b) x = -x;
    auto c = minPlusConvexConvex(a, b);
    for (ll& x : c) x = -x;
    return c;
}
// ONLY ONE CONVEX -> the matrix X[k][i] = a[i] + b[k-i] is totally monotone (Monge), so the row
// minima can be found with SMAWK in O(n + m), or with divide & conquer in O(n log n):
//   solve(kl, kr, il, ir): take k = mid, scan i in [il, ir] for the best, recurse with the
//   split at the optimum. Same skeleton as DivideAndConquerDP.
// `valid(k)` must return the legal range of i for output index k - for the convolution above
// that is [max(0, k-m+1), min(n-1, k)]. Intersecting with it is what keeps `best` well defined;
// without it an empty candidate set leaves best = -1 and poisons both recursive calls.
template <class F, class V>
void dcOpt(int kl, int kr, int il, int ir, vector<ll>& c, F cost, V valid) {
    if (kl > kr) return;
    int km = (kl + kr) / 2, best = -1;
    ll bv = LLONG_MAX;
    auto [lo, hi] = valid(km);
    for (int i = max(il, lo); i <= min(ir, hi); i++) {
        ll v = cost(km, i);
        if (v < bv) bv = v, best = i;
    }
    c[km] = bv;
    if (best < 0) best = max(il, lo);                // no candidate: keep the bounds sane
    dcOpt(kl, km - 1, il, best, c, cost, valid);
    dcOpt(km + 1, kr, best, ir, c, cost, valid);
}
// NEITHER CONVEX -> O(n*m) is the best you should plan for, and that is not laziness: min-plus
// convolution, 0/1 knapsack, unbounded knapsack and tree knapsack are subquadratic-EQUIVALENT,
// so a general fast merge would break all four. Look for convexity, a small value range, or a
// bitset instead of a cleverer convolution.
// WHERE THIS APPEARS: merging two "cost as a function of how many you take" curves - group
// knapsack, tree DP where each child contributes a convex cost curve, and combining the
// per-part curves produced by Alien's trick.
