// Catalan, Stirling (both kinds), Bell, derangements, partitions. Needs Mint + buildFact().

Mint catalan(int n) { return C(2 * n, n) * inv_[n + 1]; }
// paths from 0 to k in n steps (+-1) that stay strictly positive - ballot / cycle lemma
Mint ballot(int n, int k) {
    if (n == 0) return k == 0;                        // the empty path
    if (k <= 0 || k > n || (n + k) % 2) return 0;     // k <= 0 was unguarded: Mint(-2) wraps
    return Mint(k) * inv_[n] * C(n, (n + k) / 2);
}

Mint derange(int n) {                                   // D(n) = (n-1)(D(n-1)+D(n-2))
    Mint a = 1, b = 0;                                  // D0=1, D1=0
    for (int i = 2; i <= n; i++) { Mint c = Mint(i - 1) * (a + b); a = b, b = c; }
    return n ? b : a;
}
// permutations of n with exactly k fixed points
Mint fixedPoints(int n, int k) { return C(n, k) * derange(n - k); }

// Stirling 2nd kind, single value: S2(n,k) = (1/k!) sum_i (-1)^i C(k,i) (k-i)^n     O(k log n)
Mint stirling2(ll n, int k) {
    Mint r = 0;
    for (int i = 0; i <= k; i++) {
        Mint t = C(k, i) * Mint(k - i).pow(n);
        (i & 1) ? r -= t : r += t;
    }
    return r * invFact[k];
}
Mint surjections(ll n, int k) { return stirling2(n, k) * fact[k]; }

// Whole ROW of Stirling 2nd (all k for fixed n) in O(n^2), or O(n log n) as a convolution of
// a_i = (-1)^i/i! and b_i = i^n/i!  (NTT).
vector<Mint> stirling2Row(int n) {
    vector<Mint> a(n + 1), b(n + 1), r(n + 1, 0);
    for (int i = 0; i <= n; i++) a[i] = (i & 1 ? Mint(0) - invFact[i] : invFact[i]), b[i] = Mint(i).pow(n) * invFact[i];
    for (int k = 0; k <= n; k++) for (int i = 0; i <= k; i++) r[k] += a[i] * b[k - i];
    return r;
}
// Stirling 1st kind (unsigned), whole row: prod_{i=0..n-1} (x + i)   O(n^2)
vector<Mint> stirling1Row(int n) {
    vector<Mint> p = {1};
    for (int i = 0; i < n; i++) {
        vector<Mint> q(p.size() + 1, 0);
        for (size_t j = 0; j < p.size(); j++) q[j + 1] += p[j], q[j] += p[j] * Mint(i);
        p = q;
    }
    return p;                                           // p[k] = c(n, k)
}
// Bell numbers B(0..n) via the Bell triangle, O(n^2)
vector<Mint> bell(int n) {
    vector<Mint> B(n + 1), row = {1};
    B[0] = 1;
    for (int i = 1; i <= n; i++) {
        vector<Mint> nr = {row.back()};
        for (Mint x : row) nr.push_back(nr.back() + x);
        row = nr, B[i] = row[0];
    }
    return B;
}
// Integer partitions p(0..n) via the pentagonal number theorem, O(n sqrt n)
vector<Mint> partitions(int n) {
    vector<Mint> p(n + 1, 0);
    p[0] = 1;
    for (int i = 1; i <= n; i++)
        for (int k = 1;; k++) {
            ll g1 = (ll)k * (3 * k - 1) / 2, g2 = (ll)k * (3 * k + 1) / 2;
            if (g1 > i) break;
            Mint t = p[i - g1] + (g2 <= i ? p[i - g2] : Mint(0));
            (k & 1) ? p[i] += t : p[i] -= t;
        }
    return p;
}
