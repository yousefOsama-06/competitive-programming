// RANDOM TEST GENERATORS for stress testing. 12 - StressTest.cpp is the harness; this is the
// library it feeds on. Writing a generator from scratch under pressure is where stress testing
// usually dies, so keep these on the page.
// SEED FROM argv SO A FAILING CASE IS REPRODUCIBLE:
//   int main(int argc, char** argv) { if (argc > 1) rng.seed(stoull(argv[1])); ... }
// then the driver loop passes the iteration number and you can replay any failure exactly.
ll rnd(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng); }   // inclusive
bool coin(int pct = 50) { return rnd(1, 100) <= pct; }
vector<ll> randArray(int n, ll lo, ll hi) {
    vector<ll> a(n);
    for (ll& x : a) x = rnd(lo, hi);
    return a;
}
vector<int> randPermutation(int n) {                           // 0-indexed
    vector<int> p(n);
    iota(all(p), 0);
    shuffle(all(p), rng);
    return p;
}
string randString(int n, char lo = 'a', char hi = 'z') {
    string s;
    while (n--) s += (char)rnd(lo, hi);
    return s;
}
vector<string> randGrid(int n, int m, const string& alpha = ".#") {
    vector<string> g(n, string(m, '.'));
    for (auto& row : g) for (char& c : row) c = alpha[rnd(0, alpha.size() - 1)];
    return g;
}
// --- TREES. All 1-indexed, n-1 edges, always connected. ---
vector<pair<int, int>> randTree(int n) {                       // uniform-ish, shuffled labels
    vector<int> lab = randPermutation(n);
    vector<pair<int, int>> e;
    for (int i = 1; i < n; i++) e.push_back({lab[rnd(0, i - 1)] + 1, lab[i] + 1});
    return e;
}
vector<pair<int, int>> pathTree(int n) {                       // the DEEP case: recursion, HLD
    vector<pair<int, int>> e;
    for (int i = 2; i <= n; i++) e.push_back({i - 1, i});
    return e;
}
vector<pair<int, int>> starTree(int n) {                       // the WIDE case: heavy-child logic
    vector<pair<int, int>> e;
    int c = rnd(1, n);
    for (int i = 1; i <= n; i++) if (i != c) e.push_back({c, i});
    return e;
}
vector<pair<int, int>> caterpillar(int n) {                    // a spine with legs: breaks naive
    vector<pair<int, int>> e;                                  // centroid and diameter code
    int spine = max(1LL, (ll)rnd(1, n));
    for (int i = 2; i <= spine; i++) e.push_back({i - 1, i});
    for (int i = spine + 1; i <= n; i++) e.push_back({(int)rnd(1, spine), i});
    return e;
}
// --- GRAPHS. m edges, no self loops, no duplicates unless you ask for them. ---
vector<pair<int, int>> randGraph(int n, int m) {
    vector<pair<int, int>> all;
    for (int i = 1; i <= n; i++) for (int j = i + 1; j <= n; j++) all.push_back({i, j});
    shuffle(all(all), rng);
    all.resize(min((size_t)m, all.size()));
    return all;
}
vector<pair<int, int>> connectedGraph(int n, int m) {           // a random tree plus extra edges
    auto e = randTree(n);
    set<pair<int, int>> used;
    for (auto [u, v] : e) used.insert({min(u, v), max(u, v)});
    m = min<ll>(m, (ll)n * (n - 1) / 2);
    while ((int)e.size() < m) {
        int u = rnd(1, n), v = rnd(1, n);
        if (u == v) continue;
        if (u > v) swap(u, v);
        if (used.insert({u, v}).second) e.push_back({u, v});
    }
    shuffle(all(e), rng);
    return e;
}
vector<pair<int, int>> randDAG(int n, int m) {                  // acyclic by a random topo order
    vector<int> p = randPermutation(n);
    vector<pair<int, int>> all;
    for (int i = 0; i < n; i++) for (int j = i + 1; j < n; j++) all.push_back({p[i] + 1, p[j] + 1});
    shuffle(all(all), rng);
    all.resize(min((size_t)m, all.size()));
    return all;
}
vector<pair<int, int>> randQueries(int q, int n) {              // random inclusive [l, r]
    vector<pair<int, int>> v;
    while (q--) {
        int l = rnd(1, n), r = rnd(1, n);
        if (l > r) swap(l, r);
        v.push_back({l, r});
    }
    return v;
}
/* THE CASES A UNIFORM GENERATOR NEVER PRODUCES - and which is exactly where the bug is
   n = 1, n = 2, and an EMPTY structure                     (off-by-one, .back() on empty)
   ALL VALUES EQUAL, all zero, all negative                 (tie-breaks, strict vs non-strict)
   values at the EXTREMES of the stated range               (overflow)
   a SORTED or REVERSE-SORTED array                         (quicksort, two pointers, hulls)
   a PATH tree and a STAR tree                              (recursion depth, heavy-child logic)
   a DISCONNECTED graph, an isolated vertex, a self loop, a DOUBLED edge   (bridges, DSU, flows)
   COLLINEAR / DUPLICATE points, a degenerate polygon       (all of geometry)
   Generate with a SMALL value range (say [1,3]) most of the time: collisions and ties are what
   break code, and a uniform draw from [1, 1e9] never creates one.
   SHRINK a failure before reading it: re-run the generator with progressively smaller bounds
   until the counterexample fits on one line, then stare at that. */
