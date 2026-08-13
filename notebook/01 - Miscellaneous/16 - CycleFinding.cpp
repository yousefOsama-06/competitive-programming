// CYCLE FINDING in a FUNCTIONAL GRAPH x -> f(x). Every component is a "rho": a tail feeding
// into a cycle. Answers: where does x end up after k steps, cycle length, distance to cycle.

// Floyd (tortoise & hare), O(1) memory. Returns {start index of the cycle, cycle length}.
template <class F> pair<ll, ll> floydCycle(ll x0, F f) {
    ll t = f(x0), h = f(f(x0));
    while (t != h) t = f(t), h = f(f(h));
    ll mu = 0;                                        // steps from x0 to the cycle entry
    t = x0;
    while (t != h) t = f(t), h = f(h), mu++;
    ll lam = 1;                                       // cycle length
    h = f(t);
    while (t != h) h = f(h), lam++;
    return {mu, lam};
}
// f applied k times, using the rho structure (k can be 1e18).
template <class F> ll iterate(ll x0, ll k, F f) {
    auto [mu, lam] = floydCycle(x0, f);
    if (k < mu) { while (k--) x0 = f(x0); return x0; }
    for (ll i = 0; i < mu; i++) x0 = f(x0);
    ll rem = (k - mu) % lam;
    while (rem--) x0 = f(x0);
    return x0;
}
// WHOLE-GRAPH version when f is given as an array over 0..n-1: colour DFS, O(n).
// state: 0 unvisited, 1 on the current path, 2 done.  Fills cycId/cycPos/distToCycle.
struct FuncGraph {
    int n;
    vector<int> f, state, cyc, pos, dist, cycLen;     // cyc[v] = id of the cycle v reaches
    FuncGraph(vector<int> f) : n(f.size()), f(f), state(n, 0), cyc(n, -1), pos(n, -1), dist(n, 0) {
        for (int i = 0; i < n; i++) if (!state[i]) {
            vector<int> path;
            int v = i;
            while (!state[v]) state[v] = 1, path.push_back(v), v = f[v];
            if (state[v] == 1) {                      // found a NEW cycle, starting at v
                int id = cycLen.size(), len = 0;
                for (int u = v;; u = f[u]) { cyc[u] = id, pos[u] = len++; if (f[u] == v) break; }
                cycLen.push_back(len);
            }
            for (int j = path.size() - 1; j >= 0; j--) {
                int u = path[j];
                state[u] = 2;
                if (cyc[u] == -1) cyc[u] = cyc[f[u]], dist[u] = dist[f[u]] + 1;
            }
        }
    }
    int kth(int v, ll k) {                            // f^k(v)
        while (k > 0 && dist[v] > 0) v = f[v], k--;
        if (!k) return v;
        int L = cycLen[cyc[v]], p = (pos[v] + k % L) % L, u = v;
        for (int i = 0; i < (p - pos[v] + L) % L; i++) u = f[u];
        return u;
    }
};
// USES: "apply the permutation k times", Pollard's rho, "where does the ball stop",
// detecting a repeat in a modular sequence, linked-list loop detection.
