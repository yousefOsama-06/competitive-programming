// Needs: MultSieve (04 - NT/02 - Primes and Divisors/07 - MultiplicativeSieve.cpp) for mu/phi.
// Working code for the identities on the formula sheet. Needs MultSieve (mu, phi).

// #pairs (i,j) with 1<=i,j<=n and gcd(i,j)=1        O(n)
ll coprimePairs(int n, const MultSieve& S) {
    ll r = 0;
    for (int d = 1; d <= n; d++) r += (ll)S.mu[d] * (n / d) * (n / d);
    return r;
}
// sum over all pairs of gcd(i,j)                    O(n)
ll gcdSum(int n, const MultSieve& S) {
    ll r = 0;
    for (int d = 1; d <= n; d++) r += (ll)S.phi[d] * (n / d) * (n / d);
    return r;
}
// General:  sum_{i,j<=n} f(gcd(i,j))  =  sum_d (f*mu)(d) * floor(n/d)^2
// Build g = f*mu once by sieving over multiples:  for d: for m=d,2d,..: g[m] += mu[m/d]*f[d]
vector<ll> dirichletMu(const vector<ll>& f, const MultSieve& S) {   // g = f * mu   O(n log n)
    int n = f.size() - 1;
    vector<ll> g(n + 1, 0);
    for (int d = 1; d <= n; d++) if (f[d])
        for (int m = d; m <= n; m += d) g[m] += (ll)S.mu[m / d] * f[d];
    return g;
}

// DIVISOR BLOCKING: sum_{i=1..n} floor(n/i) in O(sqrt n). Same skeleton for sum tau / sum sigma.
ll sumFloorDiv(ll n) {
    ll r = 0;
    for (ll l = 1, rr; l <= n; l = rr + 1) { rr = n / (n / l); r += (n / l) * (rr - l + 1); }
    return r;
}
// sum_{i=1..n} tau(i) = sum_{i=1..n} floor(n/i)      (count multiples)
// sum_{i=1..n} sigma(i) = sum_{i=1..n} i * floor(n/i)

// COUNT MULTIPLES / INCLUSION-EXCLUSION OVER DIVISORS:
// "how many i<=n are divisible by at least one of p1..pk" -> IE over the 2^k squarefree products,
// sign = mu of the product. For k up to ~20 enumerate submasks; beyond that, sieve mu.

// sum_{i=1..n} gcd(i, n) = sum_{d|n} d * phi(n/d)
ll gcdSumWithN(ll n) {
    ll r = 0;
    for (auto d : divisors(n)) r += (ll)d * phi(n / d);
    return r;
}
// sum_{i=1..n} lcm(i, n) = n/2 * (1 + sum_{d|n} d*phi(d))
ll lcmSumWithN(ll n) {
    ll s = 1;
    for (auto d : divisors(n)) s += (ll)d * phi(d);
    return n / 2 * s + (n % 2 ? s / 2 : 0);
}
