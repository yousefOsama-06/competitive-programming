// Point / vector core. P<ll> = exact (use for all predicates), P<ld> = real answers.
// cross(a,b) > 0  <=>  b is counter-clockwise from a.
// a.cross(b,c) > 0 <=> c lies strictly LEFT of the directed line a->b.   [= orient(a,b,c)]
int sgn(ll x) { return (x > 0) - (x < 0); }
int sgn(ld x) { return (x > eps) - (x < -eps); }
int sgn(int x) { return (x > 0) - (x < 0); }        // without these two overloads a bare int or
int sgn(double x) { return sgn((ld)x); }            // double argument is AMBIGUOUS: hard error
ll gcdll(ll a, ll b) { return b ? gcdll(b, a % b) : a; }   // std::__gcd is libstdc++-only

template <class T>
struct P {
    typedef P Pt; T x = 0, y = 0;
    P(T x = 0, T y = 0) : x(x), y(y) {}
    Pt operator+(Pt p) const { return {x + p.x, y + p.y}; }
    Pt operator-(Pt p) const { return {x - p.x, y - p.y}; }
    Pt operator*(T d) const { return {x * d, y * d}; }
    Pt operator/(T d) const { return {x / d, y / d}; }
    T dot(Pt p) const { return x * p.x + y * p.y; }
    T cross(Pt p) const { return x * p.y - y * p.x; }
    T cross(Pt a, Pt b) const { return (a - *this).cross(b - *this); }  // orient
    T dist2() const { return x * x + y * y; }
    ld dist() const { return sqrtl((ld)dist2()); }
    ld angle() const { return atan2l(y, x); }            // (-pi, pi]
    Pt perp() const { return {-y, x}; }                  // rotate +90 deg
    Pt unit() const { return *this / dist(); }
    Pt rot(ld a) const { return {x * cosl(a) - y * sinl(a), x * sinl(a) + y * cosl(a)}; }
    bool operator<(Pt p) const { return tie(x, y) < tie(p.x, p.y); }
    bool operator==(Pt p) const { return tie(x, y) == tie(p.x, p.y); }
    friend istream& operator>>(istream& i, Pt& p) { return i >> p.x >> p.y; }
    friend ostream& operator<<(ostream& o, Pt p) { return o << p.x << ' ' << p.y; }
};
typedef P<ll> Pi;
typedef P<ld> Pd;

// Signed angle from a to b in (-pi, pi]. Sign tells you the rotation direction.
template <class T> ld angleTo(P<T> a, P<T> b) { return atan2l(a.cross(b), a.dot(b)); }
// Angle a-O-b in [0, 2pi).
template <class T> ld angleAt(P<T> a, P<T> O, P<T> b) {
    ld r = angleTo(a - O, b - O);
    return r < 0 ? r + 2 * PI : r;
}

// EXACT polar sort, no atan2. Orders by angle in [0, 2pi) starting at +x axis.
// sort(all(v), angCmp<ll>);   ties (same direction) keep input order.
template <class T> bool half(P<T> p) { return p.y < 0 || (p.y == 0 && p.x < 0); }
template <class T> bool angCmp(P<T> a, P<T> b) {
    return half(a) != half(b) ? half(a) < half(b) : a.cross(b) > 0;
}

// OVERFLOW: |coord| <= 1e9  =>  cross fits in ll (max ~4e18, ll max 9.2e18).
//           dist2 of a difference fits too. Anything squared twice needs __int128.
