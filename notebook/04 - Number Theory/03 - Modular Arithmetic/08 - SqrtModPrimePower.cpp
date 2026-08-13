// Needs: sqrtMod (02 - DiscreteLogAndRoots.cpp), extGCD (01 - Euclidean Algorithm), CRT (01),
//        factorize (02 - Primes and Divisors/06 - MillerRabinPollard.cpp).
// SQUARE ROOTS MODULO A PRIME POWER, AND MODULO A COMPOSITE - what Tonelli-Shanks alone cannot
// do. Everything here is HENSEL LIFTING: a root mod p^j lifts to a root mod p^(j+1) by one
// Newton step x <- x - f(x)/f'(x), valid whenever f'(x) = 2x is invertible mod p, i.e. p odd and
// p !| x. p = 2 is the exception (the derivative is even) and is handled separately below.
// PRECONDITION for both routines: gcd(a, p) = 1. If p | a, first pull out the largest p^(2t)
// dividing a (an ODD exponent of p in a means NO solution unless it is >= k), solve for a/p^(2t)
// modulo p^(k-2t), multiply the roots by p^t, and add every multiple of p^(k-t).
// COMPLEXITY: O(k log p) per lift; O(log^2) for the composite version plus the factorisation.
ll invMod(ll a, ll m) { ll x, y; extGCD(((a % m) + m) % m, m, x, y); return ((x % m) + m) % m; }
// one root of x^2 = a (mod p^k), p an ODD prime, gcd(a, p) = 1. -1 if a is a non-residue mod p.
ll sqrtPrimePower(ll a, ll p, int k) {
    ll x = sqrtMod(((a % p) + p) % p, p);
    if (x < 0) return -1;
    ll pk = p;
    for (int j = 1; j < k; j++) {
        ll np = pk * p;
        ll fx = (((__int128)x * x - a) % np + np) % np;
        x = (ll)((((__int128)x - (__int128)fx * invMod(2 * x % np, np)) % np + np) % np);
        pk = np;
    }
    return x;                                            // the other root is p^k - x
}
// one root of x^2 = a (mod 2^k), a ODD. -1 if there is none.
ll sqrtPow2(ll a, int k) {
    a &= (1LL << k) - 1;
    if (k == 1) return 1;                                // a odd => x = 1
    if (k == 2) return (a & 3) == 1 ? 1 : -1;
    if ((a & 7) != 1) return -1;                         // a = 1 (mod 8) is necessary for k >= 3
    ll x = 1;
    for (int j = 3; j < k; j++) {                        // lift 2^j -> 2^(j+1); pick the right one
        ll nm = 1LL << (j + 1);
        if ((((__int128)x * x - a) % nm + nm) % nm) x += 1LL << (j - 1);
    }
    return x;
}
// NUMBER OF SOLUTIONS of x^2 = a (mod p^k) with gcd(a, p) = 1:
//   p odd: 2 if a is a residue mod p, else 0.  The roots are +-x.
//   p = 2: k = 1 -> 1 root; k = 2 -> 2 roots (a = 1 mod 4); k >= 3 -> 4 roots (a = 1 mod 8),
//          namely +-x and +-x + 2^(k-1).
// COMPOSITE m: factor m = prod p_i^k_i, solve each, and CRT every COMBINATION of signs - the
// solution count is the product of the per-prime-power counts, so it can be 2^omega(m) many.
// THE SPECIAL CASE a = 1 is worth memorising: the number of x with x^2 = 1 (mod m) is
//   prod over odd p^k || m of 2, times (1 if 2||m is 2^1, 2 if 4||m, 4 if 8|m).
ll numSqrt1(ll m) {                                      // count of x in [0, m) with x^2 = 1
    ll res = 1;
    for (auto [p, e] : factorize(m))
        res *= (p != 2 ? 2 : (e == 1 ? 1 : e == 2 ? 2 : 4));
    return res;
}
// USES: counting self-inverse elements, "how many x with x^2 = 1" in group-theory counting,
// solving quadratic congruences ax^2 + bx + c = 0 (complete the square: (2ax + b)^2 = b^2 - 4ac,
// needs gcd(2a, m) = 1), and lifting solutions of ANY polynomial f - the same Newton step works
// whenever gcd(f'(x0), p) = 1.
