// STOER-WAGNER GLOBAL MIN CUT - minimum cut over ALL vertex pairs, undirected weighted, O(V^3).
// No source/sink. Returns {cut weight, one side of the cut}.
// Key lemma: in a maximum-adjacency order, if s is the second-to-last and t the last vertex
// added, then the minimum s-t cut is exactly {t}. Merge s and t and repeat n-1 times.
pair<ll, vector<int>> globalMinCut(vector<vector<ll>> w) {
    int n = w.size();
    if (n < 2) return {0, {}};
    vector<char> gone(n, 0);
    vector<vector<int>> grp(n);
    for (int i = 0; i < n; i++) grp[i] = {i};
    ll bestW = llinf;
    vector<int> best;
    for (int phase = 0; phase + 1 < n; phase++) {
        vector<ll> ww(n, 0);
        vector<char> added(n, 0);
        int prev = -1, last = -1;
        for (int it = 0; it < n - phase; it++) {       // maximum-adjacency order
            int sel = -1;
            for (int j = 0; j < n; j++)
                if (!gone[j] && !added[j] && (sel < 0 || ww[j] > ww[sel])) sel = j;
            added[sel] = 1, prev = last, last = sel;
            for (int j = 0; j < n; j++) if (!gone[j] && !added[j]) ww[j] += w[sel][j];
        }
        ll cut = 0;                                    // cut-of-the-phase separates {last}
        for (int j = 0; j < n; j++) if (!gone[j] && j != last) cut += w[last][j];
        if (cut < bestW) bestW = cut, best = grp[last];
        gone[last] = 1;                                // merge last into prev
        grp[prev].insert(grp[prev].end(), all(grp[last]));
        for (int j = 0; j < n; j++) if (!gone[j] && j != prev)
            w[prev][j] += w[last][j], w[j][prev] = w[prev][j];
    }
    return {bestW, best};
}
// USES: "split the network into two non-empty parts as cheaply as possible", edge connectivity
// of a weighted undirected graph, "minimum number of edges to delete to disconnect the graph"
// (unit weights). Doing this with n-1 max-flows is both slower and longer to write.
// KARGER (randomized contraction) finds a min cut with probability >= 1/C(n,2) per run; the
// useful corollary is that a graph has AT MOST C(n,2) distinct minimum cuts.
