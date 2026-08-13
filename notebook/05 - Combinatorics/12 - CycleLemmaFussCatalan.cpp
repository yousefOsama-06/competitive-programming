// CYCLE LEMMA (Dvoretzky-Motzkin) and the FUSS-CATALAN / rational-Catalan family.
// The general-k version of the ballot problem: it handles every "+1 / -k step" path problem.
//
// CYCLE LEMMA: a word with m a's and n b's, m >= k*n, has EXACTLY m - k*n cyclic shifts that
// are k-dominating (every non-empty prefix has #a > k * #b).
//   k = 1 ballot: p A's and q B's with p > q  ->  exactly p - q dominating shifts,
//                 so the probability that A leads the whole count is (p - q) / (p + q).

// #paths with p up-steps and q down-steps that stay STRICTLY positive after the first step.
Mint ballotStrict(ll p, ll q) {                        // p > q >= 0
    if (p <= q) return 0;
    return C(p + q, q) * Mint(p - q) * inv_[(int)(p + q)];
}
// FUSS-CATALAN: A_m(p, r) = (r / (m*p + r)) * C(m*p + r, m)
Mint fussCatalan(ll m, ll p, ll r) {
    if (m < 0) return 0;
    return C(m * p + r, m) * Mint(r) * Mint(m * p + r).inv();
}
// #p-ary trees with m internal nodes = A_m(p, 1) = C(m*p + 1, m) / (m*p + 1)
Mint pAryTrees(ll m, ll p) { return fussCatalan(m, p, 1); }
// Catalan is the case p = 2, r = 1.
// #sequences of m opens and (p-1)*m closes where every prefix keeps the stack non-negative
// is also A_m(p, 1).

// RATIONAL CATALAN: for gcd(a, b) = 1, the number of monotone lattice paths from (0,0) to
// (b, a) that never rise above the line a*x = b*y is  C(a + b, a) / (a + b).
Mint rationalCatalan(ll a, ll b) { return C(a + b, a) * Mint(a + b).inv(); }

// CHUNG-FELLER: among the C(2n,n) balanced +-1 paths of length 2n, the number having exactly
// 2r steps above the axis is Catalan(n) for EVERY r in 0..n.  Hence C(2n,n) = (n+1)*Cat(n).
// Use it for "count sequences with exactly k flaws" - the answer does not depend on k.

// REFLECTION PRINCIPLE (the tool behind all of the above):
//   #paths (0,0)->(x,y) touching the line y = m  =  #paths (0,0)->(x, 2m - y)
//   so "#paths avoiding the line" = C(total, up) - C(total, up shifted by the reflection).
// PRECONDITION: the barrier must SEPARATE the start (height 0) from the end y, i.e. m != 0
// and 0, y strictly on the same side of m. Without it the reflection subtracts too much:
// (x,y,m) = (2,2,1) returns -1 where the true count is 0.
Mint pathsAvoidingLine(ll x, ll y, ll m) {
    if (m == 0 || (m > 0 && y >= m) || (m < 0 && y <= m)) return 0;             // steps +1/-1, must never reach y = m
    if (((x + y) & 1) || y > x) return 0;
    ll up = (x + y) / 2, up2 = (x + 2 * m - y) / 2;
    return C(x, up) - ((x + 2 * m - y) % 2 || up2 < 0 || up2 > x ? Mint(0) : C(x, up2));
}
