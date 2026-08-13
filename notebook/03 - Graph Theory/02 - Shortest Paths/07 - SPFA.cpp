// SPFA (Bellman-Ford with a queue) - shortest paths with NEGATIVE edges, plus negative-cycle
// detection. O(V*E) worst case and constructible-against, but usually far faster than plain
// Bellman-Ford because only vertices whose distance actually improved get re-examined.
// Returns false iff a negative cycle is reachable from s: a vertex relaxed n times must be on
// (or reachable from) one, since a shortest walk uses at most n-1 edges.
// WHEN: negative weights and you also want the cycle check. With NON-negative weights always use
// Dijkstra - SPFA is asymptotically worse and hackable. For a guaranteed bound use Bellman-Ford.
bool spfa(int n, int s, const vector<vector<pair<int, ll>>>& g, vector<ll>& d) {
    d.assign(n, llinf);
    vector<int> cnt(n, 0);
    vector<char> inq(n, 0);
    deque<int> q;
    d[s] = 0, q.push_back(s), inq[s] = 1;
    while (!q.empty()) {
        int v = q.front(); q.pop_front();
        inq[v] = 0;
        for (auto [to, w] : g[v]) if (d[v] + w < d[to]) {
            d[to] = d[v] + w;
            if (!inq[to]) {
                if (++cnt[to] >= n) return false;              // negative cycle
                inq[to] = 1;
                // SLF: a smaller tentative distance goes to the FRONT. Cheap, and it is what
                // makes SPFA fast in practice; without it the queue degenerates.
                if (!q.empty() && d[to] < d[q.front()]) q.push_front(to);
                else q.push_back(to);
            }
        }
    }
    return true;
}
// RECOVERING THE CYCLE: keep par[] and, on detection, walk n parent pointers back (that lands you
// ON the cycle - the detected vertex may only be reachable from it), then follow parents until a
// vertex repeats. See 06 - CyclesAndConstraints.cpp, which uses exactly this for difference
// constraints and for the minimum mean cycle.
// KILLER TESTS EXIST: grid-like graphs make SPFA quadratic. If the problem looks adversarial,
// use Bellman-Ford (predictable O(VE)) or reweight with Johnson potentials and run Dijkstra.
