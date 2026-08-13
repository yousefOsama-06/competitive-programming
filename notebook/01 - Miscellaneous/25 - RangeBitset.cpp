// RANGE BITSET - a bitset with SET / RESET / FLIP of a whole range and POPCOUNT of a range, all
// in O((r - l) / 64 + 1). std::bitset cannot do range updates at all (its set()/reset()/flip()
// are either one bit or the whole thing), so anything that wants "turn on bits [l, r]" needs
// this. Bits are 0..n-1, ranges are INCLUSIVE on both ends, and l > r is a silent no-op.
// WHEN: a sweep that paints intervals into one bitset per colour ("recolour [l, r] to c" over
// 100 colours: AND out the range from the old bitset, OR it into the new one); interval DP
// reachability; "is any position in [l, r] still free" answered by findNext; knapsack-style DP
// where the transition is x |= x << k.
// COMPLEXITY: n/64 words. O((r-l)/64 + 1) per range op, O(1) test/set of one bit, O(n/64) for
// the word-parallel operators and for a shift. The point is the 64x constant, not the O.
// EXACT: pure unsigned integer, nothing can overflow.
// THE WORD-BOUNDARY TRAP - the only thing that ever goes wrong in this file:
//  - a range touches THREE pieces: the tail of word l>>6 (bits l&63..63), the whole words
//    strictly between, and the head of word r>>6 (bits 0..r&63).
//  - if l>>6 == r>>6 you must do ONE masked write and return. Falling through to the three-piece
//    path writes bits l&63..63 AND bits 0..r&63 of that same word, i.e. the entire word - the
//    classic off-by-one that makes this silently wrong only when l and r are 64-aligned near
//    each other. Test n = 64, 65, 128 and l, r in the same word.
//  - never shift by 64: ~0ULL << 64 is UB (x86 shifts by 0 and hands you all-ones). mask(a, b)
//    below shifts by a and by 63-b with 0 <= a <= b <= 63, so both counts are in [0, 63].
//  - keep the bits >= n zero (trim()), or count() and findNext() over the whole bitset lie.
typedef unsigned long long ull;
struct RangeBitset {
    int n; vector<ull> w;                                   // w[i] holds bits 64i .. 64i + 63
    RangeBitset(int n = 0) : n(n), w((n + 63) / 64, 0) {}
    static ull mask(int a, int b) {                         // bits a..b of ONE word, 0<=a<=b<=63
        return (~0ULL << a) & (~0ULL >> (63 - b));
    }
    template <class F> void each(int l, int r, F f) {        // f(word, mask) over the range
        if (l > r) return;
        int a = l >> 6, b = r >> 6;
        if (a == b) { f(a, mask(l & 63, r & 63)); return; }  // SAME word: one write, then STOP
        f(a, mask(l & 63, 63)), f(b, mask(0, r & 63));       // the two partial words
        for (int i = a + 1; i < b; i++) f(i, ~0ULL);         // the full words between them
    }
    void set(int l, int r)   { each(l, r, [&](int i, ull m) { w[i] |=  m; }); }
    void reset(int l, int r) { each(l, r, [&](int i, ull m) { w[i] &= ~m; }); }
    void flip(int l, int r)  { each(l, r, [&](int i, ull m) { w[i] ^=  m; }); }
    int count(int l, int r) {
        int c = 0;
        each(l, r, [&](int i, ull m) { c += ones(w[i] & m); });
        return c;
    }
    int count() { return count(0, n - 1); }                  // n == 0 gives l > r, so 0
    bool test(int i) const { return w[i >> 6] >> (i & 63) & 1; }
    void set(int i)   { w[i >> 6] |=  1ULL << (i & 63); }
    void reset(int i) { w[i >> 6] &= ~(1ULL << (i & 63)); }
    void flip(int i)  { w[i >> 6] ^=  1ULL << (i & 63); }
    int findNext(int i) {                                    // first set bit >= i, or n if none
        if (i < 0) i = 0;
        if (i >= n) return n;
        int b = i >> 6;
        ull c = w[b] & (~0ULL << (i & 63));                  // drop the bits before i
        while (!c) { if (++b == sz(w)) return n; c = w[b]; }
        return b * 64 + lsb(c);
    }
    void trim() { if (n & 63) w.back() &= mask(0, (n - 1) & 63); }   // bits >= n must stay 0
    void operator|=(const RangeBitset& o) { for (int i = 0; i < sz(w); i++) w[i] |= o.w[i]; }
    void operator&=(const RangeBitset& o) { for (int i = 0; i < sz(w); i++) w[i] &= o.w[i]; }
    void operator^=(const RangeBitset& o) { for (int i = 0; i < sz(w); i++) w[i] ^= o.w[i]; }
    void orShl(int k) {                                      // x |= x << k, the subset-sum step
        int q = k >> 6, s = k & 63;
        for (int i = sz(w) - 1; i >= q; i--) {               // DESCENDING: every source word is
            ull v = w[i - q] << s;                           // below i, so still unmodified
            if (s && i - q) v |= w[i - q - 1] >> (64 - s);   // s == 0 would be a shift by 64
            w[i] |= v;
        }
        trim();                                              // bits that slid past n
    }
};
// VARIANTS / RELATED
// - Whole-bitset AND/OR/XOR plus _Find_first/_Find_next only: use std::bitset, it is faster and
//   already written. Come here the moment a RANGE update appears.
// - Range ASSIGN with O(log) amortized per op and no n/64 term: the interval set ("Chtholly",
//   a std::set of maximal constant runs). It does assign, not flip, and needs random data.
// - Range assign + range count with true polylog: a lazy segment tree. This struct wins up to
//   n around 1e7 purely on constant factor, and is a third of the code.
// - Static "count ones in [l, r]": a prefix popcount array, no structure needed.
// - findPrev: mask with mask(0, i & 63) and scan words downward, taking msb().
