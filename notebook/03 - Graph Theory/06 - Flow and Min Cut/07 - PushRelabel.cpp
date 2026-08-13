// HIGHEST-LABEL PUSH-RELABEL max flow, O(V^2 sqrt(E)), 0-indexed. Same interface as Dinic (01).
// WHEN TO PREFER IT OVER DINIC: dense graphs (V ~ 1000-5000, E ~ V^2/2) and graphs with large
// capacities, where Dinic's O(V^2 E) bound bites - usually 3-10x faster there. On unit-capacity
// or bipartite-matching graphs Dinic is O(E sqrt V) and wins, so keep both.
// It does NOT maintain a flow at intermediate times - only at the end - so it cannot be used for
// the incremental "add capacity and continue" tricks; Dinic can.
// IDEA: give every vertex a HEIGHT and let excess flow downhill (push); a vertex with excess and
// nowhere to go is lifted (relabel). Source starts at height V, so nothing can return to it.
// The GAP HEURISTIC (if no vertex has height h any more, lift everything above h out of range) is
// what makes it fast in practice - it is the `co[]` bookkeeping below, do not drop it.
// addEdge(a, b, cap, rcap): rcap = cap gives an UNDIRECTED edge. After calc, leftOfMinCut(v) marks
// the SOURCE side of a minimum cut, and e.f is the flow on that arc.
struct PushRelabel {
    struct Edge { int to, rev; ll f, c; };
    int n;
    vector<vector<Edge>> g;
    vector<ll> ec;
    vector<int> H, cur, co;
    vector<vector<int>> hs;
    PushRelabel(int n) : n(n), g(n), ec(n), H(n), cur(n), co(2 * n + 1), hs(2 * n + 1) {}
    void addEdge(int a, int b, ll cap, ll rcap = 0) {
        if (a == b) return;
        g[a].push_back({b, (int)g[b].size(), 0, cap});
        g[b].push_back({a, (int)g[a].size() - 1, 0, rcap});
    }
    void addFlow(int u, Edge& e, ll f) {
        Edge& back = g[e.to][e.rev];
        if (!ec[e.to] && f) hs[H[e.to]].push_back(e.to);
        e.f += f, e.c -= f, ec[e.to] += f;
        back.f -= f, back.c += f, ec[u] -= f;
    }
    ll calc(int s, int t) {
        fill(all(H), 0), fill(all(ec), 0), fill(all(co), 0), fill(all(cur), 0);
        for (auto& v : hs) v.clear();
        H[s] = n, ec[t] = 1, co[0] = n - 1;
        for (auto& e : g[s]) addFlow(s, e, e.c);
        for (int hi = 0;;) {
            while (hs[hi].empty()) if (!hi--) return -ec[s];
            int u = hs[hi].back(); hs[hi].pop_back();
            while (ec[u] > 0) {                                // discharge u
                if (cur[u] == (int)g[u].size()) {              // relabel + gap heuristic
                    H[u] = 2 * n;
                    for (int i = 0; i < (int)g[u].size(); i++)
                        if (g[u][i].c && H[u] > H[g[u][i].to] + 1)
                            H[u] = H[g[u][i].to] + 1, cur[u] = i;
                    if (++co[H[u]], !--co[hi] && hi < n)
                        for (int i = 0; i < n; i++)
                            if (hi < H[i] && H[i] < n) --co[H[i]], H[i] = n + 1;
                    hi = H[u];
                } else if (g[u][cur[u]].c && H[u] == H[g[u][cur[u]].to] + 1)
                    addFlow(u, g[u][cur[u]], min(ec[u], g[u][cur[u]].c));
                else ++cur[u];
            }
        }
    }
    bool leftOfMinCut(int a) { return H[a] >= n; }
};
// COMPLEXITY NOTES. FIFO push-relabel is O(V^3); the highest-label rule above is O(V^2 sqrt E).
// The gap heuristic and the global relabel (BFS from t every so often) are the two practical
// speedups; the version here has the gap only, which is already enough for contests.
// TIE-BREAKING WITH DINIC: if you also need the actual min cut EDGES, both give them the same way
// (an edge from a leftOfMinCut vertex to a non-leftOfMinCut one). If you need the flow DECOMPOSED
// into paths, repeatedly DFS from s along positive-flow arcs and subtract the bottleneck.
