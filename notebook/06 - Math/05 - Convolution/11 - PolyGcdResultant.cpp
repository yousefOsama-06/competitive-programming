// Needs: multiply / npow (02 - NTT.cpp), Poly / divmod / cut (04 - Polynomial.cpp).
// POLYNOMIAL GCD and RESULTANT over F_p (p = NMOD = 998244353, prime).
// deg gcd(a, b) = the number of COMMON ROOTS of a and b counted with multiplicity (over the
// algebraic closure), so gcd answers "do these two polynomials share a root" and "how many".
// Euclid with the fast divmod is O(n log n) per step and O(n) steps; the half-gcd algorithm brings
// it to O(n log^2 n) but is long - this is the version you write in a contest.
Poly trim(Poly a) { while (a.size() > 1 && !a.back()) a.pop_back(); return a; }

Poly polyGcd(Poly a, Poly b) {                   // monic gcd; returns {1} for coprime inputs
    a = trim(a), b = trim(b);
    while (b.size() > 1 || b[0]) { Poly r = trim(divmod(a, b).second); a = b, b = r; }
    if (a.size() == 1 && !a[0]) return a;                // gcd(0, 0)
    ll ic = npow(a.back(), NMOD - 2);                    // normalise to monic
    for (ll& x : a) x = x * ic % NMOD;
    return a;
}
// RESULTANT Res(a, b) = lc(b)^deg(a) * prod over roots beta of b of a(beta)
//                = (-1)^(deg a * deg b) * lc(a)^deg(b) * prod over roots alpha of a of b(alpha)
// It is 0 EXACTLY when a and b share a root, and it is the determinant of the Sylvester matrix -
// so it is the polynomial-time way to eliminate a variable from two equations.
// Euclidean recursion: with r = a mod b,
//   Res(a, b) = lc(b)^(deg a - deg r) * (-1)^(deg b * deg r) * Res(b, r).
// O(n log n) per step, O(n) steps.
ll resultant(Poly a, Poly b) {
    a = trim(a), b = trim(b);
    if ((a.size() == 1 && !a[0]) || (b.size() == 1 && !b[0])) return 0;
    ll res = 1;
    while (b.size() > 1) {
        int da = a.size() - 1, db = b.size() - 1;
        Poly r = trim(divmod(a, b).second);
        int dr = r.size() - 1;
        if (r.size() == 1 && !r[0]) return 0;            // common root
        res = res * npow(b.back(), da - dr) % NMOD;
        if ((ll)db * dr % 2) res = (NMOD - res) % NMOD;
        a = b, b = r;
    }
    return res * npow(b[0], a.size() - 1) % NMOD;        // Res(a, const c) = c^deg(a)
}
// USES
//  DISCRIMINANT of f: Res(f, f') / lc(f) - zero iff f has a repeated root. gcd(f, f') isolates the
//    squarefree part, which is the first step of any polynomial factorisation.
//  ELIMINATION: given P(x, y) = 0 and Q(x, y) = 0, Res_y(P, Q) is a polynomial in x alone whose
//    roots are exactly the x-coordinates of the common solutions.
//  det(A + x*B) style problems and "determinant of a circulant" (03 - Matrices/04) are resultants.
//  COMMON ROOT COUNTING mod p for two polynomials given as products, without factoring either.
// PRECONDITION on the Euclidean chain: p must be PRIME (we invert leading coefficients). Over a
// composite modulus use the Sylvester determinant with detMod (07 - Numerical/02) instead.
// DEGREE GOTCHA: divmod in 04 - Polynomial.cpp keeps a padded remainder, so trim() before reading
// any degree - the sign in the resultant recursion depends on the TRUE degree.
