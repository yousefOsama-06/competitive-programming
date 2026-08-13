// MAXIMUM MATCHING IN A GENERAL (NON-BIPARTITE) GRAPH - Edmonds' blossom algorithm, O(V^3).
// Everything else in this folder assumes bipartite. Reach for this the moment the two sides are
// not fixed: "pair up people who are compatible", "tile a board where the colouring argument
// fails", "maximum set of disjoint edges" on an arbitrary graph.
// The trick it exists for: an ODD cycle can be traversed in two directions, so an alternating
// search can enter and leave it on the same side. Edmonds CONTRACTS such a cycle (a "blossom")
// into a single vertex, searches the contracted graph, and expands afterwards.
// Certificate: the answer equals the Tutte-Berge bound (see 10 - Theorems/01 - GraphTheorems).
struct Blossom {
    int n;
    vector<vector<char>> g;                                    // adjacency MATRIX
    vector<int> match, p, base;
    vector<char> used, inBlossom;
    Blossom(int n) : n(n), g(n, vector<char>(n, 0)), match(n, -1), p(n), base(n),
                     used(n), inBlossom(n) {}
    void addEdge(int u, int v) { g[u][v] = g[v][u] = 1; }
    int lca(int a, int b) {
        vector<char> seen(n, 0);
        for (;;) { a = base[a], seen[a] = 1; if (match[a] < 0) break; a = p[match[a]]; }
        for (;;) { b = base[b]; if (seen[b]) return b; b = p[match[b]]; }
    }
    void mark(int v, int b, int child) {                       // walk v up to the blossom base
        while (base[v] != b) {
            inBlossom[base[v]] = inBlossom[base[match[v]]] = 1;
            p[v] = child, child = match[v], v = p[match[v]];
        }
    }
    bool augment(int root) {                                   // one BFS for an augmenting path
        fill(all(used), 0), fill(all(p), -1);
        iota(all(base), 0);
        used[root] = 1;
        queue<int> q({root});
        while (!q.empty()) {
            int v = q.front(); q.pop();
            for (int to = 0; to < n; to++) {
                if (!g[v][to] || base[v] == base[to] || match[v] == to) continue;
                if (to == root || (match[to] >= 0 && p[match[to]] >= 0)) {   // odd cycle -> contract
                    int b = lca(v, to);
                    fill(all(inBlossom), 0);
                    mark(v, b, to), mark(to, b, v);
                    for (int i = 0; i < n; i++) if (inBlossom[base[i]]) {
                        base[i] = b;
                        if (!used[i]) used[i] = 1, q.push(i);
                    }
                } else if (p[to] < 0) {
                    p[to] = v;
                    if (match[to] < 0) {                        // free vertex: flip the path
                        for (int u = to, pv, nv; u >= 0; u = nv)
                            pv = p[u], nv = match[pv], match[u] = pv, match[pv] = u;
                        return true;
                    }
                    used[match[to]] = 1, q.push(match[to]);
                }
            }
        }
        return false;
    }
    int solve() {
        int r = 0;
        for (int v = 0; v < n; v++)                            // greedy start: fewer BFS rounds
            if (match[v] < 0)
                for (int u = 0; u < n; u++)
                    if (g[v][u] && match[u] < 0) { match[v] = u, match[u] = v, r++; break; }
        for (int v = 0; v < n; v++) if (match[v] < 0) r += augment(v);
        return r;                                              // match[v] = partner, or -1
    }
};
// PERFECT MATCHING EXISTS iff solve() * 2 == n. Tutte's theorem gives the certificate when it
// does not: some U with odd(G - U) > |U|.
// MINIMUM EDGE COVER (no isolated vertices) = n - maximum matching (Gallai).
// MAXIMUM INDEPENDENT SET is still NP-hard here - Konig's alpha = n - nu is BIPARTITE ONLY.
// WEIGHTED general matching needs the dual-variable version (~150 lines); if the graph is small
// (n <= 20) a bitmask DP over matched vertices is far shorter and usually enough.
// CHINESE POSTMAN (undirected): make every degree even by duplicating a minimum-weight perfect
// matching on the ODD-degree vertices, using all-pairs shortest paths as the edge weights, then
// take an Eulerian circuit. The MIXED version (some edges directed) is NP-hard.
// THE MATCHING GAME (a token moves along edges to unvisited vertices; stuck loses): the first
// player wins iff the start vertex is covered by EVERY maximum matching, i.e. iff deleting it
// decreases the matching size. With this file that works on a general graph, not just bipartite.
