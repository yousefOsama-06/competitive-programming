// LowLink: BRIDGES + ARTICULATION POINTS + 2-EDGE-CONNECTED COMPONENTS - O(V + E)
// Skips the parent by EDGE ID, so MULTI-EDGES are handled correctly
// (a doubled edge is never a bridge). 0-based vertices.
// comp[v] = id of v's 2-edge-connected component; contracting those gives the BRIDGE TREE,
// on which "number of bridges between u and v" is just a tree path length.
struct LowLink {
    int n, timer = 0, nc = 0;
    vector<vector<pair<int, int>>> adj;                 // {to, edge id}
    vector<int> tin, low, comp;
    vector<bool> isCut;
    vector<pair<int, int>> bridges;
    vector<int> st;

    LowLink(int n = 0) : n(n), adj(n), tin(n, -1), low(n), comp(n, -1), isCut(n, false) {}
    void add_edge(int u, int v, int id) { adj[u].push_back({v, id}), adj[v].push_back({u, id}); }

    void dfs(int u, int pe) {
        tin[u] = low[u] = timer++;
        st.push_back(u);
        int kids = 0;
        for (auto [v, id] : adj[u]) {
            if (id == pe) continue;                     // skip the edge we came in on, not the vertex
            if (tin[v] != -1) low[u] = min(low[u], tin[v]);
            else {
                dfs(v, id), kids++;
                low[u] = min(low[u], low[v]);
                if (low[v] > tin[u]) bridges.push_back({u, v});
                if (low[v] >= tin[u] && pe != -1) isCut[u] = true;
            }
        }
        if (pe == -1 && kids > 1) isCut[u] = true;
        if (low[u] == tin[u]) {                         // u is the top of a 2-edge-connected comp
            while (true) {
                int v = st.back(); st.pop_back();
                comp[v] = nc;
                if (v == u) break;
            }
            nc++;
        }
    }
    void solve() { for (int i = 0; i < n; i++) if (tin[i] == -1) dfs(i, -1); }

    // Bridge tree: nc nodes, one edge per bridge.
    vector<vector<int>> bridgeTree() {
        vector<vector<int>> t(nc);
        for (auto [u, v] : bridges) t[comp[u]].push_back(comp[v]), t[comp[v]].push_back(comp[u]);
        return t;
    }
};
