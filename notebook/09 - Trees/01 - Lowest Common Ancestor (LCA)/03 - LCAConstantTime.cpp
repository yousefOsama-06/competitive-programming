// LCA IN O(1) after O(n log n) preprocessing - Euler tour + sparse table on DEPTH.
// Binary lifting (02) is O(log n) per query and needs O(n log n) memory too, so reach for this
// when the query count dominates: O(1) LCA turns "distance between every pair in a list" and
// "is u an ancestor of v" from n log n into n. It is also the primitive behind O(1) LCE on a
// suffix tree and behind virtual trees built in bulk.
// The tour has 2n-1 entries; the LCA of u and v is the SHALLOWEST vertex between their first
// occurrences, and since consecutive tour depths differ by exactly +-1 a plain sparse table over
// (depth, vertex) pairs is enough - min over an overlapping range is idempotent.
struct LCA {
    int n, LOG;
    vector<int> first, depth, tour;
    vector<vector<pair<int, int>>> sp;                         // (depth, vertex)
    LCA(int n, const vector<vector<int>>& adj, int root = 0)
        : n(n), first(n, -1), depth(n, 0) {
        tour.reserve(2 * n);
        vector<pair<int, int>> st{{root, -1}};                 // iterative: no stack overflow
        vector<int> it(n, 0);
        depth[root] = 0;
        while (!st.empty()) {
            auto& [u, p] = st.back();
            if (first[u] < 0) first[u] = tour.size();
            if (it[u] < (int)adj[u].size()) {
                int v = adj[u][it[u]++];
                if (v == p) continue;
                tour.push_back(u), depth[v] = depth[u] + 1;
                st.push_back({v, u});
            } else {
                tour.push_back(u);
                st.pop_back();
            }
        }
        int m = tour.size();
        LOG = 1;
        while ((1 << LOG) <= m) LOG++;
        sp.assign(LOG, vector<pair<int, int>>(m));
        for (int i = 0; i < m; i++) sp[0][i] = {depth[tour[i]], tour[i]};
        for (int k = 1; k < LOG; k++)
            for (int i = 0; i + (1 << k) <= m; i++)
                sp[k][i] = min(sp[k - 1][i], sp[k - 1][i + (1 << (k - 1))]);
    }
    int lca(int u, int v) const {
        int a = first[u], b = first[v];
        if (a > b) swap(a, b);
        int k = 31 - __builtin_clz(b - a + 1);
        return min(sp[k][a], sp[k][b - (1 << k) + 1]).second;
    }
    int dist(int u, int v) const { return depth[u] + depth[v] - 2 * depth[lca(u, v)]; }
    bool isAncestor(int u, int v) const { return lca(u, v) == u; }
};
// TRUE O(n) PREPROCESSING is possible (Farach-Colton-Bender): split the +-1 depth array into
// blocks of size log(n)/2, precompute every possible block pattern (there are only sqrt(n) of
// them), and put a sparse table over the block minima. It is ~60 lines and only worth it at
// n >= 5e5, where the O(n log n) table's memory is the real problem.

// --- SUBTREE HASHING: are these two subtrees the same shape? O(n log n). ---
// Canonical form: sort the children's ids and look the sorted vector up in a map. Two subtrees
// get the same id iff they are isomorphic as ROOTED trees.
// Shorter than AHU (09 - Isomorphism) and merges naturally into other tree DPs; AHU is still the
// one to use when you need the UNROOTED comparison (root at the centres and take the minimum).
map<vector<int>, int> subtreeId;
vector<int> subHash;
int hashSubtree(int u, int p, const vector<vector<int>>& adj) {
    vector<int> ch;
    for (int v : adj[u]) if (v != p) ch.push_back(hashSubtree(v, u, adj));
    sort(all(ch));
    auto it = subtreeId.find(ch);
    if (it == subtreeId.end()) it = subtreeId.emplace(ch, subtreeId.size()).first;
    return subHash[u] = it->second;
}
// RANDOMISED VARIANT, O(n) and mergeable: h(u) = XOR over children of f(h(child)) with f a fixed
// random 64-bit mixing function (splitmix64). No map, no sorting, and it composes under rerooting
// - which is what you want when you need the hash of every subtree AND of every "rest of tree".

// --- MERGING TWO TREE PATHS. A segment tree node stores the MINIMAL PATH CONTAINING all the
// vertices in its range, as that path's two endpoints ({-1,-1} = empty, {-2,-2} = they do not fit
// on any path). Merging is O(1) given an O(1) LCA: the covering path of two paths, if it exists,
// has its endpoints among the four given ones, so test each of the four candidate pairs (u, v)
// with dist(u,v) == dist(u,w) + dist(w,v) for every endpoint w.
// NOTE THE SEMANTIC: this is the smallest path CONTAINING both, not their union - two disjoint
// pieces of one long path merge into that long path, which is what a covering query wants.
// USES: "is this set of vertices contained in one path", "the smallest path covering a range of
// queries", and segment-tree-over-queries problems on trees.
struct PathNode { int x = -1, y = -1; };
PathNode mergePaths(PathNode a, PathNode b, const LCA& L) {
    if (a.x < 0) return b;
    if (b.x < 0) return a;
    int e[4] = {a.x, a.y, b.x, b.y};                           // ALL SIX pairs, not just four:
    for (int i = 0; i < 4; i++) for (int j = i; j < 4; j++) {   // (a.x,b.x) and (a.y,b.y) are
        int u = e[i], v = e[j];                                 // spanning pairs too
        bool ok = true;
        for (int w : e) if (L.dist(u, v) != L.dist(u, w) + L.dist(w, v)) ok = false;
        if (ok) return {u, v};
    }
    return {-2, -2};                                           // not a path: mark it invalid
}
