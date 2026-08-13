// Needs: powmod (02 - Primes and Divisors/06 - MillerRabinPollard.cpp).
// Discrete log, modular square root, primitive root, discrete k-th root, multiplicative order.
// All need mulmod/powmod from MillerRabinPollard.

// BSGS: smallest x >= 0 with a^x = b (mod m). Works for ANY m (handles gcd(a,m) != 1). O(sqrt m).
ll discreteLog(ll a, ll b, ll m) {
    a %= m, b %= m;
    ll k = 1, add = 0, g;
    while ((g = gcd(a, m)) > 1) {                     // strip common factors
        if (b == k) return add;
        if (b % g) return -1;
        b /= g, m /= g, add++, k = k * (a / g) % m;
    }
    ll n = (ll)sqrtl((ld)m) + 1, an = 1;
    for (ll i = 0; i < n; i++) an = an * a % m;
    unordered_map<ll, ll> vals;
    for (ll q = 0, cur = b; q <= n; q++) vals[cur] = q, cur = cur * a % m;
    for (ll p = 1, cur = k; p <= n; p++) {
        cur = cur * an % m;
        if (vals.count(cur)) { ll ans = n * p - vals[cur] + add; if (ans >= 0) return ans; }
    }
    return -1;
}

// Tonelli-Shanks: x with x^2 = a (mod p), p odd prime. Returns -1 if a is not a QR.
ll sqrtMod(ll a, ll p) {
    a %= p; if (a < 0) a += p;
    if (a == 0) return 0;
    if (powmod(a, (p - 1) / 2, p) != 1) return -1;      // Euler's criterion
    if (p % 4 == 3) return powmod(a, (p + 1) / 4, p);
    ll s = p - 1, n = 2; int r = 0;
    while (s % 2 == 0) s /= 2, r++;
    while (powmod(n, (p - 1) / 2, p) != p - 1) n++;     // find a non-residue
    ll x = powmod(a, (s + 1) / 2, p), b = powmod(a, s, p), g = powmod(n, s, p);
    for (int m = r;; m = r) {
        ll t = b;
        for (r = 0; r < m && t != 1; r++) t = t * t % p;
        if (r == 0) return x;
        ll gs = powmod(g, 1LL << (m - r - 1), p);
        g = gs * gs % p, x = x * gs % p, b = b * g % p;
    }
}

// Smallest primitive root of a PRIME p (generator of Z_p^*).
ll primitiveRoot(ll p) {
    if (p == 2) return 1;
    auto f = factorize(p - 1);
    for (ll g = 2; g < p; g++) {
        bool ok = true;
        for (auto [q, e] : f) if (powmod(g, (p - 1) / q, p) == 1) { ok = false; break; }
        if (ok) return g;
    }
    return -1;
}

// Multiplicative order of a mod m (smallest e > 0 with a^e = 1). Requires gcd(a, m) = 1.
ll multOrder(ll a, ll m) {
    ll e = phi(m), r = e;
    for (auto [q, k] : factorize(e))
        while (r % q == 0 && powmod(a, r / q, m) == 1) r /= q;
    return r;
}

// All x with x^k = a (mod p), p prime. Reduce to a discrete log in base g.
vector<ll> discreteRoot(ll k, ll a, ll p) {
    if (a == 0) return {0};
    ll g = primitiveRoot(p), y = discreteLog(powmod(g, k, p), a, p);
    if (y < 0) return {};
    ll x0 = powmod(g, y, p), d = gcd(k, p - 1), step = powmod(g, (p - 1) / d, p);
    vector<ll> r;
    for (ll i = 0, cur = x0; i < d; i++, cur = cur * step % p) r.push_back(cur);
    sort(all(r)), r.erase(unique(all(r)), r.end());
    return r;
}
