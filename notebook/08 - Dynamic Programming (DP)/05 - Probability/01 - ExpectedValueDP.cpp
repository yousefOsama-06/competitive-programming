// Needs: Mint (06 - Math/01) and solveLinear (06 - Math/04 - Linear Algebra/02 - GaussianElimination.cpp).
// EXPECTED VALUE / PROBABILITY DP.
// LINEARITY OF EXPECTATION FIRST: E[sum of indicators] = sum of P(indicator). Most "expected
// number of X" problems collapse to summing per-element probabilities with no DP at all.
// Only when states depend on each other CYCLICALLY do you need a linear system.

// E[i] = 1 + sum_j P[i][j] * E[j], with E[absorbing] = 0.
// Move the unknowns left:  E[i] - sum_j P[i][j] E[j] = 1  ->  (I - P) E = 1. O(n^3).
// Needs solveLinear from GaussianElimination.cpp (values are Mint, so probabilities must be
// given as modular fractions p * inv(q)).
vector<Mint> hittingTimes(const vector<vector<Mint>>& P, const vector<char>& absorbing) {
    int n = P.size();
    vector<vector<Mint>> A(n, vector<Mint>(n, 0));
    vector<Mint> b(n, 0);
    for (int i = 0; i < n; i++) {
        if (absorbing[i]) { A[i][i] = 1, b[i] = 0; continue; }
        A[i][i] = 1;
        for (int j = 0; j < n; j++) A[i][j] -= P[i][j];
        b[i] = 1;
    }
    return solveLinear(A, b);
}
/* ABSORBING MARKOV CHAIN, canonical form P = [Q R ; 0 I]:
     fundamental matrix N = (I - Q)^-1 ;  expected steps before absorption = N * 1
     absorption probabilities B = N * R  (B[i][j] = P(absorbed in j | started in i))

   TRIDIAGONAL SYSTEMS (grid random walks that only move left/right within a row) - O(n) with
   the Thomas algorithm instead of O(n^3):
     a_i x_{i-1} + b_i x_i + c_i x_{i+1} = d_i , a_1 = c_n = 0
     forward:  c'_i = c_i / (b_i - a_i c'_{i-1}) ,  d'_i = (d_i - a_i d'_{i-1}) / (b_i - a_i c'_{i-1})
     back:     x_n = d'_n ,  x_i = d'_i - c'_i x_{i+1}
   Diagonally dominant (always true for hitting times), so it is stable.

   TREES: substitute E_v = A_v * E_parent + B_v, push A and B up in one DFS, then resolve at the
   root and push back down. O(n). (Folklore - no canonical reference; derive it, do not trust it.)

   COMMON TRAPS
     - "expected value of a maximum" is NOT the maximum of expected values; sum over thresholds:
       E[max] = sum_{t>=1} P(max >= t).
     - Probabilities mod p: store p*inv(q); never mix doubles and Mint.
     - Infinite processes: E exists only if absorption has probability 1; check reachability first.
     - Conditional expectation: E[X] = sum_y P(Y=y) * E[X | Y=y]  - condition on the FIRST step.  */
