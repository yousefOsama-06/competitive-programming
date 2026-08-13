// Needs: crt2 (04 - CongruencesAndTowers.cpp) or CRT (01 - CRT.cpp).
// INTERSECTION OF TWO ARITHMETIC PROGRESSIONS  {a1 + d1*i : i >= 0} and {a2 + d2*j : j >= 0}.
// The intersection is itself an AP with difference lcm(d1, d2), whose first term is the smallest
// common value that is >= max(a1, a2). Empty exactly when the CRT system is unsolvable, i.e.
// when (a1 - a2) is not divisible by gcd(d1, d2).
// Returns {first, step}; step = 0 means EMPTY. O(log) via one CRT.
// WHEN: merging periodic events ("both traffic lights green"), intersecting the answer sets of
// several modular constraints, and range-counting over a set defined by many congruences.
// OVERFLOW: lcm(d1, d2) can reach d1*d2 - use __int128 inside CRT (the notebook version does).
pair<ll, ll> intersectAP(ll a1, ll d1, ll a2, ll d2) {
    auto [r, m] = crt2(((a1 % d1) + d1) % d1, d1, ((a2 % d2) + d2) % d2, d2);
    if (m == -1) return {0, 0};                         // no common term at all
    ll start = max(a1, a2);
    if (r < start) r += (start - r + m - 1) / m * m;    // advance to the first term >= start
    return {r, m};
}
// COUNTING the common terms in [lo, hi] is then (hi - first)/step + 1 when first <= hi.
// MANY progressions: fold them left to right with this function; the difference multiplies up to
// the lcm of all of them, so it overflows fast - keep an eye on it, or cap at "beyond the range".
// THE INFINITE-IN-BOTH-DIRECTIONS version (i and j any integer) is just the CRT answer with no
// max(a1, a2) clamp - that is the subgroup a1 + gcd-shifted lattice, see also
// 01 - Euclidean Algorithm/10 - IntegerLattice2D.cpp for the two-coordinate analogue.
