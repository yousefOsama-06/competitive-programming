// Needs: WeightedBlossom (07 - WeightedBlossom.cpp).
// CHINESE POSTMAN (route inspection) - the cheapest CLOSED WALK that uses EVERY edge at least
// once, on an undirected connected weighted graph. 0-indexed in, O(n^3) via general matching.
// THEORY. A closed walk using every edge exists with no repeats iff every degree is even (Euler).
// Otherwise you must DUPLICATE some edges, and the duplicated set must fix exactly the odd-degree
// vertices - i.e. it is a union of paths pairing them up. So: take all-pairs shortest paths, build
// the complete graph on the ODD-degree vertices (there is an even number of them, by handshake),
// and find a MINIMUM WEIGHT PERFECT MATCHING there. Answer = total edge weight + matching weight.
// Reach for it for "walk every street and come back, minimum distance", "traverse every corridor",
// and "make every degree even at minimum cost" (that last one IS the matching part alone).
ll chinesePostman(int n, const vector<array<ll, 3>>& e) {      // e = {u, v, w}, undirected
    vector<vector<ll>> d(n, vector<ll>(n, llinf));
    vector<int> deg(n, 0);
    ll total = 0;
    for (int i = 0; i < n; i++) d[i][i] = 0;
    for (auto [u, v, w] : e) {
        d[u][v] = d[v][u] = min(d[u][v], w), total += w, deg[u]++, deg[v]++;
    }
    for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
        if (d[i][k] < llinf && d[k][j] < llinf) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
    vector<int> odd;
    for (int i = 0; i < n; i++) if (deg[i] & 1) odd.push_back(i);
    int k = odd.size();
    if (!k) return total;                                      // already Eulerian
    ll mx = 0;                                                 // shift so that MIN becomes MAX and
    for (int i = 0; i < k; i++) for (int j = 0; j < k; j++)    // a PERFECT matching always wins
        if (d[odd[i]][odd[j]] < llinf) mx = max(mx, d[odd[i]][odd[j]]);
    ll B = mx * k + 1;
    WeightedBlossom M(k);
    for (int i = 0; i < k; i++) for (int j = i + 1; j < k; j++)
        if (d[odd[i]][odd[j]] < llinf) M.addEdge(i + 1, j + 1, B - d[odd[i]][odd[j]]);
    auto [val, cnt] = M.solve();
    if (cnt * 2 != k) return -1;                               // odd vertices cannot be paired
    return total + (ll)cnt * B - val;
}
// RECOVERING THE ROUTE: duplicate every edge on the shortest path between each matched pair, then
// run an Eulerian circuit (05 - Eulerian Path) on the resulting even-degree multigraph.
// DIRECTED CHINESE POSTMAN is a MIN COST FLOW, not a matching: give each vertex a supply of
// (outdeg - indeg), send flow from the surplus vertices to the deficit ones along the original
// arcs with cost = weight and infinite capacity, and add that cost to the total weight.
// MIXED (some edges directed, some not) is NP-hard.
// RURAL POSTMAN (only a SUBSET of edges must be covered) is NP-hard as well - it contains TSP.
// OPEN version ("start anywhere, end anywhere"): leave two odd vertices unmatched, i.e. run a
// maximum-weight (not perfect) matching, which is what solve() gives you without the count check.
