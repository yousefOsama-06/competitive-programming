// Needs: Core.cpp, Lines.cpp, Polygon.cpp.
// POLYGON AGAINST POLYGON: clipping, intersection, containment, distance, and the O(log n)
// queries you get once one side is a convex hull.

// --- IS THIS POLYGON SIMPLE? O(n^2), exact on P<ll>. ---
// Every polygon routine (inPoly, area, triangulate, Pick, polyUnion) silently returns nonsense on
// a self-intersecting polygon, so this is also the oracle you stress-test them with.
template <class P> bool isSimple(const vector<P>& v) {
    int n = v.size();
    if (n < 3) return false;
    for (int i = 0; i < n; i++) {
        if (v[i] == v[(i + 1) % n]) return false;                  // zero-length edge
        for (int j = i + 1; j < n; j++) {
            int a = i, b = (i + 1) % n, c = j, d = (j + 1) % n;
            if (a == c || a == d || b == c || b == d) {            // adjacent edges: they may only
                if (a == d || b == c) {                            // share the one common vertex
                    P sh = (a == d) ? v[a] : v[b];
                    for (P q : {v[a], v[b], v[c], v[d]})
                        if (!(q == sh) && onSeg(q, v[a], v[b]) &&
                            onSeg(q, v[c], v[d])) return false;
                }
                continue;
            }
            if (segCross(v[a], v[b], v[c], v[d])) return false;
        }
    }
    return true;
}
// --- WINDING NUMBER. inPoly uses the EVEN-ODD rule, which is wrong for a self-intersecting
// outline and for "polygon with holes given as one vertex list". The nonzero rule uses this. ---
template <class P> int windingNumber(const vector<P>& v, P p) {    // 0 = outside (nonzero rule)
    int n = v.size(), w = 0;
    for (int i = 0; i < n; i++) {
        P a = v[i], b = v[(i + 1) % n];
        if (onSeg(p, a, b)) return 0;                        // on the boundary: caller decides
        if (a.y <= p.y) { if (b.y > p.y && a.cross(b, p) > 0) w++; }
        else if (b.y <= p.y && a.cross(b, p) < 0) w--;
    }
    return w;
}
// --- SUTHERLAND-HODGMAN: clip any polygon by a CONVEX window, O(n * m). ---
// The window must be convex and counter-clockwise; the subject may be concave.
// This is the general "intersect with a convex region" and it is just `cut` in a loop.
template <class P> vector<P> clipByConvex(vector<P> subject, const vector<P>& win) {
    int m = win.size();
    for (int i = 0; i < m && !subject.empty(); i++)
        subject = cut(subject, win[i], win[(i + 1) % m]);
    return subject;
}
// --- CONVEX intersect CONVEX in O(n + m) is the classic O'Rourke walk, but clipByConvex is
// O(nm) and fits on one line - at contest sizes (n, m <= a few thousand) prefer it. Use
// halfPlaneInter (06) when the polygons come as half-plane constraints, not vertex lists.

// --- MINIMUM DISTANCE BETWEEN TWO CONVEX POLYGONS, O(n * m) as written. ---
// 0 if they touch or overlap. The O(n+m) rotating-calipers version exists but this is 6 lines and
// the distance is a floating answer anyway. Equivalent trick: dist(origin, minkowski(A, -B)).
template <class P> ld convexDist(const vector<P>& A, const vector<P>& B) {
    for (P p : A) if (inPoly(B, p, false)) return 0;
    for (P p : B) if (inPoly(A, p, false)) return 0;
    ld best = 1e18;
    int n = A.size(), m = B.size();
    for (int i = 0; i < n; i++) for (int j = 0; j < m; j++) {
        P a = A[i], b = A[(i + 1) % n], c = B[j], d = B[(j + 1) % m];
        if (segCross(a, b, c, d)) return 0;
        best = min(best, segSegDist(a, b, c, d));
    }
    return best;
}
// --- TANGENTS FROM A POINT TO A CONVEX POLYGON, O(n) here (O(log n) with a binary search). ---
// Returns the two vertex indices l, r such that the whole polygon lies to one side of p->h[l]
// and of p->h[r]: the visible arc is h[r], h[r+1], ..., h[l]. p must be strictly OUTSIDE.
// USES: visibility ("how much of the wall can I see"), shadow/silhouette, and "add p to the hull".
template <class P> pair<int, int> pointTangents(const vector<P>& h, P p) {
    int n = h.size(), l = 0, r = 0;
    for (int i = 1; i < n; i++) {
        if (p.cross(h[i], h[l]) > 0) l = i;                        // h[l] is the CCW-most
        if (p.cross(h[i], h[r]) < 0) r = i;                        // h[r] is the CW-most
    }
    return {l, r};
}
// --- LINE x CONVEX POLYGON in O(log n): which two edges does the line a->b cross? ---
// Returns {-1,-1} for no hit; {i,-1} for a single touched corner i; {i,j} when the line crosses
// the edges (i,i+1) and (j,j+1). h must be a strictly convex CCW hull (no collinear vertices).
// USES: "length of the segment inside the polygon" per query, in O(log n) instead of O(n).
template <class P> pair<int, int> lineHull(const vector<P>& h, P a, P b) {
    int n = h.size();
    int endA = extreme(h, (a - b).perp()), endB = extreme(h, (b - a).perp());
    auto cmpL = [&](int i) { return sgn(a.cross(h[i], b)); };
    if (cmpL(endA) < 0 || cmpL(endB) > 0) return {-1, -1};         // whole hull on one side
    auto bs = [&](int lo, int hi) {                                // last index still >= 0
        int step = (hi - lo + n) % n;
        for (int s = step; s > 0; s /= 2)
            while (s && cmpL((lo + s) % n) >= 0) lo = (lo + s) % n, step -= s;
        return lo;
    };
    int i = bs(endB, endA), j = bs(endA, endB);
    if (i == j) return {i, -1};
    return {i, j};
}
/* MORE POLYGON FACTS AND RECIPES
 AREA OF INTERSECTION of two CONVEX polygons: clipByConvex then area(). For two ARBITRARY simple
   polygons, triangulate one (10 - Triangulation.cpp) and clip each triangle by the other.
 AREA OF THE UNION: |A| + |B| - |A n B|, or polyUnion (07 - Unions.cpp) for many polygons at once.
 SYMMETRIC DIFFERENCE: |A| + |B| - 2|A n B|.
 POINT IN A POLYGON WITH HOLES: outer boundary CCW, every hole CW, then the sign of the total
   winding number is the answer (this is exactly why windingNumber exists).
 CONVEX POLYGON CONTAINS CONVEX POLYGON: every vertex of B is in A (inConvex, O(m log n)).
 CENTROID OF THE PERIMETER (not of the area): sum of edge midpoints weighted by edge LENGTH.
   The two differ - "balance the wire frame" vs "balance the plate".
 POLYGON DIAMETER / WIDTH / MIN RECTANGLE: 03 - Polygon.cpp (rotating calipers).
 REGULAR n-GON of circumradius R centred at c: c + R*(cos(2*pi*k/n + t), sin(2*pi*k/n + t)).
   Its area is (n/2) R^2 sin(2 pi/n); with INRADIUS r it is n r^2 tan(pi/n).
 SPLIT A POLYGON BY A LINE into both halves: cut(v, s, e) and cut(v, e, s).
 LARGEST INSCRIBED / SMALLEST ENCLOSING: smallest enclosing circle is mec (04 - Circles.cpp);
   smallest enclosing rectangle is minRectArea; largest inscribed CIRCLE in a convex polygon is
   the Chebyshev centre, 18 - InscribedCircle.cpp (a nested ternary search, no LP needed);
   largest inscribed AXIS-ALIGNED rectangle needs a sweep and is genuinely hard - do not improvise.
 OFFSET / BUFFER a convex polygon by d: push every edge outward by d along its normal and
   intersect the half-planes (06 - HalfPlaneIntersection.cpp). The exact offset of a NON-convex
   polygon has circular arcs at the reflex corners - approximate them.
 TRIANGLE CONTAINS POINT, exact: the three orientations all have the same sign (or one is 0 for
   the boundary). Faster than inPoly for n = 3 and it is the inner loop of triangulate. */
