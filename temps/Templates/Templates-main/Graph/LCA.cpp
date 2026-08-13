vector<int> adj[N];
int depth[N], up[N][LOG], n, timer, tin[N], tout[N];


void dfs(int u, int p) {
    tin[u] = timer++;
    for (auto v: adj[u]) {
        if (v == p)continue;
        depth[v] = depth[u] + 1;
        up[v][0] = u;
        dfs(v, u);
    }
    tout[u] = timer - 1;
}

bool isAncestor(int u, int v) {
    return tin[u] <= tin[v] && tout[u] >= tout[v];
}

int LCA(int u, int v) {
    if (depth[u] < depth[v])
        swap(u, v);
    int k = depth[u] - depth[v];
    for (int i = 0; i < LOG; ++i) {
        if ((1 << i) & k) {
            u = up[u][i];
        }
    }
    if (u == v)
        return u;
    for (int i = LOG - 1; i >= 0; --i) {
        if (up[u][i] != up[v][i]) {
            u = up[u][i];
            v = up[v][i];
        }
    }
    return up[u][0];
}

int Kthancestor(int u,int k){
    if(k > depth[u])return 0;
    for (int j = LOG - 1; j >= 0; --j) {
        if(k&(1<<j)){
            u = up[u][j];
        }
    }
    return u;
}

void build() {
    dfs(0, 0);
    for (int j = 1; j < LOG; ++j) {
        for (int i = 0; i < n; ++i) {
            up[i][j] = up[up[i][j - 1]][j - 1];
        }
    }
}
