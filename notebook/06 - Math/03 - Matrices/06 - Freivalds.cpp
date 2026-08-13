// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp), rnd (01 - Miscellaneous/04 - random.cpp).
// FREIVALDS: verify A * B == C in O(t * n^2) instead of computing the product in O(n^3).
// Error probability <= 2^-t (one-sided: it never rejects a correct product). t = 30..40 is plenty.
// REACH FOR IT when the problem is "is this claimed product right", when A or B changes between
// queries so recomputing is too slow, or as a subroutine inside a bigger randomised check.
bool freivalds(const vector<vector<Mint>>& a, const vector<vector<Mint>>& b,
               const vector<vector<Mint>>& c, int t = 40) {
    int n = a.size();
    vector<Mint> r(n), x(n), y(n);
    while (t--) {
        for (int i = 0; i < n; i++) r[i] = rnd(0, 1);            // random 0/1 vector is enough
        for (int i = 0; i < n; i++) {                            // x = B r,  y = C r
            x[i] = y[i] = 0;
            for (int j = 0; j < n; j++) x[i] += b[i][j] * r[j], y[i] += c[i][j] * r[j];
        }
        for (int i = 0; i < n; i++) {                            // check A x == y
            Mint s = 0;
            for (int j = 0; j < n; j++) s += a[i][j] * x[j];
            if (!(s == y[i])) return false;
        }
    }
    return true;
}
// WHY IT WORKS: D = AB - C is non-zero, so some row of D is non-zero; for a uniform 0/1 vector r,
// Pr[D r == 0] <= 1/2 (fix all coordinates but one - at most one of the two choices can cancel).
// Independent rounds multiply, hence 2^-t. Do the products RIGHT to LEFT: A(Br), never (AB)r.

// THE GENERAL PATTERN - RANDOMISED ALGEBRAIC IDENTITY TESTING
// SCHWARTZ-ZIPPEL: a non-zero polynomial of total degree d over a field, evaluated at a uniform
// point of S^n, is zero with probability <= d / |S|. Every check below is one application.
//  - "are these two expressions equal?" - evaluate both at a random point mod a large prime.
//  - PERFECT MATCHING in a general graph: build the TUTTE matrix T with T[i][j] = +x_ij and
//    T[j][i] = -x_ij for each edge i < j, substitute random values, and G has a perfect matching
//    iff det T != 0 (whp). O(n^3) existence check with no Blossom. rank(T) = 2 * (max matching).
//  - BIPARTITE matching: same with the Edmonds matrix (random value on each edge, 0 elsewhere).
//  - "do these two multisets/strings/trees agree?" - hash them as polynomial evaluations.
//  - CHECKING A PRODUCT OF SPARSE OBJECTS: a random-vector probe generalises far past matrices
//    (it is exactly the idea behind Wiedemann in 03 - SparseDeterminant.cpp).
// Prefer a random FIELD element over 0/1 when the identity has degree > 1; the failure bound is
// then d / MOD rather than 1/2 per round.
