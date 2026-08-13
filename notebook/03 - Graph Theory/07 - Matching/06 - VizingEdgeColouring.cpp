// VIZING EDGE COLOURING - colour the edges of a SIMPLE undirected graph with at most D+1 colours
// (D = maximum degree), constructively, in O(n * m). 0-indexed in, colours returned 0-indexed.
// VIZING'S THEOREM says D or D+1 always suffices; deciding WHICH is NP-hard, so D+1 is the answer
// you ship. (Bipartite graphs always need only D - use 05 instead, it is much shorter.)
// Reach for it for "split the matches/tasks into as few rounds as possible so no player/machine is
// used twice per round" on a general graph, and for "orient/partition the edges into D+1 matchings"
// arguments. A D+1 colouring also proves the D+1 bound constructively, which some problems ask for.
// MULTIGRAPHS are NOT covered (Shannon's bound is 3D/2 there); remove parallel edges first.
// Method: insert the edges one at a time. For a new edge (u, v) build a MAXIMAL FAN around u -
// a sequence of neighbours where each one's free colour is used by the next - and either rotate the
// fan (when some colour is free at both ends) or flip an alternating two-colour path and rotate.
// C[u][c] = the neighbour joined to u by colour c (0 = none); X[u] = smallest free colour at u.
vector<int> vizing(int n, const vector<pair<int, int>>& e) {
    int D = 0;
    vector<int> deg(n, 0);
    for (auto [u, v] : e) D = max({D, ++deg[u], ++deg[v]});
    int K = D + 2;                                             // colours 1..D+1, 0 = uncoloured
    vector<vector<int>> C(n, vector<int>(K + 1, 0)), col(n, vector<int>(n, 0));
    vector<int> X(n, 1);
    auto update = [&](int u) { for (X[u] = 1; C[u][X[u]]; X[u]++); };
    auto color = [&](int u, int v, int c) {                    // give edge (u,v) colour c
        int p = col[u][v];
        col[u][v] = col[v][u] = c, C[u][c] = v, C[v][c] = u, C[u][p] = C[v][p] = 0;
        if (p) X[u] = X[v] = p; else update(u), update(v);
        return p;
    };
    auto flip = [&](int u, int c1, int c2) {                   // swap colours c1/c2 at u
        int p = C[u][c1];
        swap(C[u][c1], C[u][c2]);
        if (p) col[u][p] = col[p][u] = c2;
        if (!C[u][c1]) X[u] = c1;
        if (!C[u][c2]) X[u] = c2;
        return p;
    };
    for (int t = 0; t < (int)e.size(); t++) {
        int u = e[t].first, v0 = e[t].second, v = v0, c0 = X[u], c = c0, d = 0, a;
        vector<pair<int, int>> L;                              // the fan: (neighbour, its free col)
        vector<char> vis(K + 1, 0);
        while (!col[u][v0]) {
            L.push_back({v, d = X[v]});
            if (!C[v][c]) for (a = (int)L.size() - 1; a >= 0; a--) c = color(u, L[a].first, c);
            else if (!C[u][d])
                for (a = (int)L.size() - 1; a >= 0; a--) color(u, L[a].first, L[a].second);
            else if (vis[d]) break;                            // fan is maximal -> flip a path
            else vis[d] = 1, v = C[u][d];
        }
        if (!col[u][v0]) {
            for (; v; v = flip(v, c, d), swap(c, d));           // alternating c/d path from v
            if (C[u][c0]) {
                for (a = (int)L.size() - 2; a >= 0 && L[a].second != c; a--);
                for (; a >= 0; a--) color(u, L[a].first, L[a].second);
            } else t--;                                        // retry this edge, now it fits
        }
    }
    vector<int> res(e.size());
    for (int i = 0; i < (int)e.size(); i++) res[i] = col[e[i].first][e[i].second] - 1;
    return res;                                                // res[i] in [0, D]
}
// D IS ENOUGH (class 1) for: bipartite graphs, every graph with a vertex of degree D that is not
// "overfull", and every planar graph with D >= 7. D+1 is forced (class 2) for odd cycles, the
// Petersen graph, and any OVERFULL graph (m > D * floor(n/2)) - a cheap sufficient test.
// EDGE COLOURING = VERTEX COLOURING OF THE LINE GRAPH, so chi'(G) = chi(L(G)); that is usually the
// wrong direction to compute, but it is the right way to see a problem stated on the line graph.
// MEMORY is O(n^2) for the col matrix; at n ~ 2000 that is 16 MB of int - use short if it is tight.
