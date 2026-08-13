// COUNTING SOLUTIONS OF  x_1 xor x_2 xor ... xor x_n = X  with  0 <= x_i <= a_i,  in O(n * B).
// The bounds are DIFFERENT per variable, which is what makes this hard: with a common bound
// 2^B - 1 the answer is just 2^(B(n-1)) [X < 2^B]. Digit DP over the bits with an "is this
// variable still tight" flag per variable would need 2^n states - the trick below avoids that.
// THE TRICK: classify a solution by the HIGHEST bit k at which some x_i first drops below a_i.
// Above bit k every variable equals a_i exactly, so that prefix is forced. At bit k, at least one
// variable with a_i's bit set chooses 0 and becomes FREE below k; every other variable is either
// still tight (its low bits are then bounded by a_i's low bits) or already free (2^k choices).
// One designated free variable absorbs the xor constraint on the low bits - it has 2^k choices
// and exactly ONE of them fixes the remaining xor - which is why the first freed variable
// contributes a factor of 1 and every later one a factor of 2^k.
// If NOBODY drops at bit k, the parity of the k-th bits is forced to popcount-parity, so the
// recursion continues to bit k-1 only when that parity matches X's k-th bit.
// PRECONDITION: a_i >= 0 and X >= 0; B must cover every a_i and X (bit B-1 is the top).
ll countXorSolutions(const vector<ll>& a, ll X, int B = 60) {
    int n = a.size();
    ll res = 0;
    for (int k = B - 1; k >= 0; k--) {
        ll low = (1LL << k) % MOD;
        // dp[freed][parity of the k-th bits chosen so far]
        ll dp[2][2] = {{1, 0}, {0, 0}};
        for (int i = 0; i < n; i++) {
            ll lo = a[i] & ((1LL << (k + 1)) - 1);             // bits k..0 of a_i
            ll nd[2][2] = {{0, 0}, {0, 0}};
            if (a[i] >> k & 1) {
                ll keep = (lo - (1LL << k) + 1) % MOD;         // x_i bit k = 1, low bits <= a_i
                for (int f = 0; f < 2; f++)
                    for (int p = 0; p < 2; p++) {
                        if (!dp[f][p]) continue;
                        nd[f][p ^ 1] = (nd[f][p ^ 1] + dp[f][p] * keep) % MOD;   // stay tight
                        ll fac = f ? low : 1;                  // the FIRST freed one absorbs xor
                        nd[1][p] = (nd[1][p] + dp[f][p] * fac) % MOD;            // drop to 0 here
                    }
            } else {
                ll ch = (lo + 1) % MOD;                        // x_i bit k = 0, low bits <= a_i
                for (int f = 0; f < 2; f++)
                    for (int p = 0; p < 2; p++) nd[f][p] = dp[f][p] * ch % MOD;
            }
            memcpy(dp, nd, sizeof dp);
        }
        res = (res + dp[1][(X >> k) & 1]) % MOD;
        int par = 0;                                           // nobody dropped at bit k
        for (int i = 0; i < n; i++) par ^= (a[i] >> k) & 1;
        if (par != ((X >> k) & 1)) return res;                 // the forced prefix already fails
    }
    return (res + 1) % MOD;                                    // the all-tight tuple x_i = a_i
}
/* VARIANTS
 SUM OF THE SOLUTIONS, or counting with x_i in [lo_i, hi_i]: inclusion-exclusion over the lower
   bounds (subtract the count with x_i <= lo_i - 1) - 2^n terms, so only for tiny n; otherwise
   carry a second "already below lo_i" flag, which doubles the state, not the exponent.
 AND / OR CONSTRAINTS instead of xor: the same "first variable that drops" decomposition works,
   with the parity condition replaced by the AND/OR of the k-th bits.
 XOR EQUAL TO AT MOST X: sum the answers over the prefixes of X, i.e. for each bit where X has a
   1, count solutions matching X above that bit and 0 there - one extra outer loop.
 LINEAR ALGEBRA ALTERNATIVE: if the bounds are all 2^B - 1 (or the variables range over a
   subspace), this is a rank computation over GF(2), not a DP - see 06 - Math/07 XOR basis.
 SANITY: n = 1 gives [X <= a_1]; all a_i = 2^B - 1 gives 2^(B(n-1)). */
