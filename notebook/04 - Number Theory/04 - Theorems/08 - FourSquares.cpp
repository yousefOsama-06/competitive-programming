// Needs: isPrime (02 - Primes and Divisors/06 - MillerRabinPollard.cpp),
//        sumOfTwoSquares (04 - Representations.cpp).
// LAGRANGE'S FOUR-SQUARE DECOMPOSITION - actually CONSTRUCT x, y, z, w with
//     n = x^2 + y^2 + z^2 + w^2
// for n up to 1e18, in expected O(log^2 n) primality tests. The existence theorem is famous; the
// construction is the part people cannot reproduce under pressure, and brute force over two of
// the four variables is O(n) - hopeless past 1e12.
// THE ALGORITHM (Rabin-Shallit, simplified):
//   1. strip factors of 4: n = 4^a * m, solve for m and multiply every answer by 2^a;
//   2. if m = 7 (mod 8) then m is NOT a sum of three squares, so spend one square: m -= 1;
//   3. now m IS a sum of three squares. Pick x at random and test whether m - x^2 is a PRIME
//      congruent to 1 mod 4 (or a trivial 0/1/2). Such x are dense, so a handful of tries suffice;
//   4. split that prime into two squares with Cornacchia (04 - Representations.cpp).
// PRECONDITION: n >= 0. Uses randomness, so it is expected-time, not worst-case.
array<ll, 4> fourSquares(ll n) {
    array<ll, 4> r{0, 0, 0, 0};
    if (n <= 0) return r;
    ll mul = 1;
    while (n % 4 == 0) n /= 4, mul *= 2;                // 4^a factors out of every square
    int idx = 0;
    if (n % 8 == 7) r[idx++] = 1, n -= 1;               // 7 mod 8 needs a fourth nonzero square
    if (n < 2000) {                                     // small: brute force, and always succeeds
        for (ll x = 0; x * x <= n && idx < 4; x++)
            for (ll y = x; x * x + y * y <= n; y++) {
                ll t = n - x * x - y * y, z = (ll)sqrtl((ld)t);
                while (z * z > t) z--;
                while ((z + 1) * (z + 1) <= t) z++;
                if (z * z == t) { r[idx++] = x, r[idx++] = y, r[idx++] = z; break; }
            }
    } else {
        mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
        ll lim = (ll)sqrtl((ld)n);
        while (lim * lim > n) lim--;
        while (true) {
            ll x = rng() % (lim + 1), m = n - x * x;
            if (m % 4 != 1 && m != 2) continue;         // want a prime = 1 (mod 4), or 2
            if (m != 2 && !isPrime(m)) continue;
            auto [u, v] = sumOfTwoSquares(m);
            if (u < 0) continue;
            r[idx++] = x, r[idx++] = u, r[idx++] = v;
            break;
        }
    }
    for (ll& v : r) v *= mul;
    return r;
}
// FACTS WORTH KNOWING
//   THREE squares suffice unless n = 4^a * (8b + 7)  (Legendre); TWO suffice iff every prime
//   3 mod 4 has an even exponent; ONE iff n is a perfect square.
//   r_4(n) = 8 * sum of the divisors of n that are not multiples of 4  (Jacobi) - so the number
//   of ORDERED signed representations is easy even though finding one is not.
//   Every n is a sum of at most 9 cubes and at most 19 fourth powers (Waring); the "at most 4
//   squares" bound is tight exactly on the numbers 4^a(8b+7).
// IF n IS ALREADY FACTORED, a deterministic route exists: multiply Hurwitz quaternions the way
// sumOfTwoSquares multiplies Gaussian integers. The randomized version above is far shorter.
