// DINIC MAX FLOW. O(V^2 E) in general, O(E sqrt(V)) on unit-capacity graphs, O(V^2 E) worst case but
// fast in practice. After maxflow, vertices reachable from s in the residual graph form the min cut.
// Scaling (start with big capacities) helps on dense graphs with large capacities.
// DINIC MAX FLOW. O(V^2 E) general, O(E sqrt(E)) with unit capacities, O(E sqrt(V)) for bipartite
// matching. This is the SCALING variant (big capacities first), so it stays fast on wide ranges.
// addEdge(a, b, cap, rcap): pass rcap = cap for an UNDIRECTED edge. Edge::flow() recovers the flow
// on each arc. After calc(s,t), leftOfMinCut(v) is true exactly for the SOURCE side of a minimum
// cut - the cut edges are those from a left vertex to a right vertex. 0-indexed.
struct Dinic {
    struct Edge {
        int to, rev;
        ll c, oc;
        ll flow() { return max(oc - c, 0LL); } // if you need flows
    };

    vector<int> lvl, ptr, q;
    vector<vector<Edge> > adj;

    Dinic(int n) : lvl(n), ptr(n), q(n), adj(n) {
    }

    void addEdge(int a, int b, ll c, ll rcap = 0) {
        adj[a].push_back({b, (int) adj[b].size(), c, c});
        adj[b].push_back({a, (int) adj[a].size() - 1, rcap, rcap});
    }

    ll dfs(int v, int t, ll f) {
        if (v == t || !f) return f;
        for (int &i = ptr[v]; i < (int) adj[v].size(); i++) {
            Edge &e = adj[v][i];
            if (lvl[e.to] == lvl[v] + 1)
                if (ll p = dfs(e.to, t, min(f, e.c))) {
                    e.c -= p, adj[e.to][e.rev].c += p;
                    return p;
                }
        }
        return 0;
    }

    ll calc(int s, int t) {
        ll flow = 0;
        q[0] = s;
        for (int L = 0; L < 31; L++)
            do {
                // 'int L=30' maybe faster for random data
                lvl = ptr = vector<int>((int) q.size());
                int qi = 0, qe = lvl[s] = 1;
                while (qi < qe && !lvl[t]) {
                    int v = q[qi++];
                    for (Edge e: adj[v])
                        if (!lvl[e.to] && e.c >> (30 - L))
                            q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
                }
                while (ll p = dfs(s, t, LLONG_MAX)) flow += p;
            } while (lvl[t]);
        return flow;
    }

    bool leftOfMinCut(int a) { return lvl[a] != 0; }
};