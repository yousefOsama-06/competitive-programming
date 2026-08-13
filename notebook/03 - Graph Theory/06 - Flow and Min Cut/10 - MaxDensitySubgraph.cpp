// Needs: Dinic (01 - Dinic.cpp).
// MAXIMUM DENSITY SUBGRAPH - choose a NON-EMPTY vertex set S maximising |E(S)| / |S|, the number
// of induced edges per vertex. Undirected, 0-indexed, O(log(n^2) * maxflow) via parametric min cut.
// (Note this is NOT the maximum CLIQUE relaxation - it is polynomial, unlike everything nearby.)
// Reach for it for "find the most tightly knit group", "the subgraph with the best edges-to-people
// ratio", spam/fraud ring detection, and as the LP-relaxation certificate for densest-subgraph
// style scoring problems.
// FRACTIONAL PROGRAMMING. Ask "is there S with |E(S)| - g|S| > 0?" for a guess g. Build
//   s -> v (capacity m), v -> t (capacity m + 2g - deg(v)), u <-> v (capacity 1 each way)
// then min cut = n*m - 2 * max over S of (|E(S)| - g|S|), so the answer is yes iff the min cut is
// strictly below n*m. Binary search g. Two distinct densities differ by at least 1/(n(n-1)), so
// O(log(n^2)) iterations of the binary search are enough for an EXACT answer with rationals.
// Below every capacity is scaled by K = n^2 so they stay INTEGRAL and the guess is g = g2 / K.
pair<ld, vector<int>> maxDensitySubgraph(int n, const vector<pair<int, int>>& e) {
    int m = e.size();
    if (n == 1 || m == 0) return {0, {0}};
    ll K = (ll)n * n;
    vector<int> deg(n, 0), best{0};
    for (auto [u, v] : e) deg[u]++, deg[v]++;
    ll lo = 0, hi = (ll)m * K;                                 // density * K, density <= m
    while (lo < hi) {
        ll g2 = (lo + hi + 1) / 2;                             // test the density g = g2 / K
        Dinic din(n + 2);
        int s = n, t = n + 1;
        for (auto [u, v] : e) din.addEdge(u, v, K, K);          // undirected, capacity 1 scaled
        for (int v = 0; v < n; v++) {
            din.addEdge(s, v, (ll)m * K);
            din.addEdge(v, t, (ll)m * K + 2 * g2 - (ll)deg[v] * K);
        }
        if (din.calc(s, t) < (ll)n * m * K) {                  // some S beats density g
            lo = g2, best.clear();
            for (int v = 0; v < n; v++) if (din.leftOfMinCut(v)) best.push_back(v);
        } else hi = g2 - 1;
    }
    return {(ld)lo / K, best};                                 // {density, the vertex set}
}
// The source side of the min cut IS the densest set - no separate extraction pass is needed.
// MAXIMUM DENSITY with VERTEX WEIGHTS or edge weights: replace deg(v) by the weighted degree and
// the capacity-1 edges by the edge weights; the same cut works.
// AT LEAST k VERTICES / AT MOST k VERTICES turns the problem NP-hard - do not try to patch this.
// MINIMUM density is trivial (a single vertex, density 0), so the question is always maximum.
// RELATED: "maximum (sum of vertex profits) - (sum of edge penalties)" is plain project selection
// (05 - MinCutModelling); the RATIO is what forces the binary search.
// GOLDBERG'S ORIGINAL formulation uses capacities m + 2g - deg(v) exactly as above; if you prefer
// floating point, binary search g as a double for ~40 iterations and skip the scaling.
