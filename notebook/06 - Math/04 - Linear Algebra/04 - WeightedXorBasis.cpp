// WEIGHTED XOR BASIS - keep the MAXIMUM-WEIGHT independent set of vectors over GF(2). O(B) per
// insert, online, no sorting and no rollback needed.
// The vectors of a matroid's ground set with a "keep the heaviest" greedy: linear independence is
// a MATROID, so the greedy is optimal. Standard basis insertion keeps whichever vector arrives
// first; this one keeps the heavier and pushes the lighter down to be re-inserted, which is the
// same as running Kruskal's exchange argument implicitly.
// REACH FOR IT: "pick a maximum-weight subset of vectors that stays linearly independent",
// "maximum spanning forest over GF(2) constraints", cycle-space problems where each edge carries
// both an xor value and a cost, and any "add items online, report the best independent set now".
struct WBasis {
    static const int B = 62;
    ll b[B + 1] = {};                                   // b[i] has leading bit i
    ll w[B + 1] = {};                                   // weight of the vector stored in b[i]
    int sz = 0;

    void insert(ll x, ll wt) {
        for (int i = B; i >= 0; i--) if (x >> i & 1) {
            if (!b[i]) { b[i] = x, w[i] = wt, sz++; return; }
            if (w[i] < wt) swap(b[i], x), swap(w[i], wt);   // keep the heavier one at this pivot
            x ^= b[i];
        }
    }
    ll totalWeight() const { ll s = 0; for (int i = 0; i <= B; i++) s += w[i]; return s; }
    ll maxXor(ll r = 0) const { for (int i = B; i >= 0; i--) r = max(r, r ^ b[i]); return r; }
};
// MINIMUM-weight independent set: negate the weights (or compare with > instead of <).
// THE TRAP: you may NOT just sort by weight and insert greedily into a plain basis if the vectors
// arrive online - but if they are all known up front, sorting descending and using the ordinary
// Basis::insert gives exactly the same answer and is simpler. Use this version when insertions are
// interleaved with queries.
// MATROID INTERSECTION generalises this to "independent in TWO matroids at once" (e.g. linearly
// independent AND a forest); that needs augmenting paths in an exchange graph, not a greedy.
// LINEAR MATROID over a general field: same greedy with Gaussian row-reduction instead of xor.
// TYPICAL ENCODING TRICK: to force "at most one vector per group", append group-identifier bits
// above the value bits (a 128-bit word via __int128 gives you 62 value bits plus 66 tag bits).
// That turns "spanning-forest + xor" constraints into one basis insert per edge.
