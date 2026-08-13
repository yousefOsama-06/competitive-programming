// Kuhn's bipartite matching, O(V*E) but tiny and easy to modify. Left size n, right size m.
// In practice fast; randomise the adjacency order + greedy pre-match if you need more speed.
struct Kuhn {
    int n, m;
    vector<vector<int>> adj;                     // left -> right
    vector<int> mt;                              // mt[right] = matched left, or -1
    vector<bool> used;

    Kuhn(int n = 0, int m = 0) : n(n), m(m), adj(n), mt(m, -1) {}
    void add_edge(int u, int v) { adj[u].push_back(v); }

    bool try_(int u) {
        for (int v : adj[u]) if (!used[v]) {
            used[v] = true;
            if (mt[v] < 0 || try_(mt[v])) { mt[v] = u; return true; }
        }
        return false;
    }
    int maxMatching() {
        int r = 0;
        vector<int> ord(n);
        iota(all(ord), 0);
        shuffle(all(ord), rng);
        for (int u : ord) { used.assign(m, false); r += try_(u); }
        return r;
    }
    // KONIG: minimum VERTEX COVER = maximum matching. Recover it:
    //   alternate from every UNMATCHED left vertex; let Z = reached set.
    //   cover = (left \ Z) union (right and Z);  max independent set = complement of the cover.
    pair<vector<int>, vector<int>> minVertexCover() {
        vector<int> matchL(n, -1);
        for (int v = 0; v < m; v++) if (mt[v] >= 0) matchL[mt[v]] = v;
        vector<bool> zl(n, false), zr(m, false);
        vector<int> st;
        for (int u = 0; u < n; u++) if (matchL[u] < 0) zl[u] = true, st.push_back(u);
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            for (int v : adj[u]) if (!zr[v]) {
                zr[v] = true;
                if (mt[v] >= 0 && !zl[mt[v]]) zl[mt[v]] = true, st.push_back(mt[v]);
            }
        }
        vector<int> L, R;
        for (int u = 0; u < n; u++) if (!zl[u]) L.push_back(u);
        for (int v = 0; v < m; v++) if (zr[v]) R.push_back(v);
        return {L, R};                           // |L| + |R| == maxMatching()
    }
};
/* THEOREMS you actually quote (bipartite graphs):
   Konig:    max matching = min vertex cover
   max independent set = V - min vertex cover
   MIN PATH COVER of a DAG (VERTEX-DISJOINT paths covering all vertices)
             = n - max matching of the split graph (u_out -> v_in for every edge)
             CAREFUL: this is the vertex-disjoint cover. To get Dilworth (FINITE poset)'s width (minimum
             number of CHAINS, i.e. paths allowed to share vertices) run it on the
             TRANSITIVE CLOSURE of the DAG first.
   Hall:     a perfect matching on the left exists iff |N(S)| >= |S| for every subset S of the LEFT part X
   Dilworth: min chain cover of a poset = longest antichain (build the DAG, TRANSITIVELY CLOSE
             it, then use the path cover above)
   Mirsky:   the dual - longest chain = minimum number of antichains partitioning the poset
   Gallai:   alpha + tau = n always;  nu + rho = n only if there are NO ISOLATED VERTICES
             (alpha = max independent set, tau = min vertex cover, nu = max matching,
              rho = min edge cover)
   Hall defect: nu(G) = |X| - max over W subset of X of ( |W| - |N(W)| )
   Tutte:    G has a perfect matching iff odd(G - U) <= |U| for every U
   Tutte-Berge: nu(G) = (1/2) * min over U of ( |V| + |U| - odd(G - U) )
   Menger:   min edge cut(s,t) = max edge-disjoint s-t paths;
             min vertex cut(s,t) = max internally-vertex-disjoint s-t paths (s,t non-adjacent) */
