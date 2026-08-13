// Needs: Dinic (01 - Dinic.cpp).
// IS THE MINIMUM s-t CUT UNIQUE? One extra pair of DFS over the residual graph, O(V + E) after the
// max flow. 0-indexed, directed or undirected.
// After a max flow, let A = the vertices REACHABLE FROM s in the residual graph (the smallest
// minimum cut) and B = the vertices that can REACH t in the residual graph (the largest minimum
// cut's source side is V \ B). Every minimum cut lies "between" these two, so:
//   the minimum cut is UNIQUE  <=>  A and B together cover every vertex.
// A vertex in neither set can be put on either side without changing the cut value, and moving it
// gives a genuinely different cut of the same weight.
// Reach for it for "is the answer unique / is the bottleneck forced", "which edges are in EVERY
// minimum cut", and for problems that ask you to output AMBIGUOUS when a tie exists.
bool uniqueMinCut(Dinic& din, int s, int t) {                  // call AFTER din.calc(s, t)
    int n = din.adj.size();
    vector<char> a(n, 0), b(n, 0);
    vector<int> st{s};
    a[s] = 1;
    while (!st.empty()) {                                      // forward residual reachability
        int u = st.back(); st.pop_back();
        for (auto& e : din.adj[u]) if (e.c > 0 && !a[e.to]) a[e.to] = 1, st.push_back(e.to);
    }
    st = {t}, b[t] = 1;
    while (!st.empty()) {                                      // backward: residual arcs INTO u
        int u = st.back(); st.pop_back();
        for (auto& e : din.adj[u]) if (din.adj[e.to][e.rev].c > 0 && !b[e.to])
            b[e.to] = 1, st.push_back(e.to);
    }
    for (int v = 0; v < n; v++) if (!a[v] && !b[v]) return false;
    return true;
}
// THE TWO EXTREME CUTS come out of the same two searches: A is the source side of the minimum cut
// with the FEWEST source-side vertices, and V \ B is the one with the MOST.
// AN EDGE IS IN SOME MINIMUM CUT iff it is saturated and its endpoints are in different strongly
// connected components of the residual graph (contract the residual SCCs and the min cuts are
// exactly the closed sets of the resulting DAG).
// AN EDGE IS IN EVERY MINIMUM CUT iff it is saturated and removing its capacity lowers the max
// flow, i.e. iff u is in A and v is in B - checkable directly from these two arrays.
// COUNTING MINIMUM CUTS = counting antichains/closed sets in that residual-SCC DAG, which is
// #P-hard in general but easy when the DAG is a path or has few vertices.
