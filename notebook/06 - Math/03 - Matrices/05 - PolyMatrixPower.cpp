// Needs: multiply (05 - Convolution/02 - NTT.cpp), Poly / cut (05 - Convolution/04).
// MATRIX EXPONENTIATION WHERE THE ENTRIES ARE POWER SERIES, truncated mod x^K.
// M^e in O(n^3 * K log K * log e).  Everything is mod NMOD = 998244353.
// REACH FOR IT for a linear recurrence in n that also tracks a SECOND statistic you want the full
// distribution of: put x^(contribution) in the transition instead of 1, and the (i, j) entry of
// M^e becomes the generating function over that statistic. Classic: "count length-n walks by
// number of times a marked edge is used", "f(n) for every k = number of ..." (CF 755G), any DP
// whose answer is asked for ALL values of a parameter at once rather than one value.
// A plain Mat over Mint gives you one number; this gives you K numbers for a log K factor.
typedef vector<vector<Poly>> PMat;

PMat pmul(const PMat& a, const PMat& b, int K) {
    int n = a.size(), m = b[0].size(), k = b.size();
    PMat c(n, vector<Poly>(m, Poly(1, 0)));
    for (int i = 0; i < n; i++)
        for (int t = 0; t < k; t++) {
            if (a[i][t].size() == 1 && !a[i][t][0]) continue;      // skip the zero series
            for (int j = 0; j < m; j++) c[i][j] = c[i][j] + cut(multiply(a[i][t], b[t][j]), K);
        }
    return c;
}
PMat peye(int n) {
    PMat r(n, vector<Poly>(n, Poly(1, 0)));
    for (int i = 0; i < n; i++) r[i][i] = Poly(1, 1);
    return r;
}
PMat ppow(PMat a, ll e, int K) {                 // a^e mod x^K
    PMat r = peye(a.size());
    for (; e > 0; e >>= 1, a = pmul(a, a, K)) if (e & 1) r = pmul(r, a, K);
    return r;
}
// EXAMPLE (Fibonacci refined by the number of "1" steps, CF 755G shape):
//   M = {{1 + x, x}, {1, 0}} and (M^(n-1) * v)[0] is the generating function you want.
// CHEAPER ALTERNATIVES, try these first:
//  - if K is tiny (<= 3), just hand-roll the truncated series as k separate Mint matrices.
//  - if the statistic is "how many times did we take transition t", one variable suffices; if it
//    is a pair of statistics you need a bivariate series - flatten it to x^(i*W + j).
//  - if n (the matrix size) is 1, this is just power-series exponentiation: use pow_ directly.
//  - if you want ONE coefficient rather than all K, Bostan-Mori on the rational function is
//    O(d log d log e) and much faster (06 - Interpolation/03 - BostanMori.cpp).
// COST WARNING: n^3 poly multiplications per squaring. n = 8, K = 1e5, e = 1e18 is already at the
// edge; keep the matrix small, and truncate (cut to K) after EVERY product, never at the end.
