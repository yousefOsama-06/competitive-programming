// Needs: latticeUnderLine (05 - LatticeUnderLine.cpp).
// HOW MANY x IN [xl, xr] HAVE (a*x mod m) INSIDE [cl, cr]?   O(log m), any a, m >= 1.
// The classic use: "how many of the first n multiples of a land in a window mod m" - collision
// counting, clock/lap problems, counting i with (a*i + b) mod m <= c (shift by b: replace the
// window [cl, cr] by [cl - b, cr - b] reduced mod m, splitting it if it wraps past 0).
// modCountLE counts 0 <= x <= n with (a*x mod m) <= c; everything else is inclusion-exclusion
// on that. OVERFLOW: latticeUnderLine is called with c ~ 2*m*n, so m*n must stay below ~4e18
// (use __int128 inside latticeUnderLine if it does not).
ll modCountLE(ll a, ll m, ll c, ll n) {                 // count 0 <= x <= n with a*x mod m <= c
    if (n < 0) return 0;
    ++c, ++n;                                           // work with half-open [0, c) and [0, n)
    a %= m; if (a < 0) a += m;
    ll ec = c / m; c %= m; if (c < 0) ec--, c += m;     // c outside [0, m) means whole periods
    ll ans = ec * n, en = n / m; n %= m;
    if (n < 0) en--, n += m;
    auto blk = [&](ll len) {                            // x in [0, len): len < m, uses a + m > 0
        return latticeUnderLine(m, a + m, (a + m) * (len - 1))
             - latticeUnderLine(m, a + m, (a + m) * (len - 1) - c);
    };
    if (en) ans += en * blk(m);                         // en full periods of length m
    if (n) ans += blk(n);
    return ans;
}
ll modCountRange(ll a, ll m, ll cl, ll cr, ll xl, ll xr) {
    return modCountLE(a, m, cr, xr) - modCountLE(a, m, cr, xl - 1)
         - modCountLE(a, m, cl - 1, xr) + modCountLE(a, m, cl - 1, xl - 1);
}
// SANITY CHECK: with g = gcd(a, m) the values a*x mod m are exactly the multiples of g, and over
// one full period x in [0, m) each of them is hit exactly g times. So modCountRange over a full
// period must equal g * (number of multiples of g inside [cl, cr]).
// COMPANION ROUTINES: the smallest such x is 07 - SmallestModInRange.cpp; if you need the whole
// set of x, note that it is a union of O(sqrt m) arithmetic progressions
// (03 - Modular Arithmetic/09 - ModularAPBlocks.cpp).
