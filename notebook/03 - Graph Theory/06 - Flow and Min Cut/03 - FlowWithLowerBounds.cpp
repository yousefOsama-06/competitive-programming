// FLOWS WITH LOWER BOUNDS (circulation with demands). Every edge needs d(e) <= f(e) <= c(e).
// Build an auxiliary network, run one max flow; feasible iff every edge out of s' saturates.
//   excess(v) = (sum of d over edges INTO v) - (sum of d over edges OUT of v)
//   excess(v) > 0 -> edge s'->v of capacity excess(v);  < 0 -> edge v->t' of capacity -excess
//   every original edge (u,v) keeps capacity c - d
// For an s-t flow, add t->s with capacity INF first, which turns it into a circulation.
// Needs Dinic (01 - Dinic.cpp) with addEdge(u, v, cap) and calc(s, t).
struct LowerBoundFlow {
    int n, S, T;                                      // S, T are the auxiliary super source/sink
    Dinic din;
    vector<ll> excess, low;
    vector<array<int, 2>> eid;                        // per original edge: {u, index in din.adj[u]}

    LowerBoundFlow(int n) : n(n), S(n), T(n + 1), din(n + 2), excess(n + 2, 0) {}
    void addEdge(int u, int v, ll lo, ll hi) {
        low.push_back(lo);
        eid.push_back({u, (int)din.adj[u].size()});
        din.addEdge(u, v, hi - lo);
        excess[v] += lo, excess[u] -= lo;
    }
    bool feasible() {                                 // circulation only (no s, t)
        ll need = 0;
        for (int v = 0; v < n; v++) {
            if (excess[v] > 0) din.addEdge(S, v, excess[v]), need += excess[v];
            else if (excess[v] < 0) din.addEdge(v, T, -excess[v]);
        }
        return din.calc(S, T) == need;
    }
    // Min / max s-t flow subject to the bounds - see the RECIPES block below for the ONLY
    // correct order of operations. You must DELETE the t->s edge (zero both residual directions)
    // before the second max flow, otherwise Dinic's first augmenting path runs s -> t backwards
    // along that arc and CANCELS the feasible flow instead of extending it.
    // feasible() appends the S/T edges every time it is called - call it exactly once.
    ll flowOn(int i) { return low[i] + din.adj[eid[i][0]][eid[i][1]].flow(); }
};
/* RECIPES
   "each edge must be used at least once"          -> d(e) = 1
   "each vertex must be visited between L and R"   -> split v into v_in -> v_out with [L, R]
   "exact degree subgraph"                         -> d = c = required degree on the split edge
   MAX s-t FLOW WITH BOUNDS: add t->s with capacity INF, run feasible(); then delete that edge
     and run one more max flow from s to t on the residual graph. The answer is the flow that
     was on t->s plus the extra augmentation.
   MIN s-t FLOW WITH BOUNDS: same, but augment from t to s to cancel as much as possible. */
