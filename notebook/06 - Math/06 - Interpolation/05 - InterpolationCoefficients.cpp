// Needs: Mint (01 - Modular Arithmetic/01 - Mint.cpp); binomialBasis needs 02 - NTT.cpp instead.
// GETTING THE COEFFICIENTS, not just one value. 01 - LagrangeAndFaulhaber.cpp evaluates an
// interpolating polynomial at a point; sometimes the problem wants the polynomial itself - to
// integrate it, to compose it, to read off a single coefficient, or to answer queries at points
// that are not known in advance.

// ARBITRARY sample points, explicit coefficients, O(n^2). x_i must be pairwise DISTINCT.
// Build M(x) = prod (x - x_i) once, then divide out each (x - x_i) by synthetic division.
vector<Mint> interpCoeffs(const vector<Mint>& x, const vector<Mint>& y) {
    int n = x.size();
    vector<Mint> M(n + 1, 0), r(n, 0);
    M[0] = 1;
    for (int i = 0; i < n; i++)                          // M *= (x - x_i)
        for (int j = n; j >= 0; j--) { M[j] *= Mint(0) - x[i]; if (j) M[j] += M[j - 1]; }
    for (int i = 0; i < n; i++) {
        vector<Mint> q(n);                               // q = M / (x - x_i), synthetic division
        Mint rem = M[n];
        for (int j = n - 1; j >= 0; j--) q[j] = rem, rem = M[j] + rem * x[i];
        Mint den = 1;
        for (int j = 0; j < n; j++) if (j != i) den *= x[i] - x[j];
        Mint sc = y[i] / den;
        for (int j = 0; j < n; j++) r[j] += q[j] * sc;
    }
    return r;                                            // r[j] = coefficient of x^j
}
// THE INVERSE VANDERMONDE MATRIX is exactly this: V[i][j] = x_i^j, and (V^-1)[j][i] is the
// coefficient of x^j in the i-th Lagrange basis polynomial - the vector interpCoeffs produces when
// y = e_i. So "solve V a = y" IS interpolation, in O(n^2) instead of O(n^3), and
//   det V = prod_{i < j} (x_j - x_i).
// If the first column starts at x_i instead of 1 (i.e. V[i][j] = x_i^(j+1)), divide row i by x_i.

// BINOMIAL (Newton forward-difference) BASIS, from CONSECUTIVE samples p(0..n), in O(n log n):
//   p(x) = sum_i a_i C(x, i),  a_i = i-th finite difference at 0 = sum_j (-1)^(i-j) C(i,j) p(j)
// which is one convolution of p[j]/j! with (-1)^j/j!. The a_i are integers whenever p maps
// integers to integers, so this is THE basis for "p has integer values" arguments, and evaluating
// p at a HUGE x is then sum a_i * C(x, i) with C(x, i) built by a running product - O(n) and no
// division by anything that could vanish.
// Written mod NMOD on top of multiply() from 02 - NTT.cpp (Poly = vector<ll>); if your modulus is
// not NTT-friendly, run the same two lines with an O(n^2) convolution or with convMod.
vector<ll> binomialBasis(const vector<ll>& p) {          // p[j] = value at j, j = 0..n
    int n = p.size();
    vector<ll> f(n), fi(n), u(n), v(n);
    f[0] = 1;
    for (int i = 1; i < n; i++) f[i] = f[i - 1] * i % NMOD;
    fi[n - 1] = npow(f[n - 1], NMOD - 2);
    for (int i = n - 1; i > 0; i--) fi[i - 1] = fi[i] * i % NMOD;
    for (int i = 0; i < n; i++) {
        u[i] = p[i] % NMOD * fi[i] % NMOD;
        v[i] = (i & 1) ? (NMOD - fi[i]) % NMOD : fi[i];
    }
    vector<ll> w = multiply(u, v);
    vector<ll> a(n);
    for (int i = 0; i < n; i++) a[i] = w[i] * f[i] % NMOD;
    return a;
}
// FALLING FACTORIAL basis x^(i) = x(x-1)...(x-i+1) is the same thing scaled: a_i * i! are its
// coefficients. Converting between the monomial and falling-factorial bases is exactly the
// STIRLING transform (Stirling numbers of the first/second kind are the two change-of-basis
// matrices), which is how "sum of i^k" becomes "sum of C(i, j)" and telescopes.
// WHICH ROUTINE WHEN
//   one value, consecutive samples ....... lagrangeConsecutive, O(n)
//   one value, arbitrary samples ......... lagrange, O(n^2)
//   coefficients, arbitrary samples ...... interpCoeffs, O(n^2)  (or 04 - MultipointEval)
//   coefficients in the binomial basis ... binomialBasis, O(n log n)
