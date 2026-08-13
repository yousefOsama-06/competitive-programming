// BERNOULLI NUMBERS and the CLOSED-FORM FAULHABER polynomial.
// Use this (not Lagrange interpolation) when you need the POLYNOMIAL COEFFICIENTS of
// sum_{k=1..n} k^p, or when p is large and many n are queried.
// Convention here: B_1 = -1/2  (the "B^-" convention that the formula below expects).
//   B_0 = 1 ;   sum_{k=0}^{m} C(m+1, k) * B_k = [m == 0]
//   =>  B_m = -(1/(m+1)) * sum_{k=0}^{m-1} C(m+1, k) * B_k
vector<Mint> bernoulli(int m) {                        // B[0..m], O(m^2)
    vector<Mint> B(m + 1, 0);
    B[0] = 1;
    for (int i = 1; i <= m; i++) {
        Mint s = 0;
        for (int k = 0; k < i; k++) s += C(i + 1, k) * B[k];
        B[i] = Mint(0) - s * inv_[i + 1];
    }
    return B;                                          // 1, -1/2, 1/6, 0, -1/30, 0, 1/42, ...
}
// O(m log m) alternative: B_m / m! = [t^m] inv( sum_{i>=0} t^i / (i+1)! )  -- one Poly::inv.

// FAULHABER:  sum_{k=1}^{n} k^p = (1/(p+1)) * sum_{r=0}^{p} (-1)^r * C(p+1, r) * B_r * n^(p+1-r)
Mint powerSumClosed(ll n, int p, const vector<Mint>& B) {
    Mint s = 0, np = Mint(n).pow(p + 1);               // n^(p+1-r), decreasing r
    vector<Mint> pw(p + 2);
    pw[0] = 1;
    for (int i = 1; i <= p + 1; i++) pw[i] = pw[i - 1] * Mint(n);
    for (int r = 0; r <= p; r++) {
        Mint t = C(p + 1, r) * B[r] * pw[p + 1 - r];
        (r & 1) ? s -= t : s += t;
    }
    return s * inv_[p + 1];
}
// The COEFFICIENTS of the Faulhaber polynomial in n (degree p+1):
//   [n^(p+1-r)] = (-1)^r * C(p+1, r) * B_r / (p+1)
vector<Mint> faulhaberPoly(int p, const vector<Mint>& B) {
    vector<Mint> c(p + 2, 0);
    for (int r = 0; r <= p; r++) {
        Mint t = C(p + 1, r) * B[r] * inv_[p + 1];
        c[p + 1 - r] = (r & 1) ? Mint(0) - t : t;
    }
    return c;                                          // c[i] = coefficient of n^i
}
// RELATED SUMS
//   sum_{k=0}^{n} C(k, r)      = C(n+1, r+1)                     (hockey stick)
//   sum_{k=1}^{n} k^p * r^k    : degree-p polynomial times r^n; solve by ansatz or by
//                                differentiating the geometric series p times.
//   Bernoulli also gives Euler-Maclaurin and the von Staudt-Clausen denominator facts.
