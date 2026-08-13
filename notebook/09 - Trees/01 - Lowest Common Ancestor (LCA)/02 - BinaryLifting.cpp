// BINARY LIFTING - k-th ancestor, LCA, and "jump while a predicate holds", all O(log n).
// This is NOT only a tree tool: the same table jumps 2^k steps in ANY functional graph
// (i -> f(i)), which is how you do "apply the operation 1e18 times", permutation powers,
// and "min lamps to cover [L,R]" (jump by farthest-reach).
struct Lift {
    int n, B;
    vector<vector<int>> up;                     // up[k][v] = 2^k-th ancestor (-1 above the root)
    vector<int> d;

    template <class G>
    Lift(int n, const G& adj, int root = 0) : n(n), B(1), d(n, 0) {
        while ((1 << B) < n) B++;
        B++;
        up.assign(B, vector<int>(n, -1));
        dfs(root, -1, adj);
        for (int k = 1; k < B; k++)
            for (int v = 0; v < n; v++)
                up[k][v] = up[k - 1][v] < 0 ? -1 : up[k - 1][up[k - 1][v]];
    }
    template <class G>
    void dfs(int u, int p, const G& adj) {
        up[0][u] = p;
        for (int v : adj[u]) if (v != p) d[v] = d[u] + 1, dfs(v, u, adj);
    }
    int kth(int v, ll k) const {                // k-th ancestor, -1 if it does not exist
        for (int i = 0; i < B && v >= 0; i++) if (k >> i & 1) v = up[i][v];
        return k >> B ? -1 : v;
    }
    int lca(int u, int v) const {
        if (d[u] < d[v]) swap(u, v);
        u = kth(u, d[u] - d[v]);
        if (u == v) return u;
        for (int i = B - 1; i >= 0; i--) if (up[i][u] != up[i][v]) u = up[i][u], v = up[i][v];
        return up[0][u];
    }
    int dist(int u, int v) const { return d[u] + d[v] - 2 * d[lca(u, v)]; }
    int onPath(int u, int v, int k) const {     // k-th vertex on the path u -> v (k = 0 gives u)
        int a = lca(u, v), du = d[u] - d[a];
        return k <= du ? kth(u, k) : kth(v, d[u] + d[v] - 2 * d[a] - k);
    }
    // highest ancestor of v still satisfying pred (monotone: true near v, false near the root)
    template <class F>
    int jumpWhile(int v, F pred) const {
        for (int i = B - 1; i >= 0; i--) if (up[i][v] >= 0 && pred(up[i][v])) v = up[i][v];
        return v;
    }
};
// STANDALONE version for a functional graph f (no tree needed):
//   up[0][v] = f(v); up[k][v] = up[k-1][up[k-1][v]];  then apply k steps by the bits of k.
