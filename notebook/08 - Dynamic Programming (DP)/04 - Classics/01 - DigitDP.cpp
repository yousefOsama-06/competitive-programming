// DIGIT DP - count numbers in [L, R] with a digit property. f(R) - f(L-1).
// State: (position, tight, started, problem state). `started` handles leading zeros.
// THIS CODE IS BASE 10 (to_string, digit cap 9, memo[20] positions). For another base, write the
// digits yourself, change the 9, and size memo by the digit count (base 2 needs 64 positions).
// L - 1 IS NOT ALWAYS AVAILABLE: if the bounds are given as huge decimal STRINGS you cannot
// subtract one, so carry two tight flags (tightLo, tightHi) and count [L, R] in a single pass.
string DIG;
ll memo[20][2][2][200];
bool vis[20][2][2][200];

ll go(int p, int tight, int started, int st) {
    if (p == (int)DIG.size()) return started ? /* ACCEPT? */ (st == 0) : 0;   // <- edit
    if (vis[p][tight][started][st]) return memo[p][tight][started][st];
    vis[p][tight][started][st] = true;
    ll res = 0;
    int hi = tight ? DIG[p] - '0' : 9;
    for (int d = 0; d <= hi; d++) {
        int nStarted = started || d > 0;
        int nSt = nStarted ? (st + d) % 10 : 0;                               // <- edit
        res += go(p + 1, tight && d == hi, nStarted, nSt);
    }
    return memo[p][tight][started][st] = res;
}
bool qualifiesZero = 1;                          // does the number 0 satisfy the predicate?
ll countUpTo(ll x) {
    if (x < 0) return 0;
    DIG = to_string(x);
    memset(vis, 0, sizeof vis);
    return go(0, 1, 0, 0) + qualifiesZero;       // set qualifiesZero = 1 only if 0 itself counts
}
ll countRange(ll L, ll R) { return countUpTo(R) - countUpTo(L - 1); }
// VARIANTS: track (sum of digits), (value mod k), (last digit), (a bitmask of used digits),
// (whether we already saw "13"), (count so far) - anything with a small state.
// For "k-th number with the property", binary search on countUpTo.
