// Needs: Mint (06 - Math/01 - Modular Arithmetic/01 - Mint.cpp).
// TRANSFORMS OVER THE SUBSET LATTICE. n bits, arrays of size 2^n.

// ZETA (sum over subsets) and its inverse MOBIUS. O(n 2^n). This is SOS DP.
void zetaSub(vector<Mint>& f, int n) {                    // f[m] := sum_{s subset of m} f[s]
    for (int i = 0; i < n; i++)
        for (int m = 0; m < (1 << n); m++) if (m >> i & 1) f[m] += f[m ^ (1 << i)];
}
void mobiusSub(vector<Mint>& f, int n) {                  // exact inverse of zetaSub
    for (int i = 0; i < n; i++)
        for (int m = 0; m < (1 << n); m++) if (m >> i & 1) f[m] -= f[m ^ (1 << i)];
}
void zetaSuper(vector<Mint>& f, int n) {                  // f[m] := sum_{m subset of s} f[s]
    for (int i = 0; i < n; i++)
        for (int m = 0; m < (1 << n); m++) if (!(m >> i & 1)) f[m] += f[m | (1 << i)];
}
void mobiusSuper(vector<Mint>& f, int n) {
    for (int i = 0; i < n; i++)
        for (int m = 0; m < (1 << n); m++) if (!(m >> i & 1)) f[m] -= f[m | (1 << i)];
}
// OR-convolution: h[m] = sum_{a|b = m} f[a] g[b]      -> zetaSub, multiply, mobiusSub
// AND-convolution: h[m] = sum_{a&b = m} f[a] g[b]     -> zetaSuper, multiply, mobiusSuper
vector<Mint> orConv(vector<Mint> f, vector<Mint> g, int n) {
    zetaSub(f, n), zetaSub(g, n);
    for (int m = 0; m < (1 << n); m++) f[m] *= g[m];
    return mobiusSub(f, n), f;
}
vector<Mint> andConv(vector<Mint> f, vector<Mint> g, int n) {
    zetaSuper(f, n), zetaSuper(g, n);
    for (int m = 0; m < (1 << n); m++) f[m] *= g[m];
    return mobiusSuper(f, n), f;
}

// SUBSET (disjoint) CONVOLUTION: h[m] = sum_{a|b = m, a&b = 0} f[a] g[b].   O(2^n n^2).
// The ranked-zeta trick: index by popcount, convolve the ranks like polynomials.
vector<Mint> subsetConv(const vector<Mint>& f, const vector<Mint>& g, int n) {
    int N = 1 << n;
    vector<vector<Mint>> F(n + 1, vector<Mint>(N, 0)), G = F, H = F;
    for (int m = 0; m < N; m++) F[__builtin_popcount(m)][m] = f[m], G[__builtin_popcount(m)][m] = g[m];
    for (int i = 0; i <= n; i++) zetaSub(F[i], n), zetaSub(G[i], n);
    for (int i = 0; i <= n; i++)
        for (int j = 0; i + j <= n; j++)
            for (int m = 0; m < N; m++) H[i + j][m] += F[i][m] * G[j][m];
    for (int i = 0; i <= n; i++) mobiusSub(H[i], n);
    vector<Mint> h(N);
    for (int m = 0; m < N; m++) h[m] = H[__builtin_popcount(m)][m];
    return h;
}
// USES: exact set cover / "partition the ground set into k valid groups" (repeated subset
// convolution, or exponentiate the EGF-like series); counting perfect matchings by subsets;
// "for each mask, best way to split it into two disjoint valid halves".
// SUBMASK ENUMERATION over all masks is O(3^n):  for (s = m; s; s = (s-1) & m)
