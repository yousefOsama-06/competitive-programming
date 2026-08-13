// PERSISTENT LAZY SEGMENT TREE - range add and range sum on ANY past version, O(log n) time and
// O(log n) new nodes per update.
// WHEN: you need RANGE updates and old versions. 03 - PersistentSegmentTree only does point
// updates, which is all the "k-th smallest in a range" trick needs - prefer it when that suffices,
// it is half the code and half the memory. If you only ever query the LATEST version, use
// 02 - LazySegmentTree.
// KEY INVARIANT: a tag is NEVER pushed down - that would mutate a node an old version still points
// at. Instead t[u].sum is always the CORRECT sum of u's whole range (tag included) and t[u].add is
// the amount still owed to the two children. So a query carries the accumulated ancestor tags down
// with it, and an update fixes sum on the way back up as sum(l) + sum(r) + add * len.
struct PLazySeg {
    struct Node { int l = 0, r = 0; ll sum = 0, add = 0; };
    vector<Node> t;
    int n;
    PLazySeg(int n) : t(1), n(n) {}                            // node 0 = the shared all-zero node
    int build(const vector<ll>& a, int lo, int hi) {
        int u = t.size();
        t.push_back({});
        if (lo == hi) { t[u].sum = a[lo]; return u; }
        int m = (lo + hi) / 2, L = build(a, lo, m), R = build(a, m + 1, hi);
        t[u].l = L, t[u].r = R, t[u].sum = t[L].sum + t[R].sum;
        return u;
    }
    int build(const vector<ll>& a) { return build(a, 0, n - 1); }
    int update(int u, int ql, int qr, ll v, int lo, int hi) { // add v to [ql, qr], returns new root
        int c = t.size();
        t.push_back(t[u]);
        if (ql <= lo && hi <= qr) { t[c].sum += v * (hi - lo + 1), t[c].add += v; return c; }
        int m = (lo + hi) / 2;
        if (ql <= m) t[c].l = update(t[c].l, ql, qr, v, lo, m);
        if (qr > m) t[c].r = update(t[c].r, ql, qr, v, m + 1, hi);
        t[c].sum = t[t[c].l].sum + t[t[c].r].sum + t[c].add * (hi - lo + 1);
        return c;
    }
    int update(int u, int l, int r, ll v) { return update(u, l, r, v, 0, n - 1); }
    ll query(int u, int ql, int qr, ll acc, int lo, int hi) { // acc = sum of the ancestors' tags
        if (ql <= lo && hi <= qr) return t[u].sum + acc * (hi - lo + 1);
        int m = (lo + hi) / 2;
        acc += t[u].add;
        ll res = 0;
        if (ql <= m) res += query(t[u].l, ql, qr, acc, lo, m);
        if (qr > m) res += query(t[u].r, ql, qr, acc, m + 1, hi);
        return res;
    }
    ll query(int u, int l, int r) { return query(u, l, r, 0, 0, n - 1); }
};
// MEMORY is the real constraint: O((n + q log n)) nodes, ~32 bytes each. reserve() up front.
// RANGE ASSIGN instead of range add: the same no-push-down trick works for any tag that composes,
// but the accumulated `acc` must then be "the last assign seen on the way down", so pass the tag
// itself rather than a sum, and let a deeper tag override a shallower one.
// MIN/MAX aggregate: replace v * len by v and the pull by min(sum(l), sum(r)) + add.
// A COMMON ALTERNATIVE that avoids all of this: make the update offline, sort by version, and use
// an ordinary lazy segment tree with rollback (14 - OfflineDynamicConnectivity's timeline trick).
