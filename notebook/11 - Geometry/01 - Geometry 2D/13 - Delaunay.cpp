// Needs: Core.cpp.
// DELAUNAY TRIANGULATION, O(n log n), divide and conquer over the quad-edge structure.
// Returns index triples into the input, each one COUNTER-CLOCKWISE. The defining property, and
// the only one you ever use: the circumcircle of every returned triangle is EMPTY of input points.
// WHEN: EUCLIDEAN MST of points in the plane - the EMST is a subgraph of the Delaunay graph, so
// Delaunay (O(n) edges) + Kruskal replaces the O(n^2) complete graph. Also the nearest-neighbour
// graph, the closest pair, the largest empty circle, "maximise the minimum angle" meshing, and
// the VORONOI DIAGRAM, which is exactly the dual (14 - Voronoi.cpp).
// COMPLEXITY: O(n log n) time. At most 2n - 2 - h triangles and 3n - 3 - h edges, h = hull size.
// Allocates quad-edges with new and never frees them; budget ~200 bytes per point.
// DEGENERATE: DUPLICATE POINTS ARE NOT ALLOWED - dedupe before calling. Fewer than three points,
// or ALL POINTS COLLINEAR, returns {} - there is genuinely no triangle, handle it upstream.
// Four or more CONCYCLIC points make the triangulation non-unique; you get one legal answer.
// EXACT on P<ll> for |coord| <= 1e9: orient fits in ll (<= 8e18) and inCircle is __int128.
typedef struct QuadEdge* QE;
Pi qArb(LLONG_MAX, LLONG_MAX);                      // a sentinel unequal to any real point
struct QuadEdge {
    QE rot, o; Pi p = qArb; bool mark = 0;
    Pi& F() { return r()->p; }                      // the far endpoint of this directed edge
    QE& r() { return rot->rot; }                     // the same edge reversed
    QE prev() { return rot->o->rot; }
    QE next() { return r()->prev(); }
};
bool inCirc(Pi p, Pi a, Pi b, Pi c) {               // is p strictly inside circumcircle(a, b, c)?
    __int128 p2 = p.dist2(), A = a.dist2() - p2, B = b.dist2() - p2, C = c.dist2() - p2;
    return p.cross(a, b) * C + p.cross(b, c) * A + p.cross(c, a) * B > 0;
}
QE qeEdge(Pi orig, Pi dest) {
    QE q[] = {new QuadEdge{0, 0, orig}, new QuadEdge{0, 0, qArb}, new QuadEdge{0, 0, dest},
             new QuadEdge{0, 0, qArb}};
    for (int i = 0; i < 4; i++) q[i]->o = q[-i & 3], q[i]->rot = q[(i + 1) & 3];
    return *q;
}
void qeSplice(QE a, QE b) { swap(a->o->rot->o, b->o->rot->o), swap(a->o, b->o); }
QE qeConnect(QE a, QE b) {
    QE q = qeEdge(a->F(), b->p);
    qeSplice(q, a->next()), qeSplice(q->r(), b);
    return q;
}
QE dNext(QE e, bool lf) { return lf ? e->o : e->prev(); }        // walk left / right of the base
bool dValid(QE e, QE base) { return e->F().cross(base->F(), base->p) > 0; }
QE dDel(QE init, QE base, bool lf) {                   // drop edges that the new base invalidates
    QE e = dNext(init, lf);
    if (dValid(e, base))
        while (inCirc(dNext(e, lf)->F(), base->F(), base->p, e->F())) {
            QE t = dNext(e, lf);
            qeSplice(e, e->prev()), qeSplice(e->r(), e->r()->prev());
            e = t;
        }
    return e;
}
pair<QE, QE> dRec(const vector<Pi>& s) {        // {ccw hull edge at left end, cw at right}
    if (s.size() <= 3) {
        QE a = qeEdge(s[0], s[1]), b = qeEdge(s[1], s.back());
        if (s.size() == 2) return {a, a->r()};
        qeSplice(a->r(), b);
        auto side = s[0].cross(s[1], s[2]);
        QE c = side ? qeConnect(b, a) : 0;             // side == 0: three collinear points, no face
        return {side < 0 ? c->r() : a, side < 0 ? c : b->r()};
    }
    QE A, B, ra, rb;
    int half = s.size() / 2;
    tie(ra, A) = dRec({s.begin(), s.end() - half});
    tie(B, rb) = dRec({s.begin() + (s.size() - half), s.end()});
    while ((B->p.cross(A->F(), A->p) < 0 && (A = A->next())) ||     // find the lower common tangent
           (A->p.cross(B->F(), B->p) > 0 && (B = B->r()->o)));
    QE base = qeConnect(B->r(), A);
    if (A->p == ra->p) ra = base->r();
    if (B->p == rb->p) rb = base;
    for (;;) {                                      // zip the two hulls together, bottom to top
        QE lc = dDel(base->r(), base, 1), rc = dDel(base, base, 0);
        if (!dValid(lc, base) && !dValid(rc, base)) break;
        if (!dValid(lc, base) || (dValid(rc, base) && inCirc(rc->F(), rc->p, lc->F(), lc->p)))
            base = qeConnect(rc, base->r());
        else base = qeConnect(base->r(), lc->r());
    }
    return {ra, rb};
}
vector<array<int, 3>> delaunay(const vector<Pi>& pts) {
    vector<Pi> p = pts;
    sort(all(p));
    vector<array<int, 3>> res;
    if (p.size() < 3) return res;
    bool line = true;                               // guard collinear input: dRec spins on it
    for (int i = 2; i < sz(p) && line; i++) line = p[0].cross(p[1], p[i]) == 0;
    if (line) return res;
    QE e = dRec(p).first;
    vector<QE> q = {e};
    while (e->o->F().cross(e->F(), e->p) < 0) e = e->o;
    vector<Pi> out;
    auto walk = [&](QE e) {                          // emit one face, queue its neighbours
        QE c = e;
        do { c->mark = 1, out.pb(c->p), q.pb(c->r()), c = c->next(); } while (c != e);
    };
    walk(e), out.clear();                           // the first face walked is the OUTER one
    for (int i = 0; i < sz(q); i++) if (!(e = q[i])->mark) walk(e);
    map<pair<ll, ll>, int> id;
    for (int i = 0; i < sz(pts); i++) id[{pts[i].x, pts[i].y}] = i;
    for (int i = 0; i + 2 < sz(out); i += 3)
        res.pb({id[{out[i].x, out[i].y}], id[{out[i + 1].x, out[i + 1].y}],
                id[{out[i + 2].x, out[i + 2].y}]});
    return res;
}
// EUCLIDEAN MST: collect the 3 edges of every triangle, dedupe, sort by squared length, Kruskal.
//   vector<array<ll,3>> E; for (auto& t : delaunay(p)) for (int k = 0; k < 3; k++)
//       E.pb({(p[t[k]] - p[t[(k+1)%3]]).dist2(), t[k], t[(k+1)%3]});
// LARGEST EMPTY CIRCLE whose centre is inside the hull: its centre is a Delaunay circumcentre or
// a point on the hull boundary - check all O(n) circumcentres, then the hull edges.
// The DELAUNAY GRAPH also contains the nearest-neighbour graph and the Gabriel graph, so any
// "for each point, its closest other point" question is answered by scanning its Delaunay edges.
// FLIP ALGORITHM (any triangulation -> Delaunay by flipping illegal edges) is O(n^2) and simple,
// but you already have this; the paraboloid lift (x, y, x^2+y^2) + LOWER 3D hull is the other
// derivation and is the one to remember when you need the connection to convexity.
