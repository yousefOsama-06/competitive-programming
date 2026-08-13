// HUNGARIAN ALGORITHM - minimum-cost perfect matching on a DENSE bipartite graph, O(n^2 m).
// a is 1-indexed: a[1..n][1..m] with n <= m. Returns the cost; ans[j] = row matched to column j.
// For MAXIMUM cost, negate the matrix.
pair<ll, vector<int>> hungarian(const vector<vector<ll>>& a) {
    int n = a.size() - 1, m = a[0].size() - 1;
    vector<ll> u(n + 1, 0), v(m + 1, 0);
    vector<int> p(m + 1, 0), way(m + 1, 0);
    for (int i = 1; i <= n; i++) {
        p[0] = i;
        int j0 = 0;
        vector<ll> minv(m + 1, llinf);
        vector<char> used(m + 1, false);
        do {
            used[j0] = true;
            int i0 = p[j0], j1 = 0;
            ll delta = llinf;
            for (int j = 1; j <= m; j++) if (!used[j]) {
                ll cur = a[i0][j] - u[i0] - v[j];
                if (cur < minv[j]) minv[j] = cur, way[j] = j0;
                if (minv[j] < delta) delta = minv[j], j1 = j;
            }
            for (int j = 0; j <= m; j++)
                if (used[j]) u[p[j]] += delta, v[j] -= delta;
                else minv[j] -= delta;
            j0 = j1;
        } while (p[j0] != 0);
        do { int j1 = way[j0]; p[j0] = p[j1], j0 = j1; } while (j0);
    }
    vector<int> ans(m + 1);
    for (int j = 1; j <= m; j++) ans[j] = p[j];
    return {-v[0], ans};                             // -v[0] is the optimal total cost
}
// Use it for: assignment problems, "match every task to a worker minimising total time",
// minimum-cost perfect matching in a complete bipartite graph.
// FORBIDDEN PAIRS: use a large finite cost (1e15), NOT llinf - the reduced cost a[i][j]-u[i]-v[j]
// would overflow.  ans[j] == 0 means column j is unmatched (possible only when n < m).
// WHICH ONE: on a DENSE / complete graph this beats MCMF - n^2*m here versus n augmentations of
// Dijkstra over n^2 arcs (n^3 log n) there. Use MCMF only when the graph is genuinely SPARSE.
