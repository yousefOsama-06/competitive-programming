// Needs: Geometry3D.cpp.
// 3D CONVEX HULL, incremental. Input: n points (duplicates allowed, any order). Output: the hull
// surface as TRIANGULAR faces, each an index triple {i, j, k} into the input vector, ordered so
// that (p[j]-p[i]) x (p[k]-p[i]) points OUTWARD - equivalently vol6(p[i], p[j], p[k], x) <= 0 for
// every input point x. Coplanar facets come back cut into triangles by an arbitrary
// triangulation, and points strictly inside a facet are simply absent from the output.
// Reach for it for "volume/surface of the hull", "is this point inside the hull", "diameter of a
// 3D point set", "smallest enclosing box", or any time a 2D hull idea has to survive one more
// coordinate. O(n^2) faces touched (O(n^2 log n) as written, the log being the sort of the
// horizon edges - swap in an n x n matrix to drop it; it has never mattered).
// Memory O(n) for the face list, O(n) for the horizon.
// DEGENERATE, and this is the part people get wrong: n < 4, ALL POINTS EQUAL, ALL POINTS
// COLLINEAR and ALL POINTS COPLANAR each return an EMPTY face list - a flat hull has no volume
// and no consistently outward orientation, so it cannot be expressed in this format. Detect it
// (the return is empty) and rerun the problem in 2D through 03 - CoplanarFrame.cpp, which is
// where a coplanar input belongs. Duplicate points are harmless: a repeat never strictly sees a
// face, so it is skipped and only one copy can appear in the output.
// EXACTNESS: no divisions anywhere - only vol6 signs - so on P3<ll> the hull is EXACT with
// |coord| <= 3.6e5 (the orientation test sums four vol6, 192*C^3 <= 9.2e18; make vol6 return
// __int128 to reach 1e9). The three seeding tests convert to ld to compare against eps, which is
// what makes the same code safe on P3<ld>; there eps is absolute, so scale your input to O(1).
template <class P> vector<array<int, 3>> hull3d(const vector<P>& p) {
    int n = sz(p), k;
    if (n < 4) return {};                                    // no solid hull exists
    vector<array<int, 3>> F;
    vector<int> id(n);
    iota(all(id), 0);
    auto nz = [](P v) { return max({fabsl((ld)v.x), fabsl((ld)v.y), fabsl((ld)v.z)}) > eps; };
    for (k = 1; k < n && !nz(p[id[k]] - p[id[0]]); k++);      // a second DISTINCT point
    if (k == n) return {};
    swap(id[1], id[k]);
    for (k = 2; k < n && !nz((p[id[1]] - p[id[0]]).cross(p[id[k]] - p[id[0]])); k++);
    if (k == n) return {};                                   // every point on one line
    swap(id[2], id[k]);
    for (k = 3; k < n && fabsl((ld)vol6(p[id[0]], p[id[1]], p[id[2]], p[id[k]])) < eps; k++);
    if (k == n) return {};                                   // every point on one plane
    swap(id[3], id[k]);
    // Orient against the centroid of the seed tetrahedron, which is strictly inside it and so
    // stays strictly inside every later hull. Summing the four vol6 IS testing that centroid,
    // scaled by 4, so the test never ties and never divides.
    auto face = [&](int a, int b, int c) {
        auto s = vol6(p[a], p[b], p[c], p[id[0]]) + vol6(p[a], p[b], p[c], p[id[1]])
               + vol6(p[a], p[b], p[c], p[id[2]]) + vol6(p[a], p[b], p[c], p[id[3]]);
        F.push_back(s > 0 ? array<int, 3>{a, c, b} : array<int, 3>{a, b, c});
    };
    for (int i = 0; i < 4; i++) face(id[(i + 1) % 4], id[(i + 2) % 4], id[(i + 3) % 4]);
    for (int t = 4; t < n; t++) {
        int u = id[t];
        vector<array<int, 3>> keep;
        vector<pii> E;                                       // directed edges of the dying faces
        for (auto& f : F)
            if (vol6(p[f[0]], p[f[1]], p[f[2]], p[u]) > eps) {          // u strictly SEES f
                for (int j = 0; j < 3; j++) E.push_back({f[j], f[(j + 1) % 3]});
            } else keep.push_back(f);
        if (E.empty()) continue;                             // u is inside the hull: nothing to do
        F = keep;
        sort(all(E));
        // The HORIZON is the directed edges whose reverse did not also die: those are the ones
        // whose other face survives. Cone them to u.
        for (auto& [a, b] : E) if (!binary_search(all(E), pii(b, a))) face(a, b, u);
    }
    return F;
}
/* RELATED
 VOLUME / AREA / CENTROID of the result: build the face list and call 06 - Polyhedron.cpp -
     vector<vector<P>> fs; for (auto& f : hull3d(p)) fs.push_back({p[f[0]], p[f[1]], p[f[2]]});
     the faces are already consistently outward, so polyVol6 comes out POSITIVE.
 IS A POINT INSIDE THE HULL   vol6(p[f[0]], p[f[1]], p[f[2]], q) <= 0 for every face, O(F).
 EXTREME POINT in a direction d: max dot over the hull VERTICES (the indices that appear in F).
 DIAMETER of a 3D point set: no rotating calipers in 3D worth typing - take the hull vertices
     and brute force the pairs, it is O(h^2) with a tiny h.
 O(n log n) HULL exists (divide and conquer, or randomised incremental with a conflict graph).
     It is 150 lines of merge bookkeeping for a constant factor you do not need: n = 2000 here is
     4e6 orientation tests, a few milliseconds.
 DELAUNAY TRIANGULATION of 2D points: lift each (x, y) to (x, y, x^2 + y^2) on the paraboloid,
     take the 3D hull, and keep the faces whose outward normal has NEGATIVE z (the LOWER hull) -
     those triangles, projected back, are exactly the Delaunay triangulation. The upper hull is
     the farthest-point Delaunay. Lifting squares the coordinates, so |coord| <= 1e4 to stay in
     ll here; the Voronoi diagram is the dual of what comes out.
 HALFSPACE INTERSECTION in 3D is the dual problem: map each plane n.x = d (d > 0, so that the
     origin is interior) to the point n/d, take the hull of those, and dualise the faces back. */
