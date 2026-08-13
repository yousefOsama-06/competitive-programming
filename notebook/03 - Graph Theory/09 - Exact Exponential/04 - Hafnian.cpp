// Needs: Mint (06 - Math/01 - Mint.cpp).
// HAFNIAN of a symmetric matrix = the NUMBER OF PERFECT MATCHINGS of a general weighted graph
// (the sum over perfect matchings of the product of the chosen entries). O(2^(n/2) * n^3), which
// is about 2 s for a 38x38 matrix. 0-indexed, n must be EVEN, the diagonal is ignored.
// The hafnian is to the permanent what the general graph is to the bipartite one:
//   permanent(A) = #perfect matchings of a BIPARTITE graph  -> Ryser, O(2^n n)
//   hafnian(A)   = #perfect matchings of a GENERAL graph    -> this file
//   determinant  = the SIGNED version, polynomial, and counts nothing on its own
// Reach for it for "count the perfect matchings / domino tilings / pairings" when the graph is not
// bipartite and n is around 30-40. If the graph IS bipartite use the permanent (2x more vertices
// here would be hopeless); if you only need EXISTENCE, use Blossom or a Tutte matrix.
// METHOD (Bjorklund): keep, for every pair (i, j) of the first m vertices, a POLYNOMIAL in a
// formal variable that counts partial matchings by size. Removing the last two vertices splits
// into "they are matched to each other" and "they are not", and the recursion has depth n/2 with
// two branches that share all the work below them.
struct Hafnian {
    int h;
    void add(vector<Mint>& ans, const vector<Mint>& a, const vector<Mint>& b) {
        for (int i = 0; i < h; i++)
            for (int j = 0; j < h - 1 - i; j++) ans[i + j + 1] += a[i] * b[j];
    }
    vector<Mint> rec(const vector<vector<vector<Mint>>>& v) {
        vector<Mint> ans(h, 0);
        if (v.empty()) { ans[0] = 1; return ans; }
        int m = (int)v.size() - 2;
        auto V = v;
        V.resize(m);
        vector<Mint> zero = rec(V);                            // last two vertices dropped
        for (int i = 0; i < m; i++) for (int j = 0; j < i; j++) {
            add(V[i][j], v[m][i], v[m + 1][j]);                // ... or routed through them
            add(V[i][j], v[m + 1][i], v[m][j]);
        }
        vector<Mint> one = rec(V);
        for (int i = 0; i < h; i++) ans[i] += one[i] - zero[i];
        add(ans, one, v[m + 1][m]);
        return ans;
    }
    Mint solve(const vector<vector<Mint>>& a) {                // a symmetric, a[i][i] unused
        int n = a.size();
        h = n / 2 + 1;
        vector<vector<vector<Mint>>> v(n);
        for (int i = 0; i < n; i++) {
            v[i].resize(i);
            for (int j = 0; j < i; j++) v[i][j].assign(h, 0), v[i][j][0] = a[i][j];
        }
        return rec(v).back();
    }
};
// 0/1 MATRIX -> plain count of perfect matchings. General weights -> weighted count, so this also
// answers "sum over perfect matchings of the product of edge weights" (partition functions).
// PERMANENT by Ryser, for comparison, is 6 lines and O(2^n n):
//   perm(A) = (-1)^n * sum over subsets S of columns of (-1)^|S| * prod_i (sum_{j in S} A[i][j]).
// COUNTING MATCHINGS OF ALL SIZES (not just perfect): pad the matrix with a zero-weight vertex per
// vertex - or read off rec()'s whole coefficient vector, which is exactly that generating function.
// PLANAR GRAPHS have a POLYNOMIAL count via the FKT algorithm (Pfaffian orientation + determinant),
// O(n^3) - that is the one to use for domino tilings of a grid.
