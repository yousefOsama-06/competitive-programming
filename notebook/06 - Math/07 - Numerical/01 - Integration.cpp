// ADAPTIVE SIMPSON - numeric integral of a black-box f on [a,b] to a requested absolute error.
// Simpson approximates f by a parabola through the two ends and the midpoint; "adaptive" means it
// splits an interval only where the parabola does not yet agree with the two half-parabolas.
// Reach for it when the answer is an integral you cannot do in closed form: area of a union of
// circles/ellipses cut by a line, area swept by a moving shape, probability over a continuous
// parameter, arc lengths. Cost is proportional to how wiggly f is, not to (b-a).
template <class F> ld simpson(F f, ld a, ld b) {
    return (b - a) * (f(a) + 4 * f((a + b) / 2) + f(b)) / 6;
}
template <class F> ld asr(F f, ld a, ld b, ld eps, ld S, int dep) {
    ld c = (a + b) / 2, L = simpson(f, a, c), R = simpson(f, c, b);
    if (dep <= 0 || fabsl(L + R - S) <= 15 * eps) return L + R + (L + R - S) / 15;
    return asr(f, a, c, eps / 2, L, dep - 1) + asr(f, c, b, eps / 2, R, dep - 1);
}
template <class F> ld integrate(F f, ld a, ld b, ld eps = 1e-9) {
    return asr(f, a, b, eps, simpson(f, a, b), 50);
}
// TRAPS. (1) If f has a KINK or a jump, split the interval at that point by hand and integrate the
// pieces - adaptive Simpson will burn its depth budget trying to resolve it. (2) The (L+R-S)/15
// term is Richardson extrapolation; keeping it makes the result much more accurate for free.
// (3) A too-loose eps on a nearly flat region can stop early and miss a spike: if f can be zero on
// most of the range and huge somewhere, integrate the pieces separately.
//
// OTHER NUMERICAL TOOLS WORTH THE PAGE:
// TERNARY SEARCH on a strictly unimodal f over reals: 100 iterations of
//   m1 = l + (r-l)/3, m2 = r - (r-l)/3;  f(m1) < f(m2) ? r = m2 : l = m1;   (minimising)
//   fixes the answer to ~1e-30 relative. On INTEGERS use binary search on the difference
//   f(m+1) - f(m) instead - the three-way split has off-by-one traps at the last two points.
// NEWTON'S METHOD for a root of g: x -= g(x) / g'(x). Doubles the correct digits per step but
//   needs a good start; bisect first if the sign changes, then Newton.
// BISECTION on a monotone predicate: 100 iterations of (l+r)/2 is always safe and never diverges.
// GOLDEN-SECTION SEARCH: like ternary search but one function evaluation per step instead of two -
//   worth it only when f is expensive.
// SOLVING f(x) = 0 WHERE f IS A POLYNOMIAL: find all real roots by recursing on the derivative
//   (roots of f' split the line into monotone pieces; bisect inside each).
// SIMULATED ANNEALING / HILL CLIMBING when the objective is not unimodal: start from several
//   random points, take a step of size T, accept worse solutions with probability exp(-dE/T),
//   cool T geometrically (T *= 0.995). Good for geometric-median / facility-location style
//   problems where an exact method is out of reach; always seed the RNG from the clock.
// WEISZFELD for the geometric median (minimise sum of Euclidean distances): iterate
//   x <- (sum p_i / |x - p_i|) / (sum 1 / |x - p_i|), guarding the case x == p_i. Converges
//   linearly; nested ternary search is the safer contest answer (the objective is convex).
