// Needs: 06 - Math/03 - Matrices/01 - MatrixExponentiation.cpp   (Mat, operator*, mpow)
// TRANSFER-MATRIX DP - when the DP has a HUGE time dimension (n up to 1e18) but few states.
// RECOGNITION TEST: dp[t] is a vector over S states, and dp[t+1] = M * dp[t] with M INDEPENDENT
// of t. Then dp[n] = M^n * dp[0], computed in O(S^3 log n). If M depends on t only through a
// small period p, multiply the p matrices once and exponentiate the product.
// WHEN: count strings of length 1e18 avoiding a pattern, count walks of a given length in a
// graph, count tilings of a 4 x 1e18 strip, any linear recurrence with n astronomically large.
// PRECONDITIONS, all checkable: (1) the transition is LINEAR in the dp values (sums of products
// with CONSTANTS - "max" is not linear, see min-plus below); (2) the state set does not grow with
// t; (3) the same matrix applies at every step (or with a fixed period).
// Build the matrix by RUNNING ONE STEP of the DP from each unit vector - that is mechanical and
// far less error-prone than deriving the entries by hand.
template <class Step>
Mat transferMatrix(int S, Step step) {                         // step(from, add) enumerates edges
    Mat M(S, vector<Mint>(S, 0));
    for (int i = 0; i < S; i++) step(i, [&](int j, Mint w) { M[j][i] += w; });
    return M;                                                  // COLUMN convention: dp' = M * dp
}
// EXAMPLE - count strings of length n over an alphabet of size A that avoid a set of patterns:
// states = the nodes of the Aho-Corasick automaton with terminal nodes deleted, and the step is
//   for each node u, for each character c: v = advance(u, c); if v is not terminal, add 1 edge.
// dp[0] = e_root, and the answer is the sum of the entries of M^n * dp[0].     O(S^3 log n)
/* THE VARIANTS
 MIN-PLUS ("shortest walk with exactly n edges", "minimum cost after n steps"): replace (+, *)
   with (min, +) and exponentiate the same way - see 06 - Math/03/07 - MinPlusMatrixPower.cpp.
   The matrix product is still associative, which is ALL that exponentiation needs; only the
   O(S^3) per multiply survives, there is no Strassen and no inverse.
 KITAMASA / BOSTAN-MORI (06 - Math/06/03) computes the n-th term of a linear recurrence of order
   k in O(k log k log n) instead of O(k^3 log n). If your matrix is a companion matrix - i.e. the
   DP is a plain linear recurrence - use that instead; k = 1000 is hopeless for matrices and easy
   for Bostan-Mori.
 BERLEKAMP-MASSEY finds the recurrence for you: generate 2k terms with the slow DP, recover the
   order-k recurrence, then jump to n. This is the practical route when the state space is
   structured but you cannot describe the matrix cleanly.
 PERIODIC / TIME-DEPENDENT transitions: if M_t has period p, exponentiate (M_{p-1} ... M_0).
 PROFILE DPs on a k x n grid become transfer matrices over the 2^k profiles - that is how "count
   tilings of a 5 x 1e18 board" is solved (and 2^k must stay under ~200 for the cube to fit).
 EIGENVALUES / CLOSED FORMS are almost never worth it in a contest: the matrix power is O(S^3
   log n) with no numerical risk, and a closed form needs distinct eigenvalues you cannot verify.
 SANITY: check M^1 and M^2 against the slow DP for small n before trusting M^1e18. */
