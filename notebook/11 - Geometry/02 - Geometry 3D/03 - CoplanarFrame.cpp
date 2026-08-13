// Needs: Geometry3D.cpp, Core.cpp, Polygon.cpp.
// THE COPLANAR FRAME - the single most valuable file in 3D geometry, because it deletes 3D.
// Given points known to lie on one plane, it builds a frame (o, dx, dy, dz) with dx, dy in the
// plane and dz along the normal, and maps each point to a 2D point: pos2d. From there EVERY 2D
// routine in the notebook (hull, area2, inPoly, inConvex, rotating calipers, cut, minkowski,
// Pick, ...) applies UNCHANGED, and to3d maps the answer back. pos3d keeps the third coordinate,
// which is the signed offset off the plane - 0 exactly for the coplanar points.
// You reach for it whenever a statement says "a polygon in space", "the points lie on a common
// plane", or you have just cut a polyhedron by a plane and hold the cross-section as a bag of
// unordered 3D points. Building it is O(1); mapping is O(1) per point.
//
// TWO FRAMES, AND THE CHOICE IS THE WHOLE POINT.
// ORTHONORMAL (ortho = true, the default): dx, dy, dz are unit and mutually perpendicular, so the
//   map is a rigid motion - DISTANCES, ANGLES and AREAS are preserved exactly as they are in 3D.
//   It normalises, so it needs P3<ld> and inherits float error.
// AFFINE (ortho = false): dx = q - p and dy = r - p as they came in, no normalisation, so it
//   stays on P3<ll> and every 2D predicate downstream stays EXACT. It does NOT preserve distances
//   or angles - a square can come out as a parallelogram - but it is an invertible linear map of
//   the plane, so it preserves COLLINEARITY, CONVEXITY, ORIENTATION, ratios along a line and
//   inside/outside. That is exactly enough for hull, inPoly, inConvex, isConvex, orientation
//   tests and point location. Areas are multiplied by the constant |dx x dy| (the 2D area2 comes
//   out |dx x dy|^2 times the area in plane coordinates, and the true 3D area is the 2D area
//   divided by |dx x dy|), so ratios of areas survive too. Never feed the affine map to anything
//   metric: closest pair, minimum enclosing circle, width, perimeter, angle sorting.
// DEGENERATE: p, q, r must not be collinear (both constructors produce dz = 0 otherwise, and the
// orthonormal one divides by 0). pos2d does not check that its argument is on the plane - an
// off-plane point is silently flattened onto it, so test (p - o).dot(dz) == 0 first if you are
// not sure. to3d is the inverse of pos2d for the ORTHONORMAL frame only; for the affine frame
// carry the input INDEX through the 2D routine instead of mapping back (see hullCoplanar), or
// solve the 2x2 Gram system: with A = dx.dx, B = dx.dy, C = dy.dy, det = AC - B^2,
// a = (C*X - B*Y)/det, b = (A*Y - B*X)/det and the point is o + dx*a + dy*b.
// EXACTNESS: affine frame on P3<ll> is exact; pos2d is a dot product, so |coord| <= 1e9 keeps it
// in ll, and a downstream area2 squares that again - drop to |coord| <= 3e4 or use __int128
// there. Orthonormal frame is floating throughout.
template <class Q> struct Frame {
    typedef decltype(Q().dot(Q())) T;                        // ll for P3<ll>, ld for P3<ld>
    Q o, dx, dy, dz;                                         // dz is the plane NORMAL
    Frame(Q p, Q q, Q r, bool ortho = true) : o(p), dx(q - p), dy(r - p) {
        dz = dx.cross(dy);                                   // exact normal, before any scaling
        if (ortho) { dx = dx.unit(); dz = dz.unit(); dy = dz.cross(dx); }
    }
    P<T> pos2d(Q p) const { return {(p - o).dot(dx), (p - o).dot(dy)}; }
    Q pos3d(Q p) const { return {(p - o).dot(dx), (p - o).dot(dy), (p - o).dot(dz)}; }
    Q to3d(P<T> p) const { return o + dx * p.x + dy * p.y; }  // ORTHONORMAL frames only
};
// CONVEX HULL OF COPLANAR 3D POINTS, returned as 3D points, ccw when seen from the dz side.
// Exact on P3<ll>: it uses the affine frame and carries the point itself through a map, so
// nothing is ever mapped back. O(n log n).
template <class Q> vector<Q> hullCoplanar(vector<Q> p) {
    sort(all(p)); p.erase(unique(all(p)), p.end());
    int n = sz(p), j = 2;
    if (n < 3) return p;
    while (j < n && (p[1] - p[0]).cross(p[j] - p[0]) == Q()) j++;
    if (j == n) return {p[0], p[n - 1]};                     // all collinear: the two extremes
    Frame<Q> F(p[0], p[1], p[j], false);
    vector<decltype(F.pos2d(p[0]))> q;
    map<decltype(F.pos2d(p[0])), Q> back;                    // pos2d is injective on the plane
    for (Q x : p) q.push_back(F.pos2d(x)), back[q.back()] = x;
    vector<Q> res;
    for (auto& y : hull(q)) res.push_back(back[y]);
    return res;
}
// IS A 3D POINT INSIDE A COPLANAR POLYGON? Exact on P3<ll>. The plane test comes FIRST: without
// it a point floating above the polygon projects straight into it and reports true.
template <class Q> bool inCoplanar(const vector<Q>& v, Q p, bool strict = true) {
    int n = sz(v), j = 2;
    while (j < n && (v[1] - v[0]).cross(v[j] - v[0]) == Q()) j++;
    if (j == n) return false;                                // degenerate polygon: no plane
    Frame<Q> F(v[0], v[1], v[j], false);
    if (sgn((p - F.o).dot(F.dz)) != 0) return false;         // off the plane
    vector<decltype(F.pos2d(p))> q;
    for (Q x : v) q.push_back(F.pos2d(x));
    return inPoly(q, F.pos2d(p), strict);
}
/* RECIPES
 AREA OF A PLANAR 3D POLYGON   no frame needed: |sum p_i x p_(i+1)| / 2, the vector area
                               (faceArea in 06 - Polyhedron.cpp). Through the frame you get the
                               same number as area(pos2d(.)) for an ORTHONORMAL frame, and
                               area(pos2d(.)) / |dx x dy| for the affine one - use that identity
                               as your sanity check that the frame is built right.
 IS A SET OF 3D POINTS COPLANAR - fix the first three non-collinear ones, then vol6 == 0 for the
                               rest. O(n), exact.
 CUT A CONVEX POLYHEDRON BY A PLANE - clip each face, collect every new point on the cutting
                               plane, and close the hole with hullCoplanar of those points.
 SORT COPLANAR POINTS AROUND THEIR CENTROID - map with the affine frame and use angCmp (01 -
                               Core.cpp) - exact, and gives the ccw face order a mesh wants.
 POLYGON x POLYGON IN SPACE    only meaningful if they are coplanar (planePlane first); then map
                               both with ONE frame and run the 2D clip.
 A 3D ROTATION MATRIX from the frame: the rows dx, dy, dz of an orthonormal frame ARE the matrix
                               that takes the plane to z = 0, and its transpose takes it back. */
