// Needs: multiply (02 - NTT.cpp), Poly / divmod / deriv / cut / operator+ (04 - Polynomial.cpp).
// MULTIPOINT EVALUATION and FAST INTERPOLATION at ARBITRARY points, both O(n log^2 n), mod NMOD.
// Evaluating a degree-n polynomial at n points naively is O(n^2); interpolating from n arbitrary
// pairs naively is O(n^2). These are the log-squared versions, built on the same PRODUCT TREE:
// node [l, r] stores prod_{i in [l,r]} (x - x_i).
// REACH FOR IT when n is 1e5 and the points are NOT consecutive integers (if they are, use
// lagrangeConsecutive, O(n)) and NOT a geometric progression (then use chirp-z, O(n log n)).
// Typical: "evaluate this generating function at n given points", "recover the polynomial behind
// n scattered samples", shifting a sampled polynomial to a new set of arguments.
vector<Poly> ptree;                                      // 4n nodes; call buildTree first
void buildTree(int v, int l, int r, const vector<ll>& x) {
    if (l == r) { ptree[v] = Poly{(NMOD - x[l] % NMOD) % NMOD, 1}; return; }
    int m = (l + r) / 2;
    buildTree(2 * v, l, m, x), buildTree(2 * v + 1, m + 1, r, x);
    ptree[v] = multiply(ptree[2 * v], ptree[2 * v + 1]);
}
void evalRec(int v, int l, int r, Poly f, vector<ll>& out) {
    f = divmod(f, ptree[v]).second;                      // f mod prod (x - x_i) keeps the values
    if (l == r) { out[l] = f.empty() ? 0 : f[0]; return; }
    int m = (l + r) / 2;
    evalRec(2 * v, l, m, f, out), evalRec(2 * v + 1, m + 1, r, f, out);
}
vector<ll> evalAll(const Poly& f, const vector<ll>& x) {
    int n = x.size();
    if (!n) return {};
    ptree.assign(4 * n, {});
    buildTree(1, 0, n - 1, x);
    vector<ll> out(n);
    evalRec(1, 0, n - 1, f, out);
    return out;
}
// INTERPOLATION: with M(x) = prod (x - x_i), the Lagrange basis denominator at x_i is M'(x_i), so
// one multipoint evaluation of M' gives every denominator; then merge the leaves bottom-up with
// result = A * (right subtree product) + B * (left subtree product).
Poly interpRec(int v, int l, int r, const vector<ll>& d) {
    if (l == r) return Poly{d[l]};
    int m = (l + r) / 2;
    Poly A = interpRec(2 * v, l, m, d), B = interpRec(2 * v + 1, m + 1, r, d);
    return multiply(A, ptree[2 * v + 1]) + multiply(B, ptree[2 * v]);
}
Poly interpolate(const vector<ll>& x, const vector<ll>& y) {      // all x_i must be DISTINCT
    int n = x.size();
    if (!n) return {};
    ptree.assign(4 * n, {});
    buildTree(1, 0, n - 1, x);
    Poly root = ptree[1];
    vector<ll> out(n);
    evalRec(1, 0, n - 1, deriv(root), out);              // out[i] = M'(x_i) != 0 since x_i distinct
    vector<ll> d(n);
    for (int i = 0; i < n; i++) d[i] = y[i] % NMOD * npow(out[i], NMOD - 2) % NMOD;
    return cut(interpRec(1, 0, n - 1, d), n);
}
// COMPLEXITY: each level of the tree does O(n) total polynomial work with logarithmic factors, so
// n log^2 n; the constant is big - expect ~1s at n = 1e5, and prefer the specialised routines.
// PRECONDITIONS: NMOD prime; the x_i must be pairwise distinct for interpolate (duplicates make
// M'(x_i) = 0 and the inverse blows up). deriv/divmod come from 04 - Polynomial.cpp.
// APPLICATIONS BEYOND THE OBVIOUS
//  Composing "evaluate then re-interpolate" implements ANY linear map given by its action on
//  points, e.g. shifting a sampled polynomial by a constant in O(n log^2 n) (or O(n log n) with
//  the Taylor shift in 05 - Convolution/10).
//  The product tree alone is worth having: prod (x - x_i) in O(n log^2 n) is how you build the
//  characteristic polynomial of a set of roots, the elementary symmetric polynomials of n values,
//  and "the number of ways to pick k of these items" for every k at once.
