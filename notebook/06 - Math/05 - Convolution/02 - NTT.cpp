// NTT modulo 998244353 (= 119*2^23 + 1, primitive root 3). O(n log n).
// multiply() is the entry point. MAX LENGTH 2^23 for 998244353 (see the assert in ntt()).
// For an ARBITRARY modulus: run this under three of the primes below and Garner them together
// (06 - Garner.cpp is written for exactly that), or split the coefficients at sqrt(M) and use
// convMod in 01 - FFT.cpp.
const ll NMOD = 998244353, NROOT = 3;
ll npow(ll a, ll e, ll m = NMOD) { ll r = 1; a %= m; for (; e; e >>= 1, a = a * a % m) if (e & 1) r = r * a % m; return r; }

void ntt(vector<ll>& a, bool inv) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        assert((NMOD - 1) % len == 0);   // 998244352 = 119*2^23: len > 2^23 truncates the division
        ll w = npow(NROOT, (NMOD - 1) / len);   // and returns GARBAGE with no other symptom
        if (inv) w = npow(w, NMOD - 2);
        for (int i = 0; i < n; i += len) {
            ll cur = 1;
            for (int j = 0; j < len / 2; j++) {
                ll u = a[i + j], v = a[i + j + len / 2] * cur % NMOD;
                a[i + j] = (u + v) % NMOD;
                a[i + j + len / 2] = (u - v + NMOD) % NMOD;
                cur = cur * w % NMOD;
            }
        }
    }
    if (inv) { ll ninv = npow(n, NMOD - 2); for (ll& x : a) x = x * ninv % NMOD; }
}
vector<ll> multiply(vector<ll> a, vector<ll> b) {
    if (a.empty() || b.empty()) return {};
    int rs = a.size() + b.size() - 1, n = 1;
    while (n < rs) n <<= 1;
    a.resize(n), b.resize(n);
    ntt(a, false), ntt(b, false);
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % NMOD;
    ntt(a, true);
    a.resize(rs);
    return a;
}
// ARBITRARY MODULUS: split each coefficient as a = hi*2^15 + lo and combine three products
// (or run NTT under three NTT-friendly primes and CRT). Values must stay below ~1e18.
// COMMON NTT PRIMES: 998244353 (g=3), 167772161 (g=3), 469762049 (g=3), 1004535809 (g=3).
// USES: polynomial multiplication; "count pairs/triples with a given sum"; convolving
// distributions; string matching with wildcards; sum of C(a_i, k) over all i for every k.
