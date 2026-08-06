int n;
vector<int> adj[N];

bool vis[N];
int in[N], low[N];
int timer;

set<pair<int, int>> bridges;

void dfs(int u, int p = 0) {
    vis[u] = true;
    in[u] = low[u] = timer++;
    for (int v: adj[u]) {
        if (v == p) continue;
        if (vis[v]) {
            low[u] = min(low[u], in[v]);
        } else {
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] > in[u]) {
                /// (u, v) is a Bridge
                bridges.insert({u, v});
                bridges.insert({v, u});
            }
        }
    }
}

void find_bridges() {
    timer = 0;
    fill(vis, vis + n + 1, false);
    for (int i = 1; i <= n; ++i) {
        if (!vis[i])
            dfs(i);
    }
}