// Exact rational. Always normalised (gcd reduced, denominator > 0).
// Comparison uses __int128 so it is exact for |num|,|den| up to ~9e18.
// Use whenever comparing ratios: slopes, "maximum average", scheduling exchange arguments.
struct Frac {
    ll n = 0, d = 1;
    Frac(ll n = 0, ll d = 1) : n(n), d(d) { norm(); }
    void norm() {
        if (d < 0) n = -n, d = -d;
        ll g = gcd(n < 0 ? -n : n, d);
        if (g) n /= g, d /= g;
        if (!d) d = 1;                                // 1/0 -> treat as 0/1; guard your input
    }
    Frac operator+(Frac o) const { return Frac(n * o.d + o.n * d, d * o.d); }
    Frac operator-(Frac o) const { return Frac(n * o.d - o.n * d, d * o.d); }
    Frac operator*(Frac o) const { return Frac(n * o.n, d * o.d); }
    Frac operator/(Frac o) const { return Frac(n * o.d, d * o.n); }
    bool operator<(Frac o) const { return (__int128)n * o.d < (__int128)o.n * d; }
    bool operator==(Frac o) const { return n == o.n && d == o.d; }
    bool operator<=(Frac o) const { return !(o < *this); }
    ld val() const { return (ld)n / d; }
    friend ostream& operator<<(ostream& os, Frac f) { return os << f.n << '/' << f.d; }
};
// OVERFLOW: + and * multiply denominators, so chains of operations blow up fast. If you only
// need COMPARISONS, keep the raw pair and cross-multiply in __int128 - do not normalise.
// For unbounded exactness use Python/BigInt, or the Stern-Brocot search (ContinuedFractions).
