// The four searches. Getting the boundary right here saves more contest time than any template.

// BINARY SEARCH ON A PREDICATE. pred must be monotone: F F F T T T.
// firstTrue returns the first x in [lo, hi] with pred(x), or hi+1 if none.
template <class F> ll firstTrue(ll lo, ll hi, F pred) {
    hi++;
    while (lo < hi) { ll m = lo + (hi - lo) / 2; pred(m) ? hi = m : lo = m + 1; }
    return lo;
}
// lastTrue returns the last x in [lo, hi] with pred(x), or lo-1 if none.  T T T F F F
template <class F> ll lastTrue(ll lo, ll hi, F pred) {
    lo--;
    while (lo < hi) { ll m = lo + (hi - lo + 1) / 2; pred(m) ? lo = m : hi = m - 1; }
    return lo;
}
// Reals: fixed iteration count beats an epsilon loop (no infinite loops, predictable time).
// 100 iterations halve the range 2^100 times - always enough.
// Returns the boundary: pred is false on [lo, x) and true on [x, hi]. (`lo` and `hi` converge.)
template <class F> ld binReal(ld lo, ld hi, F pred, int it = 100) {
    while (it--) { ld m = (lo + hi) / 2; pred(m) ? hi = m : lo = m; }
    return hi;                                                 // first TRUE, matching firstTrue
}

// TERNARY SEARCH on a UNIMODAL function (strictly decreasing then increasing => minimum).
// Integer version: stop when the window is tiny, then scan. Never use `l < r` alone.
template <class F> ll ternaryInt(ll lo, ll hi, F f) {          // returns argmin
    while (hi - lo > 2) {
        ll m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3;
        f(m1) <= f(m2) ? hi = m2 : lo = m1;                    // <= keeps the LEFTmost min
    }
    ll best = lo;
    for (ll x = lo; x <= hi; x++) if (f(x) < f(best)) best = x;
    return best;
}
template <class F> ld ternaryReal(ld lo, ld hi, F f, int it = 200) {
    while (it--) { ld m1 = lo + (hi - lo) / 3, m2 = hi - (hi - lo) / 3; f(m1) < f(m2) ? hi = m2 : lo = m1; }
    return (lo + hi) / 2;
}
// If f is only WEAKLY unimodal (has flat regions), ternary search is WRONG: one comparison
// f(m1) == f(m2) inside a plateau discards the side holding the optimum. For any DISCRETELY
// CONVEX f (non-decreasing difference sequence, plateaus allowed) use the derivative instead -
// this is strictly safer AND uses half the evaluations:
template <class F> ll convexArgmin(ll lo, ll hi, F f) {
    return firstTrue(lo, hi - 1, [&](ll x) { return f(x + 1) >= f(x); });
}

// NEWTON'S METHOD for roots of a smooth f (quadratic convergence; needs a decent start).
template <class F, class DF> ld newton(ld x, F f, DF df, int it = 60) {
    while (it--) { ld d = f(x) / df(x); x -= d; if (fabsl(d) < 1e-15L * max((ld)1, fabsl(x))) break; }
    return x;
}
// Integer sqrt / cbrt with correction - NEVER trust sqrt() alone near 1e18.
ll isqrt(ll n) { ll r = (ll)sqrtl((ld)n); while (r > 0 && r * r > n) r--; while ((r + 1) * (r + 1) <= n) r++; return r; }
ll icbrt(ll n) { ll r = (ll)cbrtl((ld)n); while (r > 0 && r * r * r > n) r--; while ((r + 1) * (r + 1) * (r + 1) <= n) r++; return r; }

// BINARY SEARCH ON THE ANSWER - the pattern, not the code:
//   "minimise the maximum X" / "maximise the minimum X" / "can we finish within T"
//   -> guess the answer, write a GREEDY feasibility check, binary search on it.
// PARALLEL BINARY SEARCH - when you need this for MANY queries at once: bucket the queries by
// their current midpoint, sweep the shared structure once per level, O(log) sweeps total.
