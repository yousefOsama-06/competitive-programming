// COUNT THE NONNEGATIVE LATTICE POINTS UNDER A LINE:  #{(x, y) : x, y >= 0, a*x + b*y <= c}
// in O(log min(a, b)) - a Euclidean-style descent that peels off a full rectangle of solutions
// each round, never touching a single point individually.
// This is the "how many ways to pay at most c with coins a and b" primitive, and the engine
// behind 06 - ModCountInRange.cpp. Requires a >= 1, b >= 1; returns 0 for c < 0 (a = 0 or b = 0
// would be infinite - handle that case yourself).
// OVERFLOW: the answer is about c^2 / (2ab) and the intermediate (f+1)*f/2*k is of the same
// order, so it must fit in ll; c up to ~4e18 is fine as long as the count itself does.
ll latticeUnderLine(ll a, ll b, ll c) {
    if (c < 0) return 0;
    if (a > b) swap(a, b);                              // descend on the smaller coefficient
    ll ans = 0;
    while (c >= 0) {                                    // b = k*a + l, then swap roles
        ll k = b / a, l = b % a, f = c / b, e = c % b / a, g = c % b % a;
        ans += (f + 1) * (e + 1) + (f + 1) * f / 2 * k; // the axis-aligned part
        c = f * l - a + g;                              // l = 0 makes c < 0: the loop ends
        b = a, a = l;
    }
    return ans;
}
// VARIANTS, all from this one call:
//   a*x + b*y <  c        -> latticeUnderLine(a, b, c - 1)
//   a*x + b*y <= c, x >= 1, y >= 1  -> latticeUnderLine(a, b, c - a - b)
//   a*x + b*y == c exactly -> latticeUnderLine(a,b,c) - latticeUnderLine(a,b,c-1), or solve the
//     Diophantine equation and count the admissible k directly (02 - LinearDiophantine.cpp).
//   x in [0, X] as well   -> subtract the shifted count latticeUnderLine(a, b, c - a*(X+1)).
// Related: sum_{x} floor((c - a x)/b) is the same quantity as a floor sum, so 03 - FloorSum.cpp
// also answers it in O(log) - use whichever shape the problem hands you.
