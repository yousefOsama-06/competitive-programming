template<typename T>
struct FloydWarshall {
    const T INF = numeric_limits<T>::max() / 2; // Prevent overflow on addition
    int n;
    vector<vector<T>> dist;

    FloydWarshall(int n) : n(n), dist(n + 1, vector<T>(n + 1, INF)) {
        for (int i = 0; i <= n; ++i) dist[i][i] = 0;
    }

    void add_edge(int u, int v, T w, bool directed = false) {
        dist[u][v] = min(dist[u][v], w);
        if (!directed) dist[v][u] = min(dist[v][u], w);
    }

    void solve() {
        for (int k = 1; k <= n; ++k) {
            for (int i = 1; i <= n; ++i) {
                for (int j = 1; j <= n; ++j) {
                    if (dist[i][k] < INF && dist[k][j] < INF) {
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                    }
                }
            }
        }
    }
};
