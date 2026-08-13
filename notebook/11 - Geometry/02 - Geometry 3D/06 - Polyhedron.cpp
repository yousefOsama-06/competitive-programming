// Needs: Geometry3D.cpp, Spherical.cpp (winding3 only).
// POLYHEDRA GIVEN AS A FACE LIST: fs[i] is one face, a vector of its vertices in order around it,
// coplanar, no repeat of the first vertex at the end. Faces may be any polygon, not just
// triangles. Everything here wants the list CLOSED (every edge shared by exactly two faces) and
// CONSISTENTLY oriented (all normals outward, or all inward); reorient produces the consistency
// from a face list that has it in neither direction, and the sign of polyVol6 tells you which of
// the two you ended up with. Computes: signed 6*volume, surface area, volume centroid, the
// orientation fixup, and the 3D winding number that answers "is the origin inside this thing".
// You reach for this file after a 3D hull, after cutting a solid by planes, or when the statement
// hands you a mesh (an .obj-shaped input) and asks for volume, mass centre, or containment.
// COMPLEXITY: volume, area, centroid O(total vertices). reorient O(E log E) for the edge map,
// O(E) memory. winding3 O(total vertices) with a few trig calls each.
// DEGENERATE: an empty face list gives volume 0, area 0 and centroid 0/0 - guard it. A face with
// fewer than 3 vertices contributes nothing (its fan is empty). A polyhedron that is NOT closed
// still returns a number from polyVol6, and that number is meaningless - there is no cheap check,
// so verify that every directed edge appears exactly once (which is what reorient's map does).
// reorient BFSes the face adjacency graph, so a DISCONNECTED surface (two separate shells) comes
// back with only the component of face 0 fixed; run it per component. Non-manifold edges (three
// faces meeting) break the map, which keeps only the first face per directed edge.
// EXACTNESS: polyVol6 and faceArea's vector sum are exact on P3<ll> (|coord| <= 5.7e5 for vol6);
// faceArea then takes one sqrt, and centroid divides, so those return ld / need P3<ld>. reorient
// compares vertices for EQUALITY, so on P3<ld> two faces only share an edge if the coordinates
// are bit-identical - with float input, or any input where the same vertex was computed twice,
// index your vertices and run reorient on vector<vector<int>> with the map keyed by pii instead.
// SIGNED 6*VOLUME, by the divergence theorem: fan every face from the ORIGIN and sum the
// tetrahedra. The parts outside the solid cancel exactly. Positive iff the faces are OUTWARD.
template <class P> auto polyVol6(const vector<vector<P>>& fs) {
    decltype(fs[0][0].dot(fs[0][0])) s = 0;
    for (auto& f : fs) for (int i = 1; i + 1 < sz(f); i++) s += vol6(P(), f[0], f[i], f[i + 1]);
    return s;
}
template <class P> ld polyVolume(const vector<vector<P>>& fs) {
    return fabsl((ld)polyVol6(fs)) / 6;
}
// AREA of one planar face, from its VECTOR AREA sum p_i x p_(i+1) (which is 2A times the unit
// normal, and is independent of where the origin sits because the face is closed and planar).
template <class P> ld faceArea(const vector<P>& f) {
    P s;
    for (int i = 0, m = sz(f); i < m; i++) s = s + f[i].cross(f[(i + 1) % m]);
    return s.dist() / 2;
}
template <class P> ld surfaceArea(const vector<vector<P>>& fs) {
    ld a = 0;
    for (auto& f : fs) a += faceArea(f);
    return a;
}
// CENTROID of the SOLID (not of the vertices, not of the surface): the volume-weighted average of
// the tetrahedron centroids of the same fan. Needs P3<ld>. Signs cancel exactly as in polyVol6.
template <class P> P polyCentroid(const vector<vector<P>>& fs) {
    P c; ld V = 0;
    for (auto& f : fs) for (int i = 1; i + 1 < sz(f); i++) {
        ld v = (ld)vol6(P(), f[0], f[i], f[i + 1]);          // 6 * signed volume of O-f0-fi-fi+1
        c = c + (f[0] + f[i] + f[i + 1]) * (v / 4);          // its centroid is (O+a+b+c)/4
        V += v;
    }
    return c / V;
}
// FACE-ORIENTATION FIXUP. Flip a subset of the faces so that all normals agree (either all out or
// all in - which one you get depends on face 0; check sgn(polyVol6) afterwards and reverse every
// face if you wanted the other). THE RULE: two faces sharing an edge agree exactly when they
// traverse that edge in OPPOSITE directions, so seeing the SAME directed edge twice means exactly
// one of the two faces must flip. BFS that parity over the face adjacency graph.
template <class P> vector<vector<P>> reorient(vector<vector<P>> fs) {
    int n = sz(fs);
    if (!n) return fs;
    vector<vector<pii>> g(n);                                // {neighbour, same direction?}
    map<pair<P, P>, int> es;
    for (int u = 0; u < n; u++)
        for (int i = 0, m = sz(fs[u]); i < m; i++) {
            P a = fs[u][i], b = fs[u][(i + 1) % m];
            if (es.count({b, a})) { int v = es[{b, a}]; g[u].pb({v, 0}); g[v].pb({u, 0}); }
            else if (es.count({a, b})) { int v = es[{a, b}]; g[u].pb({v, 1}); g[v].pb({u, 1}); }
            else es[{a, b}] = u;
        }
    vector<int> vis(n, 0), flip(n, 0), q = {0};
    vis[0] = 1;                                              // face 0 defines the common direction
    for (int i = 0; i < sz(q); i++)
        for (auto& [v, same] : g[q[i]])
            if (!vis[v]) vis[v] = 1, flip[v] = flip[q[i]] ^ same, q.push_back(v);
    for (int u = 0; u < n; u++) if (flip[u]) reverse(all(fs[u]));
    return fs;
}
// 3D WINDING NUMBER about the ORIGIN: how many times the surface wraps around it. 0 = the origin
// is OUTSIDE; +1 = inside with the faces oriented outward; -1 = inside with them oriented inward.
// Each face is projected onto the unit sphere, where it covers a signed solid angle; the total is
// 4*pi per wrap. remainder folds each face's contribution into (-2pi, 2pi] so that faces covering
// more than a hemisphere still add up right. To test a point other than the origin, subtract it
// from every vertex first. The origin exactly ON the surface makes a face's solid angle
// undefined - handle that case before calling (segTri / inCoplanar in 02 and 03).
template <class P> int winding3(const vector<vector<P>>& fs) {
    ld s = 0;
    for (auto& f : fs) s += remainderl(sphPolyArea(f, 1), 4 * PI);
    return (int)llroundl(s / (4 * PI));
}
/* RELATED
 IS A POINT INSIDE A CONVEX POLYHEDRON - do not call winding3: test sgn of the side of every face
     plane, O(F), exact on integers. Convex + outward faces => inside iff all sides <= 0.
 EULER    V - E + F = 2 for any closed surface of genus 0; a triangulated one has E = 3F/2, so
     F = 2V - 4 and E = 3V - 6. Use it as a free assertion on the output of a 3D hull.
 SURFACE OF A NON-PLANAR "face" - faceArea silently returns the area of its projection onto the
     best-fit plane. Triangulate first if the input can be skew.
 MOMENTS / INERTIA - the same fan works: sum the tetrahedron inertia tensors weighted by vol6.
 CUT BY A PLANE - clip every face (keep the vertices with side <= 0 plus the crossing points), then
     close the hole with hullCoplanar of the new points (03 - CoplanarFrame.cpp).
 MINKOWSKI SUM of two convex polyhedra: hull of the pairwise vertex sums, O((nm)^2) but n and m
     are tiny in practice.
 VOLUME OF A UNION of polyhedra has no cheap formula - sweep a plane and integrate the
     cross-section area, or inclusion-exclusion on convex pieces. */
