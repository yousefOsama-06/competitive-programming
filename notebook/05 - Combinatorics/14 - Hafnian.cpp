// Needs: Mint (06 - Math/01 - Modular Arithmetic/01 - Mint.cpp).
// HAFNIAN - the number of PERFECT MATCHINGS of a GENERAL (non-bipartite) graph, weighted:
//   haf(A) = sum over perfect matchings M of prod over {i,j} in M of A[i][j],
// for a symmetric A with a zero diagonal and n EVEN. O(2^(n/2) * n^2), n up to ~38 in a couple of
// seconds. This is the general-graph analogue of the permanent (which counts perfect matchings of
// a BIPARTITE graph, 10 - PermanentAndSymmetric.cpp); haf(A) is the "square root" of the permanent
// of the corresponding symmetric matrix.
// REACH FOR IT for "count the ways to pair up 2m items with forbidden or weighted pairs", domino
// tilings of an irregular board, "partition into pairs" counting, and any #matching problem where
// Blossom only gives you the MAXIMUM matching and you need the COUNT of perfect ones.
// COUNTING is much harder than finding: max matching is polynomial (Blossom), counting is #P-hard.
// For PLANAR graphs use the FKT algorithm (a Pfaffian orientation, then one determinant) - that is
// polynomial, and is how you count domino tilings of a grid.
// The recursion removes the last two rows/columns and tracks a truncated polynomial in a marker
// variable whose exponent counts how many pairs have been committed; only degree <= n/2 survives.
namespace Hafnian {
int H;                                                   // = n/2 + 1, the truncation length
void addTo(vector<Mint>& r, const vector<Mint>& a, const vector<Mint>& b) {
    for (int i = 0; i < H; i++) if (a[i].v)
        for (int j = 0; j + i + 1 < H; j++) r[i + j + 1] += a[i] * b[j];
}
vector<Mint> rec(const vector<vector<vector<Mint>>>& v) {
    vector<Mint> r(H, 0);
    if (v.empty()) { r[0] = 1; return r; }
    int m = (int)v.size() - 2;
    auto V = v;
    V.resize(m);
    vector<Mint> zero = rec(V);                          // vertex pair (m, m+1) left unmatched here
    for (int i = 0; i < m; i++)
        for (int j = 0; j < i; j++) {                    // ... or routed through i and j
            addTo(V[i][j], v[m][i], v[m + 1][j]);
            addTo(V[i][j], v[m + 1][i], v[m][j]);
        }
    vector<Mint> one = rec(V);
    for (int i = 0; i < H; i++) r[i] += one[i] - zero[i];
    addTo(r, one, v[m + 1][m]);                          // ... or matched to each other
    return r;
}
Mint solve(const vector<vector<Mint>>& a) {              // a symmetric, zero diagonal, n even
    int n = a.size();
    H = n / 2 + 1;
    vector<vector<vector<Mint>>> v(n);
    for (int i = 0; i < n; i++) {
        v[i].resize(i);
        for (int j = 0; j < i; j++) v[i][j].assign(H, 0), v[i][j][0] = a[i][j];
    }
    return rec(v).back();
}
}
// SMALL n: a plain bitmask DP over "set of matched vertices", always matching the lowest unmatched
// vertex, is O(2^n * n) - simpler and faster up to n = 22. Use the Hafnian only past that.
// RELATED
//  PFAFFIAN: for a SKEW-symmetric matrix, pf(A)^2 = det(A) and pf is computable in O(n^3); the
//    Hafnian is its "no signs" version, which is exactly why one is easy and the other is not.
//  NUMBER OF MATCHINGS OF EVERY SIZE (not just perfect): that is the matching polynomial; the same
//    recursion with a longer truncation, or O(2^n) subset DP.
//  MODULUS: everything here is Mint arithmetic, so the count comes out mod MOD; for exact small
//    counts use unsigned long long and drop Mint.
