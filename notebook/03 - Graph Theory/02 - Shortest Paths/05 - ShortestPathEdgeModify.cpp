// SHORTEST PATH WITH ONE EDGE CHANGED, answered OFFLINE in O(1) per query after O((n+m) log n).
// Query: "if edge id had weight x instead, what is dist(s,t)?" - each query is independent.
// IDEA. Let ds/dt be the distance arrays from s and to t, P the shortest path, base = ds[t].
//   * Edge NOT on P: the new path either ignores it (base) or crosses it once:
//         min(base, ds[u] + x + dt[v], ds[v] + x + dt[u]).
//   * Edge ON P at position i: either still use it (base - w + x), or avoid it entirely, and
//     "the best s->t path avoiding path-edge i" is precomputed for every i at once:
//     for a non-path edge (u,v,w), the path ds[u] -> u -> v -> dt[v] skips exactly the path edges
//     in the window [L[u], R[v]), where L[x] = last path vertex on some shortest s->x path and
//     R[x] = first path vertex on some shortest x->t path. Sweep i from 0 to k-1 with a heap.
struct SPEdgeModify {
    struct Edge { int u, v; ll w; };
    int n, k;                                                  // k = #edges on the shortest path
    ll base;
    vector<Edge> E;
    vector<ll> ds, dt, best;                                   // best[i] = s->t avoiding path edge i
    vector<int> pos;                                           // pos[e] = index on P, or -1
    SPEdgeModify(int n, vector<Edge> E, int s, int t) : n(n), E(E) {
        int m = E.size();
        vector<vector<pair<int, int>>> adj(n);
        for (int i = 0; i < m; i++)
            adj[E[i].u].push_back({E[i].v, i}), adj[E[i].v].push_back({E[i].u, i});
        vector<int> par(n, -1), pare(n, -1), ordS, ordT;
        auto dij = [&](int src, vector<ll>& d, vector<int>& ord, bool tree) {
            d.assign(n, llinf);
            priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
            d[src] = 0, pq.push({0, src});
            while (!pq.empty()) {
                auto [dd, u] = pq.top(); pq.pop();
                if (dd > d[u]) continue;
                ord.push_back(u);
                for (auto [v, id] : adj[u]) if (d[u] + E[id].w < d[v]) {
                    d[v] = d[u] + E[id].w;
                    if (tree) par[v] = u, pare[v] = id;
                    pq.push({d[v], v});
                }
            }
        };
        dij(s, ds, ordS, true), dij(t, dt, ordT, false);
        base = ds[t];
        vector<int> pv, pe;                                    // the path, s ... t
        for (int c = t; c != s; c = par[c]) pv.push_back(c), pe.push_back(pare[c]);
        pv.push_back(s), reverse(all(pv)), reverse(all(pe));
        k = pe.size();
        pos.assign(m, -1);
        vector<char> onP(n, 0);
        for (int x : pv) onP[x] = 1;
        for (int i = 0; i < k; i++) pos[pe[i]] = i;
        vector<int> L(n, 0), R(n, k);
        for (int i = 0; i <= k; i++) L[pv[i]] = R[pv[i]] = i;
        for (int u : ordS) for (auto [v, id] : adj[u])          // relaxed in shortest-path order
            if (!onP[v] && ds[v] == ds[u] + E[id].w) L[v] = max(L[v], L[u]);
        for (int u : ordT) for (auto [v, id] : adj[u])
            if (!onP[v] && dt[v] == dt[u] + E[id].w) R[v] = min(R[v], R[u]);
        vector<vector<pair<ll, int>>> ev(max(k, 1));            // ev[start] = {cost, end}
        for (int i = 0; i < m; i++) {
            if (pos[i] >= 0) continue;
            auto [u, v, w] = E[i];
            if (L[u] < R[v]) ev[L[u]].push_back({ds[u] + w + dt[v], R[v]});
            if (L[v] < R[u]) ev[L[v]].push_back({ds[v] + w + dt[u], R[u]});
        }
        best.assign(max(k, 1), llinf);
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> sw;
        for (int i = 0; i < k; i++) {
            for (auto& e : ev[i]) sw.push(e);
            while (!sw.empty() && sw.top().second <= i) sw.pop();
            if (!sw.empty()) best[i] = sw.top().first;
        }
    }
    ll query(int id, ll x) const {                             // edge id gets weight x
        auto [u, v, w] = E[id];
        if (pos[id] < 0) return min({base, ds[u] + x + dt[v], ds[v] + x + dt[u]});
        return min(base - w + x, best[pos[id]]);
    }
};
// RELATED, same precomputation:
//   * "Does edge e lie on SOME shortest path?"  <=>  ds[u] + w + dt[v] == base (either direction).
//   * "Does e lie on EVERY shortest path?"      <=>  removing it increases the distance; the sweep
//     above answers this for path edges (best[i] > base means path edge i is essential).
//   * REPLACEMENT PATHS (delete one edge, ask the new distance) = query(id, +infinity).
//   * SECOND SHORTEST WALK (may reuse edges): min over edges of ds[u] + w + dt[v] strictly > base.
//   * If instead the whole graph changes per query, you need a fresh Dijkstra - this trick only
//     works because s, t and every other weight stay fixed.
