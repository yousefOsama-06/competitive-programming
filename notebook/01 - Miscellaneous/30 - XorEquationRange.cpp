// COUNT THE SOLUTIONS OF x_0 xor x_1 xor ... xor x_{n-1} == X with 0 <= x_i <= a[i], modulo M.
// Returns the count mod M. a[i] >= 0, X >= 0, M >= 1.
// WHEN: "how many tuples bounded componentwise xor to X" - Nim-position counting, POJ 3986, the
// stone-game family. The bounds are ARBITRARY; if every a[i] were 2^k - 1 the answer would just be
// 2^(k(n-1)) [X < 2^k], and this file is what you type when they are not.
// HOW: process bit k from the top. Split on whether every x_i copies a[i]'s bit k ("all tight") or
// at least one x_i drops a bit that a[i] has ("relaxed"). Relaxed is countable in one pass: the
// dropper is then free over ALL k lower bits, so designate the LOWEST-indexed dropper as the
// absorber - its lower bits are forced to whatever makes the xor come out right, factor 1 - and
// every other variable contributes its own independent count: a[i] - 2^k + 1 if it keeps bit k,
// 2^k if it drops it too, a[i] + 1 if a[i] has no bit k at all. A 4-state DP over
// (parity of chosen k-bits, has anyone dropped yet) does that in O(n) per bit. The all-tight case
// forces the k-th bit of the xor to the parity of #{i : a[i] has bit k}; if that misses X's bit k
// nothing survives, otherwise strip bit k everywhere and recurse to bit k-1. Bottoming out leaves
// exactly the single tuple x_i == a[i], hence the final +1.
// COMPLEXITY: O(n log(max a)) time, O(1) extra memory. No recursion, no globals.
// DEGENERATE: n == 0 returns [X == 0]. All a[i] == 0 returns [X == 0]. X with a bit above every
// a[i] returns 0. X == 0 always returns >= 1 (the all-zero tuple). M == 1 returns 0.
// EXACTNESS: exact modulo M for any M up to 9.2e18 - every product goes through __int128. Bounds
// a[i] up to 9.2e18 are fine (a[i] + 1 must not overflow, so keep a[i] < 2^63 - 1).
ll xorEqCount(vector<ll> a, ll X, ll M) {
    auto mul = [&](ll p, ll q) { return (ll)((__int128)p * q % M); };
    int K = 0;
    for (ll v : a) while (v >> K) K++;                        // every a[i] < 2^K
    if (X >> K) return 0;                                     // xor of values < 2^K stays < 2^K
    ll res = 0;
    for (int k = K - 1; k >= 0; k--) {
        ll two = (1LL << k) % M, dp[2][2] = {{1 % M, 0}, {0, 0}};   // dp[parity of bit k][dropped?]
        for (ll v : a) {
            ll nd[2][2] = {}, cnt = ((v & ~(1LL << k)) + 1) % M;   // v - 2^k + 1 if bit k is set,
            for (int p = 0; p < 2; p++) for (int f = 0; f < 2; f++) {   // v + 1 if it is not: the
                ll c = dp[p][f];                                        // SAME expression
                if (!c) continue;
                if (v >> k & 1) {
                    nd[p ^ 1][f] = (nd[p ^ 1][f] + mul(c, cnt)) % M;    // keep it: x_i in [2^k, v]
                    nd[p][1] = (nd[p][1] + (f ? mul(c, two) : c)) % M;  // drop it: x_i in [0, 2^k),
                } else nd[p][f] = (nd[p][f] + mul(c, cnt)) % M;         // and the FIRST dropper is
            }                                                           // the absorber: factor 1
            memcpy(dp, nd, sizeof dp);
        }
        res = (res + dp[X >> k & 1][1]) % M;                  // [1]: at least one variable dropped
        int par = 0;
        for (ll& v : a) if (v >> k & 1) par ^= 1, v ^= 1LL << k;        // strip, stay all-tight
        if (par != (int)(X >> k & 1)) return res;                       // all-tight branch dies
    }
    return (res + 1) % M;                                     // the surviving tuple is x_i == a[i]
}
// VARIANTS
//  - LOWER BOUNDS TOO (lo_i <= x_i <= hi_i): you cannot shift, xor is not linear under +. Split
//    each range into O(log) aligned blocks "fixed prefix + j free bits"; a block with j >= 1 free
//    bits plays the role of a dropper (one absorber, the rest contribute 2^j), so the same count
//    works over the O(n log A) blocks - at the cost of a product over per-variable block choices.
//  - AND / OR instead of xor: per bit the constraint decouples completely, so it is a plain
//    per-bit product, no absorber trick needed.
//  - Sum over all X of the answer is prod(a[i] + 1) - a free sanity check on the implementation.
//  - CONSTRUCTING one solution: run the same DP keeping the counts, then walk it back greedily.
//  - If all a[i] are equal to 2^k - 1 the answer is 2^(k(n-1)) for X < 2^k, 0 otherwise.
