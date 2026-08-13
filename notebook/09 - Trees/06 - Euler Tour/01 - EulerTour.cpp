// Euler Tour Technique (Tree Flattening into 1D Array Intervals) - O(V + E)
// Subtree of node u corresponds to contiguous range [in_time[u], out_time[u]] in flat_tree
struct EulerTour {
    int n, timer = 0;
    vector<vector<int>> adj;
    vector<int> in_time, out_time, flat_tree;

    EulerTour(int n = 0) : n(n), adj(n + 1), in_time(n + 1), out_time(n + 1) {}

    void add_edge(int u, int v) {
        adj[u].pb(v);
        adj[v].pb(u);
    }

    void dfs(int u, int p = -1) {
        in_time[u] = ++timer;
        flat_tree.pb(u);
        for (int v : adj[u]) {
            if (v != p) dfs(v, u);
        }
        out_time[u] = timer;
    }

    void build(int root = 1) {
        timer = 0;
        flat_tree.clear();
        dfs(root);
    }

    // Returns [L, R] 1-based index range for subtree queries on node u
    pair<int, int> get_subtree_range(int u) const {
        return {in_time[u], out_time[u]};
    }
};

/* THE EULER-TOUR TOOLBOX - four different tours, each answering a different query family.
 CONVENTION HERE: in_time is 1-based (in_time[u] = ++timer) but flat_tree is a 0-based vector,
 so flat_tree[in_time[u] - 1] == u. Keep that straight when you index a BIT.

 1) SUBTREE = a contiguous range [in[v], out[v]].
      point update, subtree query  -> BIT over in[]:  add at in[u], query [in[v], out[v]]
      subtree update, point query  -> DIFFERENCE BIT: +x at in[v], -x at out[v]+1,
                                      then the value at u is the prefix sum up to in[u]
 2) PATH TO THE ROOT, using the SAME difference trick in the other direction:
      subtree update, path-to-root query  <-> path update, point query (they are duals).
      PATH UPDATE u..v by +x, point query:  +x at in[u], +x at in[v], -x at in[lca],
                                            -x at in[parent(lca)]
      PATH QUERY u..v, point update:        query(in[u]) + query(in[v]) - 2*query(in[lca])
                                            (+ the LCA's own value if vertices carry weights)
 3) THE 2n TOUR (+1 on entry, -1 on exit) turns ancestor tests and LCA into RMQ:
      u is an ancestor of v  iff  in[u] <= in[v] && out[v] <= out[u]
      LCA(u,v) = the vertex of minimum DEPTH in the tour range [in[u], in[v]] - and consecutive
      depths differ by exactly 1, which is what the O(n)/O(1) +-1 RMQ exploits.
 4) THE EDGE TOUR (each edge appears twice) is what Mo's-on-trees uses: a vertex appearing TWICE
      inside the range cancels, so a path becomes one contiguous range plus the LCA by hand.
      See 02 - Data Structures/06 - Square Root Decomposition/03 - MoOnPaths.cpp.

 EDGE WEIGHTS instead of vertex weights: push each edge's weight onto its DEEPER endpoint, then
 every path formula above drops the LCA term (the LCA's incoming edge is not on the path). */
