// Needs: Core.cpp.
// TOTAL LENGTH OF A LINE INSIDE A POLYGON, including NON-CONVEX and self-intersecting ones, in
// O(n log n). The line a -> b generally enters and leaves the region many times; this sums all of
// the pieces. Works by collecting the parameters t at which the line crosses each edge, tagging
// each crossing with the direction the boundary sweeps past it, sorting, and integrating the
// stretches where the running WINDING NUMBER is non-zero. cut() (03 - Polygon.cpp) only handles
// the convex case and only gives you one side; this gives you the measure.
// WHEN: "how much of this laser / road / sight line is inside the region", area of a polygon
// swept by a moving line, and any integral over a region done by integrating over parallel lines
// (fix a direction, sweep the offset, and this is the inner integrand).
// COMPLEXITY: O(n log n) for the sort, O(n) memory.
// DEGENERATE: a vertex lying exactly ON the line is handled by treating a zero orientation as
// POSITIVE, which is the same as nudging the line infinitesimally toward its right; that is
// consistent, so a vertex merely touching the line contributes nothing and a vertex the boundary
// genuinely passes through contributes once. An EDGE lying along the line is skipped for the same
// reason - if you need boundary-along-the-line to count, add its length yourself.
// SELF-INTERSECTING input is fine and uses the non-zero winding rule (windingNumber, 11), so a
// doubly-wound region counts once, not twice. Orientation (CW or CCW) does not matter.
// The crossing TEST is exact on P<ll> (|coord| <= 1e9); only the parameter t is floating point,
// so nearly-tangent edges can shift a boundary by ~1e-9 relative.
template <class P> ld lineInPoly(const vector<P>& v, P a, P b) {
    P d = b - a;
    int n = sz(v);
    vector<pair<ld, int>> ev;
    for (int i = 0; i < n; i++) {
        P p = v[i], q = v[(i + 1) % n];
        int s1 = sgn(d.cross(p - a)), s2 = sgn(d.cross(q - a));
        if (!s1) s1 = 1;                                // a point ON the line counts as LEFT
        if (!s2) s2 = 1;
        if (s1 == s2) continue;
        ev.pb({(ld)(p - a).cross(q - p) / d.cross(q - p), s1 > s2 ? 1 : -1});
    }
    sort(all(ev));
    ld len = 0;
    int c = 0;
    for (int i = 0; i + 1 < sz(ev); i++)
        if ((c += ev[i].second)) len += ev[i + 1].fi - ev[i].fi;        // inside <=> winding != 0
    return len * d.dist();
}
// SEGMENT instead of a line: same events, but clamp the accumulated interval to [0, 1] before
// adding, i.e. len += max((ld)0, min((ld)1, ev[i+1].fi) - max((ld)0, ev[i].fi)).
// RAY: clamp to [0, infinity) the same way.
// THE PIECES THEMSELVES, not just their total length: emit the interval [ev[i].fi, ev[i+1].fi]
// whenever the running count is non-zero; a + d * t gives the endpoints.
// AREA OF THE POLYGON, as a sanity check on this file: integrate lineInPoly over a family of
// parallel lines. That is also how you get "the area of the part of a polygon on one side of an
// arbitrary curve" when the curve is not a line and cut() does not apply.
// LINE x CONVEX POLYGON in O(log n): lineHull (11 - PolygonOps.cpp) gives the two crossed edges
// directly; there is exactly one interval and you do not need this file.
// LINE x CIRCLE is circleLine (04); LINE x DISC intersected with a polygon: clip against both.
