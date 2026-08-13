// DISJOINT SPARSE TABLE - O(1) query for ANY associative operation (not just idempotent ones).
// Build O(n log n) time and memory. Use when a plain sparse table would double-count:
// sum, product, matrix product, hashing, "merge two structs", min-count pairs, ...
// (For min/max/gcd/or/and a normal sparse table is smaller - use that instead.)
template <typename T, class F = function<T(T, T)>>
struct DisjointST {
    int n, LOG;
    vector<vector<T>> t;
    F op;

    DisjointST(const vector<T>& a, F op) : n(a.size()), op(op) {
        LOG = 1;
        while ((1 << LOG) < n) LOG++;
        t.assign(LOG + 1, a);
        for (int lvl = 1; lvl <= LOG; lvl++) {
            int half = 1 << lvl;
            for (int mid = half; mid < n + half; mid += 2 * half) {
                if (mid - 1 < n) {                            // build leftwards from the split
                    t[lvl][mid - 1] = a[mid - 1];
                    for (int i = mid - 2; i >= mid - half && i >= 0; i--)
                        t[lvl][i] = op(a[i], t[lvl][i + 1]);
                }
                if (mid < n) {                                // and rightwards
                    t[lvl][mid] = a[mid];
                    for (int i = mid + 1; i < min(n, mid + half); i++)
                        t[lvl][i] = op(t[lvl][i - 1], a[i]);
                }
            }
        }
    }
    T query(int l, int r) const {                             // inclusive [l, r]
        if (l == r) return t[0][l];
        int lvl = 63 - __builtin_clzll((unsigned long long)(l ^ r));   // highest differing bit
        return op(t[lvl][l], t[lvl][r]);
    }
};
// Every query splits at the unique power-of-two boundary between l and r, and both halves are
// already prefix/suffix-accumulated at that level - hence exactly one op() per query.
