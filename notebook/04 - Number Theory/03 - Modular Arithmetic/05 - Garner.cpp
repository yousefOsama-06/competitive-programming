// GARNER'S ALGORITHM - reconstruct x from residues modulo PAIRWISE COPRIME m_i, in mixed-radix
// form, so you can emit x modulo a target MOD without ever building prod(m_i).
// This is what CRT cannot do once the product overflows 64 bits.
//   x = x1 + x2*m1 + x3*m1*m2 + ... ,   0 <= xi < mi
ll garner(vector<ll> a, vector<ll> m, ll MODOUT) {
    int k = a.size();
    vector<ll> x(k);
    auto inv = [](ll a, ll M) {                       // requires gcd(a, M) = 1
        ll g = M, X = 0, x1 = 1, A = ((a % M) + M) % M;
        while (A) { ll q = g / A; g -= q * A, swap(g, A); X -= q * x1, swap(X, x1); }
        return (X % M + M) % M;
    };
    for (int i = 0; i < k; i++) {
        x[i] = ((a[i] % m[i]) + m[i]) % m[i];
        ll mul = 1;
        for (int j = 0; j < i; j++) {
            x[i] = (x[i] - x[j] % m[i] * mul) % m[i];
            mul = mul * (m[j] % m[i]) % m[i];
        }
        x[i] = (x[i] % m[i] + m[i]) % m[i] * inv(mul, m[i]) % m[i];
    }
    ll res = 0, mul = 1;
    for (int i = 0; i < k; i++) {
        res = (res + x[i] % MODOUT * mul) % MODOUT;
        mul = mul * (m[i] % MODOUT) % MODOUT;
    }
    return (res % MODOUT + MODOUT) % MODOUT;
}
// MAIN USE: arbitrary-modulus convolution. Run NTT under three NTT primes, then Garner the
// three residues down to the target modulus.
//   NTT PRIMES:  998244353 = 119*2^23 + 1  (g=3)     167772161 =   5*2^25 + 1  (g=3)
//                469762049 =   7*2^26 + 1  (g=3)    1004535809 = 479*2^21 + 1  (g=3)
//                754974721 =  45*2^24 + 1  (g=11)   <- note: NOT 9*2^23+1, a common misprint
// The 3-prime product 5.95e25 covers n * (MOD-1)^2 for any MOD < 2^30 and n up to ~5e7.
