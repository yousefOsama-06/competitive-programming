// Bellman-Ford Shortest Path with Negative Cycle Detection - O(V * E)
struct Edge {
    int u, v;
    ll w;
};

struct BellmanFord {
    int n;
    vector<Edge> edges;
    vector<ll> dist;
    vector<int> par;

    BellmanFord(int n = 0) : n(n), dist(n + 1, llinf), par(n + 1, -1) {}

    void add_edge(int u, int v, ll w) {
        edges.pb({u, v, w});
    }

    // Returns false if a negative cycle is reachable from src
    bool solve(int src) {
        fill(all(dist), llinf);
        fill(all(par), -1);
        dist[src] = 0;

        for (int i = 0; i < n - 1; i++) {
            bool relaxed = false;
            for (const auto& e : edges) {
                if (dist[e.u] < llinf && dist[e.u] + e.w < dist[e.v]) {
                    dist[e.v] = dist[e.u] + e.w;
                    par[e.v] = e.u;
                    relaxed = true;
                }
            }
            if (!relaxed) break;
        }

        for (const auto& e : edges) {
            if (dist[e.u] < llinf && dist[e.u] + e.w < dist[e.v]) {
                return false; // Negative cycle detected
            }
        }
        return true;
    }
};
