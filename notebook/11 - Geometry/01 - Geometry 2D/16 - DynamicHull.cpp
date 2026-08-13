// Needs: Core.cpp.
// DYNAMIC CONVEX HULL - insert points ONLINE, ask "is this point inside the hull so far" in
// O(log n), amortised O(log n) per insert. The structure below is the UPPER hull only, stored as
// a map x -> max y; run a second copy fed the negated points for the lower hull and you have the
// full hull. Storing one y per x is what makes this short: the upper chain has strictly
// increasing x by definition, and if two points share an x only the higher one can be on it.
// WHEN: points arrive one at a time and you must answer containment / support / area queries
// between insertions; "add point, print hull area"; maximising a linear function over a growing
// set (that special case is CHT / Li Chao, 08 - DP Optimizations, and is simpler - use it if the
// query is always a dot product). Deletions are NOT supported; for those, use offline divide and
// conquer over time, or Kinetic / segment-tree-on-time rebuilding.
// COMPLEXITY: O(log n) amortised insert (each point is erased at most once), O(log n) query,
// O(n) memory.
// DEGENERATE: COLLINEAR points are dropped (the chain keeps only strict turns), so a set of
// collinear points collapses to its two extremes and under() still answers correctly. Inserting a
// point already inside is a no-op. An empty structure answers false to everything.
// EXACT on integers: the only arithmetic is a cross product, so |coord| <= 1e9 keeps it in ll.
struct UpHull {                                         // the upper chain, left to right
    map<ll, ll> h;
    typedef map<ll, ll>::iterator it_t;
    Pi at(it_t i) { return Pi(i->fi, i->se); }
    bool under(ll x, ll y) {                            // is (x, y) on or below the chain?
        auto it = h.lower_bound(x);
        if (it == h.end() || (it == h.begin() && it->fi != x)) return false;   // outside the span
        if (it->fi == x) return y <= it->se;
        return at(prev(it)).cross(at(it), Pi(x, y)) <= 0;                      // not strictly left
    }
    bool bad(it_t it) {                                 // does *it sit on or below its neighbours?
        if (it == h.begin() || next(it) == h.end()) return false;
        return at(prev(it)).cross(at(next(it)), at(it)) <= 0;
    }
    void add(ll x, ll y) {
        if (under(x, y)) return;
        h[x] = y;
        auto it = h.find(x);
        while (next(it) != h.end() && bad(next(it))) h.erase(next(it));
        while (it != h.begin() && bad(prev(it))) h.erase(prev(it));
    }
};
// FULL HULL: UpHull up, dn;  insert p as up.add(p.x, p.y) and dn.add(-p.x, -p.y);
//   inside(p) = up.under(p.x, p.y) && dn.under(-p.x, -p.y).
// EXTREME POINT in direction d, O(log n): binary search the upper chain for the first edge whose
// direction turns from d-positive to d-negative, i.e. the last vertex with
// dot(next - cur, d) > 0. With a map you have to walk, so keep the chain in a
// vector-backed balanced BST (or an ordered set with order_of_key) if you need this a lot.
// AREA UNDER INSERTIONS, the usual companion query: keep ll s2 = sum of cross(p_i, p_{i+1}) over
// consecutive chain vertices. Every erase of a middle vertex b between a and c does
//   s2 -= cross(a,b) + cross(b,c) - cross(a,c);
// and every insert of b between a and c does the same with a plus sign; erases and inserts at an
// end touch only the single adjacent term. The hull's area is |s2_up + s2_down| / 2 with the
// lower chain's points negated, since negating both coordinates leaves every cross product alone.
// OFFLINE ALTERNATIVE, almost always easier: if you know all the points up front and the queries
// are interleaved, sort by time and rebuild the hull in O(n log n) once per sqrt-block, or run
// divide and conquer on the time axis. Only type this file when the input is genuinely online.
