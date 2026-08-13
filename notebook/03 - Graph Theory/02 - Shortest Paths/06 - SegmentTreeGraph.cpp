// SEGMENT-TREE GRAPH - add "u -> every vertex in [l, r]" (or the reverse) with O(log n) edges
// instead of O(n). Then run Dijkstra/BFS as usual. Total O((n + q log n) log n).
// Two trees: OUT (parent -> children, cost 0) to broadcast INTO a range,
//            IN  (children -> parent, cost 0) to collect FROM a range.
// Leaves of both trees are the original vertices.
struct SegGraph {
    int n, cnt;
    vector<int> outId, inId;                          // node ids of the two trees
    vector<vector<pair<int, ll>>> g;

    SegGraph(int n) : n(n) {
        cnt = n;
        outId.assign(4 * n, -1), inId.assign(4 * n, -1);
        g.assign(n + 8 * n, {});
        buildOut(1, 0, n - 1), buildIn(1, 0, n - 1);
    }
    int node() { return cnt++; }
    void buildOut(int v, int l, int r) {
        if (l == r) { outId[v] = l; return; }
        outId[v] = node();
        int m = (l + r) / 2;
        buildOut(2 * v, l, m), buildOut(2 * v + 1, m + 1, r);
        g[outId[v]].push_back({outId[2 * v], 0});
        g[outId[v]].push_back({outId[2 * v + 1], 0});
    }
    void buildIn(int v, int l, int r) {
        if (l == r) { inId[v] = l; return; }
        inId[v] = node();
        int m = (l + r) / 2;
        buildIn(2 * v, l, m), buildIn(2 * v + 1, m + 1, r);
        g[inId[2 * v]].push_back({inId[v], 0});
        g[inId[2 * v + 1]].push_back({inId[v], 0});
    }
    void addRange(int v, int l, int r, int ql, int qr, int u, ll w, bool toRange) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr) {
            if (toRange) g[u].push_back({outId[v], w});          // u -> everything in [ql,qr]
            else g[inId[v]].push_back({u, w});                   // everything in [ql,qr] -> u
            return;
        }
        int m = (l + r) / 2;
        addRange(2 * v, l, m, ql, qr, u, w, toRange);
        addRange(2 * v + 1, m + 1, r, ql, qr, u, w, toRange);
    }
    void edgeToRange(int u, int l, int r, ll w) { addRange(1, 0, n - 1, l, r, u, w, true); }
    void edgeFromRange(int l, int r, int u, ll w) { addRange(1, 0, n - 1, l, r, u, w, false); }
    void edge(int u, int v, ll w) { g[u].push_back({v, w}); }

    vector<ll> dijkstra(int s) {
        vector<ll> d(cnt, llinf);
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
        d[s] = 0, pq.push({0, s});
        while (!pq.empty()) {
            auto [dd, u] = pq.top(); pq.pop();
            if (dd > d[u]) continue;
            for (auto [v, w] : g[u]) if (dd + w < d[v]) d[v] = dd + w, pq.push({d[v], v});
        }
        d.resize(n);                                  // only the original vertices matter
        return d;
    }
};
// The same construction gives 2-SAT range clauses and "range -> range" edges (route both
// through one fresh auxiliary node).
