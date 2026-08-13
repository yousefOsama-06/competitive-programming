// MATROID INTERSECTION - the largest set that is independent in TWO matroids at once, and the
// max-weight version. Ground set is {0..n-1}; each matroid arrives as an ORACLE object exposing
//   build(S)      prepare for the current independent set S (a sorted vector of element ids)
//   canAdd(e)     is S + e independent?          (e not in S)
//   canSwap(e, x) is S - x + e independent?      (e not in S, x in S)
// build is called once per augmenting phase, so it may rebuild everything from scratch - that is
// exactly why the concrete oracles below can get away with |S| + 1 rebuilt structures per phase.
// matroidIsect returns a maximum-CARDINALITY common independent set, sorted, as element ids.
// matroidIsectW returns a maximum-WEIGHT one over all cardinalities; force maximum cardinality
// first by adding C > sum|w| to every weight.
// WHEN: one selection problem carrying TWO independent structural constraints. Colourful spanning
// tree / spanning tree with at most c_i edges of colour i, and rainbow spanning tree (c_i = 1):
// graphic + partition. Arborescence: graphic on the underlying undirected graph + partition by
// head vertex with cap 1, so "one incoming edge per vertex, no cycle". Spanning tree with bounded
// degrees: partition the edges by an endpoint - legal ONLY if no edge joins two constrained
// vertices, otherwise an edge lands in two classes and the reduction is silently wrong. Bipartite
// matching is two partition matroids (but use Hopcroft-Karp). THREE matroids is NP-hard.
// MIN-MAX: max |I| = min over U of (rank1(U) + rank2(complement of U)), handy for proving a bound
// or for a "why is the answer stuck at k" argument.
// COMPLEXITY in oracle calls: one phase pops each element once and spends O(|S|) or O(n) calls on
// it, so O(r n) calls plus 2 builds per phase, and there are r + 1 phases: O(r^2 n) calls total.
// O(r^1.5 n) is the best known bound for this scheme and needs Cunningham's phase batching
// (augment along many shortest paths per BFS, Hopcroft-Karp style) - rarely worth typing. Budget
// r * n * (cost of one oracle call) per phase and multiply by the number of phases. Weighted:
// Bellman-Ford over n nodes and O(r n) arcs, so O(r n^2) per phase on top of the same oracle work.
// Memory is the oracles': O(r V) ints for graphic, O(63 r) for xor, O(colours) for partition.
// DEGENERATE: n = 0 or every element a LOOP (self-loop edge, zero vector, colour with cap 0)
// returns {}; loops fail canAdd on an empty S and are never picked. Duplicate / parallel elements
// are fine. Ties are broken arbitrarily - no lexicographic guarantee. Weighted with all weights
// negative returns {}. Both routines start from S = {} and cost nothing extra if the answer is 0.
// EXACT: pure combinatorics, no floating point; weights are ll and one path cost sums at most n of
// them, so |w| <= 1e17 is safe. Two traps that fail SILENTLY: the unweighted search must use BFS
// (a shortest augmenting path, fewest arcs) and the weighted one must break cost ties by fewest
// arcs - swap in a DFS or drop the tiebreak and you get a set that is not independent.
template <class M1, class M2>
vector<int> matroidIsect(int n, M1& m1, M2& m2) {
    vector<char> in(n, 0);
    vector<int> S;
    for (;;) {
        m1.build(S), m2.build(S);
        vector<int> par(n, -2), q;                    // -2 unvisited, -1 source; q is the BFS order
        int t = -1;
        for (int e = 0; e < n; e++) if (!in[e] && m1.canAdd(e)) par[e] = -1, q.pb(e);
        for (int h = 0; h < sz(q) && t < 0; h++) {
            int u = q[h];
            if (in[u]) {                              // leaving S: u out, some v in, keep M1 happy
                for (int v = 0; v < n; v++)
                    if (par[v] == -2 && !in[v] && m1.canSwap(v, u)) par[v] = u, q.pb(v);
            } else if (m2.canAdd(u)) t = u;           // reached the sink side: augment
            else for (int v : S) if (par[v] == -2 && m2.canSwap(u, v)) par[v] = u, q.pb(v);
        }
        if (t < 0) return S;                          // no augmenting path: S is maximum
        for (int v = t; v != -1; v = par[v]) in[v] ^= 1;
        S.clear();
        for (int e = 0; e < n; e++) if (in[e]) S.pb(e);
    }
}
template <class M1, class M2>
vector<int> matroidIsectW(int n, vector<ll> w, M1& m1, M2& m2) {
    vector<char> in(n, 0);
    vector<int> S, best;
    ll cur = 0, bw = 0;
    for (;;) {
        m1.build(S), m2.build(S);
        vector<pair<ll, int>> d(n, {llinf, inf});     // (path cost, arcs) - lexicographic
        vector<int> par(n, -1);
        vector<pii> ed;
        vector<char> snk(n, 0);
        for (int e = 0; e < n; e++) if (!in[e]) {
            if (m1.canAdd(e)) d[e] = {-w[e], 1};      // sources; a node costs -w in, +w out
            if (m2.canAdd(e)) snk[e] = 1;
            for (int x : S) {
                if (m1.canSwap(e, x)) ed.pb({x, e});
                if (m2.canSwap(e, x)) ed.pb({e, x});
            }
        }
        for (int it = 0; it < n; it++) {              // Bellman-Ford: weights are negative on entry
            bool ch = false;
            for (auto& [u, v] : ed) if (d[u].fi < llinf) {
                pair<ll, int> nd = {d[u].fi + (in[v] ? w[v] : -w[v]), d[u].se + 1};
                if (nd < d[v]) d[v] = nd, par[v] = u, ch = true;
            }
            if (!ch) break;
        }
        int t = -1;
        for (int e = 0; e < n; e++) if (snk[e] && d[e].fi < llinf && (t < 0 || d[e] < d[t])) t = e;
        if (t < 0) break;
        cur -= d[t].fi;                               // the path cost is minus the weight gained
        for (int v = t; v != -1; v = par[v]) in[v] ^= 1;
        S.clear();
        for (int e = 0; e < n; e++) if (in[e]) S.pb(e);
        if (cur > bw) bw = cur, best = S;             // S is now optimal among sets of size |S|
    }
    return best;
}
// --- ORACLE 1: COLOURFUL / PARTITION matroid. Independent iff colour c is used <= cap[c] times.
struct ColourM {
    vector<int> col, cap, cnt;                        // col[e] in [0, |cap|)
    ColourM(vector<int> col, vector<int> cap) : col(col), cap(cap), cnt(cap.size()) {}
    void build(const vector<int>& S) {
        fill(all(cnt), 0);
        for (int e : S) cnt[col[e]]++;
    }
    bool canAdd(int e) { return cnt[col[e]] < cap[col[e]]; }
    bool canSwap(int e, int x) { return canAdd(e) || col[e] == col[x]; }
};
// --- ORACLE 2: GRAPHIC matroid. Ground set = edges over vertices 0..V-1, independent iff acyclic.
// One DSU for S and one for each S - x, all rebuilt per phase: O(r^2) per build, O(r V) memory.
struct GraphicM {
    int V; vector<pii> e; vector<int> pos; vector<vi> d;   // d[0] = S, d[i + 1] = S without S[i]
    GraphicM(int V, vector<pii> e) : V(V), e(e) {}
    int f(vi& p, int x) { return p[x] == x ? x : p[x] = f(p, p[x]); }
    bool ok(vi& p, int i) { return f(p, e[i].fi) != f(p, e[i].se); }
    void build(const vector<int>& S) {
        int m = sz(S);
        pos.assign(sz(e), 0), d.assign(m + 1, vi(V));
        for (auto& p : d) iota(all(p), 0);
        for (int i = 0; i < m; i++) pos[S[i]] = i;
        for (int i = 0; i < m; i++) for (int j = 0; j <= m; j++) if (j != i + 1) {
            vi& p = d[j];
            p[f(p, e[S[i]].fi)] = f(p, e[S[i]].se);   // union; a no-op when already joined
        }
    }
    bool canAdd(int x) { return ok(d[0], x); }
    bool canSwap(int x, int y) { return ok(d[pos[y] + 1], x); }
};
// --- ORACLE 3: LINEAR matroid over GF(2). Ground set = masks, independent iff xor-independent.
// b[j][i] is the basis vector whose top bit is i. Masks must be in [0, 2^62); 0 is a loop.
struct XorM {
    vector<ll> v; vector<int> pos; vector<vector<ll>> b;   // b[0] = S, b[i + 1] = S without S[i]
    XorM(vector<ll> v) : v(v) {}
    bool ok(vector<ll>& b, ll x, bool ins = false) {
        for (int i = 62; i >= 0; i--) if (x >> i & 1) {
            if (!b[i]) { if (ins) b[i] = x; return true; }
            x ^= b[i];
        }
        return false;
    }
    void build(const vector<int>& S) {
        int m = sz(S);
        pos.assign(sz(v), 0), b.assign(m + 1, vector<ll>(63, 0));
        for (int i = 0; i < m; i++) pos[S[i]] = i;
        for (int i = 0; i < m; i++)
            for (int j = 0; j <= m; j++) if (j != i + 1) ok(b[j], v[S[i]], true);
    }
    bool canAdd(int x) { return ok(b[0], v[x]); }
    bool canSwap(int x, int y) { return ok(b[pos[y] + 1], v[x]); }
};
// MORE ORACLES, all trivial: UNIFORM U(k, n) - canAdd = sz(S) < k, canSwap = true. FREE matroid
// (everything independent) - both true; intersecting with it degenerates to "greedy on one
// matroid". TRANSVERSAL matroid (elements matchable into a bipartite graph) - one Kuhn augmenting
// path per query, legal but slow. COGRAPHIC (complement stays connected).
// ONE matroid only, max weight: sort by weight descending and add while canAdd - that is Kruskal,
// and matroid intersection is the price you pay for the second constraint.
// MATROID UNION and "partition the ground set into k independent sets" both reduce to intersection.
