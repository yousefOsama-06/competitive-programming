// Needs: conv (06 - Math/05 - Convolution/01 - FFT.cpp).
// NUMBER OF PATHS OF EVERY LENGTH IN A TREE - cnt[L] = how many unordered pairs {u, v} have
// dist(u, v) == L, for ALL L at once, in O(n log^2 n). 0-indexed, unweighted tree.
// The naive "for each length, count" is O(n^2); a plain centroid decomposition answers "paths of
// length <= K" in O(n log n) but not all lengths simultaneously. The fix is to convolve the depth
// HISTOGRAMS instead of merging them pairwise: at each centroid, square the histogram of all
// distances and subtract the square of each child component's histogram (inclusion-exclusion for
// the pairs that never actually cross the centroid).
// Reach for it for "for every k, how many pairs are at distance k", "sum over pairs of f(dist)"
// when f is arbitrary, and distance-distribution / diameter-profile problems.
vector<ll> pathLengthCounts(int n, const vector<vector<int>>& adj) {
    vector<ll> cnt(n + 1, 0);
    vector<int> sub_(n, 0), dep;
    vector<char> done(n, 0);
    int tot = 0;
    function<void(int, int)> calcSz = [&](int u, int p) {
        tot++, sub_[u] = 1;
        for (int v : adj[u]) if (v != p && !done[v]) calcSz(v, u), sub_[u] += sub_[v];
    };
    function<int(int, int)> centroid = [&](int u, int p) {
        for (int v : adj[u]) if (v != p && !done[v] && sub_[v] * 2 > tot) return centroid(v, u);
        return u;
    };
    function<void(int, int, int)> gather = [&](int u, int p, int d) {
        dep.push_back(d);
        for (int v : adj[u]) if (v != p && !done[v]) gather(v, u, d + 1);
    };
    function<void(int, int, int, int)> add = [&](int u, int p, int d0, int sign) {
        dep.clear(), gather(u, p, d0);
        vector<ll> h(*max_element(all(dep)) + 1, 0);
        for (int d : dep) h[d]++;
        vector<ll> r = conv(h, h);                             // ordered pairs, diagonal included
        for (int i = 0; i < (int)r.size() && i <= n; i++) cnt[i] += sign * r[i];
    };
    function<void(int, int)> decomp = [&](int u, int p) {
        tot = 0, calcSz(u, p);
        int c = centroid(u, p);
        done[c] = 1;
        add(c, p, 0, 1);                                       // all pairs inside this component
        for (int v : adj[c]) if (!done[v]) add(v, c, 1, -1);    // minus the ones inside one child
        for (int v : adj[c]) if (!done[v]) decomp(v, c);
    };
    if (n) decomp(0, -1);
    cnt[0] = 0;
    for (ll& x : cnt) x /= 2;                                  // ordered -> unordered
    return cnt;                                                // cnt[L] for L = 1..n-1
}
// WEIGHTED trees: the same decomposition works, but the histogram is indexed by weight, so it is
// only practical when the total weight is small. Otherwise sort the distance lists and use two
// pointers - that answers "pairs with dist <= K" but not all lengths at once.
// SUM OVER PAIRS OF f(dist): convolve as above and dot with f. Any f works, which is the whole
// point of getting the full distribution.
// ONE LENGTH ONLY, or "count paths with dist <= K": plain centroid decomposition with sorting,
// O(n log^2 n) and no FFT at all - see 01 - Centroid.
