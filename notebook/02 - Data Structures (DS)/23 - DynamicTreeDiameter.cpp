// DYNAMIC TREE DIAMETER - the diameter of a weighted tree under ONLINE edge-weight changes,
// O(log n) per update, O(1) to read the answer. The tree SHAPE is fixed; only the weights move.
// WHEN: "after each update print the diameter" (CF 1192B and every problem shaped like it). If the
// weights are static, two DFS or the tree DP in 09 - Trees/05 is O(n). If edges are ADDED or
// REMOVED you need a link-cut tree (09 - Trees/11) or offline divide and conquer instead.
// KEY INVARIANT: put the tree on its Euler tour and let f[p] = depth of the vertex visited at
// position p. For any x <= y <= z in the tour, f[x] - 2f[y] + f[z] <= dist between the vertices at
// x and z (with equality when y is their LCA, which is always visited between them), so the
// diameter is exactly max over x <= y <= z of f[x] - 2f[y] + f[z]. Changing an edge's weight by
// delta adds delta to f over the CONTIGUOUS tour range of the child's subtree - a range add. The
// segment tree node keeps the five best partial expressions so they merge in O(1):
//   a = max f[x],  b = max -2f[y],  c = max f[x]-2f[y],  d = max -2f[y]+f[z],  e = the answer.
struct DynDiameter {
    struct Node { ll a = 0, b = 0, c = 0, d = 0, e = 0; };
    static Node mrg(const Node& L, const Node& R) {
        Node x;
        x.a = max(L.a, R.a), x.b = max(L.b, R.b);
        x.c = max({L.c, R.c, L.a + R.b});
        x.d = max({L.d, R.d, L.b + R.a});
        x.e = max({L.e, R.e, L.c + R.a, L.a + R.d});
        return x;
    }
    int n, T = 0;
    vector<Node> t;
    vector<ll> lz, w;
    vector<int> st, en, low;                                   // low[e] = deeper endpoint of edge e
    vector<vector<pii>> g;                                     // {to, edge id}
    DynDiameter(int n) : n(n), t(16 * n), lz(16 * n, 0), st(n), en(n), g(n) {}
    void addEdge(int u, int v, ll c) {
        int e = w.size();
        w.push_back(c), low.push_back(-1);
        g[u].push_back({v, e}), g[v].push_back({u, e});
    }
    void dfs(int u, int p) {
        st[u] = ++T;
        for (auto [v, e] : g[u])
            if (v != p) low[e] = v, dfs(v, u), ++T;
        en[u] = T;
    }
    void apply(int x, ll v) {
        t[x].a += v, t[x].b -= 2 * v, t[x].c -= v, t[x].d -= v, lz[x] += v;
    }
    void push(int x) { if (lz[x]) apply(2 * x, lz[x]), apply(2 * x + 1, lz[x]), lz[x] = 0; }
    void upd(int x, int lo, int hi, int l, int r, ll v) {
        if (r < lo || hi < l) return;
        if (l <= lo && hi <= r) { apply(x, v); return; }
        int m = (lo + hi) / 2;
        push(x);
        upd(2 * x, lo, m, l, r, v), upd(2 * x + 1, m + 1, hi, l, r, v);
        t[x] = mrg(t[2 * x], t[2 * x + 1]);
    }
    void build(int root = 0) {                                 // call after every addEdge
        dfs(root, -1);
        for (int e = 0; e < (int)w.size(); e++) {
            ll c = w[e];
            w[e] = 0;
            setW(e, c);                                        // all f start at 0, so just add
        }
    }
    void setW(int e, ll c) {                                   // set edge e's weight to c
        upd(1, 1, T, st[low[e]], en[low[e]], c - w[e]);
        w[e] = c;
    }
    ll diameter() { return t[1].e; }
};
// NEGATIVE WEIGHTS break it: the argument needs f[x] - 2f[LCA] + f[z] to be maximised at the true
// deepest pair, which assumes non-negative edges. Shift all weights up if you must.
// THE SAME TOUR TRICK gives dist(u, v) queries under updates: dist = f[st[u]] + f[st[v]] -
// 2 * min f over the tour between them, so keep a min segment tree alongside.
// DIAMETER ENDPOINTS: store argmaxes next to each of the five maxima; the merge picks them up
// unchanged. Only do this if the problem asks - it triples the node.
// SIMPLER SPECIAL CASE: if updates only ever INCREASE weights, the diameter is monotone and a
// small-to-large "merge the two deepest per subtree" DP is enough.
