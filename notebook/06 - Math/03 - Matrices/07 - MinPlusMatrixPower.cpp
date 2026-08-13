// TROPICAL (MIN-PLUS) MATRIX POWER: replace (+, *) by (min, +) and matrix exponentiation answers
// "cheapest walk using EXACTLY k edges" in O(n^3 log k). The min-plus semiring has additive
// identity llinf and multiplicative identity 0, and it is associative - which is all matrix
// exponentiation ever needed. Everything below works verbatim for MAX-PLUS by flipping min to max
// and llinf to -llinf.
// REACH FOR IT when the edge count is a hard constraint and k is large: "shortest path with
// exactly k edges", "cheapest k-step schedule", periodic/cyclic transport problems, and DPs whose
// transition is the same min-plus matrix every layer (n <= 100-200, k up to 1e18).
// If k is small, just do k layers of DP - O(k * n^2) beats O(n^3 log k) whenever k < n log k.
typedef vector<vector<ll>> TMat;

TMat tmul(const TMat& a, const TMat& b) {
    int n = a.size(), m = b[0].size(), p = b.size();
    TMat c(n, vector<ll>(m, llinf));
    for (int i = 0; i < n; i++)
        for (int t = 0; t < p; t++) {
            if (a[i][t] >= llinf) continue;                        // skip, and avoid overflow
            for (int j = 0; j < m; j++)
                if (b[t][j] < llinf) c[i][j] = min(c[i][j], a[i][t] + b[t][j]);
        }
    return c;
}
TMat teye(int n) {                                                 // min-plus identity
    TMat r(n, vector<ll>(n, llinf));
    for (int i = 0; i < n; i++) r[i][i] = 0;
    return r;
}
TMat tpow(TMat a, ll k) {                                          // exactly k edges
    TMat r = teye(a.size());
    for (; k > 0; k >>= 1, a = tmul(a, a)) if (k & 1) r = tmul(r, a);
    return r;
}
// SET-UP: a[i][j] = weight of edge i->j, llinf when there is no edge. Then tpow(a, k)[u][v] is the
// minimum total weight over walks u->v with exactly k edges (llinf if none exists).
// AT MOST k EDGES: add a zero-weight self-loop at every vertex (a[i][i] = min(a[i][i], 0)).
// COUNTING instead of minimising: the same code over (+, *) is 03 - Matrices/01, and over
// (or, and) with bitsets it answers reachability in exactly k steps in O(n^3 / 64).
// LONGEST path with exactly k edges: max-plus, but ONLY if the graph is a DAG or k is bounded;
// otherwise a positive cycle makes it unbounded.
// OVERFLOW is the standard bug: guard both operands against llinf before adding, as above, or
// llinf + llinf silently wraps negative and poisons the whole matrix.
// FASTER SPECIAL CASES
//  - if the matrix is CONVEX in the Monge sense, min-plus products drop to O(n^2) via SMAWK, so
//    the whole power costs O(n^2 log k) (see 08 - DP/01/11 - MinPlusConvolution.cpp).
//  - if you only need one row (a fixed source), keep a VECTOR and multiply it by the precomputed
//    powers a^(2^i): O(n^2 log k) total instead of n^3 log k.
//  - for eventually-periodic behaviour, the tropical eigenvalue (the minimum mean cycle, Karp's
//    algorithm) gives the per-step growth rate and lets you skip the huge exponent entirely.
