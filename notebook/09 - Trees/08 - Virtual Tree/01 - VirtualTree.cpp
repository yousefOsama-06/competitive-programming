// VIRTUAL (AUXILIARY) TREE - compress k marked vertices plus their pairwise LCAs into a tree
// with at most 2k-1 nodes, in O(k log k). Run the real DP on that instead of on all n nodes.
// THE pattern for "q queries, each with k_i marked vertices, sum of k_i bounded".
// Needs tin/tout from a DFS and an LCA (binary lifting or Euler+sparse table).
struct VirtualTree {
    int n;
    vector<int> tin, tout, dep;
    vector<vector<int>> adj, vadj;                    // vadj = the compressed tree
    int timer = 0;

    VirtualTree(int n) : n(n), tin(n), tout(n), dep(n, 0), adj(n), vadj(n) {}
    void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }
    void dfs(int u, int p) {
        tin[u] = timer++;
        for (int v : adj[u]) if (v != p) dep[v] = dep[u] + 1, dfs(v, u);
        tout[u] = timer++;
    }
    bool isAnc(int u, int v) const { return tin[u] <= tin[v] && tout[v] <= tout[u]; }

    // `lca` must be a working LCA callable. Returns the roots' node list; edges land in vadj.
    template <class LCA>
    vector<int> build(vector<int> key, LCA lca) {
        if (key.empty()) return {};
        sort(all(key), [&](int a, int b) { return tin[a] < tin[b]; });
        key.erase(unique(all(key)), key.end());
        int k = key.size();
        for (int i = 0; i + 1 < k; i++) key.push_back(lca(key[i], key[i + 1]));
        sort(all(key), [&](int a, int b) { return tin[a] < tin[b]; });
        key.erase(unique(all(key)), key.end());
        for (int u : key) vadj[u].clear();
        vector<int> st = {key[0]};
        for (size_t i = 1; i < key.size(); i++) {
            int u = key[i];
            while (!isAnc(st.back(), u)) st.pop_back();
            vadj[st.back()].push_back(u);             // parent -> child (directed is enough)
            st.push_back(u);
        }
        return key;                                   // key[0] is the root of the virtual tree
    }
};
// ALWAYS clear vadj for exactly the nodes you used (done above) - never clear all n per query,
// or the total cost becomes O(q*n) and defeats the whole point.
// The compressed edge u -> v represents the real path between them; its "weight" is usually
// dep[v] - dep[u], or a path aggregate you can query in O(1) with prefix sums / HLD.
