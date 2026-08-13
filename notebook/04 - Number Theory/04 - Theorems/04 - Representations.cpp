// Needs: powmod, factorize (02 - Primes and Divisors/06 - MillerRabinPollard.cpp).
// Representing integers: sums of two squares, Pythagorean triples, Legendre symbol.
// Needs factorize / sqrtMod / isPrime.

int legendre(ll a, ll p) {                                // 1 QR, -1 non-residue, 0 if p | a
    a %= p; if (a < 0) a += p;
    if (!a) return 0;
    return powmod(a, (p - 1) / 2, p) == 1 ? 1 : -1;
}
// n is a sum of two squares  <=>  every prime p = 3 (mod 4) appears to an EVEN power.
bool isSumOfTwoSquares(ll n) {
    if (n <= 0) return n == 0;
    for (auto [p, e] : factorize(n)) if (p % 4 == 3 && (e & 1)) return false;
    return true;
}
// One representation n = x^2 + y^2 (Cornacchia). Returns {-1,-1} if none exists.
pair<ll, ll> sumOfTwoSquares(ll n) {
    if (n == 0) return {0, 0};
    ll t = n, pw2 = 1;
    while (t % 4 == 0) t /= 4, pw2 *= 2;                  // pull out squares of 2
    if (!isSumOfTwoSquares(t)) return {-1, -1};
    // Gaussian-integer product over the prime factorisation
    ll xr = 1, yr = 0;                                    // current value as xr + yr*i
    for (auto [p, e] : factorize(t)) {
        if (p == 2) { for (int i = 0; i < e; i++) { ll nx = xr - yr, ny = xr + yr; xr = nx, yr = ny; } continue; }
        if (p % 4 == 3) { ll k = 1; for (int i = 0; i < e / 2; i++) k *= p; xr *= k, yr *= k; continue; }
        ll r = sqrtMod(p - 1, p);                         // r^2 = -1 (mod p)
        ll a = p, b = r;
        while (b * b > p) { ll c = a % b; a = b, b = c; } // Cornacchia descent: p = b^2 + c^2
        ll c = (ll)sqrtl((ld)(p - b * b));
        while (c * c < p - b * b) c++;
        while (c * c > p - b * b) c--;
        for (int i = 0; i < e; i++) { ll nx = xr * b - yr * c, ny = xr * c + yr * b; xr = nx, yr = ny; }
    }
    return {llabs(xr) * pw2, llabs(yr) * pw2};
}
// #representations of n as an ORDERED pair of squares (with signs) = 4 * (d_1(n) - d_3(n)),
// where d_i counts divisors = i (mod 4).   [Jacobi's two-square theorem]

// PRIMITIVE Pythagorean triples with hypotenuse <= L:  (m^2-k^2, 2mk, m^2+k^2),
// m > k > 0, gcd(m,k) = 1, m-k odd. Every triple is a multiple of exactly one primitive triple.
vector<array<ll, 3>> pythagorean(ll L) {
    vector<array<ll, 3>> r;
    for (ll m = 2; m * m <= L; m++)
        for (ll k = 1 + (m % 2); k < m; k += 2)          // m-k must be ODD => opposite parity
            if (gcd(m, k) == 1 && m * m + k * k <= L)
                r.push_back({m * m - k * k, 2 * m * k, m * m + k * k});
    return r;
}
// n is a sum of THREE squares  <=>  n != 4^a * (8b + 7).   Every n is a sum of FOUR squares.
