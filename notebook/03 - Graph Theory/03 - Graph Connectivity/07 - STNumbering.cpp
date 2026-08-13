// ST-NUMBERING (bipolar orientation) - order the vertices v_1 = s, ..., v_n = t so that EVERY
// other vertex has at least one neighbour BEFORE it and one AFTER it. O(n + m), 0-indexed,
// undirected. Returns the order, or an EMPTY vector when none exists.
// EXISTS iff G + edge(s, t) is 2-VERTEX-CONNECTED (that is exactly what the first DFS checks).
// Why you want it: orienting every edge from the smaller number to the larger gives a DAG with a
// UNIQUE source s and a UNIQUE sink t, and every vertex lies on some s-t path. That is the tool
// for "orient the edges so the graph stays connected / has one source and one sink", for planarity
// testing and st-planar embeddings, and for "peel the graph one vertex at a time keeping both
// halves connected" (any prefix AND its complement induce connected subgraphs).
// Even-Tarjan: DFS from t, then insert the vertices into a list in preorder, each one just before
// or just after its parent according to a sign flipped at every insertion.
vector<int> stNumbering(int n, vector<vector<int>> adj, int s, int t) {
    if (n == 1) return {0};
    bool has = false;                                          // add the s-t edge if missing
    for (int v : adj[s]) has |= (v == t);
    if (!has) adj[s].push_back(t), adj[t].push_back(s);
    vector<int> dis(n, 0), low(n, 0), par(n, -1), sg(n + 2, 0), pre;
    int T = 0;
    bool bicon = true;
    function<void(int, int)> dfs1 = [&](int u, int p) {         // articulation-point check
        low[u] = dis[u] = ++T;
        int kids = 0;
        for (int v : adj[u]) {
            if (v == p) { p = -1; continue; }                   // skip ONE parent edge
            if (!dis[v]) {
                kids++, dfs1(v, u), low[u] = min(low[u], low[v]);
                if (par[u] != -2 && low[v] >= dis[u]) bicon = false;    // par[u]==-2 marks the root
            } else low[u] = min(low[u], dis[v]);
        }
        if (par[u] == -2 && kids > 1) bicon = false;
    };
    par[t] = -2, dfs1(t, -1);
    for (int i = 0; i < n; i++) if (!dis[i]) bicon = false;
    if (!bicon) return {};
    fill(all(dis), 0), fill(all(low), 0), fill(all(par), -1), T = 0;
    function<void(int, int)> dfs2 = [&](int u, int p) {          // tree rooted at t; s is fixed
        low[u] = dis[u] = ++T;
        for (int v : adj[u]) {
            if (v == p) { p = -1; continue; }
            if (!dis[v]) pre.push_back(v), par[v] = u, dfs2(v, u), low[u] = min(low[u], low[v]);
            else low[u] = min(low[u], dis[v]);
        }
    };
    dis[s] = low[s] = ++T, sg[dis[s]] = -1;                     // s takes number 1, never entered
    dfs2(t, -1);
    list<int> L{s, t};
    vector<list<int>::iterator> it(n + 2);
    it[dis[s]] = L.begin(), it[dis[t]] = next(L.begin());
    for (int v : pre) {                                         // insert before/after the parent
        if (sg[low[v]] == -1) it[dis[v]] = L.insert(it[dis[par[v]]], v);
        else it[dis[v]] = L.insert(next(it[dis[par[v]]]), v);
        sg[dis[par[v]]] = -sg[low[v]];
    }
    return vector<int>(all(L));                                 // order[0] = s, order[n-1] = t
}
// THE DAG. With pos[order[i]] = i, orient every edge from the smaller pos to the larger. Then
// every vertex is on an s-t path, so "assign directions keeping everything reachable from s and
// able to reach t" is solved. Reversing all edges swaps the roles of s and t.
// EAR DECOMPOSITION is the other standard certificate of 2-connectivity and is often what a
// problem really wants: an open ear decomposition exists iff the graph is 2-vertex-connected, and
// the ears are found from the same DFS (each back edge starts one ear).
// If the graph is only 2-EDGE-connected, use the bridge tree instead; st-numbering needs vertex
// connectivity.
