// Merge Sort Tree - Space: O(N log N), Build: O(N log N), Query: O(log^2 N)
// Stores sorted elements in each node to answer 2D range count queries
template<typename T = int>
struct MergeSortTree {
    int n;
    vector<vector<T>> tree;

    MergeSortTree(const vector<T>& a) : n(a.size()), tree(4 * n) {
        build(1, 0, n - 1, a);
    }

    void build(int node, int l, int r, const vector<T>& a) {
        if (l == r) {
            tree[node] = {a[l]};
            return;
        }
        int mid = (l + r) / 2;
        build(2 * node, l, mid, a);
        build(2 * node + 1, mid + 1, r, a);
        merge(all(tree[2 * node]), all(tree[2 * node + 1]), back_inserter(tree[node]));
    }

    // Returns number of elements <= k in index range [ql, qr]
    int query_count_le(int node, int l, int r, int ql, int qr, T k) {
        if (ql > r || qr < l) return 0;
        if (ql <= l && r <= qr) {
            return upper_bound(all(tree[node]), k) - tree[node].begin();
        }
        int mid = (l + r) / 2;
        return query_count_le(2 * node, l, mid, ql, qr, k) + query_count_le(2 * node + 1, mid + 1, r, ql, qr, k);
    }

    int query_count_le(int ql, int qr, T k) {
        return query_count_le(1, 0, n - 1, ql, qr, k);
    }
};
