// KD-TREE over 2D points: nearest-neighbour and "count points in a rectangle".
// Build O(n log n); NN query O(log n) expected, O(n) worst for NN; the O(sqrt n) bound is for RECTANGLE queries; rectangle count O(sqrt n + k).
// Use when coordinates are large/sparse and a grid or sort-sweep will not do.
struct KD {
    struct Node {
        Pi p;                                            // the splitting point
        ll x0 = LLONG_MAX, x1 = LLONG_MIN, y0 = LLONG_MAX, y1 = LLONG_MIN;   // bounding box
        int l = -1, r = -1, cnt = 0;
    };
    vector<Node> t;
    vector<Pi> pts;

    KD(vector<Pi> v) : pts(v) { if (!v.empty()) build(0, v.size(), false); }

    int build(int lo, int hi, bool divX) {
        if (lo >= hi) return -1;
        int id = t.size();
        t.push_back({});
        int mid = (lo + hi) / 2;
        nth_element(pts.begin() + lo, pts.begin() + mid, pts.begin() + hi,
                    [&](Pi a, Pi b) { return divX ? a.x < b.x : a.y < b.y; });
        Node nd;
        nd.p = pts[mid], nd.cnt = hi - lo;
        for (int i = lo; i < hi; i++)
            nd.x0 = min(nd.x0, pts[i].x), nd.x1 = max(nd.x1, pts[i].x),
            nd.y0 = min(nd.y0, pts[i].y), nd.y1 = max(nd.y1, pts[i].y);
        t[id] = nd;
        int L = build(lo, mid, !divX), R = build(mid + 1, hi, !divX);
        t[id].l = L, t[id].r = R;
        return id;
    }
    ll boxDist(int i, Pi q) const {                      // squared distance from q to the box
        const Node& n = t[i];
        ll dx = max({(ll)0, n.x0 - q.x, q.x - n.x1}), dy = max({(ll)0, n.y0 - q.y, q.y - n.y1});
        return dx * dx + dy * dy;
    }
    void nn(int i, Pi q, ll& best, Pi& bp, bool skipSelf) const {
        if (i < 0 || boxDist(i, q) >= best) return;
        ll d = (t[i].p - q).dist2();
        if (d < best && !(skipSelf && d == 0)) best = d, bp = t[i].p;
        int a = t[i].l, b = t[i].r;
        if (a >= 0 && b >= 0 && boxDist(b, q) < boxDist(a, q)) swap(a, b);
        nn(a, q, best, bp, skipSelf), nn(b, q, best, bp, skipSelf);
    }
    // skipSelf skips EVERY point at distance 0, not just q itself - with duplicate input points
    // that is wrong for "nearest OTHER point". If duplicates are possible, count multiplicity
    // separately and treat a repeated point as its own nearest neighbour at distance 0.
    pair<ll, Pi> nearest(Pi q, bool skipSelf = false) const {
        if (t.empty()) return {llinf, Pi()};
        ll best = LLONG_MAX; Pi bp;
        nn(0, q, best, bp, skipSelf);
        return {best, bp};                               // {squared distance, point}
    }
    int count(int i, ll x0, ll y0, ll x1, ll y1) const { // #points in [x0,x1] x [y0,y1]
        if (i < 0 || t[i].x1 < x0 || t[i].x0 > x1 || t[i].y1 < y0 || t[i].y0 > y1) return 0;
        if (x0 <= t[i].x0 && t[i].x1 <= x1 && y0 <= t[i].y0 && t[i].y1 <= y1) return t[i].cnt;
        int r = (x0 <= t[i].p.x && t[i].p.x <= x1 && y0 <= t[i].p.y && t[i].p.y <= y1);
        return r + count(t[i].l, x0, y0, x1, y1) + count(t[i].r, x0, y0, x1, y1);
    }
    int count(ll x0, ll y0, ll x1, ll y1) const { return t.empty() ? 0 : count(0, x0, y0, x1, y1); }
};
