// Needs: resultant (05 - Convolution/11 - PolyGcdResultant.cpp), which needs NTT + Polynomial.
// DETERMINANT OF A CIRCULANT in O(n log^2 n) instead of O(n^3).
// C[i][j] = c[(j - i) mod n]  (every row is the previous one rotated right by 1). Then
//   det C = prod_{k=0}^{n-1} f(w^k) = Res(x^n - 1, f),   f(x) = c0 + c1 x + ... + c(n-1) x^(n-1),
// where w is a primitive n-th root of unity. Eigenvalue f(w^k) has eigenvector (1, w^k, w^2k, ...)
// - the Fourier modes diagonalise EVERY circulant simultaneously, which is why circulants commute
// and why multiplying two of them is just a cyclic convolution of their first rows.
// The resultant form is what you use when n does NOT divide MOD - 1, so the roots of unity do not
// exist in the field. Everything below is mod NMOD = 998244353 (the Poly modulus).
ll circulantDet(const vector<ll>& c) {
    int n = c.size();
    Poly xn(n + 1, 0);
    xn[0] = NMOD - 1, xn[n] = 1;                 // x^n - 1
    return resultant(xn, c);
}
// If n | (NMOD - 1) you do not need the resultant at all: evaluate f at the n-th roots of unity
// with one length-n NTT and multiply the results, O(n log n).
// PERMUTANT MATRIX (a circulant along a permutation): M[i][j] = a[k] where sigma^k(i) = j,
// for a permutation sigma that is a SINGLE n-cycle. Relabel the indices along the cycle
// (q[0] = 0, q[t] = sigma^t(0)) and M becomes an honest circulant; the determinant picks up the
// SIGN of that relabelling permutation. If sigma has more than one cycle the matrix is not a
// permutant and the trick does not apply.
// BLOCK-CIRCULANT (blocks of size m, n blocks): det = prod_k det(F(w^k)) with F the matrix-valued
// symbol - same argument, m x m determinants instead of scalars.
// OTHER DETERMINANTS YOU CAN DO WITHOUT ELIMINATION
//  MATRIX DETERMINANT LEMMA: det(A + u v^T) = det(A) * (1 + v^T A^-1 u). Rank-k update:
//    det(A + U V^T) = det(A) * det(I_k + V^T A^-1 U). So det(x*I + a b^T) = x^(n-1) * (x + a.b),
//    which closes "M[i][j] = a_i b_j off the diagonal, x + a_i b_i on it" in O(n).
//  CAUCHY-BINET: for A (n x m) and B (m x n), det(AB) = sum over n-subsets S of [m] of
//    det(A[:,S]) * det(B[S,:]). This is the LGV lemma's algebraic twin.
//  VANDERMONDE: det = prod_{i<j} (x_j - x_i).  CAUCHY: det(1/(x_i+y_j)) =
//    prod_{i<j}(x_i-x_j)(y_i-y_j) / prod_{i,j}(x_i+y_j).
//  TRIDIAGONAL: d_k = b_k d_{k-1} - a_k c_{k-1} d_{k-2}, O(n) (this is the Hessenberg recurrence).
