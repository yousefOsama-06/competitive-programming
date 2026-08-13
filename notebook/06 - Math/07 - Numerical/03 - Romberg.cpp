// ROMBERG INTEGRATION - the other numerical quadrature. Repeatedly halve the step (trapezoid rule)
// and RICHARDSON-EXTRAPOLATE the error away: T(h) has an error expansion in h^2 only, so combining
// two step sizes cancels the h^2 term, three cancel h^4, and so on. Row k of the table is accurate
// to O(h^(2k)). Cost: 2^k function evaluations, each sample reused across the whole table.
// REACH FOR IT over adaptive Simpson (07 - Numerical/01) when the integrand is SMOOTH and cheap
// and you want many correct digits: Romberg reaches 1e-12 in ~10 rows where Simpson needs a deep
// adaptive recursion. Use adaptive Simpson instead when the function has kinks, spikes, or a
// singular endpoint - Romberg assumes the error expansion exists and quietly lies when it does not.
template <class F> ld romberg(F f, ld a, ld b, ld tol = 1e-10, int maxIter = 25) {
    vector<ld> t;
    ld h = b - a;
    t.push_back(h * (f(a) + f(b)) / 2);                  // T(h), one trapezoid
    for (int k = 1, m = 1; k <= maxIter; k++, m <<= 1) {
        ld s = 0, x = a + h / 2;
        for (int j = 0; j < m; j++) s += f(x), x += h;   // only the NEW midpoints
        h /= 2;
        ld cur = t[0] / 2 + h * s;                       // refined trapezoid, previous row's T
        ld prev = t.back();
        for (int j = 0; j < k; j++) {                    // Richardson: t[j] holds the old row
            ld nxt = (powl(4, j + 1) * cur - t[j]) / (powl(4, j + 1) - 1);
            t[j] = cur, cur = nxt;
        }
        t.push_back(cur);
        if (k > 3 && fabsl(prev - cur) < tol) break;
    }
    return t.back();
}
// THE TABLE: R[k][0] = trapezoid with 2^k intervals, R[k][j] = (4^j R[k][j-1] - R[k-1][j-1])
// / (4^j - 1). R[k][1] is exactly Simpson's rule; R[k][2] is Boole's. The code above keeps only
// one row, which is all you need.
// PRACTICAL NOTES
//  - convergence check on |R[k][k] - R[k-1][k-1]| is a heuristic; require a few rows first (the
//    k > 3 guard) or a flat integrand will make it stop after one step.
//  - INFINITE range: substitute x = t/(1-t) or integrate to a large cut-off and bound the tail.
//  - ENDPOINT SINGULARITY (1/sqrt(x) at 0): substitute it away, or use the tanh-sinh (double
//    exponential) rule, which handles endpoint singularities and converges even faster.
//  - OSCILLATORY integrands: split at the zeros of the oscillating factor and sum, or Romberg will
//    happily converge to nonsense.
//  - the same Richardson idea accelerates ANY sequence with a known error expansion, e.g.
//    numerical derivatives (central differences) and extrapolating a DP in a step size.
