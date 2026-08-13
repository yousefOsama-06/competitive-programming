// TREE BINARIZATION - rewrite a tree so that EVERY vertex has degree <= 3, by inserting dummy
// vertices joined with ZERO-weight edges. O(n) time, at most n - 2 new vertices, and every
// distance between original vertices is unchanged.
// WHEN: it is a PRECONDITION, not an algorithm. Reach for it whenever a per-vertex cost is
// proportional to the DEGREE and you need it to be O(1):
//   - PERSISTENT centroid decomposition (27): copying a centroid node must be O(1), so a centroid
//     may have at most 3 children in the centroid tree.
//   - "merge the children's DP tables" small-to-large / knapsack-on-tree DPs: after binarization
//     each merge has exactly two operands, which is what makes the classic O(n^2) tree knapsack
//     analysis (and the O(n log n) small-to-large bound) actually hold.
//   - dynamic-tree structures whose node stores a fixed number of child pointers (top trees).
// KEY INVARIANT: replace the star u -> c1, c2, ..., ck by a CATERPILLAR: u keeps c1 and a chain of
// dummies d1 - d2 - ... carries the rest, each dummy holding one real child. Zero-weight edges mean
// distances are preserved exactly; original vertex ids are preserved, so any answer indexed by
// vertex still makes sense. Depth grows by at most O(deg), so a DFS on the result is still O(n).
struct Binarize {
    int n, cnt;                                                // 1..n original, n+1..cnt dummies
    vector<vector<pii>> g;                                     // result: {neighbour, weight}
    Binarize(const vector<vector<pii>>& G, int root = 1)
        : n(G.size() - 1), cnt(G.size() - 1), g(2 * G.size()) { rec(G, root, 0); }
    void link(int u, int v, int w) { g[u].push_back({v, w}), g[v].push_back({u, w}); }
    void rec(const vector<vector<pii>>& G, int u, int p) {
        int last = u, k = 0, deg = G[u].size() - (p != 0);
        for (auto [v, w] : G[u]) {
            if (v == p) continue;
            if (++k == 1 || k == deg) link(last, v, w);         // first and last hang off `last`
            else {
                int d = ++cnt;                                 // dummy carrying one middle child
                link(last, d, 0), link(d, v, w), last = d;
            }
        }
        for (auto [v, w] : G[u]) if (v != p) rec(G, v, u);
    }
};
// SIZE EVERY ARRAY 2n - the vertex count can nearly double.
// WEIGHTS: dummy edges are 0, so shortest paths, diameters and centroid distances are identical.
// If your problem counts EDGES rather than weights, give the real edges weight 1 and the dummies 0.
// VERTEX VALUES: dummies must be neutral for whatever you aggregate (0 for sums, +inf for minima).
// BINARY (degree <= 3) IS THE RIGHT TARGET, not "binary rooted tree": a rooted binarization that
// keeps exactly two children per node needs the same construction plus a root of degree <= 2.
