// PARTIALLY PERSISTENT DSU - a DSU you may QUERY at any past moment, while only the present can be
// modified. O(log n) per query, O(log n) per union, and only O(n) memory - far cheaper than the
// fully persistent version in 20 - PersistentAndUndo, which costs O(log^2 n) and O(n log n).
// WHEN: "what is the earliest time at which u and v are connected", "how big was u's component
// after the first t edges", offline MST / bottleneck-path questions. Partial persistence is enough
// whenever the timeline is a LINE and not a tree of branching versions - which is nearly always.
// If you also want to UNDO, use 02 - DSURollback; if you want to delete arbitrary edges, use
// 14 - OfflineDynamicConnectivity.
// KEY INVARIANT: NO path compression and union by size, so once a vertex becomes a child it keeps
// that parent FOREVER. Every vertex therefore has a monotone timeline: a list of component sizes
// while it was a root, then (at most) one final "I became a child of p at time t" entry. Walking
// up while the stamp is <= t reconstructs the forest at time t exactly, and the height stays
// O(log n) because of the union by size.
struct PartialDSU {
    vector<vector<pair<int, int>>> par;                        // {-size or parent, timestamp}
    int T = 0;                                                 // number of unions performed
    PartialDSU(int n) : par(n, {{-1, 0}}) {}
    int root(int u, int t) {                                   // representative of u at time t
        auto b = par[u].back();
        return b.fi >= 0 && b.se <= t ? root(b.fi, t) : u;
    }
    bool same(int u, int v, int t) { return root(u, t) == root(v, t); }
    int size(int u, int t) {                                   // |component of u| at time t
        u = root(u, t);
        int lo = 0, hi = par[u].size() - 1, at = 0;
        while (lo <= hi) {                                     // last size entry stamped <= t
            int m = (lo + hi) / 2;
            par[u][m].se <= t ? at = m, lo = m + 1 : hi = m - 1;
        }
        return -par[u][at].fi;
    }
    bool join(int u, int v) {                                  // happens at time ++T
        ++T;
        if ((u = root(u, T)) == (v = root(v, T))) return false;
        if (par[u].back().fi > par[v].back().fi) swap(u, v);   // u is the bigger (more negative)
        par[u].push_back({par[u].back().fi + par[v].back().fi, T});
        par[v].push_back({u, T});
        return true;
    }
    int firstConnected(int u, int v) {                         // earliest time, or -1 if never
        int lo = 1, hi = T, ans = -1;
        while (lo <= hi) {
            int m = (lo + hi) / 2;
            same(u, v, m) ? ans = m, hi = m - 1 : lo = m + 1;
        }
        return ans;
    }
};
// TIME IS THE UNION COUNTER, not the query index: a query "after the first k edges" must first map
// k to the number of SUCCESSFUL unions among those k edges (keep an array while you insert).
// BOTTLENECK PATH / minimax edge: sort the edges by weight, union them in that order, then
// firstConnected(u, v) gives the index of the edge that answers "minimise the maximum edge on a
// path". That is exactly the Kruskal reconstruction tree (03 - Graph/08 - MST) in 25 lines less
// code, at O(log^2 n) per query instead of O(1) - use the KRT if you have many queries.
// OFFLINE ALTERNATIVE: if every query is known up front, PARALLEL BINARY SEARCH (01 - Misc/17)
// answers all of them with one plain DSU and no persistence at all.
