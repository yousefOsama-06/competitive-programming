// FIBONACCI, FAST. The notebook had the identities on a formula sheet and no routine; this is it.
// F(0)=0, F(1)=1. Fast doubling is O(log n) with two multiplications per bit and NO matrices -
// shorter and ~2x faster than 2x2 matrix exponentiation.
//   F(2k)   = F(k) * (2*F(k+1) - F(k))
//   F(2k+1) = F(k)^2 + F(k+1)^2
pair<ll, ll> fibPair(ll n, ll mod) {                           // returns {F(n), F(n+1)} mod
    if (!n) return {0, 1 % mod};
    auto [a, b] = fibPair(n >> 1, mod);
    ll c = a * ((2 * b % mod - a % mod + mod) % mod) % mod;     // F(2k)
    ll d = (a * a + b * b) % mod;                               // F(2k+1)
    return (n & 1) ? make_pair(d, (c + d) % mod) : make_pair(c, d);
}
ll fib(ll n, ll mod) { return fibPair(n, mod).first; }
// LUCAS NUMBERS L(n) = F(n-1) + F(n+1) = 2F(n+1) - F(n).
// NEGATIVE index: F(-n) = (-1)^(n+1) F(n).
// The mod must be < ~3e9 for the ll products above, or use __int128 / Mint.

// --- PISANO PERIOD pi(m): the period of F(n) mod m. Needed to reduce a huge index. ---
// pi(1) = 1, pi(2) = 3, and pi(m) is EVEN for every m > 2.
// pi(lcm(a,b)) = lcm(pi(a), pi(b))      pi(p^k) = p^(k-1) * pi(p)
// pi(m) <= 6m, with equality exactly at m = 2 * 5^k.
// p = +-1 (mod 5) => pi(p) divides p-1;   p = +-2 (mod 5) => pi(p) divides 2(p+1).
// So: factor m, get pi for each prime power by testing the divisors of that bound, then lcm.
ll pisanoPrime(ll p) {                                         // p prime
    if (p == 2) return 3;
    if (p == 5) return 20;
    ll cand = (p % 5 == 1 || p % 5 == 4) ? p - 1 : 2 * (p + 1);
    vector<ll> divs;
    for (ll d = 1; d * d <= cand; d++)
        if (cand % d == 0) divs.push_back(d), divs.push_back(cand / d);
    sort(all(divs));
    for (ll d : divs) {
        auto [a, b] = fibPair(d, p);
        if (a == 0 && b == 1 % p) return d;
    }
    return cand;
}
// Full pi(m): factorise m (Pollard rho), take pisanoPrime(p) * p^(e-1) per prime power, lcm them.
/* MORE FIBONACCI FACTS worth the page
   gcd(F_m, F_n) = F_gcd(m,n)      =>   F_m | F_n  iff  m | n   (m >= 3)
   Cassini:   F_{n-1} F_{n+1} - F_n^2 = (-1)^n
   Catalan:   F_n^2 - F_{n-r} F_{n+r} = (-1)^{n-r} F_r^2
   d'Ocagne:  F_m F_{n+1} - F_{m+1} F_n = (-1)^n F_{m-n}
   sum_{i<=n} F_i = F_{n+2} - 1        sum_{i<=n} F_i^2 = F_n F_{n+1}
   sum_{i<=n} F_{2i-1} = F_{2n}        sum_{i<=n} F_{2i} = F_{2n+1} - 1
   F_n = (phi^n - psi^n)/sqrt5;  F_92 is the last one fitting in a signed 64-bit integer.
   ZECKENDORF: every n >= 1 is a UNIQUE sum of NON-CONSECUTIVE Fibonacci numbers, found greedily
     from the largest. This is the "Fibonacci base", and it is what makes Fibonacci-nim work.
   LCM of F_1..F_n and "which F are divisible by k" both follow from the gcd identity.
   Fibonacci mod p when 5 is a QR mod p: sqrt(5) exists, so the closed form works in F_p directly.
     When it is not, work in F_p[x]/(x^2-5) (a "Phi field") - two components, same O(log n). */

// --- TRIBONACCI / any linear recurrence: matrix exponentiation (06 - Math/03), or Kitamasa /
// Bostan-Mori for a long recurrence. Berlekamp-Massey RECOVERS the recurrence from ~2k terms.
// --- LUCAS SEQUENCES U_n, V_n with U_{n+1} = P U_n - Q U_{n-1} generalise all of this and have
// the same doubling identities; they are what Baillie-PSW primality is built on.
