struct TopoSort {
    int n;
    vector<vector<int>> adj;
    vector<int> in_degree, order;

    TopoSort(int n) : n(n), adj(n + 1), in_degree(n + 1, 0) {}

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        in_degree[v]++;
    }

    bool solve() {
        queue<int> q;
        for (int i = 1; i <= n; ++i) {
            if (in_degree[i] == 0) q.push(i);
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            order.push_back(u);

            for (int v : adj[u]) {
                if (--in_degree[v] == 0) {
                    q.push(v);
                }
            }
        }
        return order.size() == n; // Returns false if there is a cycle
    }
};
