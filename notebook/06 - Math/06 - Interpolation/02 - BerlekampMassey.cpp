// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp).
// Berlekamp-Massey: find the shortest linear recurrence fitting a sequence, O(n^2).
// Then jump to the k-th term in O(len^2 log k). THE weapon for "brute force small n, find the
// pattern, extrapolate" - works on any sequence that satisfies a constant-coefficient recurrence.
// Feed it ~2*len terms (60-100 is usually plenty).
vector<Mint> berlekampMassey(vector<Mint> s) {
    int n = s.size(), L = 0, m = 0;
    vector<Mint> C(n), B(n), T;
    C[0] = B[0] = 1;
    Mint b = 1;
    for (int i = 0; i < n; i++) {
        m++;
        Mint d = s[i];
        for (int j = 1; j <= L; j++) d += C[j] * s[i - j];
        if (d.v == 0) continue;
        T = C;
        Mint coef = d / b;
        for (int j = m; j < n; j++) C[j] -= coef * B[j - m];
        if (2 * L > i) continue;
        L = i + 1 - L, B = T, b = d, m = 0;
    }
    C.resize(L + 1), C.erase(C.begin());
    for (auto& x : C) x = Mint(0) - x;
    return C;                                           // s[i] = sum_j C[j] * s[i-1-j]
}
// k-th term (0-indexed) of the recurrence tr with initial values S. O(|tr|^2 log k).
Mint linearRec(const vector<Mint>& S, const vector<Mint>& tr, ll k) {
    if (tr.empty()) return 0;                   // e[1] would be out of bounds
    int n = tr.size();
    auto comb = [&](vector<Mint> a, vector<Mint> b) {
        vector<Mint> r(2 * n + 1, 0);
        for (int i = 0; i <= n; i++) if (a[i].v)
            for (int j = 0; j <= n; j++) r[i + j] += a[i] * b[j];
        for (int i = 2 * n; i > n; i--)
            for (int j = 0; j < n; j++) r[i - 1 - j] += r[i] * tr[j];
        r.resize(n + 1);
        return r;
    };
    vector<Mint> pol(n + 1, 0), e(pol);
    pol[0] = e[1] = 1;
    for (++k; k; k /= 2) {
        if (k % 2) pol = comb(pol, e);
        e = comb(e, e);
    }
    Mint r = 0;
    for (int i = 0; i < n; i++) r += pol[i + 1] * S[i];
    return r;
}
