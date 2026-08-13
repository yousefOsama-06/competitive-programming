// SEGMENT TREE MERGING - one dynamic segment tree per vertex over the VALUE domain, merged
// bottom-up along a tree. Total O(n log n) nodes touched over ALL merges (amortised), because
// each merge destroys as many nodes as it visits.
// Solves, per subtree and in one DFS: most frequent value, k-th value, count of a value,
// "sum of values in a range" - i.e. everything a merge-sort tree gives, but online per subtree.
struct SegMerge {
    struct Node { int l = 0, r = 0; ll sum = 0; int best = 0; ll bestCnt = 0; };
    vector<Node> t;
    int V;                                            // value domain is [0, V)

    SegMerge(int V, int reserve = 0) : V(V) { t.reserve(reserve), t.push_back(Node()); }  // 0 = null
    int newNode() { t.push_back(Node()); return t.size() - 1; }

    void pull(int v) {
        int L = t[v].l, R = t[v].r;
        t[v].sum = t[L].sum + t[R].sum;
        if (t[L].bestCnt >= t[R].bestCnt) t[v].best = t[L].best, t[v].bestCnt = t[L].bestCnt;
        else t[v].best = t[R].best, t[v].bestCnt = t[R].bestCnt;
    }
    // Returns the (possibly new) node index. NEVER take an `int&` into t here: newNode() can
    // reallocate the vector and invalidate the reference mid-recursion.
    int update(int v, int lo, int hi, int p, ll d) {
        if (!v) v = newNode();
        if (hi - lo == 1) { t[v].sum += d, t[v].best = lo, t[v].bestCnt = t[v].sum; return v; }
        int m = (lo + hi) / 2;
        if (p < m) { int c = update(t[v].l, lo, m, p, d); t[v].l = c; }
        else { int c = update(t[v].r, m, hi, p, d); t[v].r = c; }
        pull(v);
        return v;
    }
    int merge(int a, int b, int lo, int hi) {          // destroys b; returns the merged root
        if (!a || !b) return a | b;
        if (hi - lo == 1) { t[a].sum += t[b].sum, t[a].bestCnt = t[a].sum; return a; }
        int m = (lo + hi) / 2;
        int L = merge(t[a].l, t[b].l, lo, m), R = merge(t[a].r, t[b].r, m, hi);
        t[a].l = L, t[a].r = R;
        pull(a);
        return a;
    }
    ll query(int v, int lo, int hi, int ql, int qr) {  // sum of counts in [ql, qr)
        if (!v || qr <= lo || hi <= ql) return 0;
        if (ql <= lo && hi <= qr) return t[v].sum;
        int m = (lo + hi) / 2;
        return query(t[v].l, lo, m, ql, qr) + query(t[v].r, m, hi, ql, qr);
    }
    int kth(int v, int lo, int hi, ll k) {             // k-th smallest (1-indexed), -1 if k too big
        if (!v || k > t[v].sum) return -1;
        if (hi - lo == 1) return lo;
        int m = (lo + hi) / 2;
        return k <= t[t[v].l].sum ? kth(t[v].l, lo, m, k) : kth(t[v].r, m, hi, k - t[t[v].l].sum);
    }
    // wrappers
    void update(int& root, int p, ll d) { root = update(root, 0, V, p, d); }
    int merge(int a, int b) { return merge(a, b, 0, V); }
    ll query(int root, int ql, int qr) { return query(root, 0, V, ql, qr); }
    int kth(int root, ll k) { return kth(root, 0, V, k); }
};
// TREE USAGE: root[v] starts with just v's own value; after recursing, merge every child's root
// into v's. Answer v's query right there. Never reuse a root after merging it away.
