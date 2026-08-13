// MIN COST MAX FLOW BY COST SCALING (push-relabel), O(V^2 E log(V*C)) but very fast in practice -
// 3e4 edges are comfortable. 0-indexed, INTEGER costs and capacities.
// WHY THIS EXISTS ALONGSIDE 02 - MCMF. The successive-shortest-path version needs the residual
// graph to have NO NEGATIVE CYCLE, so it forbids negative-cost edges (Johnson potentials only fix
// negative EDGES on a DAG-like start, not negative CYCLES). This one accepts ARBITRARY costs,
// including negative cycles: it first pushes a max flow ignoring cost, then repeatedly "refines"
// the eps-optimality of the flow, halving eps each round. It also does not depend on the flow
// VALUE, so a huge max flow is fine, whereas SSP is O(F * E log V).
// Reach for it when: costs can be negative, or the flow value is large, or the graph is dense.
// The source and sink are fixed at construction. Costs are internally multiplied by V, so keep
// |cost| * V * V inside the cost type - use long long unless you have checked.
struct CostScalingMCMF {
    struct Edge { ll c, f; int to, rev; };
    ll eps = 0;
    int n, S, T;
    vector<vector<Edge>> g;
    vector<vector<int>> hs;
    vector<int> cur, isq, co;
    vector<ll> ex, h;
    CostScalingMCMF(int n, int s, int t) : n(n), S(s), T(t), g(n) {}
    void addEdge(int a, int b, ll cost, ll cap) {              // cost per unit, cap >= 0
        if (a == b) return;
        cost *= n, eps = max(eps, abs(cost));
        g[a].push_back({cost, cap, b, (int)g[b].size()});
        g[b].push_back({-cost, 0, a, (int)g[a].size() - 1});
    }
    void addFlow(int u, Edge& e, ll f) {
        Edge& bk = g[e.to][e.rev];
        if (!ex[e.to] && f) hs[h[e.to]].push_back(e.to);
        e.f -= f, ex[e.to] += f, bk.f += f, ex[u] -= f;
    }
    ll maxFlow() {                                             // plain highest-label push-relabel
        ex.assign(n, 0), h.assign(n, 0), hs.assign(2 * n + 1, {}), co.assign(2 * n + 1, 0);
        cur.assign(n, 0), h[S] = n, ex[T] = 1, co[0] = n - 1;
        for (auto& e : g[S]) addFlow(S, e, e.f);
        if (hs[0].size()) for (int hi = 0; hi >= 0;) {
            int u = hs[hi].back(); hs[hi].pop_back();
            while (ex[u] > 0) {
                if (cur[u] == (int)g[u].size()) {
                    h[u] = 2 * n;
                    for (int i = 0; i < (int)g[u].size(); i++)
                        if (g[u][i].f && h[u] > h[g[u][i].to] + 1)
                            h[u] = h[g[u][i].to] + 1, cur[u] = i;
                    if (++co[h[u]], !--co[hi] && hi < n)
                        for (int i = 0; i < n; i++)
                            if (hi < h[i] && h[i] < n) --co[h[i]], h[i] = n + 1;
                    hi = h[u];
                } else if (g[u][cur[u]].f && h[u] == h[g[u][cur[u]].to] + 1)
                    addFlow(u, g[u][cur[u]], min(ex[u], g[u][cur[u]].f));
                else ++cur[u];
            }
            while (hi >= 0 && hs[hi].empty()) --hi;
        }
        return -ex[S];
    }
    void push(int u, Edge& e, ll amt) {
        amt = min(amt, e.f);
        e.f -= amt, ex[e.to] += amt, g[e.to][e.rev].f += amt, ex[u] -= amt;
    }
    void relabel(int v) {
        ll nh = -llinf;
        for (int i = 0; i < (int)g[v].size(); i++)
            if (g[v][i].f && nh < h[g[v][i].to] - g[v][i].c)
                nh = h[g[v][i].to] - g[v][i].c, cur[v] = i;
        h[v] = nh - eps;
    }
    pair<ll, ll> solve() {                                     // {max flow, its minimum cost}
        ll cost = 0;
        for (int i = 0; i < n; i++) for (Edge& e : g[i]) cost += e.c * e.f;
        ll flow = maxFlow();
        h.assign(n, 0), ex.assign(n, 0), isq.assign(n, 0), cur.assign(n, 0);
        queue<int> q;
        for (; eps; eps >>= 2) {                               // refine: halve eps twice per round
            fill(all(cur), 0);
            for (int i = 0; i < n; i++) for (auto& e : g[i])
                if (h[i] + e.c - h[e.to] < 0 && e.f) push(i, e, e.f);
            for (int i = 0; i < n; i++) if (ex[i] > 0) q.push(i), isq[i] = 1;
            while (!q.empty()) {                               // restore feasibility
                int u = q.front(); q.pop(), isq[u] = 0;
                while (ex[u] > 0) {
                    if (cur[u] == (int)g[u].size()) relabel(u);
                    for (int& i = cur[u]; i < (int)g[u].size(); i++) {
                        Edge& e = g[u][i];
                        if (h[u] + e.c - h[e.to] < 0) {
                            push(u, e, ex[u]);
                            if (ex[e.to] > 0 && !isq[e.to]) q.push(e.to), isq[e.to] = 1;
                            if (!ex[u]) break;
                        }
                    }
                }
            }
            if (eps > 1 && (eps >> 2) == 0) eps = 4;
        }
        for (int i = 0; i < n; i++) for (Edge& e : g[i]) cost -= e.c * e.f;
        return {flow, cost / 2 / n};
    }
    ll flowOn(const Edge& e) const { return g[e.to][e.rev].f; }
};
// MIN COST FLOW OF A GIVEN VALUE k (not necessarily maximum): cap the source arc at k, or add an
// arc T -> S of capacity INF and cost -BIG and take a min cost CIRCULATION.
// NEGATIVE COST CYCLES in the input are handled: the answer is the true minimum, which may be
// negative. With 02 - MCMF you would have to cancel the cycles by hand first (saturate every
// negative edge, then fix the resulting excesses with a circulation) - that is what this replaces.
// ASSIGNMENT PROBLEMS with n <= 500 are still better served by Hungarian (07 - Matching/03).
