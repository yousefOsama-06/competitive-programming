// MEX UNDER UPDATES - the smallest non-negative integer NOT present in the multiset, maintained
// under insert and erase in O(log V), read in O(log V) by ONE descent (not a scan).
// WHEN: "after each operation print the mex", Grundy/nim-value maintenance (10 - Game Theory),
// sliding-window mex. If the multiset only ever GROWS, a pointer that walks forward is O(1)
// amortised and this file is overkill. If you need the mex of a RANGE, see the recipe below.
// KEY INVARIANT: a segment tree over the VALUE axis [0, V] storing the MINIMUM count in each value
// range. The mex is the leftmost value with count 0, so the descent is "if the left child's
// minimum is 0 go left, else go right" - which is why the tree stores a MIN and not a sum.
// V only needs to be n + 1: with n elements the mex can never exceed n.
struct MexSet {
    int V;
    vector<int> cnt, mn;
    MexSet(int V) : V(V), cnt(V + 1, 0), mn(4 * (V + 1), 0) {}
    void upd(int x, int lo, int hi, int p) {
        if (lo == hi) { mn[x] = cnt[lo]; return; }
        int m = (lo + hi) / 2;
        p <= m ? upd(2 * x, lo, m, p) : upd(2 * x + 1, m + 1, hi, p);
        mn[x] = min(mn[2 * x], mn[2 * x + 1]);
    }
    void add(int v) { if (v <= V) cnt[v]++, upd(1, 0, V, v); }
    void del(int v) { if (v <= V) cnt[v]--, upd(1, 0, V, v); }
    int mex(int x, int lo, int hi) {
        if (lo == hi) return lo;
        int m = (lo + hi) / 2;
        return mn[2 * x] == 0 ? mex(2 * x, lo, m) : mex(2 * x + 1, m + 1, hi);
    }
    int mex() { return mex(1, 0, V); }
};
// MEX OF A RANGE [l, r], offline: sort the queries by r and keep a segment tree over VALUES whose
// leaf v holds the LAST POSITION where v occurs in the prefix ending at r (-1 if never). The mex
// of [l, r] is the leftmost v whose last position is < l - the same descent, comparing against l
// instead of against 0. Online: make that tree persistent (03 - PersistentSegmentTree), version r.
// MEX OF A RANGE with array updates: no clean polylog solution - use Mo's plus the O(1)-add/del
// counter in 06 - Square Root Decomposition/07, scanning for the first empty count bucket.
// MEX OF ALL SUBARRAYS / sum of mex over all subarrays: divide and conquer on the position of the
// value 0, or the "for each mex value, count the subarrays achieving it" sweep - not this file.
// FIRST MISSING POSITIVE (mex over 1..) is the same with the axis shifted by one.
