// Segment Tree DESCENT - answer "first / last index satisfying P" in O(log n), not O(log^2 n).
// Here: max-segtree. first_ge(l, x) = smallest i >= l with a[i] >= x, or n if none.
// Swap the stored aggregate + the two `good()` tests to get: first prefix-sum >= S,
// first zero, k-th set bit, "how far right can I extend". This is the workhorse behind
// greedy "extend as far as possible" and interval-cover problems.
struct SegMax {
    int n;
    vector<ll> t;

    SegMax(int n = 0) : n(n), t(4 * n, LLONG_MIN) {}

    void build(const vector<ll>& a, int v, int l, int r) {
        if (l == r) { t[v] = a[l]; return; }
        int m = (l + r) / 2;
        build(a, 2 * v, l, m), build(a, 2 * v + 1, m + 1, r);
        t[v] = max(t[2 * v], t[2 * v + 1]);
    }
    void build(const vector<ll>& a) { if (n) build(a, 1, 0, n - 1); }

    void set(int p, ll x, int v, int l, int r) {
        if (l == r) { t[v] = x; return; }
        int m = (l + r) / 2;
        p <= m ? set(p, x, 2 * v, l, m) : set(p, x, 2 * v + 1, m + 1, r);
        t[v] = max(t[2 * v], t[2 * v + 1]);
    }
    void set(int p, ll x) { set(p, x, 1, 0, n - 1); }

    ll qry(int ql, int qr, int v, int l, int r) {
        if (qr < l || r < ql) return LLONG_MIN;
        if (ql <= l && r <= qr) return t[v];
        int m = (l + r) / 2;
        return max(qry(ql, qr, 2 * v, l, m), qry(ql, qr, 2 * v + 1, m + 1, r));
    }
    ll qry(int l, int r) { return qry(l, r, 1, 0, n - 1); }

    // smallest i in [ql, n) with a[i] >= x   (returns n if none)
    int first_ge(int ql, ll x, int v, int l, int r) {
        if (r < ql || t[v] < x) return n;
        if (l == r) return l;
        int m = (l + r) / 2, res = first_ge(ql, x, 2 * v, l, m);
        return res < n ? res : first_ge(ql, x, 2 * v + 1, m + 1, r);
    }
    int first_ge(int ql, ll x) { return first_ge(ql, x, 1, 0, n - 1); }

    // largest i in [0, qr] with a[i] >= x    (returns -1 if none)
    int last_ge(int qr, ll x, int v, int l, int r) {
        if (l > qr || t[v] < x) return -1;
        if (l == r) return l;
        int m = (l + r) / 2, res = last_ge(qr, x, 2 * v + 1, m + 1, r);
        return res >= 0 ? res : last_ge(qr, x, 2 * v, l, m);
    }
    int last_ge(int qr, ll x) { return last_ge(qr, x, 1, 0, n - 1); }
};
