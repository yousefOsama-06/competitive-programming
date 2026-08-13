// RATIONAL RECONSTRUCTION - recover the fraction p/q from its value r = p * q^-1 (mod m).
// The answer is UNIQUE as long as |p| and q are both below sqrt(m/2), and it is found by running
// the extended Euclidean algorithm on (m, r) and stopping halfway: the remainder is p and the
// cofactor is q. O(log m).
// WHY YOU WANT IT: do a whole computation in modular arithmetic (Gaussian elimination, a DP over
// fractions, an interpolation, a determinant) and then read off the EXACT rational answer -
// no bignum fractions, no gcd-ing at every step. Also the standard way to guess a closed form
// from modular data, and to answer "print the answer as an irreducible fraction" when the
// intermediate values would explode.
// PRECONDITIONS: m >= 2, gcd(q, m) = 1 (implicit in r existing). Returns {0, 0} on failure -
// which means no such small fraction exists, so take a bigger m (or several and CRT them).
pair<ll, ll> ratRecon(ll r, ll m) {
    ll a = m, b = ((r % m) + m) % m, x = 0, y = 1;      // invariant: b = y * r (mod m)
    ll lim = (ll)sqrtl((ld)m / 2);
    while (b > lim) {
        ll q = a / b;
        ll t = a - q * b; a = b, b = t;
        t = x - q * y; x = y, y = t;
    }
    if (y < 0) b = -b, y = -y;
    if (y == 0 || gcd(b < 0 ? -b : b, y) != 1) return {0, 0};
    return {b, y};                                      // b / y = r (mod m), in lowest terms
}
// CHECK YOUR RESULT: verify b * inverse(y) mod m == r before trusting it - the reconstruction can
// return a wrong small fraction if the true one is not small enough. Verifying is O(log m).
// LARGER BOUNDS: if you know |p| <= P and q <= Q with 2*P*Q < m, stop the loop at b <= P instead
// and the same argument applies. Choosing m as a product of several NTT primes (Garner) is the
// usual way to make m big enough.
// RELATED: the same halfway-Euclid stopping rule is the Berlekamp-Massey / Pade approximant of a
// power series, and 01 - ContinuedFractions.cpp is the exact-arithmetic version of this walk.
