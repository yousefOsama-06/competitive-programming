// Needs: Geometry3D.cpp, Core.cpp (sgn).
// LINES, SEGMENTS AND TRIANGLES IN 3D. L3 is the line {o + t*d} through two points, carrying
// distance / projection / reflection of a point. On top of it the two things everyone re-derives
// under pressure: the SMALLEST DISTANCE BETWEEN TWO LINES (project the gap onto the common normal
// d1 x d2) and the PAIR OF CLOSEST POINTS realising it, one on each line. Then point-segment,
// segment-segment and point-triangle distance, and SEGMENT x TRIANGLE intersection (does segment
// d-e cross triangle abc, and where). Everything is O(1) time and memory.
// You reach for this the moment the input stops being planar: two 3D lines generically MISS, so
// lineInter is meaningless and "distance between the lines" replaces it; and mesh problems
// (visibility, is this ray blocked, closest approach of two moving points) are exactly segTri and
// segSegDist3.
// DEGENERATE: parallel lines fall back to point-line, and lineClosest then returns {a.o, its foot
// on b} - one of infinitely many optimal pairs. A zero-length segment degrades to its point. A
// collinear triangle makes triDist3 return the min over its three edges and makes segTri report
// -1. segTri with d == e reports 0 (or -1 if that point is in the plane of abc), never 1.
// EXACTNESS: the predicates are sgn(vol6), so segTri is EXACT on P3<ll> up to |coord| <= 5.7e5
// (the vol6 budget of Geometry3D.cpp; wrap vol6 in __int128 for 1e9). triDist3's inside test and
// L3::dist2 are DEGREE-4 forms (((v-u) x (p-u)).n and |d x (p-o)|^2), so they hold in ll only for
// |coord| <= 1.5e4 - again __int128 lifts that to 9e8. Anything returning a distance or a point
// divides, so it comes back in ld whatever you passed in; and note
// proj / refl / lineClosest silently TRUNCATE on P3<ll>. The eps tests below are absolute, so
// scale your input to O(1)..O(1e6) if you care about near-degenerate cases.
template <class P> struct L3 {
    P o, d;                                                  // {o + t*d}; d must be nonzero
    L3(P a = P(), P b = P()) : o(a), d(b - a) {}             // point + DIRECTION: L3(o, o + dir)
    ld dist2(P p) const { return (ld)d.cross(p - o).dist2() / d.dist2(); }
    ld dist(P p) const { return sqrtl(dist2(p)); }           // ratio of two exact integer forms
    P proj(P p) const { return o + d * ((ld)d.dot(p - o) / d.dist2()); }
    P refl(P p) const { return proj(p) * 2 - p; }
    bool cmpProj(P p, P q) const { return d.dot(p) < d.dot(q); }   // sort points ALONG the line
};
// SMALLEST DISTANCE BETWEEN TWO LINES: the gap vector measured along the common normal.
template <class P> ld lineLineDist(L3<P> a, L3<P> b) {
    P n = a.d.cross(b.d);
    if (n == P()) return a.dist(b.o);                        // parallel: any point of b will do
    return fabsl((ld)(b.o - a.o).dot(n)) / n.dist();
}
// The pair of points realising it, {on a, on b}. The point on a is where a pierces the plane
// spanned by b and the common normal, so the "closest point on a" needs only one division.
template <class P> pair<P, P> lineClosest(L3<P> a, L3<P> b) {
    P n = a.d.cross(b.d);
    if (n == P()) return {a.o, b.proj(a.o)};                 // parallel: infinitely many pairs
    P na = b.d.cross(n), nb = a.d.cross(n);
    return {a.o + a.d * ((ld)(b.o - a.o).dot(na) / a.d.dot(na)),
            b.o + b.d * ((ld)(a.o - b.o).dot(nb) / b.d.dot(nb))};
}
// POINT -> SEGMENT. Clamp the line parameter to [0, 1] and expand the norm, so no point of type
// P is ever constructed: this stays correct when you hand it P3<ll>.
template <class P> ld segDist3(P p, P a, P b) {
    P u = b - a, w = p - a;
    ld L = (ld)u.dist2(), uw = (ld)u.dot(w);
    if (L < eps) return w.dist();                            // a == b
    ld t = min((ld)1, max((ld)0, uw / L));
    return sqrtl(max((ld)0, (ld)w.dist2() - t * (2 * uw - t * L)));
}
// SEGMENT -> SEGMENT. The minimum is either at an endpoint or at the unconstrained closest pair
// of the two LINES, and that one only counts when both parameters land strictly inside (0, 1).
// den = |u|^2|v|^2 - (u.v)^2 loses absolute precision at huge coordinates, but a garbage (s, t)
// still names two REAL points of the segments, so it can only fail to improve the answer.
template <class P> ld segSegDist3(P a, P b, P c, P d) {
    P u = b - a, v = d - c, w = a - c;
    ld A = (ld)u.dist2(), B = (ld)u.dot(v), C = (ld)v.dist2();
    ld D = (ld)u.dot(w), E = (ld)v.dot(w), W = (ld)w.dist2(), den = A * C - B * B;
    ld r = min({segDist3(a, c, d), segDist3(b, c, d), segDist3(c, a, b), segDist3(d, a, b)});
    if (den > eps) {
        ld s = (B * E - C * D) / den, t = (A * E - B * D) / den;
        if (s > 0 && s < 1 && t > 0 && t < 1)                 // |w + s*u - t*v|^2, expanded
            r = min(r, sqrtl(max((ld)0, W + s * s * A + t * t * C
                                      + 2 * s * D - 2 * t * E - 2 * s * t * B)));
    }
    return r;                                                // parallel / collinear: endpoints win
}
// POINT -> TRIANGLE (the solid triangle). Project onto the plane; if the projection is inside,
// the plane distance is the answer, else the min over the three edges. The inside test uses p
// itself, NOT the projection: p and its projection differ by a multiple of n, and ((v-u) x n).n
// is 0, so the three orientations are unchanged - which keeps the test exact on integers.
template <class P> ld triDist3(P p, P a, P b, P c) {
    P n = (b - a).cross(c - a);
    ld e = min({segDist3(p, a, b), segDist3(p, b, c), segDist3(p, c, a)});
    if (n.dist2() < eps) return e;                           // collinear abc: no plane to hit
    auto f = [&](P u, P v) { return (v - u).cross(p - u).dot(n); };
    auto s1 = f(a, b), s2 = f(b, c), s3 = f(c, a);
    bool in = (s1 >= 0 && s2 >= 0 && s3 >= 0) || (s1 <= 0 && s2 <= 0 && s3 <= 0);
    return in ? fabsl((ld)n.dot(p - a)) / n.dist() : e;      // no eps: the two branches AGREE on
}                                                            // the boundary, so either is right
// SEGMENT d-e x TRIANGLE a-b-c, by four orientation signs and nothing else.
// {1, p} they meet in the single point p (grazing a vertex or an edge counts);  {0, _} disjoint;
// {-1, _} d-e is COPLANAR with abc, where the meet can be empty, a point or a whole segment -
// map both to 2D with 03 - CoplanarFrame.cpp and clip there.
template <class P> pair<int, Pd3> segTri(P a, P b, P c, P d, P e) {
    auto va = vol6(a, b, c, d), vb = vol6(a, b, c, e);
    int sa = sgn(va), sb = sgn(vb);
    if (!sa && !sb) return {-1, Pd3()};                      // (also fires when abc is collinear)
    if (sa == sb) return {0, Pd3()};                         // segment strictly on one side
    int t1 = sgn(vol6(d, e, a, b)), t2 = sgn(vol6(d, e, b, c)), t3 = sgn(vol6(d, e, c, a));
    // The LINE d-e pierces the triangle iff it turns the same way around all three edges.
    if (!((t1 >= 0 && t2 >= 0 && t3 >= 0) || (t1 <= 0 && t2 <= 0 && t3 <= 0))) return {0, Pd3()};
    Pd3 D((ld)d.x, (ld)d.y, (ld)d.z), E((ld)e.x, (ld)e.y, (ld)e.z);
    return {1, (E * (ld)va - D * (ld)vb) / ((ld)va - (ld)vb)};    // = planeLine, in vol6 terms
}
/* RELATED
 SEGMENT x PLANE          planeLine (01) then check the parameter lies in [0, 1].
 TRIANGLE -> TRIANGLE     min over the 6 (edge, triangle) pairs of segment-to-triangle distance;
                          that one is a ternary search over the segment parameter - the distance
                          from a point to a convex set is convex, so it is safe.
 SEGMENT -> TRIANGLE      same ternary search; 0 exactly when segTri says 1.
 RAY x TRIANGLE           same four signs, but replace the sa != sb test with sa != 0 and sa
                          opposite to the side the ray points at (Moller-Trumbore is this test
                          with the divisions folded in - no reason to type it).
 TWO SEGMENTS MEET        in 3D only if coplanar: vol6(a, b, c, d) == 0, then use the 2D
                          segInter inside a coplanar frame. segSegDist3 == 0 is the cheap test.
 CLOSEST APPROACH of two points moving linearly = segSegDist3 of the two trajectories with a
                          shared parameter; if the speeds differ, minimise |w + t(v1 - v2)|^2
                          directly, t* = -w.(v1-v2) / |v1-v2|^2 clamped to the time window. */
