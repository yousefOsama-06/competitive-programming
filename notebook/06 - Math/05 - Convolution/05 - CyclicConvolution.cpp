// Needs: multiply (02 - NTT.cpp)  [or swap in conv() from 01 - FFT.cpp].
// CYCLIC CONVOLUTION and CYCLIC CORRELATION of two length-n arrays, O(n log n).
// Cyclic = multiplication in the ring modulo x^n - 1: indices wrap around instead of growing.
// REACH FOR IT whenever the index set is a CIRCLE rather than a line: necklaces and rotations,
// "for every rotation r, the score of aligning a against b rotated by r", counting pairs (i, j)
// with (i + j) mod n = k, and any DP on residues modulo n.
typedef vector<ll> Poly;
Poly cyclicConv(const Poly& a, const Poly& b, int n) {   // c[k] = sum_{(i+j) mod n == k} a[i]b[j]
    Poly p = multiply(a, b), c(n, 0);
    for (size_t i = 0; i < p.size(); i++) c[i % n] = (c[i % n] + p[i]) % NMOD;
    return c;
}
// c[k] = sum_i a[i] * b[(i + k) mod n]  -- "slide b by k over a and take the dot product".
// This is the one you actually want for rotations. Reverse a cyclically, then convolve.
Poly cyclicCorr(const Poly& a, const Poly& b, int n) {
    Poly ar(n);
    for (int i = 0; i < n; i++) ar[i] = a[(n - i) % n];
    return cyclicConv(ar, b, n);
}
// NEGACYCLIC convolution (modulo x^n + 1, "wrap with a sign flip"): same fold but subtract the
// wrapped part, c[i % n] += (i / n odd ? -p[i] : p[i]). Shows up in ring-LWE style problems and in
// the Schoenhage-Strassen recursion.
// FASTER when n | (NMOD - 1): a length-n NTT IS the cyclic convolution - transform, multiply
// pointwise, transform back, with no padding to a power of two and no fold. When n is arbitrary,
// Bluestein (06 - ChirpZ.cpp) gives you the length-n DFT anyway.
// USES
//  ROTATION SCORING: given two binary necklaces, the number of positions where both are 1 under
//  rotation r is exactly cyclicCorr(a, b)[r] - one call answers all n rotations.
//  RESIDUE DP: "number of pairs whose sum is congruent to k mod n" for all k at once.
//  CIRCULAR STRING MATCHING with wildcards: encode as sums of products (see the wildcard trick in
//  07 - Strings/01 - String Matching/04) and fold cyclically.
//  A circulant matrix times a vector IS a cyclic convolution, so this is also how you apply a
//  circulant in O(n log n) instead of O(n^2).
