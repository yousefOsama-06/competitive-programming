// Needs: powmod, phi (04 - NT/02 - Primes and Divisors/06 - MillerRabinPollard.cpp).
// Linear congruences and power towers. Needs powmod / phi / gcd.

// Solve a*x = b (mod m). Returns all solutions in [0, m) (there are gcd(a,m) of them, or none).
vector<ll> linCongruence(ll a, ll b, ll m) {
    a = ((a % m) + m) % m, b = ((b % m) + m) % m;
    ll g = gcd(a, m);
    if (b % g) return {};
    ll m2 = m / g, a2 = a / g, b2 = b / g;
    // a2 is invertible mod m2
    ll x0 = 0, x1 = 1, g2 = m2, a3 = a2 % m2;
    while (a3) { ll q = g2 / a3; g2 -= q * a3, swap(g2, a3); x0 -= q * x1, swap(x0, x1); }
    ll inv = ((x0 % m2) + m2) % m2, x = b2 % m2 * inv % m2;
    vector<ll> r;
    for (ll i = 0; i < g; i++) r.push_back((x + i * m2) % m);
    return r;
}

// GENERALISED EULER:  a^k = a^(k mod phi(m) + phi(m))  (mod m)  whenever k >= log2(m).
// This is what makes power towers computable even when gcd(a, m) != 1.
ll capTower(const vector<ll>& a, int i, ll CAP = 64) {   // min(true tower value, CAP)
    if (i == (int)a.size()) return 1;
    if (a[i] == 0) return capTower(a, i + 1, CAP) == 0 ? 1 : 0;   // 0^0 = 1, 0^(k>0) = 0
    if (a[i] == 1) return 1;
    ll e = capTower(a, i + 1, CAP), r = 1;
    for (ll k = 0; k < e; k++) { r *= a[i]; if (r >= CAP) return CAP; }
    return r;
}
// a[i] ^ a[i+1] ^ ... ^ a[last]   (mod m), right-associative.
ll powTower(const vector<ll>& a, int i, ll m) {
    if (m == 1) return 0;
    if (i == (int)a.size()) return 1 % m;
    ll ph = phi(m), t = capTower(a, i + 1);
    ll e = t < 64 ? t : powTower(a, i + 1, ph) + ph;     // exact when tiny, +phi when large
    return powmod(a[i] % m, e, m);
}

// Extended CRT for NON-coprime moduli: x = a1 (mod m1), x = a2 (mod m2).
// Returns {x, lcm} or {-1, -1}. Chain it to merge many congruences.
pair<ll, ll> crt2(ll a1, ll m1, ll a2, ll m2) {
    ll g = gcd(m1, m2), l = m1 / g * m2;
    if ((a2 - a1) % g) return {-1, -1};
    ll m2g = m2 / g;
    ll x0 = 0, x1 = 1, gg = m2g, aa = (m1 / g) % m2g;
    while (aa) { ll q = gg / aa; gg -= q * aa, swap(gg, aa); x0 -= q * x1, swap(x0, x1); }
    ll inv = ((x0 % m2g) + m2g) % m2g;
    __int128 k = (__int128)((a2 - a1) / g % m2g) * inv % m2g;
    ll x = (ll)(((__int128)k * m1 + a1) % l);
    return {(x % l + l) % l, l};
}
