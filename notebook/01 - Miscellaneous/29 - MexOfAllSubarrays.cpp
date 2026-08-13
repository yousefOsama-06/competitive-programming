// MEX OF EVERY SUBARRAY - which values occur as mex(a[l..r]) over all O(n^2) subarrays, O(n log n).
// Input a[0..n-1] with a[i] >= 0. Output ok[0..n], ok[m] == 1 iff some NON-EMPTY subarray has mex
// exactly m. Nothing above n is reachable: mex m needs m distinct values, so m <= n.
// WHY IT IS ONLY O(n log n): m is reachable iff some MAXIMAL m-free block contains all of 0..m-1.
// If a block avoids m and holds 0..m-1 its mex is exactly m; if the maximal block misses some
// value < m then so does every sub-block of it, so testing the maximal blocks alone is complete.
// The blocks for m are the gaps between consecutive occurrences of m, so over all m there are only
// sum(cnt[m] + 1) <= 2n + 1 blocks to test. Each test is one range-mex query, answered offline
// sorted by right end: keep last[v] = the last position of v seen so far in a MIN segment tree over
// v, and mex(a[L..R]) is the leftmost v with last[v] < L - one descend, go left iff the left
// child's min is < L. That descend is the reusable half of this file.
// WHEN: Codeforces 1436E ("mex of the set of all subarray mexes"), "is m achievable", and any
// range-mex problem - the tree below answers those directly.
// COMPLEXITY: O(n log n) time, O(n) memory (4n tree nodes plus <= 2n + 1 buffered queries).
// DEGENERATE: n == 0 returns {0} (no subarray, nothing reachable). All-zero returns only ok[1].
// All-distinct 0..n-1 returns every ok[0..n]. Duplicates are fine. Values > n are ignored - they
// can neither BE a mex (too big) nor block one - but they still occupy a position and split
// nothing, which is correct. Negative values are undefined input, mex assumes non-negative.
// EXACTNESS: pure integer index arithmetic, no overflow anywhere (positions fit int).
struct MexTree {                                   // min, over a range of VALUES, of "last seen at"
    int V; vector<int> t;                          // values 0..V; -1 = never seen
    MexTree(int V) : V(V), t(4 * (V + 1), -1) {}
    void see(int v, int p, int x, int l, int r) {
        if (l == r) { t[x] = p; return; }
        int m = (l + r) / 2;
        v <= m ? see(v, p, 2 * x, l, m) : see(v, p, 2 * x + 1, m + 1, r);
        t[x] = min(t[2 * x], t[2 * x + 1]);
    }
    void see(int v, int p) { if (v <= V) see(v, p, 1, 0, V); }    // v > V cannot affect a mex <= V
    int mex(int L, int x, int l, int r) {                        // leftmost v with last[v] < L
        if (l == r) return l;                                    // a leaf always exists: [L..R] has
        int m = (l + r) / 2;                                     // <= V+1 elements so some value
        return t[2 * x] < L ? mex(L, 2 * x, l, m)                // in 0..V must be missing
                            : mex(L, 2 * x + 1, m + 1, r);
    }
    int mex(int L) { return mex(L, 1, 0, V); }     // == mex(a[L..R]), R = last position fed to see
};
vector<char> allSubarrayMex(const vector<int>& a) {
    int n = sz(a);
    vector<char> ok(n + 1, 0);
    if (!n) return ok;
    vector<vi> pos(n + 1);                                       // pos[m] = positions holding m
    for (int i = 0; i < n; i++) if (a[i] <= n) pos[a[i]].pb(i);
    vector<vector<pii>> qs(n);                                   // at R: (L, m), "is mex[L..R] m?"
    for (int m = 0; m <= n; m++) {                               // the maximal m-free blocks
        int lo = 0;
        for (int p : pos[m]) { if (lo < p) qs[p - 1].pb({lo, m}); lo = p + 1; }
        if (lo < n) qs[n - 1].pb({lo, m});
    }
    MexTree T(n);
    for (int R = 0; R < n; R++) {
        T.see(a[R], R);
        for (auto& [L, m] : qs[R]) if (T.mex(L) == m) ok[m] = 1;  // block avoids m, so mex <= m
    }
    return ok;
}
// CF1436E wants the mex OF THAT SET: int r = 0; while (r <= n && ok[r]) r++;
// VARIANTS
//  - RANGE MEX, offline: drop the block machinery, bucket the real queries by R and run the same
//    sweep - T.mex(L) after feeding a[0..R] is mex(a[L..R]). O((n + q) log n).
//  - RANGE MEX, online: make the same "last occurrence" tree PERSISTENT, one version per R, and
//    descend in version R. O(log n) per query, O(n log n) memory. Merge-sort tree also works.
//  - "smallest missing value >= x in a range": identical descend restricted to values [x, V].
//  - MEX OF A MULTISET under insert/erase: segment tree over counts, descend to the leftmost zero.
//  - MEX OF THE WHOLE ARRAY is O(n) with a bucket array; never build a tree for that.
//  - mex(a[l..r]) is non-increasing in l for fixed r, and non-decreasing in r for fixed l - that
//    monotonicity is what lets a two-pointer answer "shortest subarray with mex >= k".
