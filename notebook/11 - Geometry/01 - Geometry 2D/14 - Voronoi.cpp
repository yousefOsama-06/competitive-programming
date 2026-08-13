// Needs: Core.cpp, HalfPlaneIntersection.cpp (Delaunay.cpp for the fast variant).
// VORONOI DIAGRAM. The cell of site i is the set of points closer to site i than to any other
// site; it is a convex polygon (unbounded for sites on the convex hull, so everything here is
// clipped to a box of half-width BOX - make BOX comfortably larger than any coordinate you care
// about, and remember that a vertex of the clipped cell may be an artefact of the box).
// The cell is just the intersection of the half-planes "closer to i than to j" over all j != i,
// and each of those is the side of the perpendicular bisector of s_i s_j containing s_i.
// WHEN: nearest-site queries with a planar point location structure on top; largest empty circle;
// "which post office serves this house"; any facility-location or growth/flood simulation where
// regions expand at equal speed. If you only need nearest neighbours, a KD-tree (09) is shorter.
// COMPLEXITY: O(n^2 log n) for all n cells as written (O(n log n) per cell). That is fine up to
// n around 2000. For O(n log n) total, build the DELAUNAY dual instead - see the tail.
// DEGENERATE: duplicate sites give two identical cells that are both empty ({} is returned);
// dedupe first. n == 1 gives the whole box. Collinear sites give slab cells, handled correctly.
// FLOATING POINT: the bisectors are exact only if you keep sites integral, but halfPlaneInter is
// an eps algorithm, so the cell vertices carry the usual 1e-9 relative error. Four or more
// cocircular sites make a Voronoi vertex of degree > 3 and the cell may pick up a zero-length
// edge; strip edges shorter than eps if you are counting them.
vector<Pd> voronoiCell(const vector<Pd>& s, int i, ld BOX = 1e9) {
    vector<HP> h;
    for (int j = 0; j < sz(s); j++) if (j != i) {
        Pd m = (s[i] + s[j]) / 2;                       // the perpendicular bisector, directed so
        h.pb(HP(m, m + (s[j] - s[i]).perp()));          // that the LEFT side contains s[i]
    }
    return halfPlaneInter(h, BOX);
}
vector<vector<Pd>> voronoi(const vector<Pd>& s, ld BOX = 1e9) {
    vector<vector<Pd>> c;
    for (int i = 0; i < sz(s); i++) c.pb(voronoiCell(s, i, BOX));
    return c;
}
// O(n log n) VIA THE DELAUNAY DUAL (13 - Delaunay.cpp). One Voronoi vertex per Delaunay triangle,
// namely its CIRCUMCENTRE; one Voronoi edge per Delaunay edge, joining the circumcentres of the
// two triangles that share it. Recipe: run delaunay(p); for every triangle t store
// circumcenter(p[t[0]], p[t[1]], p[t[2]]); for each site collect the circumcentres of the
// triangles incident to it and sort them by angle around the site - that is its cell, in CCW
// order. A site on the CONVEX HULL has an unbounded cell: its two hull edges contribute rays
// along their outward perpendicular bisectors, so clip them to the box by hand.
// NEAREST SITE for a query point = the cell containing it: build the diagram, then run planar
// point location (15). For a handful of queries just scan all sites.
// LARGEST EMPTY CIRCLE with centre inside the hull: its centre is a Voronoi vertex (= a Delaunay
// circumcentre) or a point where a Voronoi edge crosses the hull boundary. Check both families.
// DELAUNAY EDGE <-> VORONOI EDGE, and the MST / nearest-neighbour consequences, are all in 13.
// WEIGHTED (POWER / LAGUERRE) DIAGRAM: replace the bisector by the RADICAL AXIS of the two
// circles (04 - Circles.cpp) - the cells stay convex polygons, so this exact code works with the
// bisector swapped out. The MULTIPLICATIVELY weighted (Apollonius) diagram has circular arcs for
// boundaries and none of this applies.
