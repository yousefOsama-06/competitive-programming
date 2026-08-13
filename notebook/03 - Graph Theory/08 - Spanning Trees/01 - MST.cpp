// Needs: DSU (02 - Data Structures/05 - DSU/01).
// MINIMUM SPANNING TREE. Kruskal O(m log m) - use on sparse graphs / when you also want the
// KRUSKAL RECONSTRUCTION TREE. Prim O(n^2) - use on dense or IMPLICIT complete graphs.
struct Edge { int u, v; ll w; bool operator<(const Edge& o) const { return w < o.w; } };

ll kruskal(int n, vector<Edge> e, vector<Edge>* used = nullptr) {
    sort(all(e));
    DSU d(n);
    ll tot = 0;
    for (auto& x : e) if (d.join(x.u, x.v)) { tot += x.w; if (used) used->push_back(x); }
    return tot;                                  // check d.size(0) == n for connectivity
}
// Dense / implicit graphs: cost(i,j) given by a function. O(n^2), no edge list needed.
template <class F>
ll prim(int n, F cost) {
    vector<ll> best(n, llinf);
    vector<bool> in(n, false);
    best[0] = 0;
    ll tot = 0;
    for (int it = 0; it < n; it++) {
        int u = -1;
        for (int i = 0; i < n; i++) if (!in[i] && (u < 0 || best[i] < best[u])) u = i;
        in[u] = true, tot += best[u];
        for (int v = 0; v < n; v++) if (!in[v]) best[v] = min(best[v], cost(u, v));
    }
    return tot;
}
// KRUSKAL RECONSTRUCTION TREE: binary tree with 2n-1 nodes; internal node = merging edge weight.
// Then min possible MAX edge on a path u->v = val[lca(u,v)], and "all vertices reachable using
// edges of weight <= w" is exactly a subtree. Turns "binary search the threshold + DSU" into O(1).
struct KRT {
    int n, cnt;
    vector<ll> val;
    vector<array<int, 2>> ch;
    KRT(int n, vector<Edge> e) : n(n), cnt(n), val(2 * n, 0), ch(2 * n, {-1, -1}) {
        sort(all(e));
        DSU d(2 * n);
        vector<int> top(2 * n);
        iota(all(top), 0);
        for (auto& x : e) {
            int a = top[d.find(x.u)], b = top[d.find(x.v)];
            if (a == b) continue;
            val[cnt] = x.w, ch[cnt] = {a, b};
            d.join(x.u, x.v);
            top[d.find(x.u)] = cnt++;
        }
    }                                            // root = cnt-1 (if connected); then build LCA on it
};
// SECOND-BEST MST: build the MST, then for each non-tree edge (u,v,w) the answer candidate is
// mst - maxEdgeOnPath(u,v) + w. Get maxEdgeOnPath with binary lifting or the KRT.
