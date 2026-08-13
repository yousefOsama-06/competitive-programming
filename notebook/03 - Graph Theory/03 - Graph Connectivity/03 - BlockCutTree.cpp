// BLOCK-CUT TREE (vertex biconnected components). O(V + E).
// Builds a forest whose nodes are: the original CUT VERTICES, plus one node per BICONNECTED
// COMPONENT ("block"). Every cut vertex is joined to each block containing it.
// Answers "is there a path u -> v avoiding vertex w", "which blocks does a path cross",
// "articulation structure of the graph" - all become tree queries.
struct BlockCut {
    int n, timer = 0;
    vector<vector<int>> adj, comps, tree;            // comps[i] = vertices of block i
    vector<int> tin, low, stk, id;                   // id[v] = tree node of original vertex v
    vector<bool> isCut;

    BlockCut(int n = 0) : n(n), adj(n), tin(n, -1), low(n), id(n, -1), isCut(n, false) {}
    void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }

    void dfs(int u, int p) {
        tin[u] = low[u] = timer++;
        stk.push_back(u);
        int kids = 0;
        for (int v : adj[u]) {
            if (v == p) { p = -1; continue; }         // skip ONE parent edge (multi-edge safe)
            if (tin[v] != -1) low[u] = min(low[u], tin[v]);
            else {
                dfs(v, u), kids++;
                low[u] = min(low[u], low[v]);
                if (low[v] >= tin[u]) {               // u closes a block
                    isCut[u] = (tin[u] > 0 || kids > 1);
                    comps.push_back({u});
                    while (true) {
                        int w = stk.back(); stk.pop_back();
                        comps.back().push_back(w);
                        if (w == v) break;
                    }
                }
            }
        }
    }
    void build() {
        for (int v = 0; v < n; v++) if (adj[v].empty()) comps.push_back({v});   // isolated vertex
        for (int i = 0; i < n; i++) if (tin[i] == -1) { timer = 0; dfs(i, -1); stk.clear(); }
        tree.assign(comps.size(), {});
        for (int v = 0; v < n; v++) if (isCut[v]) id[v] = tree.size(), tree.push_back({});
        for (int i = 0; i < (int)comps.size(); i++)
            for (int v : comps[i]) {
                if (!isCut[v]) id[v] = i;
                else tree[i].push_back(id[v]), tree[id[v]].push_back(i);
            }
    }
};
// A vertex that is NOT a cut vertex lies in exactly one block -> id[v] is that block.
// An edge (u,v) always lies in exactly one block. Leaves of the tree are blocks; a graph is
// biconnected iff the tree has a single node.
