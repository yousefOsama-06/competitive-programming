// Needs: multiply / npow (02 - NTT.cpp), Poly / inv / cut (04 - Polynomial.cpp).
// ALL POWER SUMS AT ONCE:  s[i] = sum_{p=0}^{t} p^i   for EVERY i = 0..k, in O(k log k).
// (Convention 0^0 = 1, so s[0] = t + 1.) powerSum in 06 - Interpolation/01 gives one exponent in
// O(k); this gives all k+1 of them for the same asymptotic cost, which is what you need when the
// answer is "for each k, output sum i^k" or when a Newton-identity / symmetric-function step needs
// the whole vector of power sums.
// EGF IDENTITY:  sum_i s[i] x^i / i! = sum_{p=0}^{t} e^(p x) = (e^(n x) - 1) / (e^x - 1)
//                                    = [(e^(n x) - 1) / x] * [x / (e^x - 1)],   n = t + 1.
// The right factor is the BERNOULLI generating function, B[i] = i! * [x^i] x/(e^x - 1), and
// x/(e^x - 1) = 1 / sum_{i>=0} x^i / (i+1)! - one power-series inverse, O(k log k). That beats the
// O(k^2) Bernoulli recurrence in 05 - Combinatorics/11.
Poly bernoulliEGF(int m) {                       // returns B[i] / i! for i = 0..m-1
    Poly d(m);
    d[0] = 1;
    for (int i = 1; i < m; i++) d[i] = d[i - 1] * npow(i + 1, NMOD - 2) % NMOD;   // 1 / (i+1)!
    return inv(d, m);
}
Poly powerSums(ll t, int k) {                    // s[i] = sum_{p=0}^{t} p^i, i = 0..k
    Poly B = bernoulliEGF(k + 2), P(k + 2, 0);
    ll n = (t + 1) % NMOD, cur = 1;
    for (int i = 1; i <= k + 1; i++) {                             // P[i] = n^i / i!
        cur = cur * n % NMOD * npow(i, NMOD - 2) % NMOD;
        P[i] = cur;
    }
    Poly Q = cut(multiply(P, B), k + 2);
    Poly s(k + 1);
    ll f = 1;
    for (int i = 0; i <= k; i++) { s[i] = Q[i + 1] * f % NMOD; f = f * (i + 1) % NMOD; }
    return s;
}
// RELATED ONE-LINERS BUILT FROM THE SAME PIECES
//  SUM OVER A LIST rather than an interval: sum_{j} a_j^i for all i = 0..k in O((n + k) log^2 n) -
//    build the rational function sum_j 1 / (1 - a_j x) by divide and conquer over the list (keep
//    numerator and denominator, merge with up = A*D + B*C, down = B*D), then expand it mod x^(k+1).
//    Those coefficients ARE the power sums; feed them to Newton's identities (05 - Comb/10)
//    to get the elementary symmetric polynomials.
//  sum_{p=0}^{t} p^i * r^p for all i: same shape, use 07 - ExpPolynomialSum.cpp per i, or convolve
//    with the EGF of the geometric series.
//  The Faulhaber POLYNOMIAL in n (coefficients rather than a value) is faulhaberPoly in
//    05 - Combinatorics/11 - BernoulliFaulhaber.cpp.
// PRECONDITION: NMOD prime and > k + 2 (we invert 1..k+1 and take a series inverse whose constant
// term is 1). t may be huge - only t + 1 mod NMOD enters.
