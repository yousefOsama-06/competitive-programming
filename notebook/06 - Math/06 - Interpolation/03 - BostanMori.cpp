// Needs: multiply / npow (02 - NTT.cpp).
// BOSTAN-MORI: the k-th term of a linear recurrence of order d in O(d log d log k), mod NMOD.
// This is the fast replacement for Kitamasa (O(d^2 log k), 06 - Interpolation/02 - linearRec) and
// for companion-matrix exponentiation (O(d^3 log k)). At d = 500, k = 1e18 it is the only one that
// runs. Pair it with Berlekamp-Massey: guess the recurrence from brute-forced terms, then jump.
// INPUT: a[0..d-1] the first d terms, c[0..d-1] with a[n] = sum_i c[i] * a[n-1-i] for n >= d.
// IDEA: the generating function is RATIONAL, sum a_n x^n = P(x) / Q(x) with
//   Q = 1 - c0 x - ... - c(d-1) x^d,   P = (A * Q) mod x^d.
// Then [x^k] P/Q: multiply top and bottom by Q(-x). The new denominator V(x) = Q(x)Q(-x) is EVEN,
// so V(x) = W(x^2); keeping the even or odd part of the numerator (by the parity of k) halves k.
ll bostanMori(vector<ll> a, const vector<ll>& c, ll k) {
    int d = c.size();
    if (k < (ll)a.size() && k < d) return a[k] % NMOD;
    vector<ll> Q(d + 1, 0);
    Q[0] = 1;
    for (int i = 0; i < d; i++) Q[i + 1] = (NMOD - c[i] % NMOD) % NMOD;
    a.resize(d, 0);
    vector<ll> P = multiply(a, Q);
    P.resize(d);                                         // P = (A * Q) mod x^d
    while (k) {
        vector<ll> Qm = Q;
        for (size_t i = 1; i < Qm.size(); i += 2) Qm[i] = (NMOD - Qm[i]) % NMOD;
        vector<ll> U = multiply(P, Qm), V = multiply(Q, Qm);
        P.assign((U.size() + 1) / 2, 0);
        for (size_t i = k & 1; i < U.size(); i += 2) P[i / 2] = U[i];
        Q.assign((V.size() + 1) / 2, 0);
        for (size_t i = 0; i < V.size(); i += 2) Q[i / 2] = V[i];
        P.resize(d), Q.resize(d + 1);
        k >>= 1;
    }
    return P[0] * npow(Q[0], NMOD - 2) % NMOD;
}
// THE RATIONAL GENERATING FUNCTION IS THE POINT, not just the k-th term:
//  sum_n a_n = P(1) / Q(1)   (formally; use it when the series really does terminate/converge,
//    e.g. absorbing-state probabilities where a_n is the chance of stopping at step n).
//  sum_n n * a_n = (P'(1) Q(1) - P(1) Q'(1)) / Q(1)^2 - this is the EXPECTED STOPPING TIME, the
//    reason this trick shows up in probability problems: brute-force 2d terms, run Berlekamp-
//    Massey, build P/Q, differentiate at x = 1. Beware Q(1) = 0, which means the series does not
//    sum and you must handle the pole separately.
//  PARTIAL FRACTIONS of P/Q give the closed form a_n = sum c_i r_i^n over the roots of Q.
//  PREFIX SUMS: sum_{n<=k} a_n is the k-th coefficient of P / ((1-x) Q) - just multiply Q by
//    (1 - x) and run the same routine.
// PRECONDITION: NMOD prime, Q[0] = 1 (automatic here). The recurrence must be exact; if you got it
// from Berlekamp-Massey, feed BM at least 2*d terms or the answer is silently wrong.
