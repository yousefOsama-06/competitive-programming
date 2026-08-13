// Needs: Edge, kruskal (01 - MST.cpp).
// SECOND-BEST SPANNING TREE - cheapest spanning tree whose weight is STRICTLY greater than the MST.
// It differs from the MST in exactly one edge: add a non-tree edge (u,v,w), drop the heaviest edge
// on the tree path u->v whose weight is < w. By the cycle property every edge on that path has
// weight <= w, so "heaviest edge < w" is either the path maximum (if it is < w) or the second
// LARGEST DISTINCT value on the path. Binary lifting carries both. O(m log n).
ll secondBestMST(int n, const vector<Edge>& e) {
    vector<Edge> used;
    ll mst = kruskal(n, e, &used);
    if ((int)used.size() != n - 1) return -1;                  // disconnected
    vector<vector<pair<int, ll>>> g(n);
    for (auto& x : used) g[x.u].push_back({x.v, x.w}), g[x.v].push_back({x.u, x.w});
    typedef array<ll, 2> T;                                    // {largest, 2nd largest DISTINCT}
    const T NONE = {LLONG_MIN, LLONG_MIN};
    auto join = [&](T a, T b) {
        array<ll, 4> v = {a[0], a[1], b[0], b[1]};
        sort(v.rbegin(), v.rend());
        T r = {v[0], LLONG_MIN};
        for (ll x : v) if (x < v[0]) { r[1] = x; break; }
        return r;
    };
    int L = 1;
    while ((1 << L) < n) L++;
    vector<vector<int>> up(L + 1, vector<int>(n, 0));
    vector<vector<T>> mx(L + 1, vector<T>(n, NONE));
    vector<int> dep(n, 0), st{0};
    vector<char> vis(n, 0);
    vis[0] = 1;
    while (!st.empty()) {                                      // iterative DFS from 0
        int u = st.back(); st.pop_back();
        for (auto [v, w] : g[u]) if (!vis[v])
            vis[v] = 1, dep[v] = dep[u] + 1, up[0][v] = u, mx[0][v] = {w, LLONG_MIN}, st.push_back(v);
    }
    for (int k = 1; k <= L; k++)
        for (int v = 0; v < n; v++) {
            int m = up[k - 1][v];
            up[k][v] = up[k - 1][m], mx[k][v] = join(mx[k - 1][v], mx[k - 1][m]);
        }
    auto path = [&](int u, int v) {                            // top-2 distinct weights on u->v
        T r = NONE;
        if (dep[u] < dep[v]) swap(u, v);
        for (int k = L; k >= 0; k--) if (dep[up[k][u]] >= dep[v]) r = join(r, mx[k][u]), u = up[k][u];
        if (u == v) return r;
        for (int k = L; k >= 0; k--) if (up[k][u] != up[k][v])
            r = join(join(r, mx[k][u]), mx[k][v]), u = up[k][u], v = up[k][v];
        return join(join(r, mx[0][u]), mx[0][v]);
    };
    ll best = LLONG_MAX;
    for (auto& x : e) {                                        // tree edges give p = {w, MIN} and
        T p = path(x.u, x.v);                                  // fall through to the MIN branch
        ll rem = p[0] < x.w ? p[0] : p[1];
        if (rem != LLONG_MIN) best = min(best, mst - rem + x.w);
    }
    return best == LLONG_MAX ? -1 : best;                       // -1: no second tree exists
}
// K-TH BEST SPANNING TREE: repeat the swap argument with a priority queue over (tree, forced-in /
// forced-out edge sets) - Gabow's algorithm, O(k m log n). Rare; the second-best case is the one
// that actually shows up.
// MINIMUM BOTTLENECK SPANNING TREE = any MST (minimising the maximum edge is implied by Kruskal).
// MST OF A GRAPH PLUS ONE NEW EDGE: add it, find the cycle, drop the cycle maximum. O(n).
// MINIMUM SPANNING TREE WITH DEGREE CONSTRAINT at one vertex r: build the MST of G - r, then add
// the cheapest edge from r into each component, then improve by swaps while degree < k.
