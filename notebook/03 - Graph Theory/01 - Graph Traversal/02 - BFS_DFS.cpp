// BFS / DFS / 0-1 BFS / multi-source. 0-based. The bread and butter - keep them short.

// Unweighted shortest path + parent for reconstruction.
pair<vector<int>, vector<int>> bfs(int n, const vector<vector<int>>& adj, vector<int> src) {
    vector<int> d(n, -1), par(n, -1);
    queue<int> q;
    for (int s : src) d[s] = 0, q.push(s);       // MULTI-SOURCE: pass every source at once
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (int v : adj[u]) if (d[v] < 0) d[v] = d[u] + 1, par[v] = u, q.push(v);
    }
    return {d, par};
}
vector<int> pathTo(int t, const vector<int>& par) {
    vector<int> p;
    for (int v = t; v >= 0; v = par[v]) p.push_back(v);
    reverse(all(p));
    return p;
}

// 0-1 BFS: edge weights are only 0 or 1 -> deque instead of a heap, O(V + E).
// Use for "turning costs 1", "breaking one wall is free", "flip at most k edges".
vector<ll> bfs01(int n, const vector<vector<pair<int, int>>>& adj, int s) {
    vector<ll> d(n, llinf);
    deque<int> q;
    d[s] = 0, q.push_back(s);
    while (!q.empty()) {
        int u = q.front(); q.pop_front();
        for (auto [v, w] : adj[u]) if (d[u] + w < d[v]) {
            d[v] = d[u] + w;
            w ? q.push_back(v) : q.push_front(v);
        }
    }
    return d;
}

// Iterative DFS with entry/exit times (avoids stack overflow at n = 2e5).
void dfsIter(int n, const vector<vector<int>>& adj, int root, vector<int>& tin, vector<int>& tout) {
    tin.assign(n, -1), tout.assign(n, -1);
    vector<int> it(n, 0), st = {root};
    int timer = 0;
    tin[root] = timer++;
    while (!st.empty()) {
        int u = st.back();
        if (it[u] < (int)adj[u].size()) {
            int v = adj[u][it[u]++];
            if (tin[v] < 0) tin[v] = timer++, st.push_back(v);
        } else tout[u] = timer++, st.pop_back();
    }
}

// GRID: 4- and 8-neighbour offsets (see 03 - grid.cpp). Encode a cell as r*m+c and reuse the
// graph routines above; add extra state dimensions (mask, parity, keys) by widening the index.
// CYCLE DETECTION, directed: colour DFS (0 white, 1 grey, 2 black); a grey->grey edge closes a
// cycle - walk the recursion stack back to extract it. Undirected: any edge to an already-visited
// non-parent vertex closes one.
