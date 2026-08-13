// Needs: Core.cpp.
// ONION DECOMPOSITION (convex layers) in O(n log^2 n): strip the convex hull, then the hull of
// what remains, and so on, and report which layer each point lands in. The naive loop in
// 05 - Sweep.cpp rebuilds a hull per layer and is O(n^2 log n), which dies at n = 1e5 on the
// worst input (points on nested circles give Theta(n^(2/3)) layers, and a spiral gives Theta(n)).
// THE STRUCTURE: a DECREMENTAL hull - it only supports DELETE, which is exactly what peeling
// needs. A segment tree over the points sorted by y stores, in every node, the two endpoints of
// the BRIDGE joining the hulls of its two children; pull() finds that bridge by the classic
// simultaneous descent in O(log n), so a delete costs O(log^2 n) and reading off the current hull
// costs O(hull size). Two copies handle the two sides: the LEFT hull of the points, and the left
// hull of the negated-and-reversed points, which is the right hull.
// WHEN: "how many peels until the set is empty", "which layer is point i on", k-th layer queries,
// and the classic "repeatedly remove the hull" simulation. Also the fastest route to the DEPTH of
// a point set (the number of layers), which some rotating-calipers-free arguments need.
// COMPLEXITY: O(n log^2 n) time, O(n) memory. Beats the naive version from about n = 3000.
// DEGENERATE: ALL POINTS MUST BE DISTINCT - dedupe first. COLLINEAR points on a hull edge are
// KEPT on that layer (the bridge search uses strict turns, so a point in the middle of an edge is
// still reported), which differs from hull() in 03 - Polygon.cpp; if you need them dropped,
// filter each layer afterwards. n <= 2 puts everything on layer 1.
// EXACT on P<ll>: cross fits in ll for |coord| <= 1e9, and the one product of a cross with a
// coordinate inside pull() is widened to __int128, so nothing here overflows at that budget.
struct LeftHull {                                       // points must be sorted by (y, x)
    struct Nd { int bl, br, L, R, lc, rc; };
    vector<Pi> ps;
    vector<Nd> t;
    int root = 0;
    bool leaf(int w) { return t[w].lc < 0 && t[w].rc < 0; }
    void pull(int w) {                                  // find the bridge between the two children
        int l = t[w].lc, r = t[w].rc;
        ll sy = ps[t[r].L].y;
        while (!leaf(l) || !leaf(r)) {
            int a = t[l].bl, b = t[l].br, c = t[r].bl, d = t[r].br;
            if (a != b && ps[a].cross(ps[b], ps[c]) > 0) l = t[l].lc;
            else if (c != d && ps[b].cross(ps[c], ps[d]) > 0) r = t[r].rc;
            else if (a == b) r = t[r].lc;
            else if (c == d) l = t[l].rc;
            else {
                ll s1 = ps[a].cross(ps[b], ps[c]), s2 = ps[b].cross(ps[a], ps[d]);
                if (s1 + s2 == 0 || (__int128)s1 * ps[d].y + (__int128)s2 * ps[c].y <
                                    (__int128)sy * (s1 + s2)) l = t[l].rc;
                else r = t[r].lc;
            }
        }
        t[w].bl = t[l].L, t[w].br = t[r].L;
    }
    void build(int w, int L, int R) {
        t[w].L = L, t[w].R = R;
        if (R - L == 1) { t[w].lc = t[w].rc = -1, t[w].bl = t[w].br = L; return; }
        int M = (L + R) / 2;
        t[w].lc = w + 1, t[w].rc = w + 2 * (M - L);
        build(t[w].lc, L, M), build(t[w].rc, M, R), pull(w);
    }
    int erase(int w, int L, int R) {
        if (R <= t[w].L || L >= t[w].R) return w;
        if (L <= t[w].L && R >= t[w].R) return -1;
        t[w].lc = erase(t[w].lc, L, R), t[w].rc = erase(t[w].rc, L, R);
        if (t[w].lc < 0) return t[w].rc;
        if (t[w].rc < 0) return t[w].lc;
        return pull(w), w;
    }
    void walk(int w, int l, int r, vector<int>& res) {
        if (leaf(w)) res.pb(t[w].L);
        else if (r <= t[w].bl) walk(t[w].lc, l, r, res);
        else if (l >= t[w].br) walk(t[w].rc, l, r, res);
        else walk(t[w].lc, l, t[w].bl, res), walk(t[w].rc, t[w].br, r, res);
    }
    LeftHull(const vector<Pi>& p) : ps(p), t(max<size_t>(2, p.size() * 2)) { build(0, 0, sz(p)); }
    void erase(int i) { root = erase(root, i, i + 1); }
    vector<int> hull() {
        vector<int> res;
        if (root >= 0) walk(root, 0, sz(ps) - 1, res);
        return res;
    }
};
// layer[i] = 1-based convex-layer index of the ORIGINAL point i. All points must be distinct.
vector<int> onionLayers(const vector<Pi>& p) {
    int n = sz(p);
    if (!n) return {};
    vector<int> ord(n), lay(n), res(n);
    iota(all(ord), 0);
    sort(all(ord), [&](int i, int j) { return tie(p[i].y, p[i].x) < tie(p[j].y, p[j].x); });
    vector<Pi> a(n), b(n);
    for (int i = 0; i < n; i++) a[i] = p[ord[i]];
    for (int i = 0; i < n; i++) b[i] = Pi(-a[n - 1 - i].x, -a[n - 1 - i].y);   // the right hull
    LeftHull L(a), R(b);
    for (int id = 1, cnt = 0; cnt < n; id++) {
        set<int> h;
        for (int i : L.hull()) h.insert(i);
        for (int i : R.hull()) h.insert(n - 1 - i);
        for (int i : h) cnt++, lay[i] = id, L.erase(i), R.erase(n - 1 - i);
    }
    for (int i = 0; i < n; i++) res[ord[i]] = lay[i];
    return res;
}
// The same structure answers "hull of the set after these deletions" at any moment, so it also
// does OFFLINE DELETIONS on a hull: process the deletes in order and read hull() in between.
// INSERTIONS instead of deletions are 16 - DynamicHull.cpp; you cannot have both here.
// LAYER COUNT bounds worth knowing: n points in convex position give 1 layer, a spiral gives n/3,
// and a uniform random square gives Theta(n^(2/3)) - so "peel until empty" is NOT O(sqrt n).
