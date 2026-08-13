// ARITHMETIC IN THE FINITE FIELD GF(2^K): elements are K-bit masks = polynomials over GF(2)
// reduced mod a fixed IRREDUCIBLE polynomial of degree K. Add = xor, multiply = carry-less
// multiply then reduce. mul is O(K), pow / inverse O(K log) - or O(1) with the CLMUL intrinsic.
// REACH FOR IT when a problem is "linear algebra over a field of characteristic 2 with more than
// two elements": random algebraic identity testing over GF(2^K) (Freivalds / Schwartz-Zippel with
// xor-shaped constraints), Reed-Solomon-flavoured coding, "assign field values to symbols and test
// whether a determinant vanishes", and Wiedemann over GF(2).
// PRECONDITION: POLY below must be irreducible over GF(2) and of degree exactly K.
//   K=8: 0x11B   K=16: 0x1002B   K=30: 0x40000003   K=31: 0x80000009   K=32: 0x1000000AF
//   K=64: x^64 + x^4 + x^3 + x + 1 (constant 0x1B), the usual choice for unsigned long long.
namespace GF2K {
const int K = 30;
const ll POLY = (1LL << K) | 3;                  // x^30 + x + 1, irreducible

ll add(ll a, ll b) { return a ^ b; }             // subtraction is the same operation
ll mul(ll a, ll b) {
    ll r = 0;
    while (b) {
        if (b & 1) r ^= a;
        b >>= 1, a <<= 1;
        if (a >> K & 1) a ^= POLY;               // reduce as soon as the degree reaches K
    }
    return r;
}
ll pw(ll a, ll e) {
    ll r = 1;
    for (; e; e >>= 1, a = mul(a, a)) if (e & 1) r = mul(r, a);
    return r;
}
ll inv(ll a) { return pw(a, (1LL << K) - 2); }   // a^(2^K - 1) = 1, so a^-1 = a^(2^K - 2)
}
// FACTS YOU NEED WHEN USING IT
//  |GF(2^K)| = 2^K, the multiplicative group is CYCLIC of order 2^K - 1, and every element
//  satisfies a^(2^K) = a. Squaring is a field automorphism (FROBENIUS): (a + b)^2 = a^2 + b^2,
//  so squaring is LINEAR over GF(2) and can be tabulated as a matrix.
//  GF(2^K) contains GF(2^d) exactly when d | K.
//  x^(2^K) - x factors as the product of all irreducible polynomials whose degree divides K.
//  A random degree-K polynomial is irreducible with probability about 1/K; test with
//  gcd(x^(2^d) - x, f) == 1 for all d < K dividing K.
// NOT THE SAME AS NIMBERS. Nim-multiplication (10 - Game Theory/05 - NimbersAndSums.cpp) is a
// DIFFERENT field structure on the same set: it needs no irreducible polynomial, it is defined by
// the mex rules, and 2^(2^k) values form subfields. Use nimbers for game values, this for coding
// and linear-algebra style problems - they are isomorphic but NOT equal element by element.
// SPEED: on x86 with -mpclmul, _mm_clmulepi64_si128 does the carry-less product in one
// instruction; you then only need the Barrett-style reduction. Worth it only in a hot loop.
