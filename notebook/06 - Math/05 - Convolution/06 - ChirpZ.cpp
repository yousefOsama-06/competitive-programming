// Needs: multiply, npow (02 - NTT.cpp); Poly / cut (04 - Polynomial.cpp).
// CHIRP-Z / BLUESTEIN: evaluate a polynomial at a GEOMETRIC progression of points,
//   P(1), P(z), P(z^2), ..., P(z^(m-1))   in O((n + m) log(n + m)),
// where n = deg P. Multipoint evaluation at arbitrary points is O(n log^2 n); this is a full log
// factor cheaper and much shorter, and it covers most of the cases that actually come up.
// PRECONDITION: z must be INVERTIBLE mod NMOD (z != 0). Any m and n are fine - no power of two.
// REACH FOR IT for: a DFT of ARBITRARY length (set z = a primitive m-th root of unity - this is
// how you do a length-7 or length-1000000 transform with an NTT that only likes powers of two);
// "evaluate f at 1, r, r^2, ..." which is what q-analogue and geometric-weight problems ask for;
// and cyclic convolution of a non-power-of-two length.
// IDENTITY: i*j = C(i+j, 2) - C(i, 2) - C(j, 2), which turns the exponent z^(i*j) into a product
// of three one-index factors, so the sum becomes a convolution ("chirp" = the z^C(k,2) sequence).
Poly chirpZ(const Poly& a, ll z, int m) {
    int n = a.size() - 1;                                // deg P
    if (m <= 0) return {};
    ll iz = npow(z, NMOD - 2);
    auto tri = [&](ll k) { return k * (k - 1) / 2 % (NMOD - 1); };       // C(k, 2) mod (p-1)
    Poly f(n + 1), g(n + m);
    for (int j = 0; j <= n; j++) f[n - j] = a[j] * npow(iz, tri(j)) % NMOD;   // reversed
    for (int t = 0; t < n + m; t++) g[t] = npow(z, tri(t));
    Poly h = multiply(f, g);
    Poly r(m);
    for (int k = 0; k < m; k++) r[k] = h[n + k] * npow(iz, tri(k)) % NMOD;
    return r;
}
// ARBITRARY-LENGTH DFT: with w a primitive m-th root of unity mod NMOD (needs m | NMOD - 1),
// chirpZ(a, w, m) is the length-m DFT of a; the inverse is chirpZ(., w^-1, m) scaled by 1/m.
// That gives cyclic convolution of ANY length m in O(m log m) without a fold.
// THE OTHER DIRECTION: given the m values P(z^k), recovering P is a chirp-z with z^-1 followed by
// a rescale - the transform is its own inverse up to the chirp factors, exactly like a DFT.
// EVALUATING AT AN ARITHMETIC progression instead (P(a), P(a+d), ...): that is a Taylor SHIFT plus
// a scale, see 10 - PolyShiftAndComposition.cpp, also O(n log n).
// OVERFLOW: tri(k) is taken mod (NMOD - 1) by Fermat, so k may be as large as you like; without
// that reduction k*(k-1)/2 overflows around k = 4e9.
// RELATED: the same "split the exponent" trick underlies baby-step giant-step for discrete log and
// the O(sqrt(n)) evaluation of n! mod p.
