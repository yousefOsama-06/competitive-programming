// Needs: MCMF (02 - MCMF.cpp).
// MIN COST FLOW WITH LOWER BOUNDS (min cost circulation with demands). 0-indexed, costs >= 0.
// This is 03 - FlowWithLowerBounds with a price tag on every unit: each edge needs
// lo <= f(e) <= hi and costs c per unit, and among all FEASIBLE flows you want the cheapest.
// Reach for it for "every task must be done at least once, minimise total cost", "each shift needs
// between L and R workers", "cover every row of the matrix at least k times as cheaply as
// possible", and generally any min-cut/flow model where a constraint reads "at least".
// CONSTRUCTION. Send the mandatory lo units for free by hand, then repair the resulting imbalance:
//   base cost += lo * c ;  the arc keeps capacity hi - lo at cost c
//   excess(v) = (sum of lo entering v) - (sum of lo leaving v)
//   excess(v) > 0 -> SS -> v with capacity excess(v), cost 0;  < 0 -> v -> TT, cost -excess(v)
// One min cost max flow from SS to TT. FEASIBLE iff it saturates every SS arc; the answer is
// base + that cost. For an s-t flow, add t -> s with capacity INF and cost 0 FIRST.
struct MinCostLowerBounds {
    int n, S, T;
    MCMF f;
    ll base = 0, need = 0;
    vector<ll> low, exc;
    vector<array<int, 2>> eid;                                 // {u, index in f.g[u]} per edge
    MinCostLowerBounds(int n) : n(n), S(n), T(n + 1), f(n + 2), exc(n + 2, 0) {}
    void addEdge(int u, int v, ll lo, ll hi, ll cost) {
        low.push_back(lo), eid.push_back({u, (int)f.g[u].size()});
        f.addEdge(u, v, hi - lo, cost);
        base += lo * cost, exc[v] += lo, exc[u] -= lo;
    }
    pair<bool, ll> solve() {                                   // {feasible, minimum total cost}
        for (int v = 0; v < n; v++) {
            if (exc[v] > 0) f.addEdge(S, v, exc[v], 0), need += exc[v];
            else if (exc[v] < 0) f.addEdge(v, T, -exc[v], 0);
        }
        auto [fl, c] = f.run(S, T);
        return {fl == need, base + c};
    }
    ll flowOn(int i) { return low[i] + f.g[eid[i][0]][eid[i][1]].flow; }
};
// MIN COST *MAX* s-t FLOW WITH BOUNDS: add t -> s with capacity INF and cost 0, run solve(); the
// flow that ended up on that arc is the feasible s-t flow value. Then delete the arc and run one
// more MCMF from s to t on the residual to push the flow up to maximum.
// MIN COST *MIN* s-t FLOW: same, but afterwards cancel flow by running MCMF from t to s.
// NEGATIVE COSTS: this construction is still correct, but MCMF's Dijkstra is not - either call
// f.setPotentialsBF(S) first (no negative cycle allowed) or switch to 08 - CostScalingMCMF.
// b-FLOW (each vertex has a supply/demand b(v) instead of a single source): put b(v) directly into
// exc[v] before solving - the SS/TT construction is exactly the same machinery.
// SANITY CHECK: if lo > hi on any edge the instance is infeasible; the code will not notice.
