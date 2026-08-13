// Needs: Core.cpp, Polygon.cpp (area2).
// EAR CLIPPING - triangulates any SIMPLE polygon (convex or not, no self-intersections, no holes)
// into n-2 triangles, O(n^2). Exact with P<ll>. Returns index triples into the ORIGINAL vector.
// An "ear" is a convex vertex whose triangle contains no other vertex of the polygon.
// USES: area/integral of anything over a polygon, rendering, splitting a region for a sweep,
// point-location preprocessing, computing a polygon's centre of mass with a weight function.
template <class P>
vector<array<int, 3>> triangulate(const vector<P>& poly) {
    int n = poly.size();
    vector<array<int, 3>> tri;
    if (n < 3) return tri;
    vector<int> id(n);
    iota(all(id), 0);
    if (area2(poly) < 0) reverse(all(id));                     // work counter-clockwise
    auto inTri = [&](int a, int b, int c, int q) {             // closed triangle
        return poly[a].cross(poly[b], poly[q]) >= 0 && poly[b].cross(poly[c], poly[q]) >= 0 &&
               poly[c].cross(poly[a], poly[q]) >= 0;
    };
    while (id.size() > 2) {
        int m = id.size(), cut = -1, flat = -1;
        for (int i = 0; i < m && cut < 0; i++) {
            int a = id[(i + m - 1) % m], b = id[i], c = id[(i + 1) % m];
            auto o = poly[a].cross(poly[b], poly[c]);
            if (o == 0) { flat = i; continue; }                 // collinear vertex: just drop it
            if (o < 0) continue;                                // reflex
            bool ok = true;
            for (int j = 0; j < m && ok; j++)
                if (id[j] != a && id[j] != b && id[j] != c && inTri(a, b, c, id[j])) ok = false;
            if (ok) cut = i, tri.push_back({a, b, c});
        }
        if (cut < 0) cut = flat;
        if (cut < 0) break;                                     // not a simple polygon
        id.erase(id.begin() + cut);
    }
    return tri;
}
// FASTER: monotone decomposition (sweep, O(n log n)) then triangulate each monotone piece in O(n).
// Only worth writing for n beyond ~2000; ear clipping handles everything a contest throws at you.
// DELAUNAY TRIANGULATION maximises the minimum angle. Cheap way to get one: lift each point to the
// paraboloid (x, y, x^2+y^2) and take the LOWER hull of the 3D convex hull - its faces project to
// the Delaunay triangles. The dual graph is the VORONOI DIAGRAM.
//   Delaunay contains: the Euclidean minimum spanning tree, the nearest-neighbour graph, and the
//   closest pair. So "EMST of points in the plane" = Delaunay (O(n log n) edges) + Kruskal.
//   In-circle test (a,b,c counter-clockwise): d is strictly inside the circumcircle of a,b,c iff
//   the 3x3 determinant of rows (a-d, |a-d|^2), (b-d, |b-d|^2), (c-d, |c-d|^2) is > 0.  [__int128]
// ART GALLERY THEOREM: floor(n/3) guards always suffice for a simple n-gon, and are sometimes
// necessary (comb polygon). Proof = triangulate, 3-colour the triangulation graph, take the
// smallest colour class. The triangulation above is exactly what that construction needs.
// POLYGON WITH HOLES: connect each hole to the outer boundary with a "bridge" edge (pick the hole's
// rightmost vertex, shoot a ray right, split the hit edge) to get one simple polygon, then clip.
