// ONLINE (INCREMENTAL) BRIDGES - maintain the number of bridges while edges are ADDED, in
// O(alpha) amortised per edge. Undirected, 0-indexed, edges may repeat (a doubled edge kills the
// bridge, which is exactly right).
// LowLink (01) recomputes bridges from scratch in O(n + m); use this when the graph grows and you
// must answer after every insertion: "after each new road, how many roads are still critical",
// "the first moment u and v are 2-edge-connected".
// STRUCTURE. Contract every 2-edge-connected component to a node; what is left is the BRIDGE
// FOREST, kept as a rooted forest with parent pointers. Adding (u, v):
//   * same 2ecc                 -> nothing changes;
//   * different trees           -> a new bridge; re-root the smaller tree and hang it under v;
//   * same tree, different 2ecc -> the tree path u..v becomes a cycle: contract it, losing
//                                  (path length) bridges.
// Two DSUs: dsu2ecc = the contracted components, dsuCC = which tree of the forest you are in.
// DELETIONS are NOT supported - that needs offline divide and conquer or a link-cut tree.
struct OnlineBridges {
    vector<int> par, e2, cc, ccSize, stamp;                    // par is in the BRIDGE FOREST
    int bridges = 0, iter = 0;
    OnlineBridges(int n) : par(n, -1), e2(n), cc(n), ccSize(n, 1), stamp(n, 0) {
        iota(all(e2), 0), iota(all(cc), 0);
    }
    int find2(int u) { return u < 0 || e2[u] == u ? u : e2[u] = find2(e2[u]); }
    int findCC(int u) { u = find2(u); return cc[u] == u ? u : cc[u] = findCC(cc[u]); }
    void makeRoot(int u) {                                     // reverse the parent chain above u
        u = find2(u);
        int root = u, child = -1;
        while (u != -1) { int p = find2(par[u]); par[u] = child, cc[u] = root, child = u, u = p; }
        ccSize[root] = ccSize[child];
    }
    void mergePath(int u, int v) {                             // contract the u..v tree path
        ++iter;
        vector<int> pu, pv;
        int lca = -1;
        while (lca < 0) {
            if (u != -1) {
                u = find2(u), pu.push_back(u);
                if (stamp[u] == iter) { lca = u; break; }
                stamp[u] = iter, u = par[u];
            }
            if (v != -1) {
                v = find2(v), pv.push_back(v);
                if (stamp[v] == iter) { lca = v; break; }
                stamp[v] = iter, v = par[v];
            }
        }
        for (auto& p : {pu, pv})
            for (int x : p) { if (x == lca) break; e2[x] = lca, bridges--; }
    }
    void addEdge(int u, int v) {
        u = find2(u), v = find2(v);
        if (u == v) return;                                    // already 2-edge-connected
        int a = findCC(u), b = findCC(v);
        if (a != b) {
            bridges++;
            if (ccSize[a] > ccSize[b]) swap(u, v), swap(a, b);  // re-root the SMALLER tree
            makeRoot(u);
            par[u] = cc[u] = v, ccSize[b] += ccSize[u];
        } else mergePath(u, v);
    }
    bool sameComponent(int u, int v) { return find2(u) == find2(v); }   // 2-edge-connected?
};
// WHY IT IS AMORTISED O(alpha): makeRoot walks the smaller tree, which is the union-by-size
// argument (O(log n) total per vertex), and mergePath destroys one bridge per step.
// ONLINE ARTICULATION POINTS / biconnected components can be maintained the same way, but the
// bookkeeping is much heavier - use offline divide and conquer over time instead.
// OFFLINE DELETIONS: process the queries in reverse so deletions become insertions, or run
// divide-and-conquer on the time axis with a rollback DSU (see 02 - DS/05 - DSU).
