// Needs: DSURollback (02 - DSU/02 - DSURollback.cpp).
// OFFLINE DYNAMIC CONNECTIVITY - edges appear and disappear over time; answer queries at each
// moment. Segment tree over the TIMELINE + DSU with rollback. O(q log q * log n) - DSURollback has no path compression, so find is log n, not alpha.
// The same skeleton answers ANY "structure supports add + rollback, but not delete" problem:
// put each element on the time interval where it is alive, DFS the segment tree, add on entry,
// roll back on exit.  ("Deleting from a data structure in O(T log n)")
struct DynConn {
    int n, T;
    vector<vector<pair<int, int>>> seg;              // edges alive on each timeline node
    DSURollback d;
    vector<int> comps;                               // comps[t] = #components at time t

    DynConn(int n, int T) : n(n), T(T), seg(4 * max(1, T)), d(n), comps(T, 0) {}

    void addSeg(int v, int l, int r, int ql, int qr, pair<int, int> e) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) { seg[v].push_back(e); return; }
        int m = (l + r) / 2;
        addSeg(2 * v, l, m, ql, qr, e), addSeg(2 * v + 1, m + 1, r, ql, qr, e);
    }
    // edge (u,v) exists during times [tl, tr] (inclusive)
    void addEdge(int u, int v, int tl, int tr) { if (tl <= tr) addSeg(1, 0, T - 1, tl, tr, {u, v}); }

    void dfs(int v, int l, int r, int cur) {
        int snap = d.get_state();
        for (auto [a, b] : seg[v]) cur -= d.join(a, b);
        if (l == r) comps[l] = cur;
        else {
            int m = (l + r) / 2;
            dfs(2 * v, l, m, cur), dfs(2 * v + 1, m + 1, r, cur);
        }
        d.rollback_to(snap);
    }
    void solve() { if (T) dfs(1, 0, T - 1, n); }
};
// USAGE: collect every edge's [birth, death] interval first (an edge that is never removed dies
// at T-1), then one solve() fills comps[]. To answer "are u and v connected at time t", instead
// record the queries per leaf and test d.same(u, v) inside the l == r branch.
