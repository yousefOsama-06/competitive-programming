// Fenwick Tree (Binary Indexed Tree) - 0-based indexing
// Space: O(N), Time: O(log N) per update/query
template<typename T = ll>
struct FenwickTree {
    int n;
    vector<T> tree;

    FenwickTree(int n = 0) : n(n), tree(n + 1, 0) {}

    void add(int i, T delta) {
        for (i++; i <= n; i += i & -i) tree[i] += delta;
    }

    void set(int i, T val) {
        add(i, val - query(i, i));
    }

    T query(int i) {
        T sum = 0;
        for (i++; i > 0; i -= i & -i) sum += tree[i];
        return sum;
    }

    T query(int l, int r) {
        if (l > r) return 0;
        return query(r) - query(l - 1);
    }

    // Returns first 0-based index where prefix sum >= val
    int lower_bound(T val) {
        int pos = 0;
        for (int sz = 1 << (n ? __lg(n) : 0); sz > 0; sz >>= 1) {
            if (pos + sz <= n && tree[pos + sz] < val) {
                val -= tree[pos + sz];
                pos += sz;
            }
        }
        return pos;
    }
};