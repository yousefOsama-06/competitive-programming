int n;
vector<int> adj[N];

bool vis[N];
int in[N], low[N];
int timer;

bool cutPoint[N];

void dfs(int u, int p = -1) {
    vis[u] = true;
    in[u] = low[u] = timer++;
    int children = 0;
    for (int v: adj[u]) {
        if (v == p) continue;
        if (vis[v]) {
            low[u] = min(low[u], in[v]);
        } else {
            dfs(v, u);
            low[u] = min(low[u], low[v]);
            if (low[v] >= in[u] && p != -1) {
                /// u is a Cut Point
                cutPoint[u] = true;
            }
            children++;
        }
    }
    if (p == -1 && children > 1) {
        /// u is a Cut Point
        cutPoint[u] = true;
    }
}

bool connected = true;

void find_cut_points() {
    timer = 0;
    fill(vis, vis + n + 1, false);
    for (int i = 1; i <= n; ++i) {
        if (!vis[i]) {
            if (i != 1)
                connected = false;
            dfs(i);
        }
    }
}