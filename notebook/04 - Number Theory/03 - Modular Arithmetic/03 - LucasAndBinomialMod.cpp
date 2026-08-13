// Needs: Mint (06 - Math/01), and factorize (02 - Primes and Divisors/06) for the last section.
// C(n, r) mod m for HUGE n.  Pick by what m is:
//   m prime and small           -> lucas
//   m = p^e                     -> binomPrimePower (Andrew Granville / Wilson generalisation)
//   m arbitrary (squarefree-ish)-> binomAnyMod (CRT over prime powers)

// LUCAS. p prime, needs fact/invFact built up to p. C(n,r) = prod C(n_i, r_i) over base-p digits.
Mint lucas(ll n, ll r) {
    if (r < 0 || r > n) return 0;
    Mint res = 1;
    while (n || r) {
        ll a = n % MOD, b = r % MOD;
        if (b > a) return 0;
        res *= fact[a] * invFact[b] * invFact[a - b];
        n /= MOD, r /= MOD;
    }
    return res;
}

// --- arbitrary modulus ---
ll pw(ll a, ll e, ll m) { ll r = 1; a %= m; for (; e; e >>= 1, a = a * a % m) if (e & 1) r = r * a % m; return r; }
ll inv_mod(ll a, ll m) {                                 // requires gcd(a, m) = 1
    ll g = m, x = 0, x1 = 1, a1 = a % m;
    while (a1) { ll q = g / a1; g -= q * a1, swap(g, a1); x -= q * x1, swap(x, x1); }
    return (x % m + m) % m;
}
// n! with all factors of p removed, mod p^e.  O(p^e + log n).
ll factNoP(ll n, ll p, ll pe) {
    if (n == 0) return 1;
    vector<ll> f(pe);
    f[0] = 1;
    for (ll i = 1; i < pe; i++) f[i] = (i % p ? f[i - 1] * i : f[i - 1]) % pe;
    ll r = 1;
    for (ll m = n; m; m /= p) {
        r = r * pw(f[pe - 1], m / pe, pe) % pe;
        r = r * f[m % pe] % pe;
    }
    return r;
}
// C(n, r) mod p^e
ll binomPrimePower(ll n, ll r, ll p, ll pe) {
    if (r < 0 || r > n) return 0;
    ll k = 0;                                            // exponent of p in C(n,r) (Kummer's theorem:
    for (ll m = n / p, a = r / p, b = (n - r) / p; m; m /= p, a /= p, b /= p) k += m - a - b;
    if (k >= 60 || pw(p, k, pe * p) % pe == 0) { ll t = pw(p, k, pe); if (t == 0) return 0; }
    ll num = factNoP(n, p, pe), d1 = factNoP(r, p, pe), d2 = factNoP(n - r, p, pe);
    ll res = num * inv_mod(d1, pe) % pe * inv_mod(d2, pe) % pe;
    return res * pw(p, k, pe) % pe;
}
// C(n, r) mod m for arbitrary m: factor m, solve mod each p^e, CRT.
ll binomAnyMod(ll n, ll r, ll m) {
    ll res = 0, M = m;
    for (auto [p, e] : factorize(m)) {
        ll pe = 1; for (int i = 0; i < e; i++) pe *= p;
        ll c = binomPrimePower(n, r, p, pe), t = M / pe;
        res = (res + c % pe * t % M * inv_mod(t % pe, pe)) % M;
    }
    return res;
}
// KUMMER'S THEOREM: the exponent of prime p in C(n,r) = number of CARRIES when adding
//                   r and n-r in base p.  (Used above to pull out the p-part.)
// WILSON: (p-1)! = -1 (mod p) for prime p.
