template<typename T>
struct Dijkstra {
    const T INF = numeric_limits<T>::max();
    int n;
    vector<vector<pair<int, T>>> adj;
    vector<T> dist;
    vector<int> parent;

    Dijkstra(int n) : n(n), adj(n + 1), dist(n + 1, INF), parent(n + 1, -1) {}

    void add_edge(int u, int v, T w, bool directed = false) {
        adj[u].push_back({v, w});
        if (!directed) adj[v].push_back({u, w});
    }

    void solve(int source) {
        fill(dist.begin(), dist.end(), INF);
        fill(parent.begin(), parent.end(), -1);
        priority_queue<pair<T, int>, vector<pair<T, int>>, greater<pair<T, int>>> pq;
        
        dist[source] = 0;
        pq.push({0, source});

        while (!pq.empty()) {
            auto [d, u] = pq.top();
            pq.pop();

            if (d > dist[u]) continue;

            for (auto& [v, w] : adj[u]) {
                if (dist[u] + w < dist[v]) {
                    dist[v] = dist[u] + w;
                    parent[v] = u;
                    pq.push({dist[v], v});
                }
            }
        }
    }
};
