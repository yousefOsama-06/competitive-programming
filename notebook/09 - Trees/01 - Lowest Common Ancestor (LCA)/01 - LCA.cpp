// LCA by Euler tour + sparse table: O(n log n) build, O(1) query. 0-based, rooted at `root`.
// For k-th ancestor / "jump while predicate" use BinaryLifting instead (02 - BinaryLifting.cpp).
struct LCA {
    static const int B = 21;
    vector<array<int, 2>> tour;
    vector<array<array<int, 2>, B>> T;
    vector<int> d, in, lg;

    template <class G>
    LCA(int n, const G& adj, int root = 0) : d(n), in(n), lg(2 * n + 1) {
        tour.reserve(2 * n);
        dfs(root, -1, adj);
        int m = tour.size();                    // == 2n - 1
        T.resize(m);
        for (int i = 2; i <= 2 * n; i++) lg[i] = lg[i / 2] + 1;
        for (int i = 0; i < m; i++) T[i][0] = tour[i];
        for (int j = 1; j < B; j++)
            for (int i = 0; i + (1 << j) - 1 < m; i++)
                T[i][j] = min(T[i][j - 1], T[i + (1 << (j - 1))][j - 1]);
    }
    template <class G>
    void dfs(int u, int p, const G& adj) {
        in[u] = tour.size();
        tour.push_back({d[u], u});
        for (int v : adj[u]) if (v != p) {
            d[v] = d[u] + 1;
            dfs(v, u, adj);
            tour.push_back({d[u], u});
        }
    }
    int get(int u, int v) const {
        int l = min(in[u], in[v]), r = max(in[u], in[v]), j = lg[r - l + 1];
        return min(T[l][j], T[r - (1 << j) + 1][j])[1];
    }
    int dist(int u, int v) const { return d[u] + d[v] - 2 * d[get(u, v)]; }
};
