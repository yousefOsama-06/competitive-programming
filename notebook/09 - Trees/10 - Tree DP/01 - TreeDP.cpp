// TREE DP - the most common tree shape in a contest. Root the tree, compute a value per subtree
// bottom-up, and combine children. If the answer must be "for every root", see 07 - Rerooting.

// --- MAXIMUM WEIGHT INDEPENDENT SET / MINIMUM VERTEX COVER on a tree, O(n). ---
// dp[v][0] = best for v's subtree with v NOT taken, dp[v][1] = with v taken.
// A tree is bipartite, so alpha = n - nu and (Konig) min vertex cover = maximum matching - but
// this DP is shorter than building a matching, and it handles weights, which Konig does not.
pair<ll, ll> misTree(int v, int p, const vector<vector<int>>& adj, const vector<ll>& w) {
    ll take = w[v], skip = 0;
    for (int u : adj[v]) if (u != p) {
        auto [s, t] = misTree(u, v, adj, w);
        skip += max(s, t), take += s;                          // if v is taken, no child may be
    }
    return {skip, take};                                       // answer = max(skip, take)
}
// MINIMUM VERTEX COVER (unweighted) = n - MIS. MAXIMUM MATCHING on a tree = the greedy "match
// every leaf to its parent, delete both, repeat" - and that greedy is optimal (exchange argument).

// --- TREE KNAPSACK, O(n^2) - and the one character that decides n^2 vs n^3. ---
// dp[v][j] = best value using exactly j vertices of v's subtree. Merge child by child, capping
// BOTH loops by the ALREADY ACCUMULATED size, not by n. Each unordered pair (a, b) is then
// charged exactly once, at their LCA, giving O(n^2) total. Writing `u <= n` instead of
// `u <= acc` is O(n^3) and looks identical.
vector<ll> knapTree(int v, int p, const vector<vector<int>>& adj, const vector<ll>& w) {
    vector<ll> dp = {0, w[v]};                                 // dp[0] = take nothing at all
    for (int u : adj[v]) if (u != p) {
        vector<ll> c = knapTree(u, v, adj, w);                 // c[0] = skip this child entirely
        vector<ll> nd(dp.size() + c.size() - 1, LLONG_MIN);
        nd[0] = 0;
        for (size_t i = 1; i < dp.size(); i++)                 // i >= 1: v IS taken, so a child
            for (size_t j = 0; j < c.size(); j++)              // may attach and stay connected
                if (dp[i] != LLONG_MIN && c[j] != LLONG_MIN) nd[i + j] = max(nd[i + j], dp[i] + c[j]);
        dp = nd;
    }
    return dp;                                                 // dp[j] = best CONNECTED set of j
}                                                              // vertices containing v (dp[0] = 0)
// Dropping the `i >= 1` restriction gives the UNCONSTRAINED version: best j vertices anywhere in
// the subtree, no connectivity - that is the group-knapsack-on-a-tree variant.
// WITH A CAP k: allocate min(size, k) + 1 slots instead, giving O(n * k).

// --- DIAMETER / LONGEST PATH by DP, O(n). The version you reuse inside other tree DPs. ---
// down1[v], down2[v] = the two longest downward paths from v through DIFFERENT children.
// The answer is max over v of down1[v] + down2[v]; the two-BFS trick gives the same on an
// unweighted or POSITIVELY weighted tree, but only this DP survives negative weights.

/* MORE TREE-DP SHAPES, all O(n) unless noted
   COUNT SUBTREES / connected subgraphs containing v: prod over children of (1 + f(child)).
   COUNT MATCHINGS / independent sets: same [0/1] split, sum instead of max.
   SUM OF DISTANCES from every vertex: rerooting with (subtree size, subtree distance sum).
   K-COLOURINGS with adjacent-different: k * (k-1)^(n-1). With a colour list per vertex, DP
     dp[v][c] and take the product over children of (sum over c' != c of dp[u][c']) - keep the
     total per child and subtract the forbidden term to stay O(n * k).
   LONGEST PATH WITH AT MOST K EDGES / paths of length exactly L: dp[v][d] merged small-to-large
     over the depth arrays, or long-path decomposition for O(n).
   BINARY LIFTING ON A DP: when the transition is associative you can jump 2^k ancestors at once
     (max edge on a path, "the first ancestor with property P").
   TREE + BITMASK: dp[v][mask] for k <= 20 marked vertices (Steiner tree on a tree).
   REROOTING is the answer to "compute f(v) for EVERY root" - do not run the DP n times.
   AUXILIARY / VIRTUAL TREE (09 - Trees/08) collapses q marked vertices into an O(q) tree, so a
     per-query tree DP costs O(q log q) instead of O(n).
   DSU ON TREE (09 - Trees/02) is the answer when the per-subtree state is a MULTISET you cannot
     merge algebraically (most frequent colour, k-th value), O(n log n).
   SEGMENT TREE MERGING (02 - DS/16) does the same in O(n log n) while keeping the state
     queryable, at the cost of memory.
   RECURSION DEPTH: a path-shaped tree with n = 2e5 overflows the default stack once frames get
     fat. Move big locals out of the recursive function or convert to the iterative order used
     in 07 - Rerooting.cpp. */
