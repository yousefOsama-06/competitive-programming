// MINIMUM WEIGHT CYCLE THROUGH EVERY VERTEX (and hence the global minimum weight cycle), by one
// Dijkstra per source, O(V^3) with the dense version below or O(V (E + V log V)) with a heap.
// UNDIRECTED, non-negative weights, 0-indexed, g is an adjacency MATRIX with llinf for "no edge".
// Returns best[u] = the weight of the lightest simple cycle CONTAINING u (llinf if there is none);
// the global answer is the minimum over u, and the GIRTH is the unweighted case (all weights 1).
// WHY IT WORKS. A lightest cycle through u splits into two shortest paths from u that leave along
// DIFFERENT first edges and are joined by one more edge (cur, i). So run Dijkstra from u, tag every
// vertex with the first edge (the "branch") its shortest path used, and combine any two settled
// vertices in different branches. The `par[cur] != cur` guard rejects the degenerate 2-cycle that
// walks one edge out and back.
vector<ll> minWeightCycles(int n, const vector<vector<ll>>& g) {
    vector<ll> best(n, llinf), d(n);
    vector<int> par(n), vis(n);
    for (int u = 0; u < n; u++) {
        fill(all(vis), 0), fill(all(d), llinf);
        iota(all(par), 0);
        d[u] = 0;
        for (;;) {
            int cur = -1;                                      // dense Dijkstra: O(n^2) per source
            for (int i = 0; i < n; i++) if (!vis[i] && d[i] < llinf && (cur < 0 || d[i] < d[cur]))
                cur = i;
            if (cur < 0) break;
            vis[cur] = 1;
            if (par[cur] != cur && g[cur][u] < llinf)          // close the cycle straight to u
                best[u] = min(best[u], d[cur] + g[cur][u]);
            for (int i = 0; i < n; i++) {
                if (i == u || g[cur][i] == llinf) continue;
                if (vis[i]) {
                    if (par[i] != par[cur])                    // two different branches meet
                        best[u] = min(best[u], d[cur] + d[i] + g[cur][i]);
                } else if (d[cur] + g[cur][i] < d[i])
                    d[i] = d[cur] + g[cur][i], par[i] = (cur == u ? i : par[cur]);
            }
        }
    }
    return best;
}
// GLOBAL MINIMUM WEIGHT CYCLE: min over u of best[u]. If that is all you want and the graph is
// DIRECTED, Floyd-Warshall is shorter: run it, then take min over v of d[v][v] (initialise the
// diagonal to llinf, not 0).
// GIRTH of an unweighted graph: replace Dijkstra by BFS - O(V * E), and for a cycle through a
// FIXED vertex the two-branch rule is the same.
// MINIMUM CYCLE THROUGH A GIVEN EDGE (u, v): delete the edge and take dist(u, v) + w.
// MINIMUM MEAN cycle (weight divided by length) is a different problem - Karp, see 02 - Shortest
// Paths/06 - CyclesAndConstraints.
// NEGATIVE WEIGHTS break this completely (Dijkstra); a minimum weight cycle with negative edges is
// NP-hard in general, since a Hamiltonian cycle can be encoded in it.
// LONGEST cycle is NP-hard. Shortest cycle THROUGH k GIVEN VERTICES is FPT via colour coding.
