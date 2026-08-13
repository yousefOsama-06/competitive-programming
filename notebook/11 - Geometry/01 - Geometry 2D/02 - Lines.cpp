// Lines, segments, rays. "line ab" = infinite line through a and b.
// Predicates are exact on P<ll>; anything returning a point needs P<ld>.

template <class P> ld lineDist(P p, P a, P b) {          // distance point -> line
    return fabsl((ld)(b - a).cross(p - a)) / (b - a).dist();
}
template <class P> P proj(P p, P a, P b) {               // foot of perpendicular
    return a + (b - a) * ((ld)(p - a).dot(b - a) / (b - a).dist2());
}
template <class P> P refl(P p, P a, P b) { return proj(p, a, b) * 2 - p; }

template <class P> bool onSeg(P p, P a, P b) {           // inclusive of endpoints
    return a.cross(b, p) == 0 && (a - p).dot(b - p) <= 0;
}
template <class P> ld segDist(P p, P a, P b) {           // distance point -> segment (exact-safe)
    if (a == b) return (p - a).dist();
    ld d = (ld)(b - a).dist2(), t = (ld)(p - a).dot(b - a);
    if (t <= 0) return (p - a).dist();
    if (t >= d) return (p - b).dist();
    return fabsl((ld)(b - a).cross(p - a)) / sqrtl(d);
}
template <class P> P closestOnSeg(P p, P a, P b) {
    if ((p - a).dot(b - a) <= 0) return a;
    if ((p - b).dot(b - a) >= 0) return b;
    return proj(p, a, b);
}

// LINE x LINE. first = 1 unique, 0 parallel-disjoint, -1 same line.
template <class P> pair<int, P> lineInter(P a, P b, P c, P d) {
    auto x = (b - a).cross(d - c);
    if (sgn(x) == 0) return {-(a.cross(b, c) == 0), P()};
    auto p = c.cross(b, d), q = c.cross(d, a);            // p + q == x  (affine weights)
    return {1, (a * p + b * q) / x};                      // P must be floating
}

// SEGMENT x SEGMENT. Returns 0, 1, or 2 points (2 = collinear overlap endpoints).
// Exact on P<ll> when the answer has 0 or 2 points. The 1-point case DIVIDES and the numerator
// is ~2*C^3, so it is only exact for |coordinate| <= ~1.6e6. Above that use segCross for the
// boolean, or Frac (01 - Miscellaneous/13) for an exact rational point.
template <class P> vector<P> segInter(P a, P b, P c, P d) {
    auto oa = c.cross(d, a), ob = c.cross(d, b), oc = a.cross(b, c), od = a.cross(b, d);
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0)
        return {(a * ob - b * oa) / (ob - oa)};
    set<P> s;
    if (onSeg(c, a, b)) s.insert(c);
    if (onSeg(d, a, b)) s.insert(d);
    if (onSeg(a, c, d)) s.insert(a);
    if (onSeg(b, c, d)) s.insert(b);
    return {all(s)};
}
// Just "do they touch?" - fully exact, NO division and no allocation, so it is both safe at
// |c| = 1e9 and ~30x faster than asking segInter. Use this whenever you only need a bool.
template <class P> bool segCross(P a, P b, P c, P d) {
    auto oa = c.cross(d, a), ob = c.cross(d, b), oc = a.cross(b, c), od = a.cross(b, d);
    if (sgn(oa) * sgn(ob) < 0 && sgn(oc) * sgn(od) < 0) return true;
    return onSeg(c, a, b) || onSeg(d, a, b) || onSeg(a, c, d) || onSeg(b, c, d);
}

template <class P> ld segSegDist(P a, P b, P c, P d) {
    if (segCross(a, b, c, d)) return 0;
    return min({segDist(a, c, d), segDist(b, c, d), segDist(c, a, b), segDist(d, a, b)});
}

// RAY from s through e.
template <class P> bool onRay(P p, P s, P e) {
    return s.cross(e, p) == 0 && (p - s).dot(e - s) >= 0;
}
template <class P> ld rayDist(P p, P s, P e) {
    return (p - s).dot(e - s) <= 0 ? (p - s).dist() : lineDist(p, s, e);
}

// Perpendicular bisector of ab, returned as two points on it.
// FLOATING P ONLY: (a+b)/2 truncates on P<ll> and returns a DIFFERENT line (for a=(0,0),
// b=(1,1) it gives y = -x instead of y = -x + 1). For an exact integer version work on the
// doubled lattice: the line through (a+b) and (a+b) + (b-a).perp().
template <class P> pair<P, P> perpBisector(P a, P b) {
    P m = (a + b) / 2;
    return {m, m + (b - a).perp()};
}
// Interior angle bisector direction at B for angle A-B-C (unit vectors summed).
template <class P> P bisectorDir(P a, P b, P c) { return (a - b).unit() + (c - b).unit(); }

// Line as a*x + b*y = c  ->  two points on it (needs floating P).
template <class P> pair<P, P> fromABC(ld A, ld B, ld C) {
    P n(A, B), p = n * (C / n.dist2());
    return {p, p + n.perp()};
}
