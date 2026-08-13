// COORDINATE TRANSFORMS - the tricks that turn a hard metric into an easy one.

// CHEBYSHEV <-> MANHATTAN.  rot45(p) = (x+y, x-y).
//   Manhattan distance in the original = Chebyshev distance after rot45  (and vice versa).
// Use it for: "max Manhattan distance over a set" (becomes max over 4 coordinate extremes),
// "count pairs within Manhattan distance k", chessboard-king problems.
template <class P> P rot45(P p) { return P(p.x + p.y, p.x - p.y); }
template <class P> P unrot45(P p) { return P((p.x + p.y) / 2, (p.x - p.y) / 2); }
// Max Manhattan distance between any two of the points, O(n).
ll maxManhattan(const vector<Pi>& v) {
    ll best = 0;
    for (int s = 0; s < 2; s++) {                       // the 2^1 sign patterns for (x +- y)
        ll mn = LLONG_MAX, mx = LLONG_MIN;
        for (auto& p : v) { ll t = s ? p.x - p.y : p.x + p.y; mn = min(mn, t), mx = max(mx, t); }
        best = max(best, mx - mn);
    }
    return best;
}
// In d dimensions the same idea uses all 2^(d-1) sign patterns.

// MANHATTAN MST - the MST under L1 distance, O(n log n) instead of O(n^2).
// Key fact: every point only needs edges to its nearest neighbour in each of 8 octants; by
// symmetry 4 sweeps suffice (rotate/reflect the plane between sweeps).
vector<array<ll, 3>> manhattanEdges(vector<Pi> ps) {     // {w, i, j}; indices stay ORIGINAL
    int n = ps.size();
    vector<int> id(n);
    iota(all(id), 0);
    vector<array<ll, 3>> e;
    for (int s = 0; s < 4; s++) {
        sort(all(id), [&](int a, int b) { return ps[a].x + ps[a].y < ps[b].x + ps[b].y; });
        map<ll, int> act;                                // key = -y, value = index
        for (int i : id) {
            for (auto it = act.lower_bound(-ps[i].y); it != act.end(); act.erase(it++)) {
                int j = it->second;
                Pi d = ps[i] - ps[j];
                if (d.y > d.x) break;
                e.push_back({d.x + d.y, (ll)i, (ll)j});
            }
            act[-ps[i].y] = i;
        }
        for (auto& p : ps) { if (s & 1) p.x = -p.x; else swap(p.x, p.y); }   // next octant pair
    }
    return e;                                            // feed these to Kruskal
}

// LINEAR TRANSFORMATION mapping p0->q0 and p1->q1 (rotation + scaling + translation).
Pd linTrans(Pd p0, Pd p1, Pd q0, Pd q1, Pd r) {
    Pd dp = p1 - p0, dq = q1 - q0, num(dp.cross(dq), dp.dot(dq));
    return q0 + Pd((r - p0).cross(num), (r - p0).dot(num)) / dp.dist2();
}

// POINT <-> LINE DUALITY:  point (a, b)  <->  line y = a*x - b.
//   "max points on a common line" becomes "max lines through a common point"
//   convex hull of points  <->  upper/lower envelope of lines (this IS the CHT correspondence)
//   incidences are preserved, above/below is preserved.
