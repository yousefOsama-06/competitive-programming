// 3D point / vector, planes, and the few 3D things that actually appear.

template <class T>
struct P3 {
    typedef P3 Pt; T x = 0, y = 0, z = 0;
    P3(T x = 0, T y = 0, T z = 0) : x(x), y(y), z(z) {}
    Pt operator+(Pt p) const { return {x + p.x, y + p.y, z + p.z}; }
    Pt operator-(Pt p) const { return {x - p.x, y - p.y, z - p.z}; }
    Pt operator*(T d) const { return {x * d, y * d, z * d}; }
    Pt operator/(T d) const { return {x / d, y / d, z / d}; }
    T dot(Pt p) const { return x * p.x + y * p.y + z * p.z; }
    Pt cross(Pt p) const { return {y * p.z - z * p.y, z * p.x - x * p.z, x * p.y - y * p.x}; }
    T dist2() const { return x * x + y * y + z * z; }
    ld dist() const { return sqrtl((ld)dist2()); }
    Pt unit() const { return *this / dist(); }
    ld phi() const { return atan2l(y, x); }                       // longitude, (-pi, pi]
    ld theta() const { return atan2l(sqrtl((ld)(x * x + y * y)), z); }  // colatitude, [0, pi]
    Pt rot(ld a, Pt ax) const {                                   // Rodrigues, ccw around ax
        Pt u = ax.unit(); ld s = sinl(a), c = cosl(a);
        return u * dot(u) * (1 - c) + (*this) * c + u.cross(*this) * s;
    }
    bool operator<(Pt p) const { return tie(x, y, z) < tie(p.x, p.y, p.z); }
    bool operator==(Pt p) const { return tie(x, y, z) == tie(p.x, p.y, p.z); }
    friend istream& operator>>(istream& i, Pt& p) { return i >> p.x >> p.y >> p.z; }
    friend ostream& operator<<(ostream& o, Pt p) { return o << p.x << ' ' << p.y << ' ' << p.z; }
};
typedef P3<ll> Pi3;
typedef P3<ld> Pd3;

// PLANE  n.dot(X) = d.   Build from 3 points, or from a normal and a point.
template <class P> struct Plane {
    P n; ld d;
    Plane(P n = P(), ld d = 0) : n(n), d(d) {}
    Plane(P a, P b, P c) : n((b - a).cross(c - a)), d(n.dot(a)) {}
    ld side(P p) const { return n.dot(p) - d; }                   // sign = which side
    ld dist(P p) const { return fabsl(side(p)) / n.dist(); }
    P proj(P p) const { return p - n * (side(p) / n.dist2()); }
    P refl(P p) const { return p - n * (2 * side(p) / n.dist2()); }
};
// LINE a->b vs plane. {0 parallel-disjoint, 1 unique, -1 line lies in plane}
template <class P> pair<int, P> planeLine(const Plane<P>& pl, P a, P b) {
    ld da = pl.side(a), db = pl.side(b);
    if (fabsl(da - db) < eps) return {fabsl(da) < eps ? -1 : 0, P()};
    return {1, (b * da - a * db) / (da - db)};
}
// PLANE x PLANE -> a line, given as {point on it, direction}. dir==0 means parallel.
template <class P> pair<P, P> planePlane(const Plane<P>& A, const Plane<P>& B) {
    P dir = A.n.cross(B.n);
    if (dir.dist2() < eps) return {P(), P()};
    P p = (B.n * A.d - A.n * B.d).cross(dir) / dir.dist2();
    return {p, dir};
}

// VOLUME of the tetrahedron abcd, signed. The x6 form is exact on P3<ll> only up to
// |coordinate| ~ 5.7e5, since |vol6| <= 48 * C^3; above that make the return type __int128.
template <class P> auto vol6(P a, P b, P c, P d) { return (b - a).cross(c - a).dot(d - a); }
template <class P> ld tetraVolume(P a, P b, P c, P d) { return fabsl((ld)vol6(a, b, c, d)) / 6; }

// SPHERE x LINE: points where line a->b meets the sphere (o, r).
template <class P> vector<P> sphereLine(P o, ld r, P a, P b) {
    P d = b - a, f = a - o;
    ld A = d.dist2(), B = 2 * f.dot(d), C = f.dist2() - r * r, D = B * B - 4 * A * C;
    if (D < -eps) return {};
    D = sqrtl(max((ld)0, D));
    if (D < eps) return {a + d * (-B / (2 * A))};
    return {a + d * ((-B - D) / (2 * A)), a + d * ((-B + D) / (2 * A))};
}

// GREAT-CIRCLE distance on a sphere of radius R, inputs in DEGREES (haversine).
ld greatCircle(ld lat1, ld lon1, ld lat2, ld lon2, ld R) {
    auto rad = [](ld d) { return d * PI / 180; };
    ld dl = rad(lat2 - lat1) / 2, dg = rad(lon2 - lon1) / 2;
    ld h = sinl(dl) * sinl(dl) + cosl(rad(lat1)) * cosl(rad(lat2)) * sinl(dg) * sinl(dg);
    return 2 * R * asinl(sqrtl(min((ld)1, h)));
}
