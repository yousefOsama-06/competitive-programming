// FORMAL POWER SERIES over NMOD = 998244353, on top of multiply() from 02 - NTT.cpp.
// Every routine works "mod x^k". inv/log/exp/sqrt/pow are all Newton iteration: each step
// doubles the number of correct coefficients, so the cost is the cost of the last multiply.
typedef vector<ll> Poly;

Poly cut(Poly a, int k) { a.resize(k, 0); return a; }
Poly operator+(Poly a, const Poly& b) {
    a.resize(max(a.size(), b.size()), 0);
    for (size_t i = 0; i < b.size(); i++) a[i] = (a[i] + b[i]) % NMOD;
    return a;
}
Poly operator-(Poly a, const Poly& b) {
    a.resize(max(a.size(), b.size()), 0);
    for (size_t i = 0; i < b.size(); i++) a[i] = (a[i] - b[i] + NMOD) % NMOD;
    return a;
}
Poly operator*(const Poly& a, ll c) { Poly r = a; for (ll& x : r) x = x * (c % NMOD) % NMOD; return r; }

Poly deriv(const Poly& a) {
    if (a.size() <= 1) return {0};
    Poly r(a.size() - 1);
    for (size_t i = 1; i < a.size(); i++) r[i - 1] = a[i] * i % NMOD;
    return r;
}
Poly integ(const Poly& a) {
    Poly r(a.size() + 1, 0);
    for (size_t i = 0; i < a.size(); i++) r[i + 1] = a[i] * npow(i + 1, NMOD - 2) % NMOD;
    return r;
}
// 1/a mod x^k. Requires a[0] != 0.   g_{2m} = g_m * (2 - a * g_m)
Poly inv(const Poly& a, int k) {
    Poly g = {npow(a[0], NMOD - 2)};
    for (int m = 1; m < k; m <<= 1) {
        Poly t = multiply(cut(a, 2 * m), g);
        t = cut(t, 2 * m);
        for (ll& x : t) x = (NMOD - x) % NMOD;
        t[0] = (t[0] + 2) % NMOD;
        g = cut(multiply(g, t), 2 * m);
    }
    return cut(g, k);
}
// log(a) mod x^k. Requires a[0] == 1.   log a = integral(a' / a)
Poly log_(const Poly& a, int k) { return cut(integ(cut(multiply(deriv(a), inv(a, k)), k - 1)), k); }
// exp(a) mod x^k. Requires a[0] == 0.   g_{2m} = g_m * (1 + a - log g_m)
Poly exp_(const Poly& a, int k) {
    Poly g = {1};
    for (int m = 1; m < k; m <<= 1) {
        Poly t = cut(a, 2 * m) - log_(cut(g, 2 * m), 2 * m);
        t[0] = (t[0] + 1) % NMOD;
        g = cut(multiply(g, t), 2 * m);
    }
    return cut(g, k);
}
// sqrt(a) mod x^k, for a[0] == 1 (general a[0] needs a modular square root of a[0]).
Poly sqrt_(const Poly& a, int k) {
    Poly g = {1};
    ll i2 = npow(2, NMOD - 2);
    for (int m = 1; m < k; m <<= 1) {
        Poly t = cut(a, 2 * m) + multiply(g, g);
        t = cut(t, 2 * m);
        g = cut(multiply(t, inv(cut(g, 2 * m) * 2, 2 * m)), 2 * m);
    }
    return cut(g, k);
}
// a^e mod x^k, for a[0] == 1:  exp(e * log a).  For a[0] != 1 factor out c*x^d first.
Poly pow_(const Poly& a, ll e, int k) { return exp_(log_(a, k) * (e % NMOD), k); }

// Polynomial DIVISION with remainder: a = q*b + r, deg r < deg b.  O(n log n)
pair<Poly, Poly> divmod(Poly a, Poly b) {
    while (a.size() > 1 && !a.back()) a.pop_back();    // trim BOTH, or the reversal is wrong
    while (b.size() > 1 && !b.back()) b.pop_back();
    int n = a.size(), m = b.size();
    if (n < m) return {{0}, a};
    Poly ra(a.rbegin(), a.rend()), rb(b.rbegin(), b.rend());
    Poly q = cut(multiply(cut(ra, n - m + 1), inv(cut(rb, n - m + 1), n - m + 1)), n - m + 1);
    reverse(all(q));
    Poly r = cut(a - multiply(q, b), max(1, m - 1));    // cut AFTER subtracting, not before
    return {q, r};
}
// USES: counting with generating functions (exp of a "connected" series = all structures),
// partition numbers via prod 1/(1-x^i) = exp(sum ...), Catalan-style algebraic equations via
// sqrt, k-th power of a GF, linear recurrence coefficients via inv, and Lagrange inversion.
