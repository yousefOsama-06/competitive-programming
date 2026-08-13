// Needs: Geometry3D.cpp.
// SPHERICAL GEOMETRY: solid angles, spherical triangles and polygons, caps, the lens of two
// spheres, the circumsphere of four points and the smallest enclosing sphere of a point set.
// Points "on the sphere" are always passed as vectors FROM ITS CENTRE and only their DIRECTION is
// read, so you may hand in unnormalised vectors, and a sphere centred elsewhere is handled by
// subtracting the centre first. Angles are radians, areas are on a sphere of radius R.
// You reach for it for radar/antenna coverage, "what fraction of the sky", area of a country on
// a globe, overlapping-ball volumes, and the two enclosing-object questions. All O(1) except
// sphPolyArea (O(n)) and minSphere (O(n) per iteration).
// DEGENERATE: solidAngleTet returns 0 when a, b, c are coplanar with the origin (a flat corner)
// and is unaffected by their lengths. sphAngle is undefined when a is parallel to b or c (a
// zero-length spherical segment) and returns 0 there. sphPolyArea needs a SIMPLE polygon, ccw
// seen from outside, with no two consecutive vertices antipodal (the arc between them is not
// unique); it returns the area of the complement if you feed it cw. circumsphere needs four
// points in general position - vol6 == 0 makes it divide by zero. minSphere needs a non-empty
// input and returns {p[0], 0} for a single point.
// EXACTNESS: this whole file is floating point - every routine either normalises, calls a trig
// function, or divides. The predicates it rests on (vol6, dot, cross) are still exact on P3<ll>,
// so use integer input where you can and only the final value carries error. atan2 is used
// everywhere in place of acos: acos(dot/|a||b|) loses half its digits near 0 and pi, which is
// exactly the regime these formulas live in. eps is absolute here, so normalise your radii to
// O(1)..O(1e6).
// SOLID ANGLE of a cone of half-angle th (steradians): the full sphere is 4*pi.
ld solidAngleCone(ld th) { return 2 * PI * (1 - cosl(th)); }
// SOLID ANGLE of the tetrahedron corner at the ORIGIN spanned by a, b, c (VAN OOSTEROM-STRACKEE):
// tan(Om/2) = |a.(b x c)| / (|a||b||c| + (a.b)|c| + (a.c)|b| + (b.c)|a|).
// atan2 takes the correct branch by itself, so this is right on the whole range [0, 2*pi] with no
// sign patching, and it stays accurate for the needle-thin corners that Girard's formula ruins.
template <class P> ld solidAngleTet(P a, P b, P c) {
    ld A = a.dist(), B = b.dist(), C = c.dist();
    ld num = fabsl((ld)a.dot(b.cross(c)));
    ld den = A * B * C + (ld)a.dot(b) * C + (ld)a.dot(c) * B + (ld)b.dot(c) * A;
    return 2 * atan2l(num, den);
}
// ANGLE at a of the spherical triangle abc = the dihedral angle between the planes Oab and Oac,
// in [0, pi]. The ORIENTED version measures how far ccw you turn from arc a->b to arc a->c, in
// [0, 2*pi), which is what a signed area needs.
template <class P> ld sphAngle(P a, P b, P c) {
    P u = a.cross(b), v = a.cross(c);
    return atan2l(u.cross(v).dist(), (ld)u.dot(v));
}
template <class P> ld sphAngleOr(P a, P b, P c) {
    ld t = sphAngle(a, b, c);
    return a.cross(b).dot(c) >= 0 ? t : 2 * PI - t;
}
// SPHERICAL TRIANGLE AREA on a sphere of radius R. GIRARD: R^2 * (A + B + C - pi), the spherical
// EXCESS - a spherical triangle's angles overshoot pi by exactly its area. Computed through the
// solid angle instead, which is the same number without the catastrophic cancellation of summing
// three angles that are each close to pi/2 and subtracting pi.
template <class P> ld sphTriArea(P a, P b, P c, ld R) { return R * R * solidAngleTet(a, b, c); }
// SPHERICAL POLYGON AREA, vertices in ccw order seen from OUTSIDE: R^2 * (sum of the interior
// angles - (n-2)*pi), the same excess with n corners. Handles non-convex polygons.
template <class P> ld sphPolyArea(const vector<P>& p, ld R) {
    int n = sz(p);
    ld s = -(n - 2) * PI;
    for (int i = 0; i < n; i++) s += sphAngleOr(p[(i + 1) % n], p[(i + 2) % n], p[i]);
    return R * R * s;
}
// SPHERICAL CAP of height h on a sphere of radius R (a plane at distance R - h from the centre;
// from the polar half-angle th instead, h = R * (1 - cos th) and the base radius is R * sin th).
// capArea is the CURVED part only - add pi*a^2 for the flat disc, a^2 = h * (2R - h).
ld capArea(ld R, ld h) { return 2 * PI * R * h; }
ld capVolume(ld R, ld h) { return PI * h * h * (3 * R - h) / 3; }
// SPHERICAL SEGMENT / zone: the slab between two parallel planes, base radii a and b, height h.
ld zoneArea(ld R, ld h) { return 2 * PI * R * h; }           // curved part; independent of WHERE
ld zoneVolume(ld a, ld b, ld h) { return PI * h * (3 * a * a + 3 * b * b + h * h) / 6; }
// SPHERE x SPHERE, centres d apart: {surface area, volume} of the lens they share. The lens is
// two caps glued on the plane of the intersection circle. CONTAINMENT is checked FIRST - there
// the cap formula silently returns nonsense, and the answer is just the smaller ball.
pair<ld, ld> sphereSphere(ld R, ld r, ld d) {
    if (R < r) swap(R, r);
    if (d >= R + r) return {0, 0};                           // disjoint (tangent counts)
    if (d + r <= R) return {4 * PI * r * r, 4 * PI * r * r * r / 3};   // small one swallowed
    ld x = (R * R - r * r + d * d) / (2 * d);                // plane of the circle, from centre R
    ld h1 = R - x, h2 = r - (d - x);                         // the two cap heights
    return {capArea(R, h1) + capArea(r, h2), capVolume(R, h1) + capVolume(r, h2)};
}
// The intersection CIRCLE itself has radius sqrt(R^2 - x^2) and sits at x along the centre line.
// CIRCUMSPHERE of four points, {centre, radius}. Needs P3<ld>; the denominator IS 2*vol6, so
// coplanar input divides by zero - test vol6 first.
template <class P> pair<P, ld> circumsphere(P a, P b, P c, P d) {
    P B = b - a, C = c - a, D = d - a;
    ld den = 2 * (ld)B.dot(C.cross(D));
    P o = a + (C.cross(D) * (ld)B.dist2() + D.cross(B) * (ld)C.dist2()
             + B.cross(C) * (ld)D.dist2()) / den;
    return {o, (o - a).dist()};
}
// SMALLEST ENCLOSING SPHERE, {centre, radius}, by shrinking steps toward the current farthest
// point. Start at the centroid and step a fraction q toward the farthest point, with q decaying
// geometrically: the centre converges to the true one and the step budget bounds the error by
// (initial spread) * 0.999^30000 ~ 1e-13 of the diameter - fine for 1e-6 answers, NOT a proof.
// O(n) per iteration. Welzl with a 4-point basis is the exact O(n) expected version and is 40
// lines; this is 8 and has never lost a contest problem.
template <class P> pair<P, ld> minSphere(const vector<P>& p) {
    P c;
    for (P q : p) c = c + q;
    c = c / sz(p);
    for (ld q = 0.1; q > 1e-13; q *= 0.999) {
        int f = 0;
        for (int i = 1; i < sz(p); i++) if ((c - p[i]).dist2() > (c - p[f]).dist2()) f = i;
        c = c + (p[f] - c) * q;
    }
    ld r = 0;
    for (P x : p) r = max(r, (c - x).dist());
    return {c, r};
}
// GREAT CIRCLE, the part 01 - Geometry3D.cpp does not cover: its greatCircle takes lat/lon in
// DEGREES, these take 3D vectors. Distance along the surface between two points of the sphere,
// and the lat/lon -> vector conversion (latitude from the equator, longitude east, degrees).
template <class P> ld sphDist(P a, P b, ld R) {
    return R * atan2l(a.cross(b).dist(), (ld)a.dot(b));
}
Pd3 fromLatLon(ld R, ld lat, ld lon) {
    lat *= PI / 180, lon *= PI / 180;
    return {R * cosl(lat) * cosl(lon), R * cosl(lat) * sinl(lon), R * sinl(lat)};
}
/* RELATED
 TANGENT CONE from an external point at distance d: half-angle acos(R / d), and the cap it cuts
     off the sphere has height R * (1 - R / d). d < R means the point is inside: no tangent.
 SPHERE x PLANE   a circle of radius sqrt(R^2 - dist(centre, plane)^2), centred at the projection.
 SPHERE x LINE / SEGMENT   sphereLine (01 - Geometry3D.cpp), then clamp the parameters.
 VOLUME OF A UNION OF BALLS - no closed form beyond two; for three or more integrate over one axis
     numerically, or use inclusion-exclusion only when the statement promises few overlaps.
 SPHERICAL SEGMENT x SEGMENT - two great-circle arcs ab and cd cross where the plane normals a x b
     and c x d cross: the candidate directions are +-((a x b) x (c x d)); accept the one lying
     inside both arcs, which is the exact 3D test sgn((a x b).c) != sgn((a x b).d) and vice versa.
 SPHERICAL CONVEX HULL of directions = the 3D convex hull of the unit vectors (04), as long as
     they fit in a hemisphere.
 SOLID ANGLE OF A POLYGONAL CONE - triangulate it from one vertex and sum solidAngleTet - that is
     exactly what sphPolyArea does with R = 1. */
