// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp), berlekampMassey (06 - Interpolation/02),
//        rnd (01 - Miscellaneous/04 - random.cpp).
// WIEDEMANN: determinant and minimal polynomial of a SPARSE n x n matrix in O(n * nnz + n^2),
// using O(n) memory. MOD must be PRIME. Gaussian elimination is O(n^3) and fills in; this is the
// routine for n = 5e4 with 1e5 non-zeros (Yosupo "sparse matrix det"), for adjacency-like
// matrices, and for the Matrix-Tree determinant of a sparse graph.
// IDEA: for random vectors u, v the scalar sequence s_k = u^T A^k v obeys the same linear
// recurrence as A itself (Cayley-Hamilton), so Berlekamp-Massey on 2n+2 samples recovers the
// minimal polynomial. Each sample costs one sparse mat-vec, hence n * nnz total.
// PRECONDITIONER: right-multiplying by a random diagonal D makes minpoly(AD) = charpoly(AD) with
// probability >= 1 - O(n^2 / MOD), which is what lets us read the determinant off the last
// coefficient. det(AD) = det(A) * prod d_i, so divide the d_i back out.
struct Elem { int r, c; Mint v; };               // A[r][c] = v; all (r, c) distinct, rest zero

vector<Mint> sparseMinPoly(int n, const vector<Elem>& a) {
    vector<Mint> u(n), x(n), s;
    for (int i = 0; i < n; i++) u[i] = rnd(1, MOD - 1), x[i] = rnd(1, MOD - 1);
    for (int k = 0; k < 2 * n + 2; k++) {
        Mint t = 0;
        for (int i = 0; i < n; i++) t += u[i] * x[i];
        s.push_back(t);
        vector<Mint> y(n, 0);                    // y = A * x, one pass over the non-zeros
        for (const Elem& e : a) y[e.r] += e.v * x[e.c];
        x = y;
    }
    return berlekampMassey(s);                   // s[i] = sum_j C[j] * s[i-1-j]
}
Mint sparseDet(int n, vector<Elem> a) {
    vector<Mint> d(n);
    for (int i = 0; i < n; i++) d[i] = rnd(1, MOD - 1);
    for (Elem& e : a) e.v *= d[e.c];             // A -> A * diag(d)
    vector<Mint> c = sparseMinPoly(n, a);
    if ((int)c.size() < n) return 0;             // minpoly shorter than n => singular (whp)
    Mint r = c.back();                           // charpoly = x^n - c0 x^(n-1) - ... - c(n-1)
    if (n % 2 == 0) r = Mint(0) - r;             // det = (-1)^(n+1) * c[n-1]
    for (int i = 0; i < n; i++) r /= d[i];
    return r;
}
// RANK of a sparse matrix: same trick on a random projection; simpler in practice is to run
// blocked elimination only if the fill-in stays small.
// SOLVING A x = b sparsely: with the minimal polynomial m(x) = sum m_i x^i and m_0 != 0,
//   x = -(1/m_0) * sum_{i>=1} m_i A^(i-1) b   -- again only sparse mat-vecs.
// FAILURE MODE: if the returned recurrence is shorter than n, either the matrix is singular or
// the random draw was unlucky; rerun once before believing the 0.
// INTEGER determinant with no modulus: run this under several primes and CRT, sizing with
// Hadamard's bound |det| <= prod of the row 2-norms.
