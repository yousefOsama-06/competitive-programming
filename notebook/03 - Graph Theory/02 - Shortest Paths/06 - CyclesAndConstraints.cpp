// MINIMUM MEAN CYCLE, and SYSTEMS OF DIFFERENCE CONSTRAINTS - the two shortest-path tools that
// are not about shortest paths.

// --- KARP'S MINIMUM MEAN CYCLE, O(V*E). Returns the smallest possible (cycle weight)/(length).
// dp[k][v] = min weight of a walk of EXACTLY k edges ending at v (from a virtual source that
// reaches everything). Karp: the answer is  min over v of  max over k<n of (dp[n][v]-dp[k][v])/(n-k).
// Reach for it whenever the objective is an AVERAGE around a cycle: "minimum cost per unit time",
// "is there a cycle with negative average", profit-per-day loops.
ld minMeanCycle(int n, const vector<array<ll, 3>>& e) {        // e = {u, v, w}, directed
    vector<vector<ll>> dp(n + 1, vector<ll>(n, llinf));
    fill(all(dp[0]), 0);                                       // virtual source reaches every v
    for (int k = 1; k <= n; k++)
        for (auto [u, v, w] : e)
            if (dp[k - 1][u] < llinf) dp[k][v] = min(dp[k][v], dp[k - 1][u] + w);
    ld best = 1e18;
    for (int v = 0; v < n; v++) {
        if (dp[n][v] == llinf) continue;
        ld cur = -1e18;
        for (int k = 0; k < n; k++)
            if (dp[k][v] < llinf) cur = max(cur, (ld)(dp[n][v] - dp[k][v]) / (n - k));
        best = min(best, cur);
    }
    return best;                                               // 1e18 if the graph is acyclic
}
// MINIMUM COST-TO-TIME RATIO CYCLE (minimise sum(cost)/sum(time), time > 0): binary search x and
// test for a negative cycle with weights cost - x*time. Same skeleton, one Bellman-Ford per step.
// MAXIMUM mean cycle: negate the weights.

// --- SYSTEM OF DIFFERENCE CONSTRAINTS. Every constraint x_j - x_i <= w becomes an edge i -> j of
// weight w; add a super-source with a 0-edge to every vertex and run Bellman-Ford.
// FEASIBLE iff there is no negative cycle. The distances are then a solution, and they are the
// POINTWISE MAXIMUM one (every other solution is <= it componentwise, after fixing x_source = 0).
// TRANSLATIONS you will need: x_j - x_i >= w  is  x_i - x_j <= -w.  x_j - x_i = w is both.
// x_i <= c is x_i - x_0 <= c against the source. Strict "<" on integers is "<= w-1".
// USES: scheduling with precedence and deadlines, "arrange these values so all the gaps hold",
// consistency of a set of relative measurements, and the dual of a min-cost-flow.
bool differenceConstraints(int n, const vector<array<ll, 3>>& c, vector<ll>& x) {
    vector<array<ll, 3>> e = c;                                // c = {i, j, w} meaning x_j-x_i<=w
    for (int v = 0; v < n; v++) e.push_back({(ll)n, (ll)v, 0});    // super-source n
    x.assign(n + 1, 0);
    for (int it = 0; it <= n; it++) {
        bool any = false;
        for (auto [u, v, w] : e)
            if (x[u] + w < x[v]) x[v] = x[u] + w, any = true;
        if (!any) { x.resize(n); return true; }
    }
    return false;                                              // negative cycle: infeasible
}
// --- BELLMAN-FORD EXTRAS worth remembering
// NEGATIVE CYCLE RECOVERY: after n rounds, take a vertex that still relaxed, walk n parent
// pointers back (this lands you ON the cycle - the relaxed vertex may only be reachable from it),
// then follow parents until a vertex repeats.
// SPFA is Bellman-Ford with a queue; it is O(VE) worst case and killable, but the SLF heuristic
// (push to the front if the new distance beats the front) makes it fast in practice.
// ONLY SHORTEST PATHS WITH AT MOST K EDGES: run exactly k rounds, updating from a SNAPSHOT of the
// previous round (otherwise one round can propagate along many edges).
// JOHNSON'S APSP for sparse graphs with negative edges: Bellman-Ford from a super-source gives
// potentials h[], reweight w'(u,v) = w + h[u] - h[v] >= 0, then run n Dijkstras. O(VE + V E log V).
