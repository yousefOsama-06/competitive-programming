// Modular integer (MOD must be PRIME for inv/division) + O(N) factorial tables.
struct Mint {
    int v;
    Mint(ll x = 0) { v = int(x % MOD); if (v < 0) v += MOD; }
    Mint& operator+=(Mint o) { if ((v += o.v) >= MOD) v -= MOD; return *this; }
    Mint& operator-=(Mint o) { if ((v -= o.v) < 0) v += MOD; return *this; }
    Mint& operator*=(Mint o) { v = int(1LL * v * o.v % MOD); return *this; }
    Mint& operator/=(Mint o) { return *this *= o.inv(); }
    friend Mint operator+(Mint a, Mint b) { return a += b; }
    friend Mint operator-(Mint a, Mint b) { return a -= b; }
    friend Mint operator*(Mint a, Mint b) { return a *= b; }
    friend Mint operator/(Mint a, Mint b) { return a /= b; }
    friend bool operator==(Mint a, Mint b) { return a.v == b.v; }
    Mint pow(ll e) const { Mint r = 1, b = *this; for (; e > 0; e >>= 1, b *= b) if (e & 1) r *= b; return r; }
    Mint inv() const { return pow(MOD - 2); }           // Fermat: a^(p-2) = a^-1 (mod p)
    friend ostream& operator<<(ostream& o, Mint m) { return o << m.v; }
    friend istream& operator>>(istream& i, Mint& m) { ll x; i >> x; m = Mint(x); return i; }
};

const int N = 200005;                                   // ALSO in 01 - Template.cpp: keep one
Mint fact[N], invFact[N], inv_[N];
void buildFact() {                                      // O(N) total, not O(N log MOD)
    fact[0] = invFact[0] = inv_[1] = 1;
    for (int i = 1; i < N; i++) fact[i] = fact[i - 1] * i;
    invFact[N - 1] = fact[N - 1].inv();
    for (int i = N - 1; i > 0; i--) invFact[i - 1] = invFact[i] * i;
    for (int i = 2; i < N; i++) inv_[i] = Mint(MOD - MOD / i) * inv_[MOD % i];
}
Mint C(ll n, ll r) { if (r < 0 || r > n) return 0; return fact[n] * invFact[r] * invFact[n - r]; }
Mint Perm(ll n, ll r) { if (r < 0 || r > n) return 0; return fact[n] * invFact[n - r]; }
// C(n, r) for HUGE n and small r (no tables): prod_{i<r}(n-i) / r!
Mint Cbig(ll n, ll r) {
    if (r < 0 || n < 0 || r > n) return 0;
    Mint num = 1;
    for (ll i = 0; i < r; i++) num *= Mint(n - i);
    return num * invFact[r];
}
