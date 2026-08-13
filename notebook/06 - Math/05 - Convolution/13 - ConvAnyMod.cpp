// Needs: npow (02 - NTT.cpp)  -- self-contained otherwise: it carries its own modulus-parametric
// NTT, because 02 - NTT.cpp hardcodes NMOD.
// CONVOLUTION MODULO AN ARBITRARY M (composite, not NTT-friendly): run the NTT under THREE
// NTT-friendly primes and recombine with Garner. O(n log n) with a constant of three transforms.
// The true coefficients are bounded by len * (M-1)^2; the three primes below multiply to ~9.6e25,
// so for M <= 1e9 and len <= 1e7 the CRT reconstruction is EXACT, and only then reduced mod M.
// PREFER THIS over convMod in 01 - FFT.cpp when M is large (> ~1e9), when the arrays are long
// (>= 1e6, where the double-based split loses precision), or when you do not want to reason about
// floating point at all. It is exact by construction; the FFT version is not.
const ll CP[3] = {167772161, 469762049, 1224736769};         // all c*2^k + 1, primitive root 3
void nttP(vector<ll>& a, bool inv, ll p) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (int len = 2; len <= n; len <<= 1) {
        ll w = npow(3, (p - 1) / len, p);
        if (inv) w = npow(w, p - 2, p);
        for (int i = 0; i < n; i += len) {
            ll cur = 1;
            for (int j = 0; j < len / 2; j++) {
                ll u = a[i + j], v = a[i + j + len / 2] * cur % p;
                a[i + j] = (u + v) % p, a[i + j + len / 2] = (u - v + p) % p;
                cur = cur * w % p;
            }
        }
    }
    if (inv) { ll ni = npow(n, p - 2, p); for (ll& x : a) x = x * ni % p; }
}
vector<ll> convP(const vector<ll>& a, const vector<ll>& b, ll p) {
    int rs = a.size() + b.size() - 1, n = 1;
    while (n < rs) n <<= 1;
    vector<ll> x(n, 0), y(n, 0);
    for (size_t i = 0; i < a.size(); i++) x[i] = (a[i] % p + p) % p;
    for (size_t i = 0; i < b.size(); i++) y[i] = (b[i] % p + p) % p;
    nttP(x, false, p), nttP(y, false, p);
    for (int i = 0; i < n; i++) x[i] = x[i] * y[i] % p;
    nttP(x, true, p);
    x.resize(rs);
    return x;
}
vector<ll> convAnyMod(const vector<ll>& a, const vector<ll>& b, ll M) {
    if (a.empty() || b.empty()) return {};
    vector<ll> r0 = convP(a, b, CP[0]), r1 = convP(a, b, CP[1]), r2 = convP(a, b, CP[2]);
    ll i01 = npow(CP[0] % CP[1], CP[1] - 2, CP[1]);
    ll i02 = npow(CP[0] % CP[2], CP[2] - 2, CP[2]);
    ll i12 = npow(CP[1] % CP[2], CP[2] - 2, CP[2]);
    vector<ll> res(r0.size());
    for (size_t i = 0; i < r0.size(); i++) {                 // Garner: mixed-radix digits x0,x1,x2
        ll x0 = r0[i];
        ll x1 = (r1[i] - x0 % CP[1] + CP[1]) % CP[1] * i01 % CP[1];
        ll t = (r2[i] - x0 % CP[2] + CP[2]) % CP[2] * i02 % CP[2];
        ll x2 = (t - x1 % CP[2] + CP[2]) % CP[2] * i12 % CP[2];
        __int128 v = (__int128)x2 * CP[1] % M * CP[0] % M + (__int128)x1 * CP[0] % M + x0;
        res[i] = (ll)(v % M);
    }
    return res;
}
// PRIME MENU (all with primitive root 3; the 2-power in p-1 caps the transform length):
//   167772161 = 5*2^25 + 1    469762049 = 7*2^26 + 1     754974721 = 45*2^24 + 1
//   998244353 = 119*2^23 + 1  1004535809 = 479*2^21 + 1  1224736769 = 73*2^24 + 1
// TWO PRIMES are enough only when len * (M-1)^2 < P0 * P1 (~7.9e16) - i.e. tiny M. Use three.
// SPLIT-COEFFICIENT ALTERNATIVE: convMod in 01 - FFT.cpp writes each coefficient as hi*2^15 + lo
// and does 3 complex FFTs - shorter, but its error grows with len and with M.
// EXACT INTEGER convolution with no modulus: pass an M larger than any possible coefficient, or
// drop the final reduction and reconstruct into __int128.
