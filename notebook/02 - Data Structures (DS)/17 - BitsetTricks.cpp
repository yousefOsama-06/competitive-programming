// BITSET SPEEDUPS - the /64 family. All of these are "obviously too slow" DPs that fit anyway.

// TRANSITIVE CLOSURE (reachability) in O(n^3 / 64). n up to ~5000.
template <int N>
void transitiveClosure(vector<bitset<N>>& g, int n) {
    for (int k = 0; k < n; k++)
        for (int i = 0; i < n; i++) if (g[i][k]) g[i] |= g[k];
}
// LCS in O(n*m/64) - bit-parallel (Allison-Dix). `a` is the string whose length bounds the
// bitset width; row is a bitmask over positions of `a`.
//   x    = row | match[b_i]
//   row' = x & ((x - (row << 1 | 1)) ^ x)
// The "| 1" is the carry-in that makes the shift borrow correctly.
typedef unsigned long long u64;
int bitLCS(const string& a, const string& b) {
    int n = a.size(), W = (n + 63) / 64;
    if (!n || b.empty()) return 0;
    vector<vector<u64>> match(256, vector<u64>(W, 0));
    for (int i = 0; i < n; i++) match[(unsigned char)a[i]][i >> 6] |= 1ULL << (i & 63);
    vector<u64> row(W, 0), x(W), sh(W);
    for (char c : b) {
        const auto& M = match[(unsigned char)c];
        for (int i = 0; i < W; i++) x[i] = row[i] | M[i];
        u64 carry = 1;                                // sh = (row << 1) | 1
        for (int i = 0; i < W; i++) { u64 nc = row[i] >> 63; sh[i] = (row[i] << 1) | carry; carry = nc; }
        u64 borrow = 0;                               // row = x & ((x - sh) ^ x)
        for (int i = 0; i < W; i++) {
            u64 t = sh[i] + borrow;
            borrow = (t < sh[i]) || (x[i] < t);       // t < sh[i] catches the carry overflow
            u64 d = x[i] - t;
            row[i] = x[i] & (d ^ x[i]);
        }
    }
    int r = 0;
    for (u64 v : row) r += __builtin_popcountll(v);
    return r;
}

// SUBSET SUM / KNAPSACK FEASIBILITY: dp |= dp << w   (see KnapsackFamily.cpp)
// BIPARTITE MATCHING on dense graphs: keep each left vertex's neighbourhood as a bitset and
//   pick the next free right vertex with (adj[u] & free)._Find_first().
// COUNTING PAIRS with a property: bitset per value class, then popcount of ANDs.
// GCC EXTRAS: b._Find_first(), b._Find_next(i) iterate set bits in O(N/64) total.
// RULE OF THUMB: bitset turns 1e9 "simple" operations into ~1.5e7 word operations - budget
// about n*m/64 <= 1e8 for a comfortable pass.
