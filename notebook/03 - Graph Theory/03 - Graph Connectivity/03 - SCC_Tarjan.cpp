// assuming nodes are zero based
struct SCC {
    vector<vector<int>> adj, adjRev, comps;
    vector<pair<int, int>> edges;
    vector<int> revOut, compOf;
    vector<bool> vis;
    int N;

    void init(int n) {
        N = n;
        adj.resize(n);
        adjRev.resize(n);
        vis.resize(n);
        compOf.resize(n);
    }

    void addEdge(int u, int v) {
        edges.pb(make_pair(u, v));
        adj[u].pb(v);
        adjRev[v].pb(u);
    }

    void dfs1(int u) {
        vis[u] = true;
        for (auto v: adj[u])
            if (!vis[v])
                dfs1(v);
        revOut.pb(u);
    }

    void dfs2(int u) {
        vis[u] = true;
        comps.back().pb(u);
        compOf[u] = comps.size() - 1;
        for (auto v: adjRev[u])
            if (!vis[v])dfs2(v);
    }

    void gen() {
        fill(all(vis), false);
        for (int i = 0; i < N; ++i) {
            if (!vis[i])
                dfs1(i);
        }
        reverse(all(revOut));
        fill(all(vis), false);
        for (auto node: revOut) {
            if (vis[node])continue;
            comps.pb({});
            dfs2(node);
        }
    }

    vector<vector<int>> generateCondensedGraph() {
        vector<vector<int>> adjCon(comps.size());
        for (auto edge: edges)
            if (compOf[edge.st] != compOf[edge.nd])
                adjCon[compOf[edge.st]].pb(compOf[edge.nd]);
        return adjCon;
    }
};
