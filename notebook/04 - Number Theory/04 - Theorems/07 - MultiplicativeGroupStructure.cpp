// Needs: factorize (02 - Primes and Divisors/06 - MillerRabinPollard.cpp).
// THE STRUCTURE OF Z_n^* (the units mod n) - the facts that decide whether a modular equation
// has solutions, how many, and what the true exponent of the group is.
//   Z_n^* = product over p^e || n of Z_(p^e)^*, and
//     p odd : Z_(p^e)^* is CYCLIC of order phi(p^e) = p^(e-1)(p-1)      -> primitive root exists
//     p = 2 : Z_2^* trivial, Z_4^* cyclic of order 2, and for e >= 3
//             Z_(2^e)^* = Z_2 x Z_(2^(e-2))  -> NOT cyclic, no primitive root
//   A PRIMITIVE ROOT mod n exists iff n = 1, 2, 4, p^e or 2*p^e for an odd prime p.
// CARMICHAEL LAMBDA = the true exponent of Z_n^*: the smallest L with a^L = 1 for every unit a.
// It divides phi(n) and is often much smaller - use it (not phi) to reduce exponents, and as the
// modulus bound in power towers.
ll carmichael(ll n) {
    ll res = 1;
    for (auto [p, e] : factorize(n)) {
        ll pe = 1;
        for (int i = 0; i < e; i++) pe *= p;
        ll l = pe / p * (p - 1);                        // phi(p^e)
        if (p == 2 && e >= 3) l /= 2;                   // the one exception
        res = res / gcd(res, l) * l;                    // lcm
    }
    return res;
}
// HOW MANY DISTINCT VALUES DOES a^k mod n TAKE, over ALL a in [0, n)? (Not just the units - the
// non-units contribute the recursive term, which is what makes this more than |Z_n^*| / gcd.)
// Multiplicative in n, so handle each prime power and multiply.
ll ipow(ll b, ll e) { ll r = 1; while (e-- > 0) r *= b; return r; }
ll kthPowersPrimePower(ll p, ll e, ll k) {              // count of distinct a^k mod p^e
    if (e <= 0 || k == 0) return 1;
    if (p == 2) {
        if (e == 1) return 2;
        ll u = ipow(2, e - 2) / gcd(k, ipow(2, e - 2));
        if (k % 2) u *= 2;
        return u + kthPowersPrimePower(2, e - k, k);
    }
    ll ph = ipow(p, e) - ipow(p, e - 1);
    return ph / gcd(k, ph) + kthPowersPrimePower(p, e - k, k);
}
ll kthPowersMod(ll k, ll n) {                           // #distinct a^k mod n over all a
    ll res = 1;
    for (auto [p, e] : factorize(n)) res *= kthPowersPrimePower(p, e, k);
    return res;
}
// COMPANION FACTS (all about x^k = a mod p, p prime, a != 0):
//   the k-th powers form the subgroup of index d = gcd(k, p-1), so there are (p-1)/d of them;
//   a is a k-th power iff a^((p-1)/d) = 1, and then it has exactly d roots.
//   For a MODULUS n, x^k = a is solved prime-power by prime-power and glued with CRT.
// ORDER OF AN ELEMENT: ord(a) | lambda(n); compute it by starting from lambda(n) and dividing
// out prime factors while a^(L/q) = 1 (that is multOrder in 03 - Modular Arithmetic/02).
// EXPONENT REDUCTION: a^e = a^(e mod lambda(n) + lambda(n)) (mod n) for e >= log2(n), which is
// the correct form even when gcd(a, n) > 1 - the same guard the power tower uses.
