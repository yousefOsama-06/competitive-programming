// Needs: Core.cpp.
// PLANAR POINT LOCATION, offline, by a left-to-right sweep. Input: a set of segments that are
// pairwise NON-CROSSING (they may share endpoints) - i.e. the edges of a planar subdivision - and
// a list of query points. Output, for each query, the index of the segment DIRECTLY BELOW it, or
// -1 if the query is below everything. That single answer is the whole primitive: label every
// segment with the id of the face lying immediately ABOVE it and the query's face falls out.
// WHEN: "n disjoint polygons, q points, which polygon contains each" with n and q both 1e5, so
// the O(nq) scan is dead. Also "which cell of an arrangement", and nearest-site queries once you
// have built the Voronoi diagram (14). For ONE convex polygon use inConvex (03) instead; for a
// handful of polygons just call inPoly (03) on each.
// COMPLEXITY: O((n + q) log n) time, O(n + q) memory. Everything is offline - all queries must be
// known up front. (Online needs a persistent segment tree over the vertical slabs, which is
// O(n log n) memory and rarely worth typing.)
// DEGENERATE: NO VERTICAL SEGMENTS - the comparator below assumes every segment points strictly
// rightwards. If you have them, shear the plane with (x, y) -> (x * (2C + 1) + y, y), which is
// exact on integers and makes all x distinct, but squares your coordinates (use __int128 cross).
// A query exactly ON a segment reports that segment (endpoints included), because segments
// starting at x are inserted before the queries at x and segments ending at x are removed after.
// Zero-length segments must be filtered out first.
// EXACT on P<ll> with |coord| <= 1e9; the comparator is pure orientation signs, no division.
vector<int> locateBelow(vector<pair<Pi, Pi>> s, const vector<Pi>& qs) {
    int n = sz(s), m = sz(qs);
    for (auto& e : s) if (e.second < e.first) swap(e.first, e.second);
    s.pb({Pi(), Pi()});                                 // slot n: the current query, as a point
    // Two non-crossing segments are comparable wherever their x-ranges overlap: ask which side of
    // a's line BOTH endpoints of b fall on, and if that is inconclusive (shared endpoint) ask the
    // mirror question. Only ever consulted while both are in the sweep set, so this is a valid
    // strict weak ordering at every instant even though it changes as the sweep advances.
    auto below = [&](int i, int j) {
        auto& [a, b] = s[i]; auto& [c, d] = s[j];
        int v = sgn(a.cross(b, c)) + sgn(a.cross(b, d));
        if (v) return v > 0;
        return sgn(c.cross(d, a)) + sgn(c.cross(d, b)) < 0;
    };
    set<int, decltype(below)> t(below);
    vector<int> add(n), del(n), qi(m), res(m, -1);
    iota(all(add), 0), iota(all(del), 0), iota(all(qi), 0);
    sort(all(add), [&](int i, int j) { return s[i].first.x < s[j].first.x; });
    sort(all(del), [&](int i, int j) { return s[i].second.x < s[j].second.x; });
    sort(all(qi), [&](int i, int j) { return qs[i].x < qs[j].x; });
    int ia = 0, id = 0, iq = 0;
    while (ia < n || id < n || iq < m) {                // sweep EVERY event x, not just query x:
        ll x = LLONG_MAX;                               // the set order is only valid while all
        if (ia < n) x = min(x, s[add[ia]].first.x);     // members still span the sweep line
        if (id < n) x = min(x, s[del[id]].second.x);
        if (iq < m) x = min(x, qs[qi[iq]].x);
        while (ia < n && s[add[ia]].first.x == x) t.insert(add[ia++]);       // open,
        while (iq < m && qs[qi[iq]].x == x) {                                // then answer,
            int k = qi[iq++];
            s[n] = {qs[k], qs[k]};                      // a zero-length segment = the query point
            auto it = t.lower_bound(n);
            if (it != t.begin()) res[k] = *prev(it);
        }
        while (id < n && s[del[id]].second.x == x) t.erase(del[id++]);       // then close
    }
    return res;
}
// ATTACHING FACES. Build the subdivision as half-edges: every segment carries the face id above
// it and the face id below it (the unbounded face is -1). Then:
//   f = locateBelow(...)[k];  answer = f < 0 ? -1 : faceAbove[f];
// and the query is ON the boundary iff qs[k].cross(s[f].first, s[f].second) == 0.
// For DISJOINT SIMPLE POLYGONS the labelling is free: walk each polygon in CCW order and give
// every edge whose direction points LEFT (a.x > b.x) the polygon's id as its "above" face, and
// every rightward edge the id of whatever the polygon sits in (-1 for a top-level polygon).
// ARRANGEMENT OF LINES: n lines cut the plane into 1 + n + C(n,2) faces; build the arrangement by
// sorting the intersections along each line, then run this on the resulting O(n^2) segments.
// A different, shorter answer to "point in one of many disjoint polygons": if the polygons are
// convex and you only need containment, sort by x-range and binary search, or build the union
// hull. Reach for point location only when the subdivision is genuinely a subdivision.
