// WAVELET TREE over values in [lo, hi]. Build O(n log V).
// kth(l, r, k)      : k-th SMALLEST in a[l..r]                       O(log V)
// countLeq(l,r,x)   : how many a[i] <= x in a[l..r]                  O(log V)
// countEq(l,r,x)    : how many a[i] == x in a[l..r]                  O(log V)
// Online (no offline sorting, no persistence) - the practical alternative to a merge-sort tree.
struct Wavelet {
    int lo, hi;
    Wavelet *l = nullptr, *r = nullptr;
    vector<int> b;                                    // b[i] = #elements going left among first i

    // build on a[from, to) - the array IS reordered in place (stable partition by midpoint)
    Wavelet(vector<int>::iterator from, vector<int>::iterator to, int x, int y) : lo(x), hi(y) {
        if (from >= to) return;
        if (lo == hi) { b.assign(to - from + 1, 0); return; }
        int mid = lo + (hi - lo) / 2;
        auto f = [mid](int v) { return v <= mid; };
        b.reserve(to - from + 1);
        b.push_back(0);
        for (auto it = from; it != to; ++it) b.push_back(b.back() + f(*it));
        auto pivot = stable_partition(from, to, f);
        l = new Wavelet(from, pivot, lo, mid);
        r = new Wavelet(pivot, to, mid + 1, hi);
    }
    int kth(int L, int R, int k) {                    // 1-indexed k, inclusive [L, R] (1-based)
        if (L > R) return 0;
        if (lo == hi) return lo;
        int inLeft = b[R] - b[L - 1], lb = b[L - 1], rb = b[R];
        if (k <= inLeft) return l->kth(lb + 1, rb, k);
        return r->kth(L - lb, R - rb, k - inLeft);
    }
    int countLeq(int L, int R, int k) {               // #{i in [L,R] : a[i] <= k}
        if (L > R || k < lo) return 0;
        if (hi <= k) return R - L + 1;
        int lb = b[L - 1], rb = b[R];
        return l->countLeq(lb + 1, rb, k) + r->countLeq(L - lb, R - rb, k);
    }
    int countEq(int L, int R, int k) {
        if (L > R || k < lo || k > hi) return 0;
        if (lo == hi) return R - L + 1;
        int lb = b[L - 1], rb = b[R], mid = lo + (hi - lo) / 2;
        if (k <= mid) return l->countEq(lb + 1, rb, k);
        return r->countEq(L - lb, R - rb, k);
    }
    ~Wavelet() { delete l; delete r; }
};
// Usage: compress values to [0, V), copy the array, then Wavelet w(a.begin(), a.end(), 0, V-1);
// queries are 1-BASED on the original positions.
