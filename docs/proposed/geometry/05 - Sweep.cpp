// Global / sweep-line geometry: problems about a whole SET of objects.

// --- CLOSEST PAIR of points, O(n log n). Returns the two points. Exact on P<ll>. ---
template <class P> pair<P, P> closestPair(vector<P> v) {
    set<P> S;                                             // ordered by (x, y)
    sort(all(v), [](P a, P b) { return a.y < b.y; });
    pair<ll, pair<P, P>> best{LLONG_MAX, {P(), P()}};
    int j = 0;
    for (P p : v) {
        P d{1 + (ll)sqrtl((ld)best.first), 0};
        while (v[j].y <= p.y - d.x) S.erase(v[j++]);
        auto lo = S.lower_bound(p - d), hi = S.upper_bound(p + d);
        for (; lo != hi; ++lo) best = min(best, {(*lo - p).dist2(), {*lo, p}});
        S.insert(p);
    }
    return best.second;
}

// --- AREA OF UNION OF AXIS-ALIGNED RECTANGLES, O(n log n). ---
// rects: {x1, y1, x2, y2} half-open [x1,x2) x [y1,y2). Sweep x, segment tree counts covered y.
struct CoverTree {                                        // "count>0 length" segment tree
    int n; vector<int> cnt; vector<ll> len; vector<ll> ys;
    CoverTree(vector<ll> y) : ys(y) { n = ys.size() - 1; cnt.assign(4 * n, 0); len.assign(4 * n, 0); }
    void upd(int v, int l, int r, int ql, int qr, int d) {
        if (qr <= l || r <= ql) return;
        if (ql <= l && r <= qr) cnt[v] += d;
        else {
            int m = (l + r) / 2;
            upd(2 * v, l, m, ql, qr, d); upd(2 * v + 1, m, r, ql, qr, d);
        }
        len[v] = cnt[v] ? ys[r] - ys[l] : (r - l == 1 ? 0 : len[2 * v] + len[2 * v + 1]);
    }
    void upd(int l, int r, int d) { upd(1, 0, n, l, r, d); }
    ll get() { return len[1]; }
};
ll rectUnionArea(vector<array<ll, 4>> r) {
    vector<ll> ys;
    for (auto& q : r) ys.push_back(q[1]), ys.push_back(q[3]);
    sort(all(ys)); ys.erase(unique(all(ys)), ys.end());
    if (ys.size() < 2) return 0;
    auto id = [&](ll y) { return (int)(lower_bound(all(ys), y) - ys.begin()); };
    vector<array<ll, 4>> ev;                              // {x, +-1, y1, y2}
    for (auto& q : r) ev.push_back({q[0], 1, q[1], q[3]}), ev.push_back({q[2], -1, q[1], q[3]});
    sort(all(ev));
    CoverTree T(ys);
    ll area = 0, px = ev.empty() ? 0 : ev[0][0];
    for (auto& e : ev) {
        area += T.get() * (e[0] - px); px = e[0];
        T.upd(id(e[2]), id(e[3]), (int)e[1]);
    }
    return area;
}
// PERIMETER of the union: run the same sweep twice (once per axis) accumulating
// |len(after) - len(before)| at each event x, and add the vertical contributions.

// --- MAX POINTS ON A LINE, O(n^2 log n) ---
template <class P> int maxOnLine(vector<P> p) {
    int n = p.size(), best = min(n, 1);
    for (int i = 0; i < n; i++) {
        map<pair<ll, ll>, int> c; int dup = 0;
        for (int j = 0; j < n; j++) if (j != i) {
            ll dx = p[j].x - p[i].x, dy = p[j].y - p[i].y;
            if (!dx && !dy) { dup++; continue; }             // duplicate points
            ll g = gcdll(llabs(dx), llabs(dy)); dx /= g; dy /= g;
            if (dx < 0 || (dx == 0 && dy < 0)) dx = -dx, dy = -dy;
            c[{dx, dy}]++;
        }
        best = max(best, dup + 1);
        for (auto& [k, v] : c) best = max(best, v + dup + 1);
    }
    return best;
}

// --- COUNT PAIRS OF INTERSECTING SEGMENTS, O(n^2) exact. ---
// For O((n+k) log n) use Bentley-Ottmann; almost never needed in contests.
template <class P> ll countSegInter(vector<pair<P, P>> s) {
    ll c = 0;
    for (int i = 0; i < (int)s.size(); i++)
        for (int j = i + 1; j < (int)s.size(); j++)
            c += segCross(s[i].first, s[i].second, s[j].first, s[j].second);
    return c;
}

// --- CONVEX LAYERS (onion peeling): repeatedly strip the hull. O(n^2 log n) naive. ---
template <class P> vector<vector<P>> onion(vector<P> p) {
    vector<vector<P>> L;
    while (!p.empty()) {
        auto h = hull(p);
        L.push_back(h);
        for (P q : h) p.erase(find(all(p), q));
    }
    return L;
}
