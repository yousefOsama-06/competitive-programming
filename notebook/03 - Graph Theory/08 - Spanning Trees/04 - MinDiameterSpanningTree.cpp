// MINIMUM DIAMETER SPANNING TREE - the spanning tree whose longest path is as SHORT as possible.
// O(V^3) (Floyd-Warshall plus one sweep per edge). Undirected, non-negative weights, 0-indexed.
// This is NOT the MST: the MST minimises total weight and can have a terrible diameter.
// KEY FACT. An MDST is always a SHORTEST PATH TREE from the ABSOLUTE 1-CENTRE - a point that may
// sit in the MIDDLE OF AN EDGE, not only at a vertex. So: for every edge (u, v) of weight w, slide
// a point c at distance x along it and minimise its eccentricity ecc(x) = max over i of
// min(x + d[u][i], w - x + d[v][i]). Each i contributes a "tent" function of x, the upper envelope
// is piecewise linear, and it is enough to test the breakpoints - which is the sweep below, after
// sorting the vertices by d[u][.] descending. The answer is the minimum eccentricity, doubled.
// Reach for it for "build a network minimising the worst-case delay between any two nodes",
// "connect all cities so the farthest pair is as close as possible", and broadcast-tree problems.
// ALL WEIGHTS ARE DOUBLED INTERNALLY so the centre's position stays an integer; the returned
// diameter is already in the ORIGINAL units (it is twice the eccentricity, which cancels out).
struct MDST {
    ll diameter = llinf;
    int edge = -1;                                             // the edge holding the centre
    ll offset = 0;                                             // 2 * distance from e[edge][0]
    vector<ll> distFromCentre;                                 // 2 * dist(centre, i), after solve
    ll solve(int n, const vector<array<ll, 3>>& e) {           // e = {u, v, w}
        if (n == 1) return diameter = 0;
        vector<vector<ll>> d(n, vector<ll>(n, llinf));
        for (int i = 0; i < n; i++) d[i][i] = 0;
        for (auto [u, v, w] : e) d[u][v] = min(d[u][v], 2 * w), d[v][u] = min(d[v][u], 2 * w);
        for (int k = 0; k < n; k++) for (int i = 0; i < n; i++) for (int j = 0; j < n; j++)
            if (d[i][k] < llinf && d[k][j] < llinf) d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
        vector<int> id(n);
        iota(all(id), 0);
        for (int u = 0; u < n; u++) {
            sort(all(id), [&](int a, int b) { return d[u][a] > d[u][b]; });
            for (int k = 0; k < (int)e.size(); k++) {
                int a = e[k][0], b = e[k][1];
                if (a != u && b != u) continue;
                int v = a ^ b ^ u;
                ll W = 2 * e[k][2], opt = 0;
                int last = id[0];
                ll val = d[u][last];                           // centre at u itself
                for (int t = 1; t < n; t++) {                  // walk the envelope breakpoints
                    int i = id[t];
                    if (d[v][i] <= d[v][last]) continue;
                    ll cx = (W - d[u][i] + d[v][last]) / 2, cy = (W + d[u][i] + d[v][last]) / 2;
                    if (val > cy) val = cy, opt = cx;
                    last = i;
                }
                if (val > d[v][last]) val = d[v][last], opt = W;   // centre at v itself
                if (val < diameter) diameter = val, edge = k, offset = opt;
            }
        }
        distFromCentre.assign(n, llinf);                       // Dijkstra from the virtual centre
        int a = e[edge][0], b = e[edge][1];
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
        distFromCentre[a] = offset, distFromCentre[b] = 2 * e[edge][2] - offset;
        pq.push({distFromCentre[a], a}), pq.push({distFromCentre[b], b});
        vector<vector<pair<int, ll>>> g(n);
        for (auto [x, y, w] : e) g[x].push_back({(int)y, 2 * w}), g[y].push_back({(int)x, 2 * w});
        while (!pq.empty()) {
            auto [dd, x] = pq.top(); pq.pop();
            if (dd > distFromCentre[x]) continue;
            for (auto [y, w] : g[x]) if (dd + w < distFromCentre[y])
                distFromCentre[y] = dd + w, pq.push({distFromCentre[y], y});
        }
        return diameter;
    }
};
// RECOVERING THE TREE: take the shortest path tree of distFromCentre (each vertex keeps one
// incoming edge that is tight), plus the two halves of the centre edge. If offset is 0 or 2*w the
// centre is a vertex and the tree is just that vertex's shortest path tree.
// UNWEIGHTED graphs: the answer is the shortest path tree from the middle of the graph's own
// diameter path, and BFS from every vertex (O(VE)) is enough - do not bother with this file.
// MINIMUM RADIUS spanning tree is the same computation without the final doubling; the absolute
// 1-centre is the object both problems share.
// MINIMUM DIAMETER with a bound on the total weight, or "diameter <= k with fewest edges", is
// NP-hard - the polynomial case is exactly the unconstrained one above.
