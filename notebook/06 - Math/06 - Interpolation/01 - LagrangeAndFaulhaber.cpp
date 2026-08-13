// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp).
// Lagrange interpolation + power sums. Needs Mint + buildFact().

// Given y[0..n] = f(0), f(1), ..., f(n) for a polynomial f of degree <= n, evaluate f(x). O(n).
// The consecutive-points case is the one you actually want; it kills a whole class of problems
// whose answer is "a polynomial in n of degree d" (guess d, sample d+1 values, interpolate).
Mint lagrangeConsecutive(const vector<Mint>& y, ll x) {
    int n = y.size() - 1;
    if (0 <= x && x <= n) return y[x];          // x < 0 would index y[negative]
    vector<Mint> pre(n + 2, 1), suf(n + 2, 1);
    for (int i = 0; i <= n; i++) pre[i + 1] = pre[i] * Mint(x - i);
    for (int i = n; i >= 0; i--) suf[i] = suf[i + 1] * Mint(x - i);
    Mint r = 0;
    for (int i = 0; i <= n; i++) {
        Mint t = y[i] * pre[i] * suf[i + 1] * invFact[i] * invFact[n - i];
        ((n - i) & 1) ? r -= t : r += t;
    }
    return r;
}
// Arbitrary sample points, O(n^2).
Mint lagrange(const vector<Mint>& xs, const vector<Mint>& ys, Mint x) {
    int n = xs.size();
    Mint r = 0;
    for (int i = 0; i < n; i++) {
        Mint num = ys[i], den = 1;
        for (int j = 0; j < n; j++) if (j != i) num *= x - xs[j], den *= xs[i] - xs[j];
        r += num / den;
    }
    return r;
}
// FAULHABER: sum_{i=1..n} i^k  is a polynomial in n of degree k+1 -> sample k+2 points. O(k log k).
Mint powerSum(ll n, int k) {
    vector<Mint> y(k + 2, 0);
    for (int i = 1; i <= k + 1; i++) y[i] = y[i - 1] + Mint(i).pow(k);
    return lagrangeConsecutive(y, n);
}
// Same trick works for: sum of C(i,k), sum of i^k * r^i (degree k in n after dividing by r^n),
// and any DP whose answer is eventually polynomial.
