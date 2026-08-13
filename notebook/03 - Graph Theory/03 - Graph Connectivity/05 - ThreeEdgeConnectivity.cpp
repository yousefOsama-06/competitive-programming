// THREE-EDGE-CONNECTED COMPONENTS in ONE DFS, O(n + m). Undirected, 0-indexed, may be
// disconnected; self loops and PARALLEL EDGES are handled (parallel edges matter here - two
// vertices joined by three parallel edges are 3-edge-connected).
// u and v land in the same component iff no set of at most TWO edges separates them, i.e. iff
// there are 3 edge-disjoint u-v paths (Menger). This is the next step after LowLink (01), which
// gives the 2-edge-connected components (= no single edge separates them).
// Reach for it on "the network survives any two cable failures", "contract every pair of parallel
// bridges", and as the preprocessing for 3-edge-connectivity augmentation problems.
// Tsin's algorithm: a DFS that maintains, for each vertex, a PATH of still-undecided descendants;
// whenever a back edge proves two of them 3-edge-connected the path is absorbed into one class.
// The classes live in a circular linked list, so "merge" is a single swap.
vector<vector<int>> threeEdgeCC(const vector<vector<int>>& g) {
    int n = g.size(), cnt = 0;
    vector<int> in(n), out(n), low(n, n), deg(n, 0), path(n, n), nxt(n), it(n, 0), stk;
    vector<char> vis(n, 0);
    iota(all(nxt), 0);                                         // circular list = the components
    auto absorb = [&](int v, int w) { swap(nxt[v], nxt[w]), deg[v] += deg[w]; };
    for (int s = 0; s < n; s++) {                              // iterative DFS over the forest
        if (vis[s]) continue;
        stk = {s}, vis[s] = 1, in[s] = cnt++;
        vector<int> par(1, n), pe(1, -1);                      // pe = "parent edge already used"
        while (!stk.empty()) {
            int v = stk.back(), p = par.back();
            if (it[v] < (int)g[v].size()) {
                int w = g[v][it[v]++];
                if (w == v) continue;
                if (w == p && !pe.back()) { pe.back() = 1; continue; }   // skip ONE parent edge
                if (vis[w]) {
                    if (in[w] < in[v]) deg[v]++, low[v] = min(low[v], in[w]);
                    else {                                     // back edge to a descendant
                        deg[v]--;
                        int u = path[v];
                        while (u != n && in[u] <= in[w] && in[w] < out[u])
                            absorb(v, u), u = path[u];
                        path[v] = u;
                    }
                    continue;
                }
                vis[w] = 1, in[w] = cnt++, stk.push_back(w), par.push_back(v), pe.push_back(0);
            } else {                                           // leaving v: fold it into its parent
                out[v] = cnt, stk.pop_back(), par.pop_back(), pe.pop_back();
                if (p == n) continue;
                int w = v;
                if (path[w] == n && deg[w] <= 1) { deg[p] += deg[w], low[p] = min(low[p], low[w]); }
                else {
                    if (deg[w] == 0) w = path[w];
                    if (low[w] < low[p]) low[p] = low[w], swap(w, path[p]);
                    while (w != n) absorb(p, w), w = path[w];
                }
            }
        }
    }
    vector<vector<int>> comp;                                  // walk each circular list once
    vector<char> seen(n, 0);
    for (int i = 0; i < n; i++) if (!seen[i]) {
        comp.push_back({});
        for (int j = i; !seen[j]; j = nxt[j]) seen[j] = 1, comp.back().push_back(j);
    }
    return comp;
}
// CONTRACTING each component gives a graph in which every 3-edge cut is visible; the 2-edge-cuts
// are exactly the bridges plus the "cactus" pairs from LowLink.
// 3-VERTEX-CONNECTIVITY (SPQR / triconnected components) is a genuinely harder, ~400-line
// algorithm. If you only need to test 3-vertex-connectivity for small n, delete every pair of
// vertices and check connectivity in O(n^2 (n + m)).
