#include <bits/stdc++.h>
#pragma GCC optimize("Ofast, unroll-loops")
#ifdef ONLINE_JUDGE
    #define debug(...) ((void)0)
#else
    #include "debug.h"
#endif
using namespace std;

const int MOD = 1e9 + 7;
#define rep(aa, bb, cc) for(int aa = bb; aa < cc; aa++)
#define sz(a) (int)a.size()
using ll = long long;
#define endl '\n'
typedef complex<double> C;
typedef vector<double> vd;

void fft(vector<C>& a) {
    int n = sz(a), L = 31 - __builtin_clz(n);
    static vector<complex<long double>> R(2, 1);
    static vector<C> rt(2, 1);  // (^ 10% faster if double)
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n); rt.resize(n);
        auto x = polar(1.0L, acos(-1.0L) / k);
        rep(i,k,2*k) rt[i] = R[i] = i&1 ? R[i/2] * x : R[i/2];
    }
    vector<ll> rev(n);
    rep(i,0,n) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    rep(i,0,n) if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += 2 * k) rep(j,0,k) {
                C z = rt[j+k] * a[i+j+k]; // (25% faster if hand-rolled)
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
}

template<int M> vector<ll> convMod(const vector<ll> &a, const vector<ll> &b) {
    if (a.empty() || b.empty()) return {};
    vector<ll> res(sz(a) + sz(b) - 1);
    int B=32-__builtin_clz(sz(res)), n=1<<B, cut=int(sqrt(M));
    vector<C> L(n), R(n), outs(n), outl(n);
    rep(i,0,sz(a)) L[i] = C((int)a[i] / cut, (int)a[i] % cut);
    rep(i,0,sz(b)) R[i] = C((int)b[i] / cut, (int)b[i] % cut);
    fft(L), fft(R);
    rep(i,0,n) {
        int j = -i & (n - 1);
        outl[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
        outs[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / 1i;
    }
    fft(outl), fft(outs);
    rep(i,0,sz(res)) {
        ll av = ll(real(outl[i])+.5), cv = ll(imag(outs[i])+.5);
        ll bv = ll(imag(outl[i])+.5) + ll(real(outs[i])+.5);
        res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
    }
    return res;
}

vector<int> conv(const vector<int> &A, const vector<int> &B) {
    if (A.empty() || B.empty()) return {};
    vector<ll> a(A.begin(), A.end()), b(B.begin(), B.end());
    vector<ll> c = convMod<MOD>(a, b);
    int s = (int)c.size();
    vector<int> res(s);
    for (int i = 0; i < s; ++i) {
        ll v = c[i] % MOD;
        if (v < 0) v += MOD;
        res[i] = (int)v;
    }
    return res;
}


int fp(int b, ll p) {
    ll res = 1, base = b % MOD;
    if (base < 0) base += MOD;
    while (p > 0) {
        if (p & 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        p >>= 1;
    }
    return (int)res;
}

vector<int> poly_trim(const vector<int>& a, int sz){
    int n = min<int>(a.size(), sz);
    return vector<int>(a.begin(), a.begin() + n);
}
vector<int> poly_add(const vector<int>& a, const vector<int>& b, int sz){
    vector<int> r(sz);
    for(int i = 0; i < sz; i++){
        int va = i < (int)a.size() ? a[i] : 0;
        int vb = i < (int)b.size() ? b[i] : 0;
        int s = va + vb; if(s >= MOD) s -= MOD; r[i] = s;
    }
    return r;
}
vector<int> poly_sub(const vector<int>& a, const vector<int>& b, int sz){
    vector<int> r(sz);
    for(int i = 0; i < sz; i++){
        int va = i < (int)a.size() ? a[i] : 0;
        int vb = i < (int)b.size() ? b[i] : 0;
        int s = va - vb; if(s < 0) s += MOD; r[i] = s;
    }
    return r;
}
vector<int> poly_scalar_mul(const vector<int>& a, int scalar, int sz){
    vector<int> r(sz);
    for(int i = 0; i < sz; i++){
        int va = i < (int)a.size() ? a[i] : 0;
        r[i] = (int)(1LL * va * scalar % MOD);
    }
    return r;
}
vector<int> poly_derivative(const vector<int>& a){
    if(a.size() <= 1) return vector<int>{0};
    vector<int> d(a.size() - 1);
    for(int i = 1; i < a.size(); i++){ d[i - 1] = (int)(1LL * a[i] * (int)i % MOD); }
    return d;
}
vector<int> poly_integral(const vector<int>& a){
    int n = a.size();
    vector<int> inv(n + 1);
    inv[1] = 1;
    for(int i = 2; i <= n; i++) inv[i] = (int)(MOD - (MOD / i) * 1LL * inv[MOD % i] % MOD);
    vector<int> res(n + 1);
    res[0] = 0;
    for(int i = 0; i < n; i++) res[i + 1] = (int)(1LL * a[i] * inv[i + 1] % MOD);
    return res;
}

// F[0] must be nonzero
vector<int> poly_inv(const vector<int>& F, int m){
    assert(!F.empty());
    vector<int> G(1, fp(F[0], MOD - 2));
    int cur = 1;
    while(cur < m){
        cur <<= 1;
        vector<int> Fcut(min((int)F.size(), cur));
        for(int i = 0; i < (int)Fcut.size(); i++) Fcut[i] = F[i];
        Fcut.resize(cur);
        vector<int> GG = conv(G, G);
        GG = conv(GG, Fcut);
        G.resize(cur);
        for(int i = 0; i < cur; i++){
            int gi = i < (int)G.size() ? G[i] : 0;
            int u = i < (int)GG.size() ? GG[i] : 0;
            int val = ( (2LL * gi - u) % MOD + MOD ) % MOD;
            G[i] = val;
        }
    }
    G.resize(m);
    return G;
}
// ln F (F[0] must be 1)
vector<int> poly_ln(const vector<int>& F, int n){
    assert(!F.empty() && F[0] == 1);
    vector<int> dF = poly_derivative(F), invF = poly_inv(F, n);
    vector<int> prod = conv(dF, invF);
    prod.resize(max(0, n - 1));
    vector<int> integ = poly_integral(prod);
    integ.resize(n);
    return integ;
}
// exp(A) (A[0] must be 0)
vector<int> poly_exp(const vector<int>& A, int n){
    assert(!A.empty());
    if(A[0] != 0){
        cerr << "poly_exp requires A[0] == 0 for exact formal series. Aborting.\n";
        exit(1);
    }
    vector<int> G(1, 1); int cur = 1;
    while(cur < n){
        cur <<= 1;
        vector<int> Gcut = poly_trim(G, cur);
        if((int)Gcut.size() < cur) Gcut.resize(cur, 0);
        vector<int> lnG = poly_ln(Gcut, cur);
        vector<int> Acut = poly_trim(A, cur);
        Acut.resize(cur); vector<int> diff(cur);
        for(int i = 0; i < cur; i++){
            int ai = i < (int)Acut.size() ? Acut[i] : 0;
            int li = i < (int)lnG.size() ? lnG[i] : 0;
            diff[i] = ai - li; if(diff[i] < 0) diff[i] += MOD;
        }
        diff[0] = 0;
        vector<int> one_plus = diff; one_plus[0] = (one_plus[0] + 1) % MOD;
        vector<int> newG = conv(G, one_plus);
        newG.resize(cur); G.swap(newG);
    }
    G.resize(n);
    return G;
}
vector<int> poly_pow(const vector<int>& F, ll k, int n){
    if(n == 0) return {};
    if(k == 0){
        vector<int> r(n);
        r[0] = 1;
        return r;
    }
    int shift = 0;
    while(shift < (int)F.size() && F[shift] == 0) shift++;
    if(shift == (int)F.size()) return vector<int>(n, 0);
    if(1LL * shift * k >= n) return vector<int>(n, 0);
    int lead = F[shift];
    int invLead = fp(lead, MOD - 2);
    vector<int> G;
    for(int i = shift; i < (int)F.size(); i++)
        G.push_back(1LL * F[i] * invLead % MOD);
    G.resize(n);
    auto lnG = poly_ln(G, n);
    for(int &x : lnG) x = 1LL * x * (k % MOD) % MOD;
    auto H = poly_exp(lnG, n);
    int coef = fp(lead, k);
    for(int &x : H) x = 1LL * x * coef % MOD;
    vector<int> res(n);
    for(int i = 0; i + shift * k < n && i < (int)H.size(); i++)
        res[i + shift * k] = H[i];
    return res;
}

template <int MOD_> struct modnum {
    static constexpr int MOD = MOD_;
    static_assert(MOD_ > 0, "MOD must be positive");
private:
    static int minv(int a, int m) {
        a %= m;
        assert(a);
        return a == 1 ? 1 : int(m - ll(minv(m, a)) * ll(m) / a);
    }
public:
    int v;
    modnum() : v(0) {}
    modnum(ll v_) : v(int(v_ % MOD)) { if (v < 0) v += MOD; }
    explicit operator int() const { return v; }
    friend std::ostream& operator << (std::ostream& out, const modnum& n) { return out << int(n); }
    friend std::istream& operator >> (std::istream& in, modnum& n) { ll v_; in >> v_; n = modnum(v_); return in; }
    friend bool operator == (const modnum& a, const modnum& b) { return a.v == b.v; }
    friend bool operator != (const modnum& a, const modnum& b) { return a.v != b.v; }
    modnum inv() const { modnum res; res.v = minv(v, MOD); return res; }
    friend modnum inv(const modnum& m) { return m.inv(); }
    modnum neg() const { modnum res; res.v = v ? MOD-v : 0; return res; }
    friend modnum neg(const modnum& m) { return m.neg(); }
    modnum operator- () const { return neg(); }
    modnum operator+ () const { return modnum(*this); }
    modnum& operator ++ () { v ++; if (v == MOD) v = 0; return *this; }
    modnum& operator -- () { if (v == 0) v = MOD; v --; return *this; }
    modnum& operator += (const modnum& o) { v += o.v; if (v >= MOD) v -= MOD; return *this; }
    modnum& operator -= (const modnum& o) { v -= o.v; if (v < 0) v += MOD; return *this; }
    modnum& operator *= (const modnum& o) { v = int(ll(v) * ll(o.v) % MOD); return *this; }
    modnum& operator /= (const modnum& o) { return *this *= o.inv(); }
    friend modnum operator ++ (modnum& a, int) { modnum r = a; ++a; return r; }
    friend modnum operator -- (modnum& a, int) { modnum r = a; --a; return r; }
    friend modnum operator + (const modnum& a, const modnum& b) { return modnum(a) += b; }
    friend modnum operator - (const modnum& a, const modnum& b) { return modnum(a) -= b; }
    friend modnum operator * (const modnum& a, const modnum& b) { return modnum(a) *= b; }
    friend modnum operator / (const modnum& a, const modnum& b) { return modnum(a) /= b; }
};

using mint = modnum<MOD>;
template <typename T> mint binpow(T aa, ll b) {
    assert(b >= 0);
    mint a = aa;
    mint r = 1; while (b) { if (b & 1) r *= a; b >>= 1; a *= a; } return r;
}

template <int N>
struct counting{
    mint fact[N], inv[N], invfact[N];
    counting() {
        fact[0] = fact[1] = invfact[0] = invfact[1] = inv[1] = 1;
        for (int i = 2; i < N; i++) {
            fact[i] = mint(i) * fact[i - 1];
            inv[i] = mint(0) - mint(MOD / i) * inv[MOD % i];
            invfact[i] = invfact[i - 1] * inv[i];
        }
    }
    mint choose(int n, int r) {
        if (n < 0 || r < 0 || n < r) return mint(0);
        return fact[n] * invfact[n - r] * invfact[r];
    }
    mint stars_and_bars(int n, int r){
        return choose(n + r - 1, r - 1);
    }
    mint P(int n, int r){
        if (n < 0 || r < 0 || r > n) return mint(0);
        return fact[n] * invfact[n - r];
    }
    mint catalan(int n){
        return fact[2 * n] * invfact[n] * invfact[n + 1];
    }
};
const int N = 5e5 + 5;
counting<N> comb;

vector<int> poly(N);
void pre(){
    for(int j = 1; j < N; j <<= 1){
        poly[j] = (int)comb.inv[j];
    }
    poly = poly_exp(poly, N + 1);
}
void SOLVE(){
    int n; cin >> n;
    cout << mint(poly[n]) * comb.fact[n] << endl;
}
int main() {
    ios_base::sync_with_stdio(false), cout.tie(nullptr);
    pre();
    int o_o = 1; cin >> o_o; while(o_o --)
    SOLVE(); return 0;
}