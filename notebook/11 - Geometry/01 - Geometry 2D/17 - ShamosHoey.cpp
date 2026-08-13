// Needs: Core.cpp, Lines.cpp (segCross).
// SHAMOS-HOEY: do ANY TWO of n segments intersect? O(n log n), and it returns the witness pair.
// The sweep keeps the active segments in a set ordered by their y at the sweep line. Two segments
// can only be the FIRST crossing pair if they are adjacent in that order immediately before they
// cross, so it is enough to test a segment against its two neighbours when it is inserted, and to
// test the two segments that become neighbours when one is removed.
// WHEN: "is this polygon simple", "do these n roads/walls cross", "is this a planar graph
// drawing" - anything where you only need YES/NO or one example, with n up to 1e5 so the O(n^2)
// double loop (05 - Sweep.cpp, countSegInter) is out. isSimple (11 - PolygonOps.cpp) is the
// polygon-shaped special case and is O(n^2); replace it with this when n is large.
// COMPLEXITY: O(n log n) time, O(n) memory.
// DEGENERATE: touching at a shared endpoint COUNTS as an intersection (segCross is inclusive), so
// for "is this polygon simple" you must skip the adjacent pairs yourself - easiest is to shrink
// every polygon edge by a hair, or to special-case the pair (i, i+1 mod n) in the answer.
// Overlapping collinear segments are reported. Zero-length segments are reported against anything
// they touch. Vertical segments are fine here (unlike point location).
// The ANSWER (which pair) is exact - segCross is pure integer orientation. Only the SET ORDER
// uses floating point, and only to decide adjacency, so an eps slip can at worst make the sweep
// miss a crossing whose two segments are within 1e-9 of each other in y; with |coord| <= 1e9 and
// ld that does not happen for integer input.
struct SSeg {
    Pi a, b; int id;                                    // normalised so that a.x <= b.x
    ld yAt(ld x) const {
        return a.x == b.x ? (ld)a.y : a.y + (ld)(b.y - a.y) * (x - a.x) / (b.x - a.x);
    }
    bool operator<(const SSeg& o) const {               // compare where both are already alive
        ld x = max((ld)a.x, (ld)o.a.x);
        return yAt(x) < o.yAt(x) - eps;
    }
};
pair<int, int> anyIntersect(const vector<pair<Pi, Pi>>& in) {
    int n = sz(in);
    vector<SSeg> s(n);
    vector<array<ll, 3>> ev;                            // {x, +1 open / -1 close, id}
    for (int i = 0; i < n; i++) {
        s[i] = {in[i].fi, in[i].se, i};
        if (s[i].b < s[i].a) swap(s[i].a, s[i].b);
        ev.pb({s[i].a.x, 1, i}), ev.pb({s[i].b.x, -1, i});
    }
    sort(all(ev), [](const array<ll, 3>& p, const array<ll, 3>& q) {
        return p[0] != q[0] ? p[0] < q[0] : p[1] > q[1];               // open before close
    });
    set<SSeg> t;
    vector<set<SSeg>::iterator> pos(n);
    typedef set<SSeg>::iterator sit;
    auto hit = [&](sit x, sit y) {
        return x != t.end() && y != t.end() && segCross(x->a, x->b, y->a, y->b);
    };
    for (auto& e : ev) {
        int i = (int)e[2];
        if (e[1] > 0) {
            sit nx = t.lower_bound(s[i]), pv = nx == t.begin() ? t.end() : prev(nx);
            sit me = t.insert(nx, s[i]);                // insert first, then test both neighbours
            if (hit(nx, me)) return {nx->id, i};
            if (hit(pv, me)) return {pv->id, i};
            pos[i] = me;
        } else {
            sit nx = next(pos[i]), pv = pos[i] == t.begin() ? t.end() : prev(pos[i]);
            if (hit(nx, pv)) return {pv->id, nx->id};   // they become adjacent once i leaves
            t.erase(pos[i]);
        }
    }
    return {-1, -1};
}
// COUNT or REPORT ALL k intersections: BENTLEY-OTTMANN, O((n + k) log n). Same sweep, but the
// event queue is a priority queue that also receives the intersection points themselves; when two
// segments swap order at a crossing you re-test each against its new neighbour. It is long,
// fiddly with degeneracies (three segments through one point), and almost never what a problem
// wants - if you need a COUNT of crossing pairs of ARBITRARY segments and n is 1e5, look for
// extra structure (all horizontal/vertical -> BIT sweep; all chords of a circle -> inversions).
// IS A POLYGON SIMPLE, in O(n log n): shrink every edge towards its midpoint by a factor
// (1 - 1e-9) so consecutive edges no longer share their endpoint, then this returns {-1, -1}
// exactly for a simple polygon. Shrinking is the honest fix; filtering "adjacent indices" out of
// the answer is not, because the sweep stops at the FIRST adjacent pair it meets.
