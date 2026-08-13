// DYNAMIC SUBMASK COUNTING - a multiset of masks with INSERT, ERASE and the query
//     "how many stored masks a satisfy  a & s == a  (a is a submask of s)"
// in O(2^(B/2)) per operation, B = number of bits. SOS DP answers this in O(1) but only for a
// STATIC multiset (any insert costs a full O(B 2^B) rebuild); enumerating submasks of s per query
// is O(2^B). This is the meet-in-the-middle between the two, and the technique generalises to any
// "precompute forwards for heavy items, backwards for light items" split.
// THE SPLIT: call a mask HEAVY if popcount > B/2, LIGHT otherwise.
//   LIGHT masks are stored twice: once at their own index, and once at EVERY SUBMASK of theirs
//     (<= 2^(B/2) of them) in sup[], so sup[t] = #light masks that CONTAIN t.
//   HEAVY masks are stored at every SUPERMASK of theirs (<= 2^(B/2) of them) in sub[],
//     so sub[t] = #heavy masks that are CONTAINED in t.
// A query with a light s only needs light answers (a submask of s has popcount <= popcount(s)),
// and is a submask enumeration over s, <= 2^(B/2) steps. A query with a heavy s reads sub[s] for
// the heavy part and inclusion-exclusion over the ZERO bits of s (there are < B/2 of them) for
// the light part: #light masks meeting ~s = sum over non-empty T in ~s of (-1)^(|T|+1) sup[T].
template <int B>
struct SubmaskCounter {
    static const int SZ = 1 << B, H = B / 2;
    vector<int> own, sup, sub;                                 // own[m] = #light copies of m
    int lightTotal = 0;
    SubmaskCounter() : own(SZ, 0), sup(SZ, 0), sub(SZ, 0) {}
    void modify(int m, int d) {                                // d = +1 to insert, -1 to erase
        if (__builtin_popcount(m) <= H) {
            own[m] += d, lightTotal += d;
            for (int t = m;; t = (t - 1) & m) { sup[t] += d; if (!t) break; }
        } else {
            int zeros = ((SZ - 1) ^ m);
            for (int t = zeros;; t = (t - 1) & zeros) { sub[m | t] += d; if (!t) break; }
        }
    }
    void insert(int m) { modify(m, 1); }
    void erase(int m) { modify(m, -1); }
    int count(int s) {                                         // #stored masks that are submasks
        if (__builtin_popcount(s) <= H) {
            int r = 0;
            for (int t = s;; t = (t - 1) & s) { r += own[t]; if (!t) break; }
            return r;
        }
        int zeros = ((SZ - 1) ^ s), bad = 0;
        for (int t = zeros; t; t = (t - 1) & zeros)            // inclusion-exclusion, T non-empty
            bad += (__builtin_popcount(t) & 1) ? sup[t] : -sup[t];
        return sub[s] + lightTotal - bad;
    }
};
/* NOTES
 THE THRESHOLD H = B/2 balances 2^H against 2^(B-H); with skewed workloads (many more queries
   than inserts) move it - all four loops stay correct for any H.
 MEMORY is 3 * 2^B ints: B = 20 is 12 MB, B = 22 is 50 MB. Above that, drop `own` (it equals the
   light part of a separate map) or switch to a hash map for the sparse arrays.
 SUPERMASK COUNTING ("how many stored a with a & s == s") is the same code with the roles of sup
   and sub swapped, or just complement everything: a superset of s is a submask of ~s complemented.
 EXACTLY-EQUAL, OR "how many a with a & s == 0" (disjoint) = count(~s) - both fall out for free.
 STATIC ALTERNATIVE: if all inserts precede all queries, run SOS DP once and answer in O(1). Check
   that first; this structure is only for genuinely interleaved updates and queries. */
