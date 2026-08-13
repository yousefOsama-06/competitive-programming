// Needs: Core.cpp, Polygon.cpp (area2).
// LARGEST INSCRIBED CIRCLE IN A CONVEX POLYGON - the CHEBYSHEV CENTRE. Returns {centre, radius}.
// The trick is that f(p) = min over edges of the signed distance from p to that edge's LINE is a
// minimum of affine functions, hence CONCAVE on the whole plane, and it is exactly the distance
// to the boundary for p inside. So a nested ternary search on (x, y) finds its maximum with no
// case analysis at all: no medial axis, no half-plane shrinking, no binary search on r.
// WHEN: "place a disc of maximum radius inside this region", "the point of a convex region
// farthest from its boundary", robot clearance, and 2D LP with an objective that is a min of
// linear functions (this IS that LP - maximise r subject to n linear constraints).
// COMPLEXITY: O(n * it^2) with it = 100 iterations per axis; about 1e4 * n operations, so keep n
// below a few thousand. If n is large, run the same search on the O(n) half-plane form or binary
// search r and test emptiness with halfPlaneInter (06) at O(n log n log(1/eps)).
// DEGENERATE: the polygon must be CONVEX and given in order; the code reverses a CW input for
// you. A degenerate (zero-area) polygon returns radius 0 at some point of it. Fewer than 3
// vertices returns radius 0. For a NON-CONVEX polygon f is not concave and this silently returns
// a local optimum - the correct answer there lives on the medial axis and is genuinely hard.
// FLOATING POINT, always: the answer is irrational. 100 ternary iterations over a 2e9-wide box
// leave the centre accurate to ~1e-9 * box, which is the best ld can do anyway.
pair<Pd, ld> inscribedCircle(vector<Pd> v) {
    int n = sz(v);
    if (n < 3) return {n ? v[0] : Pd(), 0};
    if (area2(v) < 0) reverse(all(v));                  // f must be POSITIVE inside
    auto f = [&](ld x, ld y) {                          // distance to the nearest edge LINE
        ld r = 1e18;
        for (int i = 0; i < n; i++)
            r = min(r, v[i].cross(v[(i + 1) % n], Pd(x, y)) / (v[(i + 1) % n] - v[i]).dist());
        return r;
    };
    ld lo = 1e18, hi = -1e18, bo = 1e18, bi = -1e18;
    for (Pd& p : v) lo = min(lo, p.x), hi = max(hi, p.x), bo = min(bo, p.y), bi = max(bi, p.y);
    auto best = [&](ld x) {                             // max over y for a fixed x, still concave
        ld a = bo, b = bi;
        for (int it = 0; it < 100; it++) {
            ld m1 = a + (b - a) / 3, m2 = b - (b - a) / 3;
            f(x, m1) < f(x, m2) ? a = m1 : b = m2;
        }
        return (a + b) / 2;
    };
    for (int it = 0; it < 100; it++) {
        ld m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        f(m1, best(m1)) < f(m2, best(m2)) ? lo = m1 : hi = m2;
    }
    ld x = (lo + hi) / 2, y = best(x);
    return {Pd(x, y), max((ld)0, f(x, y))};
}
// WHY THE NESTED SEARCH IS LEGAL: g(x) = max over y of f(x, y) is the partial maximisation of a
// concave function, and that is concave in x. The same argument licenses geoMedian (12) and every
// other nested ternary search - check concavity of the PARTIAL max, not just of f.
// LP FORM, if you prefer it or need n large: maximise r subject to
//   cross(b_i - a_i, p - a_i) / |b_i - a_i| >= r  for every edge,
// which is n constraints in three variables (x, y, r) - Seidel's randomised LP solves it in O(n)
// expected. Equivalently, binary search r, push every edge line inward by r, and ask whether the
// half-plane intersection is non-empty.
// LARGEST INSCRIBED CIRCLE CENTRED ON A GIVEN LINE: one ternary search along the line.
// LARGEST INSCRIBED AXIS-ALIGNED RECTANGLE is a different and much harder problem - it needs a
// sweep with a stack and is not a two-line variation of this. Do not improvise it under time.
// MINIMUM ENCLOSING CIRCLE is mec (04 - Circles.cpp); minimum enclosing rectangle is minRectArea
// (03 - Polygon.cpp) by rotating calipers.
