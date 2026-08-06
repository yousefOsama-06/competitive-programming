struct EulerianPath {
    int n;
    vector<vector<pair<int, int>>> adj; // {neighbor, edge_index}
    vector<bool> used_edge;
    vector<int> in_deg, out_deg;
    vector<int> path;

    EulerianPath(int n, int m) : n(n), adj(n + 1), used_edge(m, false), in_deg(n + 1, 0), out_deg(n + 1, 0) {}

    void add_edge(int u, int v, int edge_idx, bool directed = true) {
        adj[u].push_back({v, edge_idx});
        out_deg[u]++; in_deg[v]++;
        if (!directed) {
            adj[v].push_back({u, edge_idx});
            out_deg[v]++; in_deg[u]++;
        }
    }

    bool solve(int start_node) {
        vector<int> ptr(n + 1, 0);
        stack<int> st;
        st.push(start_node);

        while (!st.empty()) {
            int u = st.top();
            if (ptr[u] < adj[u].size()) {
                auto [v, id] = adj[u][ptr[u]++];
                if (!used_edge[id]) {
                    used_edge[id] = true;
                    st.push(v);
                }
            } else {
                path.push_back(u);
                st.pop();
            }
        }
        reverse(path.begin(), path.end());
        
        // Check if all edges were visited
        for (bool used : used_edge) {
            if (!used) return false; 
        }
        return true;
    }
};
