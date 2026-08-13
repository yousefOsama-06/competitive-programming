// ZETA / MOBIUS OVER THE SUBSET LATTICE, and SUBSET CONVOLUTION. 01 - SOS_DP.cpp has the plain
// "sum over submasks"; this file adds the INVERSE (which is what turns a sum-over-subsets answer
// back into an exact-set answer) and the convolution that requires the parts to be DISJOINT.
//   OR-convolution   c[k] = sum over i | j = k of a[i] b[j]      -> zeta, multiply, mobius
//   AND-convolution  c[k] = sum over i & j = k of a[i] b[j]      -> superZeta, mul, superMobius
//   XOR-convolution  c[k] = sum over i ^ j = k of a[i] b[j]      -> FWHT, see 06 - Math/05/03
//   SUBSET conv      c[k] = sum over i | j = k AND i & j = 0     -> THIS FILE, O(2^n n^2)
// WHEN: "partition a set into groups" DPs (count ways to split into k connected pieces, minimum
// cost exact cover, chromatic number), where the naive "for each mask, for each submask" is
// O(3^n) - subset convolution replaces that with O(2^n n^2), which at n = 20 is 4e8 vs 3.5e9.
void zeta(vector<ll>& f, int n) {                              // g[s] = sum over t subset of s
    for (int i = 0; i < n; i++)
        for (int s = 0; s < (1 << n); s++)
            if (s >> i & 1) f[s] = (f[s] + f[s ^ (1 << i)]) % MOD;
}
void mobius(vector<ll>& f, int n) {                            // exact inverse of zeta
    for (int i = 0; i < n; i++)
        for (int s = 0; s < (1 << n); s++)
            if (s >> i & 1) f[s] = (f[s] - f[s ^ (1 << i)] % MOD + MOD) % MOD;
}
void superZeta(vector<ll>& f, int n) {                         // g[s] = sum over t superset of s
    for (int i = 0; i < n; i++)
        for (int s = 0; s < (1 << n); s++)
            if (!(s >> i & 1)) f[s] = (f[s] + f[s | (1 << i)]) % MOD;
}
void superMobius(vector<ll>& f, int n) {
    for (int i = 0; i < n; i++)
        for (int s = 0; s < (1 << n); s++)
            if (!(s >> i & 1)) f[s] = (f[s] - f[s | (1 << i)] % MOD + MOD) % MOD;
}
// SUBSET CONVOLUTION. Rank by POPCOUNT: after the zeta transform, a product of the rank-i and
// rank-j layers counts pairs with i | j SUBSET of s and |i| + |j| = k; reading the answer at
// k = popcount(s) forces i | j = s and |i| + |j| = |s|, which for a union is exactly i & j = 0.
// That ranking is the entire idea - without it, disjointness is not expressible as a transform.
vector<ll> subsetConvolution(const vector<ll>& A, const vector<ll>& B, int n) {
    int N = 1 << n;
    vector<vector<ll>> f(n + 1, vector<ll>(N, 0)), g = f, h = f;
    for (int s = 0; s < N; s++) {
        int p = __builtin_popcount(s);
        f[p][s] = A[s], g[p][s] = B[s];
    }
    for (int k = 0; k <= n; k++) zeta(f[k], n), zeta(g[k], n);
    for (int k = 0; k <= n; k++)
        for (int i = 0; i <= k; i++)
            for (int s = 0; s < N; s++) h[k][s] = (h[k][s] + f[i][s] * g[k - i][s]) % MOD;
    for (int k = 0; k <= n; k++) mobius(h[k], n);
    vector<ll> c(N);
    for (int s = 0; s < N; s++) c[s] = h[__builtin_popcount(s)][s];
    return c;
}
/* USING IT
 SET PARTITION / EXACT COVER: let f[s] = 1 if s is a legal piece. The number of ways to cover the
   full set with exactly k pieces is the k-th subset-convolution POWER of f at s = full; dividing
   by k! makes the pieces unordered. Chromatic number = smallest k with (independent sets)^k > 0.
 SUBSET EXP / LOG exist too (O(2^n n^2)) and turn "connected pieces" into "all pieces" and back -
   the standard route for counting CONNECTED labelled structures on subsets.
 MIN-PLUS version (minimum cost exact cover) does NOT work: min-plus has no inverse, so the
   mobius step is invalid. Use the O(3^n) submask enumeration for min/max objectives.
 O(3^n) SUBMASK ENUMERATION is `for (int t = s; t; t = (t - 1) & s)` and remains the right answer
   for n <= 15 or when the operation is not a ring - it is shorter and has no modulus.
 XOR needs FWHT, not this; AND/OR need only the transform pair with no ranking.
 OVERFLOW: everything here is mod MOD. Without a modulus use __int128 or accept unsigned wraparound
   for OR/AND (the mobius step is exact in Z, so wraparound cancels). */
