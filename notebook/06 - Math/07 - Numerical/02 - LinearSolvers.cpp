// SPECIAL-SHAPE LINEAR SOLVERS. Gaussian elimination is O(n^3); these are the shapes where the
// structure buys you a factor of n or removes a precondition.

// --- THOMAS ALGORITHM: TRIDIAGONAL system in O(n). ---
//   a[i] x[i-1] + b[i] x[i] + c[i] x[i+1] = d[i],  with a[0] = c[n-1] = 0.
// This is THE closer for random walks on a path ("expected steps to escape [0, n]"), 1-D heat /
// diffusion DPs, and any DP whose state depends on its two neighbours. Gaussian elimination on
// the same system is O(n^3) and will TLE at n = 1e5; this is 8 lines.
// STABLE without pivoting when the system is diagonally dominant (|b| > |a| + |c|), which every
// hitting-time system is.
vector<ld> thomas(vector<ld> a, vector<ld> b, vector<ld> c, vector<ld> d) {
    int n = b.size();
    for (int i = 1; i < n; i++) {
        ld m = a[i] / b[i - 1];
        b[i] -= m * c[i - 1], d[i] -= m * d[i - 1];
    }
    vector<ld> x(n);
    x[n - 1] = d[n - 1] / b[n - 1];
    for (int i = n - 2; i >= 0; i--) x[i] = (d[i] - c[i] * x[i + 1]) / b[i];
    return x;
}
// BANDED systems of half-width w (state depends on w neighbours) generalise this at O(n w^2) -
// the shape of "robot on a grid with m columns", where w = m.
// CYCLIC tridiagonal (x[0] and x[n-1] also coupled): Sherman-Morrison - solve two tridiagonal
// systems and combine, still O(n).

// --- DETERMINANT MODULO AN ARBITRARY m (composite allowed), O(n^3 log m). ---
// Gaussian elimination inverts the pivot, so it needs a PRIME modulus. This version never
// divides: it runs the Euclidean algorithm between two rows, swapping them, until the lower
// entry is 0. Use it when the modulus is 2^k, 1e9, or anything you cannot invert in.
ll detMod(vector<vector<ll>> a, ll m) {
    int n = a.size();
    ll det = 1 % m;
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++)
            while (a[j][i]) {                                  // Euclid on the two rows
                ll t = a[i][i] / a[j][i];
                for (int k = i; k < n; k++) a[i][k] = (a[i][k] - t * a[j][k]) % m;
                swap(a[i], a[j]), det = -det;
            }
        det = det * a[i][i] % m;
        if (!a[i][i]) return 0;
    }
    return (det % m + m) % m;
}
// MATRIX RANK over a field: the usual elimination, counting non-zero pivot rows.
// RANK over GF(2): use bitsets - O(n^3 / 64), see 04 - Linear Algebra/03.
// INTEGER determinant with no modulus: compute it modulo several primes and CRT. Size it with
// HADAMARD'S BOUND |det| <= prod of the row 2-norms <= n^(n/2) * max|a_ij|^n, and take enough
// primes that the product exceeds 2x that.
// SPARSE determinant (n = 1e5 with few non-zeros): Wiedemann - pick random u, v, feed the Krylov
// sequence u^T A^k v to Berlekamp-Massey to get the minimal polynomial, whose constant term is
// +-det after a random diagonal preconditioner. O(n * nnz).
// CHARACTERISTIC POLYNOMIAL in O(n^3): reduce to upper HESSENBERG form by similarity transforms
// (so det(xI - H) satisfies a short recurrence in the leading principal minors), then read the
// coefficients off. With Cayley-Hamilton that gives A^k in O(n^3 + n^2 log k) instead of
// O(n^3 log k) - worth it only for large k and small n.
