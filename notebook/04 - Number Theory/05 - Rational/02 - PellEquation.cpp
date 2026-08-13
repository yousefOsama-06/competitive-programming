// PELL'S EQUATION  x^2 - D*y^2 = 1  (D > 0 and NOT a perfect square) - the fundamental solution,
// then all of them. Every solution is a CONVERGENT of the continued fraction of sqrt(D), so walk
// the convergents and stop at the first one that satisfies the equation: O(sqrt D) steps, since
// the period of sqrt(D) is O(sqrt D log D).
// ALL SOLUTIONS from the fundamental (x1, y1):  x_{m+1} = x1*x_m + D*y1*y_m,
//                                               y_{m+1} = x1*y_m + y1*x_m,
// i.e. x_m + y_m sqrt(D) = (x1 + y1 sqrt(D))^m. So there are infinitely many, growing
// EXPONENTIALLY - D = 61 already needs x = 1766319049, and D = 661 needs 39 digits. __int128
// carries you to about D = 200; past that use bignum, or work modulo the required output.
// Returns {0, 0} when D is a square or <= 0 (only the trivial (1, 0) exists).
pair<__int128, __int128> pell(ll D) {
    ll a0 = (ll)sqrtl((ld)D);
    while (a0 * a0 > D) a0--;
    while ((a0 + 1) * (a0 + 1) <= D) a0++;
    if (D <= 0 || a0 * a0 == D) return {0, 0};
    ll m = 0, d = 1, a = a0;                            // the exact CF recurrence for sqrt(D)
    __int128 h1 = 1, h = a0, k1 = 0, k = 1;             // convergents h/k
    while (h * h - (__int128)D * k * k != 1) {
        m = d * a - m;
        d = (D - m * m) / d;
        a = (a0 + m) / d;
        __int128 h2 = h1; h1 = h; h = a * h1 + h2;
        __int128 k2 = k1; k1 = k; k = a * k1 + k2;
    }
    return {h, k};
}
// NEGATIVE PELL  x^2 - D*y^2 = -1 is solvable IFF the period of the CF of sqrt(D) is ODD; the
// same loop finds it if you test for -1 instead (and it always appears before the +1 solution).
// Necessary condition: D has no prime factor congruent to 3 mod 4 (not sufficient: D = 34).
// GENERAL x^2 - D*y^2 = k: find one solution by brute force / Lagrange-Matthews-Mollin, then
// multiply by powers of the fundamental unit to walk the whole class; there can be several
// inequivalent classes, so collect every solution with y below the standard bound.
// CF MACHINERY, exact in integers (a0 = floor(sqrt D), start m = 0, d = 1, a = a0):
//   m' = d*a - m,   d' = (D - m'^2)/d,   a' = floor((a0 + m')/d')   - the period ends when
//   a' = 2*a0, and the a-sequence between is a palindrome.
// USES: minimal solutions of "x^2 - D y^2 = 1" style Diophantine puzzles, the fundamental unit of
// the ring Z[sqrt D], square-triangular numbers (D = 8), and any recurrence whose growth factor
// is a quadratic irrational.
