// BIT OF SEGMENT TREES ("tree in tree") - k-th SMALLEST in a[l..r] WITH point assignments, in
// O(log^2 n) per operation and O(n log^2 n) nodes.
// WHEN: the array changes. Without updates the answer is a persistent segment tree over prefixes
// (01 - Segment Tree/03) at O(log n) per query and a third of the memory, or a wavelet tree (13);
// offline, Mo's or parallel binary search is usually simpler. Reach for this only for ONLINE
// "k-th smallest / count values <= x in a range" mixed with assignments.
// KEY INVARIANT: the BIT is over POSITIONS and every BIT node owns a dynamic segment tree over the
// VALUE axis counting the values of the positions that node covers. A prefix [0, r] is a sum of
// O(log n) BIT nodes, so [l, r] is O(log n) value-trees to add and O(log n) to subtract; the k-th
// smallest is then ONE simultaneous top-down descent through all of them at once, adding up the
// left-child counts to decide which way to go.
struct BITSeg {
    int n, V;                                                  // values live in [0, V)
    vector<int> rt, lc, rc, cnt, a;                            // rt[i] = value tree of BIT node i
    BITSeg(int n, int V) : n(n), V(V), rt(n + 1, 0), lc(1, 0), rc(1, 0), cnt(1, 0), a(n, -1) {}
    int upd(int x, int lo, int hi, int p, int d) {              // += d at value p, returns node id
        if (!x) x = cnt.size(), lc.push_back(0), rc.push_back(0), cnt.push_back(0);
        cnt[x] += d;
        if (lo < hi) {
            int m = (lo + hi) / 2, c;
            if (p <= m) c = upd(lc[x], lo, m, p, d), lc[x] = c;
            else c = upd(rc[x], m + 1, hi, p, d), rc[x] = c;
        }
        return x;
    }
    void add(int i, int v, int d) {
        for (i++; i <= n; i += i & -i) rt[i] = upd(rt[i], 0, V - 1, v, d);
    }
    void set(int i, int v) {                                   // a[i] = v (first call = insert)
        if (a[i] >= 0) add(i, a[i], -1);
        add(i, a[i] = v, 1);
    }
    int kth(int l, int r, int k) {                             // k-th smallest in [l, r], k >= 1
        vector<int> A, B;
        for (int i = r + 1; i > 0; i -= i & -i) A.push_back(rt[i]);
        for (int i = l; i > 0; i -= i & -i) B.push_back(rt[i]);
        int lo = 0, hi = V - 1;
        while (lo < hi) {
            int m = (lo + hi) / 2, c = 0;
            for (int x : A) c += cnt[lc[x]];
            for (int x : B) c -= cnt[lc[x]];
            bool left = k <= c;
            if (!left) k -= c, lo = m + 1;
            else hi = m;
            for (int& x : A) x = left ? lc[x] : rc[x];
            for (int& x : B) x = left ? lc[x] : rc[x];
        }
        return lo;
    }
    int less(int x, int lo, int hi, int ql, int qr) {           // count of values in [ql, qr]
        if (!x || qr < lo || hi < ql) return 0;
        if (ql <= lo && hi <= qr) return cnt[x];
        int m = (lo + hi) / 2;
        return less(lc[x], lo, m, ql, qr) + less(rc[x], m + 1, hi, ql, qr);
    }
    int count(int l, int r, int vl, int vr) {                   // how many a[i] in [vl, vr]
        int res = 0;
        for (int i = r + 1; i > 0; i -= i & -i) res += less(rt[i], 0, V - 1, vl, vr);
        for (int i = l; i > 0; i -= i & -i) res -= less(rt[i], 0, V - 1, vl, vr);
        return res;
    }
};
// COORDINATE COMPRESS first (01 - Miscellaneous/05) - and if updates introduce NEW values, compress
// the update values too, offline, before starting.
// MEMORY is the trap: n log^2 n nodes at 12 bytes is ~100 MB at n = 2e5. If that is too much,
// replace the inner dynamic tree by a MERGE SORT TREE rebuilt in blocks, or go offline.
// THE SAME SHAPE solves: "sum of the k smallest in a range", "rank of x in a range", "predecessor
// / successor of x in a range" - all are the same descent with a different accumulator.
