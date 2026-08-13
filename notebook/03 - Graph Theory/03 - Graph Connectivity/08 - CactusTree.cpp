// CACTUS -> TREE. A CACTUS is a connected graph in which every EDGE lies on at most one simple
// cycle (equivalently m <= 3(n-1)/2 and the cycles are edge-disjoint). O(n + m), 0-indexed, no
// self loops and no parallel edges.
// Build a tree on n + (number of cycles) nodes: node n + i represents the i-th cycle and is joined
// to every vertex of that cycle; every BRIDGE stays a tree edge. Then path/subtree queries on the
// cactus become ordinary tree queries (LCA, HLD, rerooting), which is the whole point.
// Reach for it whenever the input says "each edge belongs to at most one cycle" or "n vertices and
// n edges" (a single cycle with trees hanging off - a "functional graph" shape) and the question
// is about paths, distances, or counting: "number of simple paths between u and v" (2 per cycle
// the path enters and leaves through two different vertices), "shortest path in a cactus", "how
// many vertices can a path from u to v visit".
// This is really the BLOCK-CUT TREE (03) specialised: on a cactus every block is a single edge or
// a single cycle, so the block nodes ARE the cycle nodes and the construction is much shorter.
struct CactusTree {
    int n, cycles = 0;
    vector<vector<int>> tree;                                  // size n + cycles after build()
    vector<char> inCycle;                                      // per EDGE id
    vector<int> par, parE, state;
    vector<vector<pair<int, int>>> g;                          // g[u] = {neighbour, edge id}
    CactusTree(int n) : n(n), par(n, -1), parE(n, -1), state(n, 0), g(n) {}
    void addEdge(int u, int v) {
        int id = inCycle.size();
        inCycle.push_back(0), g[u].push_back({v, id}), g[v].push_back({u, id});
    }
    void dfs(int u, int p) {
        state[u] = 1, par[u] = p;
        for (auto [v, id] : g[u]) {
            if (v == p) continue;
            if (!state[v]) parE[v] = id, dfs(v, u);
            else if (state[v] == 1) {                          // back edge: close a cycle
                int c = n + cycles++;
                tree.push_back({});
                for (int x = u, e = parE[u]; x != v; x = par[x], e = parE[x])
                    tree[x].push_back(c), tree[c].push_back(x), inCycle[e] = 1;
                tree[v].push_back(c), tree[c].push_back(v), inCycle[id] = 1;
            }
        }
        state[u] = 2;
    }
    void build() {
        tree.assign(n, {});
        for (int i = 0; i < n; i++) if (!state[i]) dfs(i, -1);
        for (int u = 0; u < n; u++) for (auto [v, id] : g[u])   // the bridges stay as tree edges
            if (!inCycle[id]) tree[u].push_back(v);
    }
};
// The tree is BIPARTITE between real vertices and cycle nodes only in the cycle parts; a bridge
// joins two real vertices directly, so do not assume alternation.
// NUMBER OF SIMPLE u-v PATHS = 2^(number of CYCLE NODES on the u-v tree path), because inside a
// cycle you may go either way round. That is the standard use of this decomposition.
// SHORTEST PATH IN A CACTUS: on the tree path, a bridge contributes its weight and a cycle node
// contributes min(clockwise, anticlockwise) between its two attachment points - store each
// vertex's position and prefix sum along its cycle to get that in O(1).
// VERTEX CACTUS (every VERTEX in at most one cycle) is a strictly smaller class and the same code
// builds its tree.
// IF THE GRAPH IS NOT A CACTUS this silently produces nonsense - verify m and the block sizes, or
// just use the block-cut tree, which is correct on any graph.
