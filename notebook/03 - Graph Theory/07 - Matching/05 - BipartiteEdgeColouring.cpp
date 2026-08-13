// BIPARTITE EDGE COLOURING in exactly D colours (D = maximum degree), O(m * D). 0-indexed.
// KONIG'S EDGE COLOURING THEOREM: a bipartite (multi)graph is always D-edge-colourable, and D is
// obviously necessary. Equivalently: the edges split into D perfect matchings of the D-regular
// completion, which is the "round robin timetable" primitive.
// Reach for it for "schedule all the games/lectures in the fewest rounds so nobody plays twice in
// one round", "decompose a bipartite multigraph into matchings", "Latin square completion",
// and for the routing step of a Clos/Benes network.
// The insertion argument: give edge (u, v) the smallest colour cu free at u and look at the
// smallest colour cv free at v. If they differ, the cu/cv alternating path starting at v cannot
// reach u (it would have to ENTER u by cu, which is free there), so flipping the two colours
// along it frees cu at v as well - then colour the edge cu.
vector<int> bipartiteEdgeColouring(int nl, int nr, const vector<pair<int, int>>& e) {
    int m = e.size(), D = 0;
    vector<int> dl(nl, 0), dr(nr, 0);
    for (auto [u, v] : e) D = max({D, ++dl[u], ++dr[v]});
    vector<vector<int>> L(nl, vector<int>(D, -1)), R(nr, vector<int>(D, -1));   // colour -> edge id
    vector<int> col(m, -1);
    for (int i = 0; i < m; i++) {
        auto [u, v] = e[i];
        int cu = 0, cv = 0;
        while (L[u][cu] >= 0) cu++;
        while (R[v][cv] >= 0) cv++;
        if (cu != cv) {                                        // walk the cu/cv alternating path
            vector<int> path;
            for (int x = v, side = 1, c = cu;;) {
                int id = side ? R[x][c] : L[x][c];
                if (id < 0) break;
                path.push_back(id);
                x = side ? e[id].first : e[id].second, side ^= 1, c = (c == cu ? cv : cu);
            }
            for (int id : path) L[e[id].first][col[id]] = R[e[id].second][col[id]] = -1;
            for (int id : path) {
                col[id] = (col[id] == cu ? cv : cu);
                L[e[id].first][col[id]] = R[e[id].second][col[id]] = id;
            }
        }
        col[i] = cu, L[u][cu] = R[v][cu] = i;
    }
    return col;                                                // col[i] in [0, D)
}
// The number of colours actually used IS the maximum degree - no search needed.
// EQUAL-SIZE ROUNDS: to split the edges into k groups whose sizes differ by at most one and where
// each vertex has at most ceil(deg/k) edges per group, split every vertex into ceil(deg/k) copies
// of degree <= k and edge-colour that graph.
// GENERAL (non-bipartite) graphs need D or D+1 colours - Vizing (06), and deciding which is
// NP-hard. On a bipartite graph the answer is always D, which is why this file is separate.
// A REGULAR bipartite graph decomposes into perfect matchings; taking the colour classes one at a
// time is much faster than running a matching algorithm D times.
