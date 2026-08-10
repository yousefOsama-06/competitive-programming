// Articulation Points (Cut Vertices) in Undirected Graph - O(V + E)
struct ArticulationPoints {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, low;
    vector<bool> is_cut;

    ArticulationPoints(int n = 0) : n(n), timer(0), adj(n + 1), tin(n + 1, 0), low(n + 1, 0), is_cut(n + 1, false) {}

    void add_edge(int u, int v) {
        adj[u].pb(v);
        adj[v].pb(u);
    }

    void dfs(int u, int p = 0) {
        tin[u] = low[u] = ++timer;
        int children = 0;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u] && p != 0) is_cut[u] = true;
                children++;
            }
        }
        if (p == 0 && children > 1) is_cut[u] = true;
    }

    void solve() {
        timer = 0;
        fill(all(tin), 0);
        fill(all(is_cut), false);
        for (int i = 1; i <= n; i++) {
            if (!tin[i]) dfs(i);
        }
    }
};