// Circles. All of these want a floating P (Pd).

// --- circle x line: 0, 1 (tangent) or 2 points on segment/line ab ---
template <class P> vector<P> circleLine(P o, ld r, P a, P b) {
    P p = proj(o, a, b);
    ld h2 = r * r - (p - o).dist2();
    if (h2 < -eps) return {};
    if (h2 < eps) return {p};
    P h = (b - a).unit() * sqrtl(h2);
    return {p - h, p + h};
}
template <class P> vector<P> circleSeg(P o, ld r, P a, P b) {
    vector<P> out;
    for (P p : circleLine(o, r, a, b)) if (segDist(p, a, b) < eps) out.push_back(p);
    return out;
}

// --- circle x circle: the 0/1/2 intersection points ---
template <class P> vector<P> circleCircle(P a, ld r1, P b, ld r2) {
    P d = b - a; ld d2 = d.dist2();
    if (d2 < eps) return {};                              // concentric (equal => infinite)
    ld p = (d2 + r1 * r1 - r2 * r2) / (2 * d2), h2 = r1 * r1 - p * p * d2;
    if (r1 + r2 < sqrtl(d2) - eps || fabsl(r1 - r2) > sqrtl(d2) + eps) return {};
    P mid = a + d * p, per = d.perp() * sqrtl(max((ld)0, h2) / d2);
    if (h2 < eps) return {mid};
    return {mid + per, mid - per};
}

// --- AREA of the lens (intersection region) of two circles ---
template <class P> ld circleInterArea(P a, ld r1, P b, ld r2) {
    ld d = (a - b).dist();
    if (d >= r1 + r2) return 0;
    if (d + min(r1, r2) <= max(r1, r2)) return PI * min(r1, r2) * min(r1, r2);
    ld A = acosl((d * d + r1 * r1 - r2 * r2) / (2 * d * r1));
    ld B = acosl((d * d + r2 * r2 - r1 * r1) / (2 * d * r2));
    return r1 * r1 * (A - sinl(2 * A) / 2) + r2 * r2 * (B - sinl(2 * B) / 2);
}

// --- TANGENTS. Each result is a pair (touch point on c1, touch point on c2).
// inner=false -> the 2 external tangents; inner=true -> the 2 internal (crossing) ones.
// Tangent lines from a POINT p to circle (c,r):  tangents(p, 0, c, r, false).
template <class P> vector<pair<P, P>> tangents(P c1, ld r1, P c2, ld r2, bool inner) {
    if (inner) r2 = -r2;
    P d = c2 - c1;
    ld dr = r1 - r2, d2 = d.dist2(), h2 = d2 - dr * dr;
    if (d2 < eps || h2 < -eps) return {};
    vector<pair<P, P>> out;
    for (ld s : {-1.0L, 1.0L}) {
        P v = (d * dr + d.perp() * sqrtl(max((ld)0, h2)) * s) / d2;
        out.push_back({c1 + v * r1, c2 + v * r2});
    }
    if (h2 < eps) out.pop_back();
    return out;
}

// --- circumcircle / incircle of a triangle ---
template <class P> P circumcenter(P a, P b, P c) {
    P B = c - a, C = b - a;
    return a + (B * C.dist2() - C * B.dist2()).perp() / B.cross(C) / 2;
}
template <class P> ld circumradius(P a, P b, P c) {
    return (b - a).dist() * (c - b).dist() * (a - c).dist() / (2 * fabsl((ld)(b - a).cross(c - a)));
}
template <class P> P incenter(P a, P b, P c) {
    ld A = (b - c).dist(), B = (c - a).dist(), C = (a - b).dist();
    return (a * A + b * B + c * C) / (A + B + C);
}
template <class P> ld inradius(P a, P b, P c) {
    ld s = ((b - a).dist() + (c - b).dist() + (a - c).dist()) / 2;
    return fabsl((ld)(b - a).cross(c - a)) / 2 / s;
}
template <class P> P orthocenter(P a, P b, P c) { return a + b + c - circumcenter(a, b, c) * 2; }

// --- MINIMUM ENCLOSING CIRCLE, expected O(n) (Welzl, incremental) ---
template <class P> pair<P, ld> mec(vector<P> p) {
    shuffle(all(p), mt19937(chrono::steady_clock::now().time_since_epoch().count()));
    P o = p[0]; ld r = 0, E = 1 + 1e-9;
    for (int i = 0; i < (int)p.size(); i++) if ((o - p[i]).dist() > r * E) {
        o = p[i], r = 0;
        for (int j = 0; j < i; j++) if ((o - p[j]).dist() > r * E) {
            o = (p[i] + p[j]) / 2, r = (o - p[i]).dist();
            for (int k = 0; k < j; k++) if ((o - p[k]).dist() > r * E)
                o = circumcenter(p[i], p[j], p[k]), r = (o - p[i]).dist();
        }
    }
    return {o, r};
}

// --- AREA of (circle centered at c, radius r) INTERSECT polygon ps ---
template <class P> ld circlePolyArea(P c, ld r, vector<P> ps) {
    auto arg = [](P p, P q) { return atan2l(p.cross(q), p.dot(q)); };
    auto tri = [&](P p, P q) -> ld {
        ld r2 = r * r / 2;
        P d = q - p;
        ld a = d.dot(p) / d.dist2(), b = (p.dist2() - r * r) / d.dist2(), det = a * a - b;
        if (det <= 0) return arg(p, q) * r2;
        ld s = max((ld)0, -a - sqrtl(det)), t = min((ld)1, -a + sqrtl(det));
        if (t < 0 || 1 <= s) return arg(p, q) * r2;
        P u = p + d * s, v = p + d * t;
        return arg(p, u) * r2 + u.cross(v) / 2 + arg(v, q) * r2;
    };
    ld sum = 0;
    for (int i = 0, n = ps.size(); i < n; i++) sum += tri(ps[i] - c, ps[(i + 1) % n] - c);
    return fabsl(sum);
}

// --- power of a point / radical axis ---
// pow(p) = |p-c|^2 - r^2 : <0 inside, 0 on circle, >0 outside; equals (tangent length)^2.
template <class P> ld power(P p, P c, ld r) { return (p - c).dist2() - r * r; }
// Radical axis of two non-concentric circles, as two points on it.
template <class P> pair<P, P> radicalAxis(P a, ld r1, P b, ld r2) {
    P d = b - a; ld t = ((r1 * r1 - r2 * r2) / d.dist2() + 1) / 2;
    P m = a + d * t;
    return {m, m + d.perp()};
}
