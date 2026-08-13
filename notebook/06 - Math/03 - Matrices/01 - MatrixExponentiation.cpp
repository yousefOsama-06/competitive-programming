// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp).
// Matrix over Mint. mul O(n^3) (cache-friendly i-k-j order), pow O(n^3 log e).
// Fix the size at compile time (array<array<Mint,K>,K>) if you need the last 2x speedup.
typedef vector<vector<Mint>> Mat;

Mat operator*(const Mat& a, const Mat& b) {
    int n = a.size(), k = b.size(), m = b[0].size();
    Mat c(n, vector<Mint>(m));
    for (int i = 0; i < n; i++)
        for (int t = 0; t < k; t++) if (a[i][t].v)
            for (int j = 0; j < m; j++) c[i][j] += a[i][t] * b[t][j];
    return c;
}
Mat eye(int n) { Mat r(n, vector<Mint>(n)); for (int i = 0; i < n; i++) r[i][i] = 1; return r; }
Mat mpow(Mat a, ll e) {
    Mat r = eye(a.size());
    for (; e > 0; e >>= 1, a = a * a) if (e & 1) r = r * a;
    return r;
}
// RECIPES
//  Linear recurrence f(n) = c1*f(n-1) + ... + ck*f(n-k):
//    companion matrix, first row = (c1..ck), subdiagonal = 1.  f(n) = (M^(n-k) * [f(k-1)..f(0)]^T)[0]
//  Add a "+d" constant term: enlarge by one row/col with a 1 on the diagonal, put d in the first row.
//  Count walks of length L between u and v: (A^L)[u][v] where A is the adjacency matrix.
//  Shortest walk with exactly L edges: same, but replace (+,*) with (min,+) — "min-plus product".
