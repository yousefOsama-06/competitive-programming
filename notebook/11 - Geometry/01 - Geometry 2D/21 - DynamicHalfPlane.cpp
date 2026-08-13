// Needs: Core.cpp (angCmp).
// DYNAMIC HALF-PLANE INTERSECTION - a convex region you can CUT online, in O(log n) amortised per
// cut, while asking for its area or its extreme point in any direction between cuts. The static
// routine (06 - HalfPlaneIntersection.cpp) needs every constraint up front and re-sorts; this one
// does not, so it is what you want when the constraints arrive one at a time, or when you must
// report an answer after each one.
// REPRESENTATION: the region's boundary, as a multimap from EDGE DIRECTION to the vertex where
// that edge begins, in counter-clockwise angular order. That single choice is the whole trick -
// the edge a new cut attacks is found by one lower_bound on its direction, and every vertex is
// erased at most once, so the amortised cost is a lookup plus the erases you paid to insert.
// It starts as a huge box, so the region is always bounded; make B larger than any coordinate you
// care about and remember that a surviving box corner is an artefact, not an answer.
// WHEN: "after each of q constraints, print the area of the feasible region"; online 2D LP;
// incremental visibility / lit-region problems; kernel of a polygon built edge by edge.
// COMPLEXITY: O(log n) amortised per cut, O(log n) per extreme-point query, O(n) per area query
// (keep a running sum of cross products if you need area after every cut - update it in the same
// places the code erases and inserts).
// DEGENERATE: cutting with a line that misses the region entirely is a no-op; cutting everything
// away leaves the map EMPTY and every later cut returns immediately, so check empty() for
// infeasible. Duplicate directions are allowed (that is why it is a multimap). A cut exactly
// along an existing edge removes the now-redundant vertex.
// FLOATING POINT: directions and vertices are ld and compared with the usual eps through sgn.
// Feed it integer-valued Pd if you can; a cut that is within eps of tangent may keep or drop a
// zero-length edge, so filter edges shorter than eps before counting them.
struct DirLess { bool operator()(Pd a, Pd b) const { return angCmp<ld>(a, b); } };
struct HPSet {
    typedef multimap<Pd, Pd, DirLess> M;
    typedef M::iterator it_t;
    M m;                                                // edge direction -> start vertex of edge
    HPSet(ld B = 1e9) {
        m.insert({Pd(1, 0), Pd(-B, -B)}), m.insert({Pd(0, 1), Pd(B, -B)});
        m.insert({Pd(-1, 0), Pd(B, B)}), m.insert({Pd(0, -1), Pd(-B, B)});
    }
    it_t nx(it_t i) { return next(i) == m.end() ? m.begin() : next(i); }
    it_t pv(it_t i) { return i == m.begin() ? prev(m.end()) : prev(i); }
    it_t fix(it_t i) { return i == m.end() ? m.begin() : i; }
    void cut(Pd a, Pd b) {                              // keep only the part LEFT of a -> b
        if (m.empty()) return;
        int old = sz(m);
        Pd d = b - a;
        auto ev = [&](it_t i) { return sgn(a.cross(b, i->second)); };
        auto ix = [&](it_t i) {                         // where i's edge meets the line a -> b
            return i->second + i->first * (d.cross(a - i->second) / d.cross(i->first));
        };
        it_t it = fix(m.lower_bound(d));                // the vertex farthest RIGHT of d
        if (ev(it) >= 0) return;                        // it survives, so everything does
        while (sz(m) && ev(pv(it)) < 0) m.erase(pv(it));
        while (sz(m) && ev(nx(it)) < 0) it = fix(m.erase(it));
        if (m.empty()) return;
        if (ev(nx(it)) > 0) it->second = ix(it);        // pull this vertex onto the cutting line
        else it = fix(m.erase(it));
        if (old <= 2) return;
        it = pv(it);
        m.insert(it, {d, ix(it)});                      // the new edge starts where we re-entered
        if (ev(it) == 0) m.erase(it);                   // the old vertex is now redundant
    }
    Pd extreme(Pd dir) {                                // vertex maximising dot(v, dir), O(log n)
        return fix(m.lower_bound(dir.perp()))->second;
    }
    ld area() {
        if (sz(m) <= 2) return 0;
        ld s = 0;
        for (it_t i = m.begin(); i != m.end(); ++i) s += i->second.cross(nx(i)->second);
        return fabsl(s) / 2;
    }
    vector<Pd> poly() {                                 // the region as a CCW vertex list
        vector<Pd> v;
        for (auto& e : m) v.pb(e.second);
        return v;
    }
};
// USAGE: HPSet h; then h.cut(p, q) for each constraint "left of the directed line p -> q", which
// is the same convention as HP in 06. To cut with the half-plane {x : n.dot(x) <= c}, take a
// point p on the boundary line and cut(p, p + n.perp()).
// AREA AFTER EVERY CUT in O(log n): keep ld s = sum of cross(v_i, v_{i+1}). Every erase of a
// vertex b between a and c does s -= cross(a,b) + cross(b,c) - cross(a,c), every insert adds it,
// and moving a vertex is an erase plus an insert. Same bookkeeping as 16 - DynamicHull.cpp.
// 2D LP ONLINE: after each constraint the optimum of a linear objective c is extreme(c) - one
// lower_bound, no scan. That is the reason to prefer this over rebuilding with 06.
// DELETIONS are not supported. If constraints are added AND removed, go offline: segment tree on
// time, and rebuild with the static routine in each node.
