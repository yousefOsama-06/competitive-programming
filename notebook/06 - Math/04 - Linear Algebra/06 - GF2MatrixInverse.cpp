// INVERSE OF A MATRIX OVER GF(2) with bitsets - O(n^3 / 64). n = 2000 runs in well under a second,
// where a Mint-based inverse at the same size is 8e9 operations and hopeless.
// Returns {} if the matrix is singular. Rows are bitset<M> with bit j = entry (i, j).
// REACH FOR IT for xor-system problems that need the FULL inverse rather than one solution:
//  - "for each edge, is it in EVERY / SOME spanning structure" style queries, which reduce to a
//    single entry of the inverse (CF 736D: after inverting the incidence-style matrix, entry
//    inv[b][a] decides whether edge (a, b) is forced);
//  - answering many right-hand sides against one fixed matrix (x = A^-1 b in O(n^2 / 64) each);
//  - determinant over GF(2) (= 1 iff invertible), rank, and the null space in one pass.
template <int M>
vector<bitset<M>> inverseGF2(vector<bitset<M>> a) {
    int n = a.size();
    vector<bitset<M>> inv(n);
    for (int i = 0; i < n; i++) inv[i][i] = 1;
    for (int i = 0; i < n; i++) {                        // forward elimination
        int piv = -1;
        for (int j = i; j < n; j++) if (a[j][i]) { piv = j; break; }
        if (piv < 0) return {};                          // singular
        swap(a[i], a[piv]), swap(inv[i], inv[piv]);
        for (int j = i + 1; j < n; j++) if (a[j][i]) a[j] ^= a[i], inv[j] ^= inv[i];
    }
    for (int i = n - 1; i >= 0; i--)                     // back substitution
        for (int j = i + 1; j < n; j++) if (a[i][j]) inv[i] ^= inv[j];
    return inv;
}
// NO PIVOT VALUES to divide by: over GF(2) every non-zero entry is 1, so elimination is pure xor
// and there is no numerical anything. That is why this is the cleanest Gaussian elimination there
// is - and why you should reduce to GF(2) whenever a problem lets you.
// DETERMINANT over GF(2) is 1 iff the forward pass finds a pivot in every column; equivalently
// rank == n. For the rank alone, or a solution of A x = b, use gaussXor (04 - Linear Algebra/03)
// which is the same loop with an augmented column.
// NULL SPACE: run gaussXor to RREF; for each free column c, the vector with 1 at c and the pivot
// entries copied from column c is a basis vector of the kernel. Its size is n - rank, and the
// solution set of A x = b is (any particular solution) + (that span).
// PRODUCT over GF(2) in O(n^3 / 64): transpose b, then c[i][j] = ((a[i] & bt[j]).count() & 1).
// Or, faster, c[i] = xor of a's rows selected by b - that is O(n^2 / 64) per row with no popcounts.
// M IS A COMPILE-TIME BOUND: bitset<M> must be at least n wide; keep M a multiple of 64.
