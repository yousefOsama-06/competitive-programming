// INCLUSION-EXCLUSION recipes. Needs Mint + buildFact().

// x1+..+xk = n with 0 <= xi <= ub[i].  IE over which variables overflow their bound.
// O(2^k) - fine for k <= ~20. With equal bounds use the closed form below instead.
Mint boundedStarsBars(ll n, const vector<ll>& ub) {
    int k = ub.size();
    Mint r = 0;
    for (int m = 0; m < (1 << k); m++) {
        ll rem = n;
        for (int i = 0; i < k; i++) if (m >> i & 1) rem -= ub[i] + 1;
        if (rem < 0) continue;
        Mint t = C(rem + k - 1, k - 1);
        (__builtin_popcount(m) & 1) ? r -= t : r += t;
    }
    return r;
}
// Same with a COMMON bound c: sum_j (-1)^j C(k,j) C(n - j(c+1) + k-1, k-1).   O(k)
Mint boundedStarsBarsEqual(ll n, ll k, ll c) {
    Mint r = 0;
    for (ll j = 0; j <= k && j * (c + 1) <= n; j++) {
        Mint t = C(k, j) * C(n - j * (c + 1) + k - 1, k - 1);
        (j & 1) ? r -= t : r += t;
    }
    return r;
}
// #surjections [n] -> [k]  (equivalently k! * S2(n,k))
Mint surj(ll n, ll k) {
    Mint r = 0;
    for (ll i = 0; i <= k; i++) {
        Mint t = C(k, i) * Mint(k - i).pow(n);
        (i & 1) ? r -= t : r += t;
    }
    return r;
}
// "AT LEAST" -> "EXACTLY" (binomial / Mobius inversion over the subset lattice):
//   if A(j) = #configurations having at least a chosen set of j properties (summed over sets),
//   then E(m) = sum_{j>=m} (-1)^(j-m) C(j,m) A(j) counts those with EXACTLY m.
Mint exactlyFromAtLeast(const vector<Mint>& A, int m) {
    Mint r = 0;
    for (int j = m; j < (int)A.size(); j++) {
        Mint t = C(j, m) * A[j];
        ((j - m) & 1) ? r -= t : r += t;
    }
    return r;
}
// BINOMIAL INVERSION:  f(n) = sum_k C(n,k) g(k)   <=>   g(n) = sum_k (-1)^(n-k) C(n,k) f(k)
vector<Mint> binomialInvert(const vector<Mint>& f) {
    int n = f.size();
    vector<Mint> g(n, 0);
    for (int i = 0; i < n; i++)
        for (int k = 0; k <= i; k++) {
            Mint t = C(i, k) * f[k];
            ((i - k) & 1) ? g[i] -= t : g[i] += t;
        }
    return g;
}
// COUNTING COPRIME PAIRS / "gcd is exactly g": count multiples then Mobius-invert downwards:
//   exact[g] = cntMultiples[g] - sum_{g | d, d > g} exact[d]      (iterate g downwards)
vector<ll> exactGcdCounts(const vector<ll>& cntMultiples) {
    int n = cntMultiples.size() - 1;
    vector<ll> e(n + 1);
    for (int g = n; g >= 1; g--) {
        e[g] = cntMultiples[g];
        for (int d = 2 * g; d <= n; d += g) e[g] -= e[d];
    }
    return e;
}
