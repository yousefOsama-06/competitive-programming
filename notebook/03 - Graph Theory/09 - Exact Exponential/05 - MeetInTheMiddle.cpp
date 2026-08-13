// MEET IN THE MIDDLE ON GRAPHS - the n <= 40..50 band, where 2^n is hopeless but 2^(n/2) is not.
// 0-indexed, adj[] is an adjacency BITMASK (ll, so n <= 63), no self loops unless stated.
// THE PATTERN. Split the vertices into halves L (first k) and R (the rest). Enumerate every valid
// configuration of L; each one imposes a REQUIREMENT on R that is a single bitmask; precompute for
// every R-mask the best/total over all valid R-configurations COMPATIBLE with it, using a SOS
// (subset-sum / superset-sum) transform. Then one pass over the 2^k left masks answers everything.

// --- COUNT ALL CLIQUES (including the empty one), O(2^(n/2) * n). ---
// A set is a clique iff every chosen vertex is adjacent to all the others, i.e. the intersection of
// (adj[v] | bit v) over v in the set contains the set.
ll countCliques(int n, const vector<ll>& adj) {
    int k = n / 2, r = n - k;
    ll full = n == 64 ? -1LL : (1LL << n) - 1;
    vector<ll> g(n);
    for (int i = 0; i < n; i++) g[i] = adj[i] | 1LL << i;
    vector<ll> dp(1 << k, 0);
    dp[0] = 1;
    for (int m = 1; m < (1 << k); m++) {                       // is the left mask m a clique?
        ll nw = full;
        for (int j = 0; j < k; j++) if (m >> j & 1) nw &= g[j];
        if ((nw & m) == m) dp[m] = 1;
    }
    for (int i = 0; i < k; i++) for (int m = 0; m < (1 << k); m++)    // SOS: dp[m] = #subsets
        if (m >> i & 1) dp[m] += dp[m ^ 1 << i];
    ll ans = dp[(1 << k) - 1];                                 // right half empty
    for (int m = 1; m < (1 << r); m++) {
        ll nw = full, p = (ll)m << k;
        for (int j = 0; j < r; j++) if (m >> j & 1) nw &= g[k + j];
        if ((nw & p) == p) ans += dp[nw & ((1LL << k) - 1)];    // left must sit inside nw
    }
    return ans;
}

// --- MINIMUM COST VERTEX COVER OF A GENERAL GRAPH, O(2^(n/2) * n). ---
// Every edge needs at least one endpoint chosen; minimise the total cost of the chosen vertices.
// A left mask fixes the left choices; the UNCHOSEN left vertices force their right neighbours in.
// SELF LOOP at v (bit v of adj[v]) = "v must be taken", which is how "forbidden" vertices are said.
// (On a BIPARTITE graph this is polynomial - Konig / min cut. This is the general case.)
ll minCostVertexCover(int n, const vector<ll>& adj, const vector<ll>& cost) {
    int k = n / 2, r = n - k;
    vector<ll> cl(1 << k, 0), need(1 << k, 0), dp(1 << r, 0);
    for (int m = 0; m < (1 << k); m++) {
        for (int i = 0; i < k; i++) {
            if (m >> i & 1) { cl[m] += cost[i]; continue; }
            if (adj[i] >> i & 1) { cl[m] = llinf; break; }     // self loop: i had to be taken
            for (int j = 0; j < n; j++) if (adj[i] >> j & 1) {
                if (j < k) { if (!(m >> j & 1)) cl[m] = llinf; }   // uncovered left edge
                else need[m] |= 1 << (j - k);
            }
        }
    }
    for (int m = 0; m < (1 << r); m++) {
        for (int i = 0; i < r; i++) {
            if (m >> i & 1) { dp[m] += cost[k + i]; continue; }
            for (int j = k; j < n; j++)
                if ((adj[k + i] >> j & 1) && !(m >> (j - k) & 1)) dp[m] = llinf;
        }
    }
    for (int i = 0; i < r; i++) for (int m = (1 << r) - 1; m >= 0; m--)   // SOS over SUPERSETS
        if (!(m >> i & 1)) dp[m] = min(dp[m], dp[m ^ 1 << i]);
    ll ans = llinf;
    for (int m = 0; m < (1 << k); m++)
        if (cl[m] < llinf && dp[need[m]] < llinf) ans = min(ans, cl[m] + dp[need[m]]);
    return ans;
}
// MAXIMUM INDEPENDENT SET = (total cost) - (minimum cost vertex cover) with unit costs, so the
// second routine solves MIS for n <= 50 too. MAXIMUM CLIQUE for that range: 02 - MaxCliqueBitset.
// OTHER PROBLEMS THAT SPLIT THE SAME WAY: subset sum / knapsack on n <= 40, "count the subsets
// with property X" when X only couples the halves through one mask, and 3-colouring by enumerating
// the colour classes of one half.
// THE SOS DIRECTION MATTERS: use the subset transform when the right half must fit INSIDE a mask
// (counting), and the superset transform when the right half must CONTAIN a mask (covering).
