// Needs: multiply / npow (02 - NTT.cpp), Poly / cut (04 - Polynomial.cpp).
// TAYLOR SHIFT p(x) -> p(x + c), and COMPOSITION p(g(x)). Both mod NMOD = 998244353.

// TAYLOR SHIFT in O(n log n).  [x^k] p(x+c) = sum_{j>=k} a_j * C(j, k) * c^(j-k), which is a
// correlation once you divide by the factorials, hence ONE multiplication.
// REACH FOR IT to re-centre an interpolation ("I have f sampled at 0..n, I want it at m..m+n"),
// to turn "evaluate at an ARITHMETIC progression a, a+d, a+2d, ..." into a shift plus a scale, and
// inside falling-factorial / Stirling conversions.
Poly taylorShift(Poly a, ll c) {
    int n = a.size();
    vector<ll> f(n), fi(n);
    f[0] = 1;
    for (int i = 1; i < n; i++) f[i] = f[i - 1] * i % NMOD;
    fi[n - 1] = npow(f[n - 1], NMOD - 2);
    for (int i = n - 1; i > 0; i--) fi[i - 1] = fi[i] * i % NMOD;
    Poly A(n), B(n);
    for (int i = 0; i < n; i++) A[n - 1 - i] = a[i] * f[i] % NMOD;      // reversed a_j * j!
    ll p = 1;
    for (int i = 0; i < n; i++) B[i] = p * fi[i] % NMOD, p = p * (c % NMOD + NMOD) % NMOD;
    Poly h = multiply(A, B);
    Poly r(n);
    for (int k = 0; k < n; k++) r[k] = h[n - 1 - k] * fi[k] % NMOD;
    return r;
}
// COMPOSITION p(g(x)) mod x^m in O(sqrt(n) * m log m) by the BLOCK (baby-step giant-step) method:
// write p(y) = sum_{i<K} y^(i*D) * q_i(y) with deg q_i < D, precompute g^0..g^D once, build each
// q_i(g) by linear combinations, then Horner over the K blocks with the single power g^D.
// REACH FOR IT for "substitute one generating function into another": exp/log of a series with a
// non-constant argument, EGF of a labelled construction, "apply a transform given as a series".
Poly composition(const Poly& p, const Poly& g, int m) {
    int n = p.size(), K = 1;
    while (K * K < n) K++;                               // K blocks of D = ceil(n / K) terms
    int D = (n + K - 1) / K;
    vector<Poly> pw(D + 1, Poly{1});
    for (int i = 1; i <= D; i++) pw[i] = cut(multiply(pw[i - 1], g), m);
    Poly res(m, 0), gd{1};
    for (int i = 0; i < K; i++) {
        Poly blk(m, 0);
        for (int j = 0; j < D && i * D + j < n; j++) {
            ll c = p[i * D + j];
            if (!c) continue;
            for (int t = 0; t < (int)pw[j].size() && t < m; t++)
                blk[t] = (blk[t] + c * pw[j][t]) % NMOD;
        }
        blk = cut(multiply(blk, gd), m);
        for (int t = 0; t < m && t < (int)blk.size(); t++) res[t] = (res[t] + blk[t]) % NMOD;
        gd = cut(multiply(gd, pw[D]), m);
    }
    return res;
}
// COMPOSITIONAL INVERSE f^(-1) (the series h with f(h(x)) = x), for f[0] = 0, f[1] != 0:
// LAGRANGE INVERSION gives a single coefficient directly,
//   [x^n] h = (1/n) * [x^(n-1)] (x / f(x))^n,   and more generally
//   [x^n] G(h(x)) = (1/n) * [x^(n-1)] G'(x) * (x / f(x))^n.
// One coefficient costs O(n log n); all n of them by this route costs O(n^2 log n), so use it when
// you need ONE answer (which is the usual case: Catalan, Fuss-Catalan, tree counts).
// K-TH ROOT of a series with a[0] = 1: pow_(a, inv(k) mod (NMOD - 1)) - i.e. exp(log(a)/k). For
// a[0] != 1 factor out c * x^d first; a k-th root exists only if k | d and c is a k-th power
// residue, and then it is unique up to a k-th root of unity.
// COST WARNING: composition is the expensive one. If g is a MONOMIAL c*x^t, substitute by hand; if
// p is short, plain Horner with truncated products is O(n * m log m) and simpler.
