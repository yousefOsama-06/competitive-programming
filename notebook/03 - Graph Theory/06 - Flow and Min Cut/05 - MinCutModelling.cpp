// Needs: Dinic (01 - Dinic.cpp).
// MIN-CUT MODELLING - the reason flow appears at ICPC. The code is never the hard part; choosing
// what the vertices and the infinite capacities mean is. Rule of thumb: if the objective contains
// an "infinity", you are CUTTING, not flowing.

// --- MAXIMUM WEIGHT CLOSURE / PROJECT SELECTION ---
// Choose a set S closed under the dependency arcs (if v in S and v -> u is a dependency, u in S)
// maximising the total weight. Build: s -> v with capacity w(v) for every w(v) > 0;
// v -> t with capacity -w(v) for every w(v) < 0; u -> v with INF for every dependency u needs v.
// Answer = (sum of positive weights) - maxflow.  S = the source side of the min cut.
// USES: "each project earns p but needs these machines that cost c"; "pick cells maximising
// profit but a chosen cell forces its neighbours"; densest-subgraph style selections.
ll maxClosure(int n, const vector<ll>& w, const vector<pair<int, int>>& need, vector<int>* chosen = nullptr) {
    Dinic din(n + 2);
    int s = n, t = n + 1;
    ll pos = 0;
    for (int v = 0; v < n; v++)
        if (w[v] > 0) pos += w[v], din.addEdge(s, v, w[v]);
        else if (w[v] < 0) din.addEdge(v, t, -w[v]);
    for (auto [u, v] : need) din.addEdge(u, v, llinf);         // choosing u forces choosing v
    ll cut = din.calc(s, t);
    if (chosen) for (int v = 0; v < n; v++) if (din.leftOfMinCut(v)) chosen->push_back(v);
    return pos - cut;
}
/* THE OTHER STANDARD REDUCTIONS
 TWO LABELS WITH PAIR PENALTIES ("cats or dogs", image segmentation)
   Every vertex gets label A or B. Cost a_v for A, b_v for B, and c_uv extra if u and v differ.
   Build s -> v with b_v, v -> t with a_v, and u <-> v with c_uv in both directions.
   The min cut is the minimum total cost; the source side takes label A.
   PRECONDITION: the pair penalty must be SUBMODULAR (cost(A,A) + cost(B,B) <= cost(A,B) +
   cost(B,A)). A penalty that rewards disagreement cannot be modelled by a cut - it is NP-hard.

 VERTEX CAPACITIES / VERTEX-DISJOINT PATHS
   Split v into v_in -> v_out with the vertex capacity. Vertex-disjoint s-t paths use capacity 1.

 BIPARTITE FAMILY (see 07 - Matching and 10 - Theorems)
   max matching        = max flow with unit capacities
   min vertex cover    = max matching (Konig)          -> recover via the alternating-BFS set
   max independent set = n - max matching
   min path cover of a DAG (vertex-disjoint) = n - max matching of the split graph

 "MUST BE ON THE SOURCE SIDE" -> add s -> v with INF.   "MUST BE ON THE SINK SIDE" -> v -> t INF.
 That is how you force decisions inside an otherwise free min cut.

 EDGE / VERTEX DISJOINT PATHS = Menger. Recover the paths by decomposing the integral flow.

 LOWER BOUNDS ON EDGES -> 03 - FlowWithLowerBounds.cpp.

 MAXIMUM DENSITY SUBGRAPH (maximise |E(S)| / |S|): binary search lambda, then min cut with
   s -> v capacity m, v -> t capacity m + 2*lambda - deg(v), u <-> v capacity 1. Feasible iff
   the min cut is < n*m. O(log(n^2) * maxflow) since the density is a ratio of integers <= n^2.

 MINIMUM CUT WITHOUT s AND t (global) -> 04 - StoerWagner.cpp, O(V^3), or n-1 max flows.

 ALL-PAIRS MIN CUT -> the Gomory-Hu tree: n-1 max flows build a weighted tree in which the min
   u-v cut equals the MINIMUM EDGE on the u-v tree path. Use it when many pairs are asked.

 COMPLEXITY TABLE - read this at minute 3, not minute 90
   Dinic                  O(V^2 E) general | O(E sqrt E) unit caps | O(E sqrt V) bipartite
   Edmonds-Karp           O(V E^2)                    -- do not
   Push-relabel / HLPP    O(V^2 sqrt E)               -- dense graphs, n ~ 1000, m ~ n^2/2
   MCMF (SSP + potentials) O(F * E log V)             -- F = total flow, so bound F first
   Hungarian              O(n^2 m)                    -- dense assignment, n <= ~1000
   Hopcroft-Karp          O(E sqrt V)                 -- bipartite matching, n >= few thousand
   Kuhn                   O(V E)                      -- shorter, fine to ~2000 vertices
   Blossom (general)      O(V^3)
   Stoer-Wagner           O(V^3)
   Gomory-Hu              O(V * maxflow)
 If the flow value itself can be huge (capacities up to 1e9), an O(F ...) bound is not a bound -
 scale the capacities or use a strongly polynomial algorithm. */
