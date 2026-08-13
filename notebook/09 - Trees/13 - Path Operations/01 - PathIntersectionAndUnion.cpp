// Needs: Lift (09 - Trees/01 - Lowest Common Ancestor (LCA)/02 - BinaryLifting.cpp).
// INTERSECTION AND UNION OF TWO TREE PATHS, O(log n) each. 0-indexed, rooted tree.
// A path is the pair of its endpoints {a, b} (unordered); {-1, -1} is the EMPTY path and
// {-2, -2} is "these two paths do not union into a path".
// Reach for it for "how many of these q paths pass through a common vertex", "the longest prefix
// of the queries whose paths all intersect" (the intersection is associative, so put it in a
// segment tree or binary-search it), "merge the constraints u..v must be inside one path", and
// road/route-overlap problems.
// KEY DECOMPOSITION: a path a..b splits at l = lca(a, b) into two VERTICAL chains l..a and l..b.
// The intersection of two paths is the union of the (at most four) pairwise chain intersections,
// and each of those is again a vertical chain that is trivial to compute from depths and lca.
struct PathOps {
    const Lift& L;
    typedef pair<int, int> P;
    PathOps(const Lift& L) : L(L) {}
    bool isAnc(int u, int a) const {                           // is a an ancestor of u (or == u)?
        return L.d[u] >= L.d[a] && L.kth(u, L.d[u] - L.d[a]) == a;
    }
    int len(P p) const { return p.first < 0 ? 0 : L.dist(p.first, p.second) + 1; }   // #vertices
    // The common part of the vertical chains u..a and v..c, where u is an ancestor of a and v of c.
    P chainMeet(int u, int a, int v, int c) const {
        if (!isAnc(a, v)) return {-1, -1};                     // v must lie on the chain u..a
        int x = L.lca(a, c);
        if (L.d[v] < L.d[u]) { if (isAnc(x, u)) return {u, x}; }
        else if (isAnc(x, v)) return {v, x};
        return {-1, -1};
    }
    P joinChains(P x, P y) const {                             // union, ASSUMED to be a path
        if (x.first < 0) return y;
        if (y.first < 0) return x;
        int can[4] = {x.first, x.second, y.first, y.second}, a = can[0], b = -1;
        for (int u : can) if (L.d[u] > L.d[a]) a = u;          // deepest endpoint
        for (int u : can) if (!isAnc(a, u) && (b < 0 || L.d[b] < L.d[u])) b = u;
        if (b < 0) { b = can[0]; for (int u : can) if (L.d[u] < L.d[b]) b = u; }
        return {a, b};
    }
    P intersect(P p, P q) const {                              // {-1,-1} when they are disjoint
        if (p.first < 0 || q.first < 0) return {-1, -1};
        int u = L.lca(p.first, p.second), v = L.lca(q.first, q.second);
        P r1 = joinChains(chainMeet(u, p.first, v, q.first), chainMeet(u, p.first, v, q.second));
        P r2 = joinChains(chainMeet(u, p.second, v, q.first), chainMeet(u, p.second, v, q.second));
        return joinChains(r1, r2);
    }
    P unite(P x, P y) const {                                  // {-2,-2} if the union is not a path
        if (x.first == -2 || y.first == -2) return {-2, -2};
        if (x.first < 0) return y;
        if (y.first < 0) return x;
        int e[4] = {x.first, x.second, y.first, y.second};
        int need = len(x) + len(y) - len(intersect(x, y));     // |union| by inclusion-exclusion
        for (int i = 0; i < 4; i++) for (int j = i; j < 4; j++) {
            P r{e[i], e[j]};                                   // candidate covering path
            bool ok = len(r) == need;                          // no branch and no gap
            for (int w : e)
                if (L.dist(r.first, w) + L.dist(w, r.second) != L.dist(r.first, r.second)) ok = 0;
            if (ok) return r;
        }
        return {-2, -2};
    }
};
// BOTH OPERATIONS ARE ASSOCIATIVE, so they drop straight into a segment tree over an array of
// paths: "the intersection of a range of queries" or "the longest prefix whose union is a path"
// (descend the tree greedily, which is exactly the classic "sort a permutation along a tree" task).
// SMALLEST PATH CONTAINING two given paths is a THIRD operation, and it is not the union - see
// mergePaths in 01 - LCA/03 - LCAConstantTime, which returns the minimal COVERING path.
// DOES VERTEX w LIE ON PATH a..b? dist(a,w) + dist(w,b) == dist(a,b). That one-liner replaces this
// whole file when the query is that simple.
// PATH INTERSECTION LENGTH is len(intersect(p, q)) in VERTICES; subtract one for edges.
