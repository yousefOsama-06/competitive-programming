// COUNTING SMALL SUBGRAPHS in O(m sqrt m) - the "degree ordering" trick. Orient every edge from
// the lower (degree, index) endpoint to the higher one. In that DAG every out-degree is O(sqrt m),
// because a vertex with out-degree d has d neighbours of degree >= d, so d^2 <= 2m.
// Undirected simple graph, 0-indexed, edges given as pairs.

// --- TRIANGLES: enumerate each once. O(m sqrt m). ---
ll countTriangles(int n, const vector<pair<int, int>>& es) {
    vector<int> deg(n, 0);
    for (auto [u, v] : es) deg[u]++, deg[v]++;
    auto better = [&](int a, int b) { return deg[a] != deg[b] ? deg[a] < deg[b] : a < b; };
    vector<vector<int>> out(n);
    for (auto [u, v] : es) better(u, v) ? out[u].push_back(v) : out[v].push_back(u);
    vector<char> mark(n, 0);
    ll cnt = 0;
    for (int u = 0; u < n; u++) {
        for (int v : out[u]) mark[v] = 1;
        for (int v : out[u]) for (int w : out[v]) cnt += mark[w];
        for (int v : out[u]) mark[v] = 0;
    }
    return cnt;                                                // each triangle counted exactly once
}
// LISTING them: inside the inner loop, (u,v,w) with mark[w] IS a triangle.
// PER-VERTEX / PER-EDGE triangle counts: add 1 to each of u,v,w (or to each of the three edges)
// at that moment - this is what "local clustering coefficient" and many CF problems want.

// --- 4-CYCLES: O(m sqrt m) with the same orientation. ---
// For each u, walk two steps (u -> v -> w) in the oriented graph and count how many times each w
// is reached; a pair of distinct paths u->w gives a 4-cycle. Sum C(cnt[w], 2).
ll countFourCycles(int n, const vector<pair<int, int>>& es) {
    vector<int> deg(n, 0);
    for (auto [u, v] : es) deg[u]++, deg[v]++;
    auto better = [&](int a, int b) { return deg[a] != deg[b] ? deg[a] < deg[b] : a < b; };
    vector<vector<int>> adj(n), out(n);
    for (auto [u, v] : es) {
        adj[u].push_back(v), adj[v].push_back(u);
        better(u, v) ? out[u].push_back(v) : out[v].push_back(u);
    }
    vector<ll> cnt(n, 0);
    ll r = 0;
    for (int u = 0; u < n; u++) {
        for (int v : adj[u]) for (int w : out[v]) if (better(u, w)) r += cnt[w]++;
        for (int v : adj[u]) for (int w : out[v]) if (better(u, w)) cnt[w] = 0;
    }
    return r;
}
// WHY IT IS O(m sqrt m): the inner loop runs sum over v of deg(v) * outdeg(v) <= O(m sqrt m).
//
// OTHER COUNTING FACTS WORTH HAVING:
//   * #triangles = trace(A^3) / 6 for the adjacency matrix A; with bitsets that is O(n^3 / 64) and
//     is often the shorter answer when n <= 2000 (also gives per-pair common-neighbour counts).
//   * #paths of length 2 = sum over v of C(deg v, 2). Subtract 3 * #triangles for "paths that are
//     not part of a triangle".
//   * A graph is TRIANGLE-FREE  =>  m <= n^2/4 (Mantel / Turan for K3). More generally Turan:
//     a K_{r+1}-free graph has at most (1 - 1/r) n^2 / 2 edges.
//   * #closed walks of length k = trace(A^k) - use matrix power for k up to 1e18 on small n.
//   * COUNTING CYCLES OF LENGTH <= 5 is polynomial by similar tricks; length >= 6 is as hard as
//     matrix multiplication in general. For counting ALL simple cycles use inclusion-exclusion
//     over subsets (2^n) or DP over bitmask start-vertex.
//   * GIRTH (shortest cycle) of an unweighted graph: BFS from every vertex, O(nm). For the
//     shortest cycle THROUGH a given edge, delete it and run BFS.
