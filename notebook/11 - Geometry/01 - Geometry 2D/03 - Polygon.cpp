// Polygons. Vertices in order (CW or CCW); convex routines want CCW, no collinear points.

template <class P> auto area2(const vector<P>& v) {       // SIGNED 2*area. >0 <=> CCW.
    decltype(v[0].cross(v[0])) a = 0;                     // exact on P<ll>; safe on empty
    for (int i = 0, n = v.size(); i < n; i++) a += v[i].cross(v[(i + 1) % n]);
    return a;
}
template <class P> ld area(const vector<P>& v) { return fabsl((ld)area2(v)) / 2; }
template <class P> ld perimeter(const vector<P>& v) {
    ld r = 0;
    for (int i = 0, n = v.size(); i < n; i++) r += (v[i] - v[(i + 1) % n]).dist();
    return r;
}
template <class P> P centroid(const vector<P>& v) {       // area centroid (needs floating P)
    P r; ld A = 0;
    for (int i = 0, j = v.size() - 1, n = v.size(); i < n; j = i++) {
        auto c = v[j].cross(v[i]);
        r = r + (v[i] + v[j]) * c; A += c;
    }
    return r / A / 3;
}
// A completely collinear (zero-area) polygon reports TRUE here - guard with area2 != 0 if that
// matters. Bow-ties and spikes are correctly rejected.
template <class P> bool isConvex(const vector<P>& v) {
    int n = v.size(), pos = 0, neg = 0;
    for (int i = 0; i < n; i++) {
        int s = sgn(v[i].cross(v[(i + 1) % n], v[(i + 2) % n]));
        s > 0 ? pos++ : s < 0 ? neg++ : 0;
    }
    return !pos || !neg;
}

// Point in ANY simple polygon, O(n), exact. strict=true -> boundary counts as outside.
template <class P> bool inPoly(const vector<P>& v, P a, bool strict = true) {
    int cnt = 0, n = v.size();
    for (int i = 0; i < n; i++) {
        P q = v[(i + 1) % n];
        if (onSeg(a, v[i], q)) return !strict;
        cnt ^= ((a.y < v[i].y) - (a.y < q.y)) * a.cross(v[i], q) > 0;
    }
    return cnt;
}

// Convex hull, CCW, strictly convex (collinear points dropped). O(n log n).
// Change <= 0 to < 0 in the pop test to KEEP collinear points on the hull.
template <class P> vector<P> hull(vector<P> p) {
    if (p.size() <= 1) return p;
    sort(all(p));
    vector<P> h(p.size() + 1);
    int s = 0, t = 0;
    for (int it = 2; it--; s = --t, reverse(all(p)))
        for (P q : p) {
            while (t >= s + 2 && h[t - 2].cross(h[t - 1], q) <= 0) t--;
            h[t++] = q;
        }
    return {h.begin(), h.begin() + t - (t == 2 && h[0] == h[1])};
}

// Point in CONVEX polygon (CCW, strict), O(log n). 1 inside, 0 on boundary, -1 outside.
// h must be a strictly convex CCW hull with n >= 1. Returns 1 inside, 0 on the boundary,
// -1 outside.
template <class P> int inConvex(const vector<P>& h, P p) {
    if (h.empty()) return -1;
    int n = h.size();
    if (n == 1) return p == h[0] ? 0 : -1;
    if (n == 2) return onSeg(p, h[0], h[1]) ? 0 : -1;
    if (h[0].cross(h[1], p) < 0 || h[0].cross(h[n - 1], p) > 0) return -1;
    int l = 1, r = n - 1;
    while (r - l > 1) { int m = (l + r) / 2; (h[0].cross(h[m], p) >= 0 ? l : r) = m; }
    int s = sgn(h[l].cross(h[l + 1], p));
    if (s < 0) return -1;
    if (s == 0) return 0;
    return (h[0].cross(h[1], p) == 0 || h[0].cross(h[n - 1], p) == 0) ? 0 : 1;
}

// Index of the hull vertex furthest in direction `dir`, O(log n). CCW hull, no collinear.
// Use to get tangents from a far-away direction, or max/min dot product.
#define _cmp(i, j) sgn(dir.perp().cross(h[(i) % n] - h[(j) % n]))
#define _ext(i) (_cmp(i + 1, i) >= 0 && _cmp(i, i - 1 + n) < 0)
template <class P> int extreme(const vector<P>& h, P dir) {
    int n = h.size(), lo = 0, hi = n;
    if (_ext(0)) return 0;
    while (lo + 1 < hi) {
        int m = (lo + hi) / 2;
        if (_ext(m)) return m;
        int ls = _cmp(lo + 1, lo), ms = _cmp(m + 1, m);
        (ls < ms || (ls == ms && ls == _cmp(lo, m)) ? hi : lo) = m;
    }
    return lo;
}

// Cut polygon by the line s->e, KEEP the part on the LEFT (boundary included). O(n).
// Needs floating P. Boundary-inclusive matters: with a strict test, vertices lying exactly on
// the line are dropped from BOTH halves and you lose area. Repeat over many lines = half-plane
// intersection. cut(v,e,s) gives the other half; the two areas sum to area(v).
// Keeps the part LEFT of the directed line s->e, boundary INCLUSIVE. Vertices exactly on the
// line are kept, so a line through a vertex produces a DUPLICATE point - fine for area, but
// dedup before feeding the result to hull / isConvex / minkowski / diameter2.
template <class P> vector<P> cut(const vector<P>& v, P s, P e) {
    vector<P> r;
    for (int i = 0, n = v.size(); i < n; i++) {
        P cur = v[i], prv = v[(i + n - 1) % n];
        bool side = sgn(s.cross(e, cur)) >= 0;
        if (side != (sgn(s.cross(e, prv)) >= 0)) r.push_back(lineInter(s, e, cur, prv).second);
        if (side) r.push_back(cur);
    }
    return r;
}

// Minkowski sum of two CONVEX CCW polygons. O(n+m). Result is convex CCW.
// BOTH polygons must be CONVEX and COUNTER-CLOCKWISE. On clockwise input the merge below makes
// no progress and loops forever (an unexplained TLE/MLE, not a wrong answer) - so normalise.
template <class P> vector<P> minkowski(vector<P> A, vector<P> B) {
    if (area2(A) < 0) reverse(all(A));
    if (area2(B) < 0) reverse(all(B));
    auto low = [](vector<P>& v) {
        auto lo = [](P a, P b) { return tie(a.y, a.x) < tie(b.y, b.x); };
        rotate(v.begin(), min_element(all(v), lo), v.end());
    };
    low(A); low(B);
    A.push_back(A[0]); A.push_back(A[1]);
    B.push_back(B[0]); B.push_back(B[1]);
    vector<P> r; size_t i = 0, j = 0;
    while (i + 2 < A.size() || j + 2 < B.size()) {
        r.push_back(A[i] + B[j]);
        auto c = (A[i + 1] - A[i]).cross(B[j + 1] - B[j]);
        if (c >= 0 && i + 2 < A.size()) i++;
        if (c <= 0 && j + 2 < B.size()) j++;
    }
    return r;
}

// Rotating calipers. Hull must be CCW strictly convex.
template <class P> auto diameter2(const vector<P>& h) {   // max squared distance
    int n = h.size(), j = n < 2 ? 0 : 1;
    decltype(h[0].dist2()) r = 0;
    for (int i = 0; i < j; i++)
        for (;; j = (j + 1) % n) {
            r = max(r, (h[i] - h[j]).dist2());
            if ((h[(j + 1) % n] - h[j]).cross(h[i + 1] - h[i]) >= 0) break;
        }
    return r;
}
template <class P> ld width(const vector<P>& h) {          // min distance between parallel supports
    int n = h.size(); if (n <= 2) return 0;
    ld r = 1e18; int j = 1;
    for (int i = 0; i < n; i++) {
        while ((h[(i + 1) % n] - h[i]).cross(h[(j + 1) % n] - h[j]) >= 0) j = (j + 1) % n;
        r = min(r, lineDist(h[j], h[i], h[(i + 1) % n]));
    }
    return r;
}
// Minimum-area enclosing rectangle. One side always lies on a hull edge (that's the theorem),
// so try every edge. O(n^2) on the HULL only - fine, and immune to caliper tie bugs.
template <class P> ld minRectArea(const vector<P>& h) {
    int n = h.size(); if (n < 3) return 0;
    ld best = 1e18;
    for (int i = 0; i < n; i++) {
        P d = h[(i + 1) % n] - h[i];
        ld L = d.dist(), lo = 1e18, hi = -1e18, ht = 0;
        for (P q : h) {
            ld u = (ld)d.dot(q - h[i]) / L, v = (ld)d.cross(q - h[i]) / L;
            lo = min(lo, u); hi = max(hi, u); ht = max(ht, fabsl(v));
        }
        best = min(best, (hi - lo) * ht);
    }
    return best;
}

// PICK'S THEOREM. Lattice polygon: A = I + B/2 - 1.
template <class P> ll boundaryPts(const vector<P>& v) {    // B
    ll b = 0;
    for (int i = 0, n = v.size(); i < n; i++) {
        P d = v[(i + 1) % n] - v[i];
        b += gcdll(llabs(d.x), llabs(d.y));
    }
    return b;
}
// PICK'S THEOREM, and it needs a SIMPLE polygon with integer vertices. On a self-intersecting
// or degenerate (zero-area) polygon the result is meaningless, not merely approximate.
template <class P> ll interiorPts(const vector<P>& v) {    // I = A - B/2 + 1
    return (llabs(area2(v)) - boundaryPts(v)) / 2 + 1;
}
