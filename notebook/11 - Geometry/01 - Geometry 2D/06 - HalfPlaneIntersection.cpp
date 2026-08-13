// HALF-PLANE INTERSECTION - O(n log n). A half-plane is "everything LEFT of the directed
// line a -> b". Returns the CCW vertices of the intersection, or {} if it is empty.
// A big bounding box is added, so the result is always bounded.
// 2D LINEAR PROGRAMMING is exactly this: constraints -> half-planes, then optimise over the
// resulting convex polygon (ternary search on the boundary, or check all vertices).
struct HP {
    Pd p, d;                                    // keep the side LEFT of p -> p + d
    HP() {}
    HP(Pd a, Pd b) : p(a), d(b - a) {}
    bool out(Pd r) const { return d.cross(r - p) < -eps; }
};
Pd hpInter(const HP& a, const HP& b) {          // assumes a.d and b.d are not parallel
    return a.p + a.d * ((b.p - a.p).cross(b.d) / a.d.cross(b.d));
}
vector<Pd> halfPlaneInter(vector<HP> h, ld BOX = 1e9) {
    Pd box[4] = {Pd(-BOX, -BOX), Pd(BOX, -BOX), Pd(BOX, BOX), Pd(-BOX, BOX)};
    for (int i = 0; i < 4; i++) h.push_back(HP(box[i], box[(i + 1) % 4]));
    sort(all(h), [](const HP& a, const HP& b) {
        if (half(a.d) != half(b.d)) return half(a.d) < half(b.d);
        ld c = a.d.cross(b.d);
        if (fabsl(c) > eps) return c > 0;
        return a.out(b.p);                      // same direction: most restrictive first
    });
    deque<HP> dq;
    for (auto& x : h) {
        if (!dq.empty() && fabsl(dq.back().d.cross(x.d)) <= eps && dq.back().d.dot(x.d) > 0) continue;
        while (dq.size() > 1 && x.out(hpInter(dq[dq.size() - 1], dq[dq.size() - 2]))) dq.pop_back();
        while (dq.size() > 1 && x.out(hpInter(dq[0], dq[1]))) dq.pop_front();
        dq.push_back(x);
    }
    while (dq.size() > 2 && dq[0].out(hpInter(dq[dq.size() - 1], dq[dq.size() - 2]))) dq.pop_back();
    while (dq.size() > 2 && dq.back().out(hpInter(dq[0], dq[1]))) dq.pop_front();
    if (dq.size() < 3) return {};
    vector<Pd> r;
    for (size_t i = 0; i < dq.size(); i++) r.push_back(hpInter(dq[i], dq[(i + 1) % dq.size()]));
    return r;
}
// If you only have a handful of half-planes, repeated cut() (03 - Polygon.cpp) on a big box is
// shorter to retype and just as correct - O(n^2) but n is small.
