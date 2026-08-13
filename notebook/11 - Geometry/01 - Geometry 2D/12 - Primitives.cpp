// Needs: Core.cpp, Lines.cpp, Circles.cpp.
// THE SMALL-FUNCTION LAYER. Individually trivial, collectively the thing you actually spend
// contest minutes re-deriving. Everything here is one to six lines.

// --- ANGLE BISECTORS OF TWO LINES. Two lines have TWO bisectors, perpendicular to each other.
// Direction of the internal one = the sum of the unit direction vectors; external = difference.
// (Take the pair whose sign matches the side you want; if the lines are parallel the bisector is
// the parallel line halfway between them.)
template <class P> pair<P, P> lineBisectors(P d1, P d2) {          // returns the two DIRECTIONS
    P u = d1.unit(), v = d2.unit();
    return {u + v, u - v};
}
// Internal bisector of the angle at b in triangle a-b-c, as a direction from b:
template <class P> P angleBisector(P a, P b, P c) { return (a - b).unit() + (c - b).unit(); }
// The internal bisector from A meets BC at the point dividing it in ratio AB : AC.
template <class P> P bisectorFoot(P a, P b, P c) {
    ld x = (a - b).dist(), y = (a - c).dist();
    return (b * y + c * x) / (x + y);
}
// --- ROTATE / SCALE ABOUT AN ARBITRARY CENTRE (rot() in Core is about the origin). ---
template <class P> P rotAbout(P p, P c, ld a) { return c + (p - c).rot(a); }
template <class P> P scaleAbout(P p, P c, ld k) { return c + (p - c) * k; }
// Reflect p across the POINT c (a 180-degree rotation):
template <class P> P reflPoint(P p, P c) { return c * 2 - p; }
// --- SIGNED AREA OF A TRIANGLE, and the barycentric coordinates of p in a-b-c. ---
template <class P> auto triArea2(P a, P b, P c) { return a.cross(b, c); }         // 2 * signed
template <class P> array<ld, 3> barycentric(P a, P b, P c, P p) {
    ld s = a.cross(b, c);
    return {(ld)p.cross(b, c) / s, (ld)a.cross(p, c) / s, (ld)a.cross(b, p) / s};
}
// p is inside the triangle iff all three coordinates are >= 0 (they always sum to 1).
template <class P> bool inTriangle(P a, P b, P c, P p) {           // exact, boundary included
    int s1 = sgn(a.cross(b, p)), s2 = sgn(b.cross(c, p)), s3 = sgn(c.cross(a, p));
    return (s1 >= 0 && s2 >= 0 && s3 >= 0) || (s1 <= 0 && s2 <= 0 && s3 <= 0);
}
// --- SEGMENT x CIRCLE: clip the segment a-b to the disc, returning the surviving piece. ---
template <class P> vector<P> segCircle(P a, P b, P c, ld r) {
    auto v = circleLine(c, r, a, b);                               // 0, 1 or 2 points
    vector<P> res;
    for (P p : v) if (onSeg(p, a, b)) res.push_back(p);
    return res;
}
// --- CIRCLE THROUGH THREE POINTS = circumcenter (04 - Circles.cpp); its radius = circumradius.
// CIRCLE THROUGH TWO POINTS WITH A GIVEN RADIUS: the two centres are m +- h * (b-a).perp().unit()
// where m is the midpoint and h = sqrt(r^2 - |b-a|^2/4). None if 2r < |b-a|.
template <class P> vector<P> circlesThrough(P a, P b, ld r) {
    ld d2 = (b - a).dist2(), h2 = r * r - d2 / 4;
    if (d2 < eps || h2 < 0) return {};                             // a == b: infinitely many
    P m = (a + b) / 2, dir = (b - a).perp().unit() * sqrtl(h2);
    return h2 < eps ? vector<P>{m} : vector<P>{m - dir, m + dir};
}
// --- CIRCLE OF RADIUS r THROUGH p AND TANGENT TO THE LINE a-b. Its centre is at distance r from
// the line, so on one of the two PARALLELS at offset r, and at distance r from p, so on the
// circle (p, r). Intersect the two: 0, 1 or 2 centres. With d = distance from p to the line,
// there are 2 for d < 2r (both, tangentially, when d == 0), 1 when d == 2r, and none beyond.
template <class P> vector<P> circleTangentLine(P a, P b, P p, ld r) {
    vector<P> res;
    P n = (b - a).perp().unit() * r;
    for (int s = -1; s <= 1; s += 2)
        for (P c : circleLine(p, r, a + n * s, b + n * s)) res.push_back(c);
    return res;
}
// --- APOLLONIUS CIRCLE: the locus of p with |pa| / |pb| = k (k != 1) is a circle whose diameter
// endpoints are the two points dividing ab internally and externally in ratio k : 1.
template <class P> pair<P, ld> apollonius(P a, P b, ld k) {
    P u = (b * k + a) / (k + 1), v = (b * k - a) / (k - 1);        // internal, external
    return {(u + v) / 2, (u - v).dist() / 2};
}
// --- IS THE QUADRILATERAL a,b,c,d CONCYCLIC? Exact on P<ll> with __int128 (in-circle test). ---
// > 0: d strictly inside the circumcircle of a CCW triangle a,b,c;  == 0: concyclic.
template <class P> int inCircle(P a, P b, P c, P d) {
    auto f = [&](P p) { return (__int128)(p - d).dist2(); };
    __int128 det = (__int128)(a.x - d.x) * ((b.y - d.y) * f(c) - (c.y - d.y) * f(b))
                 - (__int128)(a.y - d.y) * ((b.x - d.x) * f(c) - (c.x - d.x) * f(b))
                 + f(a) * ((__int128)(b.x - d.x) * (c.y - d.y)
                         - (__int128)(c.x - d.x) * (b.y - d.y));
    return (det > 0) - (det < 0);
}
// --- MAXIMUM POINTS COVERED BY A CIRCLE OF RADIUS r, O(n^2 log n). ---
// For each i, every j within 2r gives an ARC of centre-angles that cover both; sweep the arcs.
template <class P> int maxCovered(const vector<P>& p, ld r) {
    int n = p.size(), best = 1;
    for (int i = 0; i < n; i++) {
        vector<pair<ld, int>> ev;
        int base = 1;                                              // p[i], plus its duplicates
        for (int j = 0; j < n; j++) if (j != i) {
            ld d = (p[j] - p[i]).dist();
            if (d > 2 * r + eps) continue;
            if (d < eps) { base++; continue; }                      // DUPLICATE of p[i]: the
                                                                    // division below would be 0/0
                                                                    // and it is always covered
            ld th = (p[j] - p[i]).angle(), ph = acosl(min((ld)1, max((ld)-1, d / (2 * r))));
            ld s = th - ph, e = th + ph;                           // the arc of valid centres
            while (s < -PI) s += 2 * PI;                           // normalise, then SPLIT the
            e = s + 2 * ph;                                        // arcs that wrap past +pi -
            // 0 = arc opens, 1 = arc closes. Pair-sorting then puts the OPEN first at an equal
            // angle, which is what makes a TANGENTIAL arc (d == 2r, so the arc is a single point)
            // still count - with {+1}/{-1} the close sorts first and the point is lost.
            if (e <= PI) ev.push_back({s, 0}), ev.push_back({e, 1});
            else {
                ev.push_back({s, 0}), ev.push_back({PI, 1});
                ev.push_back({-PI, 0}), ev.push_back({e - 2 * PI, 1});
            }
        }
        sort(all(ev));
        int cur = base;
        best = max(best, cur);
        for (auto& [a, t] : ev) best = max(best, cur += t ? -1 : 1);
    }
    return best;                                                   // the circle passes through p[i]
}
// --- GEOMETRIC MEDIAN (minimise the SUM of distances). Nested ternary search - the objective is
// convex, so this is exact to any eps and cannot get stuck. Weiszfeld iteration is faster but
// breaks when the iterate lands exactly on a sample point.
template <class P> P geoMedian(const vector<P>& p) {
    auto f = [&](ld x, ld y) { ld s = 0; for (auto& q : p) s += (P{x, y} - q).dist(); return s; };
    auto fy = [&](ld x) { ld lo = -2e9, hi = 2e9;
        for (int it = 0; it < 100; it++) { ld a = lo + (hi - lo) / 3, b = hi - (hi - lo) / 3;
            f(x, a) < f(x, b) ? hi = b : lo = a; } return (lo + hi) / 2; };
    ld lo = -2e9, hi = 2e9;
    for (int it = 0; it < 100; it++) {
        ld a = lo + (hi - lo) / 3, b = hi - (hi - lo) / 3;
        f(a, fy(a)) < f(b, fy(b)) ? hi = b : lo = a;
    }
    ld x = (lo + hi) / 2;
    return {x, fy(x)};
}
/* THE REST OF THE SMALL LAYER, as one-liners you can reconstruct instantly
 COLLINEAR a, b, c            a.cross(b, c) == 0
 PARALLEL / PERPENDICULAR     d1.cross(d2) == 0   /   d1.dot(d2) == 0
 SAME SIDE OF A LINE          sgn(a.cross(b, p)) == sgn(a.cross(b, q))
 POINT ON A RAY / SEGMENT     onRay, onSeg (02 - Lines.cpp)
 DISTANCE point-line/segment  lineDist, segDist    ray: rayDist    seg-seg: segSegDist
 PROJECT / REFLECT over a line  proj, refl (02 - Lines.cpp)
 ANGLE between two vectors    atan2(cross, dot) -> signed, in (-pi, pi]. NEVER acos(dot/|a||b|):
                              it loses all precision near 0 and pi and cannot give you a sign.
 ROTATE 90 DEGREES            p.perp() = (-y, x). Twice = negation. This is the whole of "turn
                              left"; you almost never need a general rot() for a right angle.
 SORT BY ANGLE                angCmp (01 - Core.cpp), NOT atan2 - exact and 5x faster.
 AREA OF A TRIANGLE           |cross| / 2. Heron only if you are given the three SIDES, and then
                              use the sorted stable form: sort a >= b >= c and
                              A = sqrt((a+(b+c))(c-(a-b))(c+(a-b))(a+(b-c))) / 4.
 TRIANGLE CENTRES             centroid (a+b+c)/3, circumcenter, incenter, orthocenter (04).
                              EULER LINE: O, G, N, H are collinear with OG:GN:NH = 2:1:3, and
                              H = A + B + C - 2O. Nine-point centre = midpoint of OH, radius R/2.
 TRIANGLE IDENTITIES          A = rs = abc/(4R);  R = abc/(4A);  r = A/s;  r_a = A/(s-a)
                              law of cosines c^2 = a^2 + b^2 - 2ab cos C
                              law of sines a/sin A = 2R
                              OI^2 = R(R - 2r)  =>  EULER'S INEQUALITY R >= 2r
                              Stewart (cevian d to BC split m + n): b^2 m + c^2 n = a(d^2 + mn)
                              median m_a = sqrt(2b^2 + 2c^2 - a^2)/2
 CYCLIC QUADRILATERAL         PTOLEMY: AC*BD = AB*CD + BC*AD (in cyclic order). For ANY four
                              points AC*BD <= AB*CD + BC*AD, equality iff concyclic in that order.
                              BRAHMAGUPTA area = sqrt((s-a)(s-b)(s-c)(s-d)).
                              General quadrilateral (BRETSCHNEIDER, angle-free):
                              A = sqrt(4p^2q^2 - (b^2+d^2-a^2-c^2)^2)/4 with p, q the diagonals.
 POWER OF A POINT             pow(p) = |pc|^2 - r^2; for ANY line through p meeting the circle at
                              A and B the signed product PA*PB equals it. Outside: = tangent^2.
                              RADICAL AXIS = equal power, a line perpendicular to the centre line.
                              RADICAL CENTRE of three circles with non-collinear centres: the
                              three pairwise axes are concurrent.
 DESCARTES CIRCLE THEOREM     four mutually tangent circles, curvatures k_i = +-1/r_i (negative
                              for the enclosing one):  (k1+k2+k3+k4)^2 = 2(k1^2+k2^2+k3^2+k4^2),
                              so k4 = k1+k2+k3 +- 2 sqrt(k1k2 + k2k3 + k3k1).
 LATTICE POINTS               on a segment: gcd(|dx|, |dy|) + 1 endpoints included.
                              inside a polygon: PICK, I = A - B/2 + 1 (simple polygons only).
                              under a line, in O(log): floorSum (04 - Number Theory/01/03).
 HELLY (2D)                   if every THREE of a family of convex sets meet, all of them meet.
 RADON (2D)                   any FOUR points split into two sets whose hulls intersect.
 CARATHEODORY (2D)            a point in the hull of S is in the hull of at most THREE points of S
                              - which is exactly why the minimum enclosing circle needs only 3.
 ART GALLERY                  floor(n/3) guards always suffice for a simple n-gon (triangulate,
                              3-colour, take the smallest class). Orthogonal polygon: floor(n/4).
 EULER, PLANAR                V - E + F = 1 + C. n lines in general position cut the plane into
                              1 + n + C(n,2) regions, C(n-1,2) of them bounded; n circles in
                              general position give n^2 - n + 2 regions.
 DUALITY                      point (a,b) <-> line y = ax - b. Incidence and above/below are
                              preserved, so "max collinear points" becomes "max concurrent lines"
                              and a convex hull becomes an upper envelope. */
