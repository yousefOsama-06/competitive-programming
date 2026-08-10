// Bridges (Cut Edges) in Undirected Graph - O(V + E)
struct Bridges {
    int n, timer;
    vector<vector<int>> adj;
    vector<int> tin, low;
    vector<pair<int, int>> bridges;

    Bridges(int n = 0) : n(n), timer(0), adj(n + 1), tin(n + 1, 0), low(n + 1, 0) {}

    void add_edge(int u, int v) {
        adj[u].pb(v);
        adj[v].pb(u);
    }

    void dfs(int u, int p = 0) {
        tin[u] = low[u] = ++timer;
        for (int v : adj[u]) {
            if (v == p) continue;
            if (tin[v]) {
                low[u] = min(low[u], tin[v]);
            } else {
                dfs(v, u);
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) bridges.eb(u, v);
            }
        }
    }

    void solve() {
        timer = 0;
        fill(all(tin), 0);
        bridges.clear();
        for (int i = 1; i <= n; i++) {
            if (!tin[i]) dfs(i);
        }
    }
};