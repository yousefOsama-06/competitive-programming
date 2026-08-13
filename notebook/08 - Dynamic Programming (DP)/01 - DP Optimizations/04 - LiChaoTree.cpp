// LI CHAO TREE - MAXIMUM of a set of lines y = m*x + b at any query x, with NO order requirement
// on the slopes or on the queries (that is the whole point; CHT needs monotone slopes).
// O(log X) per line insert and per query, O(log^2 X) to insert a line only on a SEGMENT [lx, rx].
// Space O(n log X). Works for any family of functions where two members cross AT MOST ONCE -
// lines, and also a*x^2 + b*x + c with a FIXED a.
// EXACT INTEGER ARITHMETIC: never compute the crossing abscissa. Compare the two candidates at
// ns, mid and ne only - a division in double is off by ~1e3 in x when the coefficients reach 1e18,
// which silently sends the loser down the wrong half.
// SET the domain [X0, X1] before use; it must cover every x you will ever query.
struct LiChao {
    struct Line { ll m = 0, b = LLONG_MIN / 4; };              // neutral = -infinity
    ll X0, X1;
    vector<Line> t;
    vector<int> lf, rt;
    LiChao(ll X0, ll X1) : X0(X0), X1(X1) { newNode(); }
    int newNode() { t.push_back({}), lf.push_back(-1), rt.push_back(-1); return t.size() - 1; }
    static ll f(const Line& L, ll x) { return L.m == 0 && L.b == LLONG_MIN / 4 ? L.b : L.m * x + L.b; }
    void add(Line nw, int v, ll l, ll r) {                     // insert on the whole node range
        while (true) {
            ll m = l + (r - l) / 2;                            // NOT (l+r)/2: negative l+r truncates up
            bool lef = f(nw, l) > f(t[v], l), mid = f(nw, m) > f(t[v], m);
            if (mid) swap(t[v], nw);
            if (l == r) return;
            if (lef != mid) {
                if (lf[v] < 0) lf[v] = newNode();
                v = lf[v], r = m;
            } else {
                if (f(nw, r) > f(t[v], r)) {
                    if (rt[v] < 0) rt[v] = newNode();
                    v = rt[v], l = m + 1;
                } else return;
            }
        }
    }
    void addLine(ll m, ll b) { add({m, b}, 0, X0, X1); }
    void addSegment(ll m, ll b, ll lx, ll rx, int v = 0, ll l = LLONG_MIN, ll r = LLONG_MIN) {
        if (l == LLONG_MIN) l = X0, r = X1;
        if (rx < l || r < lx) return;
        if (lx <= l && r <= rx) { add({m, b}, v, l, r); return; }
        ll md = l + (r - l) / 2;
        if (lf[v] < 0) lf[v] = newNode();
        if (rt[v] < 0) rt[v] = newNode();
        addSegment(m, b, lx, rx, lf[v], l, md), addSegment(m, b, lx, rx, rt[v], md + 1, r);
    }
    ll query(ll x) {
        ll r = LLONG_MIN, l = X0, rr = X1;
        for (int v = 0; v >= 0;) {
            r = max(r, f(t[v], x));
            if (l == rr) break;
            ll m = l + (rr - l) / 2;
            if (x <= m) v = lf[v], rr = m;
            else v = rt[v], l = m + 1;
        }
        return r;                                              // LLONG_MIN/4 if no line covers x
    }
};
// FOR A MINIMUM: insert (-m, -b) and negate the answer.
// PERSISTENT / MERGEABLE: see 05 - PersistentLiChaoTree.cpp. MERGING two Li Chao trees costs
// O(n log X) amortised in total (push the loser line down into the other subtree, exactly like
// segment-tree merging) - that is how you do convex-hull DP on a tree without small-to-large.
