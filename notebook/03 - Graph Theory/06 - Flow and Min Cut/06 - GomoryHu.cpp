// Needs: Dinic (01 - Dinic.cpp).
// GOMORY-HU TREE - ALL-PAIRS MINIMUM CUT from only n-1 max-flow runs.
// The result is a weighted tree on the same vertex set with the property that, for EVERY pair
// (u, v), the minimum u-v cut in the original graph equals the MINIMUM EDGE WEIGHT on the u-v
// path in the tree. (There are at most n-1 distinct min-cut values in any graph - that is the
// whole content of the theorem.)
// Reach for it when a problem asks about min cuts between many pairs: "sum/max/k-th of all
// pairwise min cuts", "order the vertices to maximise the sum of consecutive cuts", "how many
// pairs have min cut >= k". Undirected graphs only.
// Complexity: (n-1) x Dinic. The tree is returned as parent + weight arrays (par[0] = -1).
struct GomoryHu {
    int n;
    vector<array<ll, 3>> edges;                                // {u, v, capacity}, undirected
    vector<int> par;
    vector<ll> w;
    GomoryHu(int n) : n(n), par(n, 0), w(n, 0) {}
    void addEdge(int u, int v, ll c) { edges.push_back({(ll)u, (ll)v, c}); }
    void build() {
        par.assign(n, 0), w.assign(n, 0), par[0] = -1;
        for (int i = 1; i < n; i++) {
            Dinic din(n);
            for (auto [u, v, c] : edges) din.addEdge(u, v, c, c);   // undirected: both directions
            w[i] = din.calc(i, par[i]);
            for (int j = i + 1; j < n; j++)                     // re-parent the same-side vertices
                if (par[j] == par[i] && din.leftOfMinCut(j)) par[j] = i;
        }
    }
    ll minCut(int u, int v) const {                            // min edge on the tree path
        ll best = llinf;
        vector<int> du(n, 0);
        for (int x = u; x != -1; x = par[x]) du[x] = 1;
        vector<int> pv;
        int m = v;
        while (!du[m]) pv.push_back(m), m = par[m];             // m = the meeting vertex
        for (int x = u; x != m; x = par[x]) best = min(best, w[x]);
        for (int x : pv) best = min(best, w[x]);
        return best;
    }
};
// EQUIVALENT-FLOW TREE (Gusfield's variant) is the same n-1 flows without the re-parenting step;
// it gets the min-cut VALUES right but not the actual cuts. The version above is the real
// Gomory-Hu, so the tree edges are genuine cuts.
// GLOBAL MIN CUT is the minimum edge of the whole tree - but Stoer-Wagner (04) is O(V^3) and
// needs no flow at all, so prefer it when that is all you want.
// DIRECTED graphs have no Gomory-Hu tree; you genuinely need O(n^2) flows there.
