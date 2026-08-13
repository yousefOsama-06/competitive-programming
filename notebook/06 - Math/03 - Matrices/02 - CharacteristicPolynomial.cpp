// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp), Mat (03 - Matrices/01).
// CHARACTERISTIC POLYNOMIAL p(x) = det(x*I - A) of an n x n matrix in O(n^3). MOD must be PRIME.
// REACH FOR IT when you need det(A - t*I) for MANY t (n = 500, q = 1e5 queries: build p once,
// then each query is one Horner evaluation), or A^k for a huge k: by Cayley-Hamilton p(A) = 0, so
// A^k = (x^k mod p(x))(A), which costs O(n^2 log k) polynomial work plus O(n^3) to expand - much
// better than O(n^3 log k) when n is large. Also: det(A) = (-1)^n * p[0], trace(A) = -p[n-1].
// Gaussian elimination CANNOT do this directly because the entries are polynomials in x.

// STEP 1: reduce to upper HESSENBERG form (a[i][j] = 0 for i > j + 1) by SIMILARITY transforms.
// The row op "row i -= f * row (c+1)" is undone by "col (c+1) += f * col i", so A -> E A E^-1 and
// the whole spectrum (hence the characteristic polynomial) is unchanged. O(n^3).
Mat hessenberg(Mat a) {
    int n = a.size();
    for (int c = 0; c + 2 < n; c++) {
        int p = -1;
        for (int i = c + 1; i < n; i++) if (a[i][c].v) { p = i; break; }
        if (p < 0) continue;                     // column already clear below the subdiagonal
        if (p != c + 1) {                        // swapping rows AND cols keeps it a similarity;
            swap(a[c + 1], a[p]);                // column c is untouched because c < c+1 <= p
            for (int i = 0; i < n; i++) swap(a[i][c + 1], a[i][p]);
        }
        for (int i = c + 2; i < n; i++) if (a[i][c].v) {
            Mint f = a[i][c] / a[c + 1][c];
            for (int j = 0; j < n; j++) a[i][j] -= a[c + 1][j] * f;
            for (int j = 0; j < n; j++) a[j][c + 1] += a[j][i] * f;   // uses the UPDATED col i
        }
    }
    return a;
}
// STEP 2: expand det(x*I - H) along the last row. With f[m] = char poly of the leading m x m block,
//   f[m] = (x - h[m-1][m-1]) f[m-1] - sum_{i=1..m-1} h[m-1-i][m-1] * (prod_{j=m-i..m-1} h[j][j-1])
//                                                                  * f[m-1-i]
// Only Hessenberg matrices satisfy such a short recurrence; that is the whole point of step 1.
vector<Mint> charPoly(const Mat& a) {            // returns p[0..n], p[i] = coeff of x^i, p[n] = 1
    int n = a.size();
    Mat h = hessenberg(a);
    vector<vector<Mint>> f(n + 1);
    f[0] = {1};
    for (int m = 1; m <= n; m++) {
        f[m].assign(m + 1, 0);
        for (int i = 0; i < m; i++) {
            f[m][i + 1] += f[m - 1][i];
            f[m][i] -= h[m - 1][m - 1] * f[m - 1][i];
        }
        Mint prod = 1;
        for (int i = 1; i < m; i++) {
            prod *= h[m - i][m - i - 1];
            Mint c = prod * h[m - 1 - i][m - 1];
            if (!c.v) continue;
            for (int j = 0; j <= m - 1 - i; j++) f[m][j] -= c * f[m - 1 - i][j];
        }
    }
    return f[n];
}
// A^k VIA CAYLEY-HAMILTON: r(x) = x^k mod p(x) (O(n^2 log k) with schoolbook, O(n log n log k)
// with NTT), then A^k = sum r[i] A^i. If you only need u^T A^k v, that is sum r[i] * (u^T A^i v)
// and the A^i v are n matrix-vector products - O(n * nnz * n) for a sparse A, no n^3 anywhere.
// EIGENVALUES over F_p are the roots of p; over R use the QR algorithm (not a contest routine).
// det(A + t*B) as a polynomial in t: reduce (A, B) to Hessenberg/triangular pencil form, or just
// evaluate at n+1 values of t with plain Gaussian determinants and interpolate - O(n^4), fine to
// n = 100 and much easier to get right.
// SIMILAR-LOOKING BUT DIFFERENT: the MINIMAL polynomial divides p; get it from the Krylov
// sequence u^T A^k v with Berlekamp-Massey (see 03 - SparseDeterminant.cpp).
