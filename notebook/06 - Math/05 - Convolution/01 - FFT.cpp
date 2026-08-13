// FFT over complex doubles. conv() is exact while |result coefficient| < ~1e15.
// Prefer NTT when the answer is taken modulo an NTT prime; use convMod for arbitrary moduli.
typedef complex<double> C;
void fft(vector<C>& a) {
    int n = a.size(), L = 31 - __builtin_clz(n);
    static vector<complex<long double>> R(2, 1);
    static vector<C> rt(2, 1);
    for (static int k = 2; k < n; k *= 2) {
        R.resize(n), rt.resize(n);
        auto x = polar(1.0L, acosl(-1.0L) / k);
        for (int i = k; i < 2 * k; i++) rt[i] = R[i] = i & 1 ? R[i / 2] * x : R[i / 2];
    }
    vector<int> rev(n);
    for (int i = 0; i < n; i++) rev[i] = (rev[i / 2] | (i & 1) << L) / 2;
    for (int i = 0; i < n; i++) if (i < rev[i]) swap(a[i], a[rev[i]]);
    for (int k = 1; k < n; k *= 2)
        for (int i = 0; i < n; i += 2 * k)
            for (int j = 0; j < k; j++) {
                C z = rt[j + k] * a[i + j + k];
                a[i + j + k] = a[i + j] - z;
                a[i + j] += z;
            }
}
// Real convolution. Result is rounded to the nearest integer.
vector<ll> conv(const vector<ll>& a, const vector<ll>& b) {
    if (a.empty() || b.empty()) return {};
    int rs = a.size() + b.size() - 1, n = 1;
    while (n < rs) n <<= 1;
    vector<C> in(n), out(n);
    for (size_t i = 0; i < a.size(); i++) in[i].real((double)a[i]);
    for (size_t i = 0; i < b.size(); i++) in[i].imag((double)b[i]);
    fft(in);
    for (C& x : in) x *= x;                              // (a+bi)^2 = a^2-b^2 + 2abi
    for (int i = 0; i < n; i++) out[i] = in[-i & (n - 1)] - conj(in[i]);
    fft(out);
    vector<ll> res(rs);
    for (int i = 0; i < rs; i++) res[i] = llround(imag(out[i]) / (4 * n));
    return res;
}
// Convolution modulo an ARBITRARY M, splitting coefficients at sqrt(M) to keep precision.
vector<ll> convMod(const vector<ll>& a, const vector<ll>& b, ll M) {
    if (a.empty() || b.empty()) return {};
    int rs = a.size() + b.size() - 1, B = 32 - __builtin_clz(rs), n = 1 << B;
    ll cut = (ll)sqrtl((ld)M);
    vector<C> L(n), R(n), os(n), ol(n);
    for (size_t i = 0; i < a.size(); i++) L[i] = C((double)(a[i] / cut), (double)(a[i] % cut));
    for (size_t i = 0; i < b.size(); i++) R[i] = C((double)(b[i] / cut), (double)(b[i] % cut));
    fft(L), fft(R);
    for (int i = 0; i < n; i++) {
        int j = -i & (n - 1);
        ol[j] = (L[i] + conj(L[j])) * R[i] / (2.0 * n);
        os[j] = (L[i] - conj(L[j])) * R[i] / (2.0 * n) / C(0, 1);
    }
    fft(ol), fft(os);
    vector<ll> res(rs);
    for (int i = 0; i < rs; i++) {
        ll av = llround(real(ol[i])), cv = llround(imag(os[i]));
        ll bv = llround(imag(ol[i])) + llround(real(os[i]));
        res[i] = ((av % M * cut + bv) % M * cut + cv) % M;
    }
    return res;
}
