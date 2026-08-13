// SUBSET UNION SIZES - given m <= ~20 sets over the ground set {0..n-1}, get |union of the chosen
// sets| for EVERY one of the 2^m subsets, in O(n + 2^m * m). Output u[S] for every mask S over the
// m sets, u[S] = number of ground elements covered by at least one set of S; u[0] == 0.
// THE TRICK IS THE TRANSPOSE, everything else is bookkeeping. You are handed "set j contains these
// elements"; flip it to own[e] = the mask of sets that contain element e. Then element e is MISSED
// by S iff S is disjoint from own[e] iff S is a SUBSET of ~own[e]. So drop one token at ~own[e] for
// each e and run a SUPERSET-sum (SOS) transform: cnt[S] = #{e : ~own[e] contains S} = number of
// elements S misses, and the union size is n - cnt[S]. Counting the complements is what turns a
// union (no closed form under subsets) into a plain subset-lattice sum.
// WHEN: "cheapest set of towers covering >= k houses", "for each mask, how much does it cover",
// any 2^m enumeration whose inner cost is a union - it removes the O(n / 64) bitset OR per mask.
// COMPLEXITY: O(n + 2^m * m) time, 2^m ints = 4 MB at m = 20 (plus 8 MB if you also build costs).
// DEGENERATE: m == 0 gives the single answer u[0] == 0. n == 0 gives all zeros. Duplicate or empty
// sets are fine. An element in no set has own[e] == 0, is missed by every S, and is never counted.
// EXACTNESS: pure integer counting, no overflow (u fits int as long as n does). Weighted version:
// replace the ++ by += w[e] and use ll - then the budget is sum|w| <= 9e18.
vi transposeSets(int n, const vector<vi>& sets) {          // sets[j] = the elements of set j
    vi own(n, 0);                                          // own[e] = mask of sets holding e
    for (int j = 0; j < sz(sets); j++) for (int e : sets[j]) own[e] |= 1 << j;
    return own;
}
vi subsetUnionSize(int n, int m, const vi& own) {
    int full = (1 << m) - 1;
    vi u(1 << m, 0);
    for (int e = 0; e < n; e++) u[own[e] ^ full]++;        // bucket e at "the sets that MISS e"
    for (int b = 0; b < m; b++)                            // SOS over SUPERSETS: pull from S | bit
        for (int S = 0; S <= full; S++)                    // into S. Entries with bit b set are not
            if (!(S >> b & 1)) u[S] += u[S | 1 << b];      // touched in this pass, so order is free
    for (int S = 0; S <= full; S++) u[S] = n - u[S];       // missed -> covered
    return u;
}
// --- THE DUAL CONSUMER: cheapest subset covering AT LEAST k elements, for every k. O(2^m * m).
vector<ll> minCostToCover(int n, int m, const vi& own, const vector<ll>& cost) {
    vi u = subsetUnionSize(n, m, own);
    int full = (1 << m) - 1;
    vector<ll> c(1 << m, 0), best(n + 1, llinf);
    for (int S = 1; S <= full; S++) c[S] = c[S ^ (1 << lsb(S))] + cost[lsb(S)];   // cost by lowest
    for (int S = 0; S <= full; S++) best[u[S]] = min(best[u[S]], c[S]);           // set bit
    for (int k = n - 1; k >= 0; k--) best[k] = min(best[k], best[k + 1]);   // exactly -> at least
    return best;                                           // best[k] == llinf: k is unreachable
}
// VARIANTS
//  - INTERSECTION SIZES for every S: bucket at own[e] instead of ~own[e] and run the SAME superset
//    SOS - #{e : own[e] contains S} is exactly |intersection of S|. One character of difference.
//  - "covered by EXACTLY the sets in S": the raw histogram of own[e], no transform at all.
//  - MAX / MIN over the covered set instead of a count: SOS with max / min in place of +.
//  - SUBSET sum (Zeta over subsets) is the mirror: if (S >> b & 1) f[S] += f[S ^ 1 << b]. Mobius
//    (the inverse) is the same loop with -=, which recovers "exactly" counts from "at least".
//  - m > 20: meet in the middle over two halves of the sets, or drop to a greedy / randomised
//    cover - 2^m unions is the whole budget and it is 1e6 masks at m = 20.
//  - The ground set may be huge: only n and 2^m matter, never n * m or n / 64 words per mask.
