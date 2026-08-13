// SPARSE 2D SEGMENT TREE (segment tree of TREAPS) - point assign a[i][j] = v and rectangle sum on
// a grid whose columns are huge or unknown in advance. O(log n * log q) per operation and only
// O(q log n) memory - one node per touched cell per level.
// WHEN: 08 - SegTree2D is a dense n x m tree and needs O(nm) memory; a merge-sort tree (06) is
// static; an offline problem should use 02 - BIT/03 - Offline2DFenwickTree, which is smaller and
// faster because it can compress the coordinates first. Reach for THIS one only when the updates
// are ONLINE and the grid is too big to allocate.
// KEY INVARIANT: the outer tree is indexed by ROW and every outer node owns a treap keyed by
// COLUMN holding, for each column j that some row in its range touches, the aggregate of that
// column over the node's rows. So an outer node is only ever asked about columns that exist, and
// after a point update the parents repair a SINGLE column by recombining their two children's
// value at that one column.
struct Seg2D {
    struct T { int l = 0, r = 0, key = 0; unsigned pr = 0; ll val = 0, agg = 0; };
    vector<T> tr;                                              // shared pool for every treap
    vector<int> rt;                                            // treap root of each outer node
    int n;
    Seg2D(int n) : tr(1), rt(4 * n, 0), n(n) {}
    unsigned rnd() {                                           // xorshift; any prng will do
        static unsigned s = 2463534242u;
        return s ^= s << 13, s ^= s >> 17, s ^= s << 5;
    }
    void pull(int x) { tr[x].agg = tr[tr[x].l].agg + tr[x].val + tr[tr[x].r].agg; }
    void split(int x, int k, int& a, int& b) {                 // a keeps keys < k
        if (!x) { a = b = 0; return; }
        if (tr[x].key < k) a = x, split(tr[x].r, k, tr[a].r, b), pull(a);
        else b = x, split(tr[x].l, k, a, tr[b].l), pull(b);
    }
    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (tr[a].pr > tr[b].pr) { tr[a].r = merge(tr[a].r, b); pull(a); return a; }
        tr[b].l = merge(a, tr[b].l);
        pull(b);
        return b;
    }
    void set(int& root, int k, ll v) {                         // a[k] = v inside one treap
        int a, b, c;
        split(root, k, a, b), split(b, k + 1, b, c);
        if (b) tr[b].val = v, pull(b);
        else tr.push_back({0, 0, k, rnd(), v, v}), b = tr.size() - 1;
        root = merge(merge(a, b), c);
    }
    ll get(int& root, int lo, int hi) {                        // sum of keys in [lo, hi]
        int a, b, c;
        split(root, lo, a, b), split(b, hi + 1, b, c);
        ll res = tr[b].agg;
        root = merge(merge(a, b), c);
        return res;
    }
    void upd(int x, int lo, int hi, int i, int j, ll v) {      // a[i][j] = v
        if (lo == hi) { set(rt[x], j, v); return; }
        int m = (lo + hi) / 2;
        i <= m ? upd(2 * x, lo, m, i, j, v) : upd(2 * x + 1, m + 1, hi, i, j, v);
        set(rt[x], j, get(rt[2 * x], j, j) + get(rt[2 * x + 1], j, j));
    }
    void upd(int i, int j, ll v) { upd(1, 0, n - 1, i, j, v); }
    ll qry(int x, int lo, int hi, int r1, int r2, int c1, int c2) {
        if (r2 < lo || hi < r1) return 0;
        if (r1 <= lo && hi <= r2) return get(rt[x], c1, c2);
        int m = (lo + hi) / 2;
        return qry(2 * x, lo, m, r1, r2, c1, c2) + qry(2 * x + 1, m + 1, hi, r1, r2, c1, c2);
    }
    ll qry(int r1, int r2, int c1, int c2) { return qry(1, 0, n - 1, r1, r2, c1, c2); }
};
// ANY IDEMPOTENT OR GROUP AGGREGATE works (sum, xor, gcd, min): upd() rebuilds a parent's single
// column from its two children, so the operation only has to be associative and commutative.
// ROWS TOO BIG TOO: make the outer tree dynamic as well (05 - DynamicSegTree) - allocate the outer
// node on first touch. Memory stays O(q log n).
// BIT OF TREAPS is the same shape with a shorter outer layer, but it cannot do "descend to find
// the k-th" and needs a group (no min/max).
// IF THE PROBLEM IS OFFLINE, do not write this. Sort the queries by row, sweep, and keep a single
// 1D BIT over columns: O((n + q) log n), five lines, and no memory pressure.
