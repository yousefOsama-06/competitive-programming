// Needs: 26 - TreeBinarization.cpp
// PERSISTENT CENTROID DECOMPOSITION - "sum of dist(u, x) over all marked vertices x", where the
// set of marked vertices is versioned: version i = the first i marks. Insert O(log n), query
// O(log n), memory O(n log n).
// WHEN: queries of the form "consider only the marks with index in [l, r], answer for vertex u" -
// subtract two versions (CF 757G). Also "the k-th mark", online, when marks arrive in a fixed
// order. If the mark set only ever grows and you never look back, plain centroid decomposition
// (09 - Trees/04) with the same accumulators is enough and far cheaper.
// COMPLEXITY REQUIRES BINARIZATION FIRST: a centroid node must have O(1) children so that copying
// it is O(1). Feed the tree through 26 - TreeBinarization.
// KEY INVARIANT: the centroid tree has depth O(log n) and every vertex u lies in exactly one
// component per level, so dist(u, x) = d[level][u] + d[level][x] where the level is that of the
// LOWEST common centroid of u and x. Each centroid node stores cnt (marks in its component), sum
// (total distance from the centroid to them), and par (their total distance to the centroid's
// PARENT centroid). Walking from the root of the centroid tree down to u then adds, at each step,
// the contribution of exactly the marks that "branch off" there - the standard inclusion-exclusion
// that stops double counting. Marking one vertex touches only the O(log n) centroid nodes on that
// root-to-u path, so persistence costs O(log n) copies.
struct PCentroid {
    struct Node { array<int, 3> ct{{0, 0, 0}}; int id = 0, lvl = 0, cnt = 0; ll sum = 0, par = 0; };
    vector<Node> t;
    vector<vector<pii>> g;                                     // BINARISED tree, 1-indexed
    vector<vector<ll>> d;                                      // d[level][v]
    vector<int> st, en, sub, done;
    int n, DT = 0, tot = 0, root = 0;
    PCentroid(const vector<vector<pii>>& g, int nv, int LG = 20)
        : t(nv + 1), g(g), d(LG, vector<ll>(nv + 1, 0)), st(nv + 1), en(nv + 1), sub(nv + 1),
          done(nv + 1, 0), n(nv) { root = decompose(1, 0, 0); }
    void calcSz(int u, int p) {
        tot++, sub[u] = 1;
        for (auto [v, w] : g[u])
            if (v != p && !done[v]) calcSz(v, u), sub[u] += sub[v];
    }
    int findCen(int u, int p) {
        for (auto [v, w] : g[u])
            if (v != p && !done[v] && sub[v] > tot / 2) return findCen(v, u);
        return u;
    }
    void dist(int u, int p, ll cur, int l) {
        d[l][u] = cur;
        for (auto [v, w] : g[u])
            if (v != p && !done[v]) dist(v, u, cur + w, l);
    }
    int decompose(int u, int p, int l) {
        tot = 0, calcSz(u, p);
        int c = findCen(u, p);
        done[c] = 1, st[c] = ++DT, t[c].id = c, t[c].lvl = l;
        dist(c, 0, 0, l);
        int k = 0;
        for (auto [v, w] : g[c])
            if (!done[v]) t[c].ct[k++] = decompose(v, c, l + 1);
        en[c] = DT;
        return c;
    }
    bool insub(int x, int u) { return st[t[x].id] <= st[u] && en[u] <= en[t[x].id]; }
    int mark(int cur, int u) {                                 // returns the NEW version root
        t.push_back(t[cur]);
        int c = t.size() - 1;
        ll dv = d[t[c].lvl][u];
        t[c].cnt++, t[c].sum += dv;
        for (int k = 0; k < 3; k++) {
            int v = t[c].ct[k];
            if (v && insub(v, u)) {
                int nv = mark(v, u);
                t[nv].par += dv, t[c].ct[k] = nv;
            }
        }
        return c;
    }
    ll query(int cur, int u) {                                 // sum of dist(u, x) over marks
        ll ans = 0;
        while (t[cur].id != u) {
            int v = 0;
            for (int k = 0; k < 3; k++)
                if (t[cur].ct[k] && insub(t[cur].ct[k], u)) v = t[cur].ct[k];
            ans += d[t[cur].lvl][u] * (t[cur].cnt - t[v].cnt) + t[cur].sum - t[v].par;
            cur = v;
        }
        return ans + t[cur].sum;
    }
};
// USAGE: Binarize b(G); PCentroid pc(b.g, b.cnt); then ver[0] = pc.root and
// ver[i] = pc.mark(ver[i-1], x_i). The answer for marks l..r is
// pc.query(ver[r], u) - pc.query(ver[l-1], u).
// RESERVE t: n + (marks) * log n nodes; each is ~40 bytes.
// OTHER ACCUMULATORS: replace cnt/sum by whatever is additive over the marks - count within a
// distance bound (keep a BIT per centroid instead, which kills persistence), sum of weights,
// number of pairs. Anything that needs a per-centroid ARRAY does not persist cheaply.
// DUMMY VERTICES from binarization are never marked and never queried, but they DO appear as
// centroids - which is fine, since query() walks the centroid tree by vertex id and only stops at
// the id it was asked for.
