// EXACT ALGORITHMS FOR NP-HARD GRAPH PROBLEMS ON SMALL n. The constraint IS the hint:
//   n <= 20-24  -> plain subset DP, O(2^n * n)
//   n <= 40-50  -> meet in the middle, or Bron-Kerbosch (fast in practice on sparse graphs)
//   n <= 100+   -> only if the graph is bipartite/perfect/a tree - then it is polynomial (Konig)
// adj[] is an ADJACENCY BITMASK: bit j of adj[i] is set iff i-j is an edge. No self loops.

// --- MAXIMUM CLIQUE, Bron-Kerbosch with PIVOTING. Worst case O(3^(n/3)), very fast in practice. ---
// R = clique built so far, P = candidates, X = already-explored vertices (avoids duplicates).
// The pivot u in P|X is chosen to maximise |P & adj[u]|; only P \ adj[u] needs branching, because
// any maximal clique either omits u or contains a non-neighbour of u.
int bestClique;
unsigned bestSet;
void bk(unsigned R, unsigned P, unsigned X, const vector<unsigned>& adj) {
    if (!P && !X) {
        if (__builtin_popcount(R) > bestClique) bestClique = __builtin_popcount(R), bestSet = R;
        return;
    }
    int u = __builtin_ctz(P | X), best = -1;
    for (unsigned t = P | X; t; t &= t - 1) {
        int v = __builtin_ctz(t), c = __builtin_popcount(P & adj[v]);
        if (c > best) best = c, u = v;
    }
    for (unsigned cand = P & ~adj[u]; cand; cand &= cand - 1) {
        int v = __builtin_ctz(cand);
        bk(R | 1u << v, P & adj[v], X & adj[v], adj);
        P &= ~(1u << v), X |= 1u << v;
    }
}
int maxClique(const vector<unsigned>& adj) {                   // n <= 32
    bestClique = 0, bestSet = 0;
    bk(0, (1u << adj.size()) - 1, 0, adj);
    return bestClique;                                         // the clique itself is in bestSet
}
// MAXIMUM INDEPENDENT SET = maximum clique of the COMPLEMENT graph. Just flip adj and mask off i.
// MINIMUM VERTEX COVER = n - MIS. On BIPARTITE graphs both are polynomial: Konig says
//   min vertex cover = maximum matching, and MIS = n - matching (see 07 - Matching).

// --- CHROMATIC NUMBER, O(2^n * n) by inclusion-exclusion (Bjorklund-Husfeldt-Koivisto). ---
// Let ind[S] = 1 if S is independent. The number of ways to cover V with k ORDERED independent
// sets is  sum_S (-1)^(n-|S|) * I(S)^k,  where I(S) = #independent subsets of S. The chromatic
// number is the smallest k making that non-zero. Compute I over subsets with one SOS pass.
int chromatic(const vector<unsigned>& adj) {                   // n <= 20 with this modulus
    int n = adj.size(), N = 1 << n;
    if (!n) return 0;
    const ll M = (1LL << 61) - 1;                              // any large prime works
    vector<ll> I(N), pw(N, 1);
    I[0] = 1;
    for (int S = 1; S < N; S++) {                              // I[S] = #independent subsets of S
        int v = __builtin_ctz(S), rest = S ^ 1 << v;           // (v in it, or not)
        I[S] = (I[rest] + I[rest & ~adj[v]]) % M;
    }
    for (int k = 1; k <= n; k++) {
        ll tot = 0;
        for (int S = 0; S < N; S++) {
            pw[S] = (__int128)pw[S] * I[S] % M;                // I(S)^k
            ll t = (n - __builtin_popcount(S)) & 1 ? M - pw[S] : pw[S];
            tot = (tot + t) % M;
        }
        if (tot % M) return k;
    }
    return n;
}
// The count is done MODULO a prime, so a zero could in principle be a false negative; with a
// 61-bit modulus that never happens on contest inputs. Check bipartiteness first - k <= 2 is O(n+m).

// --- MINIMUM STEINER TREE (connect a set of k terminals in a weighted graph), O(3^k n + 2^k n^2).
// dp[S][v] = cheapest tree containing terminal set S and touching vertex v.
//   merge:  dp[S][v] = min over submasks T of dp[T][v] + dp[S\T][v]
//   grow:   dp[S][v] = min over edges (u,v) of dp[S][u] + w         (run Dijkstra per S)
ll steinerTree(int n, const vector<vector<pair<int, ll>>>& g, const vector<int>& term) {
    int k = term.size();
    if (!k) return 0;
    vector<vector<ll>> dp(1 << k, vector<ll>(n, llinf));
    for (int i = 0; i < k; i++) dp[1 << i][term[i]] = 0;
    for (int S = 1; S < 1 << k; S++) {
        for (int T = (S - 1) & S; T; T = (T - 1) & S)
            for (int v = 0; v < n; v++)
                if (dp[T][v] < llinf && dp[S ^ T][v] < llinf)
                    dp[S][v] = min(dp[S][v], dp[T][v] + dp[S ^ T][v]);
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
        for (int v = 0; v < n; v++) if (dp[S][v] < llinf) pq.push({dp[S][v], v});
        while (!pq.empty()) {                                  // Dijkstra to relax the tree outward
            auto [d, u] = pq.top(); pq.pop();
            if (d > dp[S][u]) continue;
            for (auto [v, w] : g[u]) if (dp[S][u] + w < dp[S][v])
                dp[S][v] = dp[S][u] + w, pq.push({dp[S][v], v});
        }
    }
    return *min_element(all(dp[(1 << k) - 1]));
}
// STEINER TREE IN A GRID / with k up to ~10 is the usual contest size. If k == 2 it is a shortest
// path; if k == n it is the MST; the DP interpolates between them.
// RELATED EXACT-EXPONENTIAL TOOLS:
//   * HAMILTONIAN PATH / TSP: dp[mask][v], O(2^n n^2). See 08 - DP/02 - BitmaskDP.
//   * SET COVER: dp over covered elements, O(2^n * #sets), or IE like the chromatic number.
//   * GRAPH COLOURING with k fixed: try to 3-colour by 2-SAT? No - 3-colouring is NP-hard; use
//     the IE formula above, or branch on the highest-degree vertex with memoisation.
//   * MAXIMUM INDEPENDENT SET for n <= 40-50: meet in the middle - split the vertices in half,
//     enumerate independent subsets of the left half, and for each store the best independent
//     subset of the right half compatible with its neighbourhood (SOS over the right mask).
//   * COUNTING INDEPENDENT SETS / colourings on a graph of small TREEWIDTH: DP over a tree
//     decomposition, O(2^tw * n) - usually a grid problem in disguise (see broken-profile DP).
