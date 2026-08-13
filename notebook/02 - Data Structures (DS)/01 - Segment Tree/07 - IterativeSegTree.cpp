// Iterative (Non-Recursive) Segment Tree - Space: O(N), Update: O(log N), Query: O(log N)
// Fast, cache-friendly implementation for point update and range query
template<typename T = ll>
struct IterativeSegTree {
    int n;
    vector<T> tree;
    T neutral;
    function<T(T, T)> combine;

    IterativeSegTree(int n = 0, T neutral = 0, function<T(T, T)> combine = [](T a, T b) { return a + b; })
        : n(n), tree(2 * n, neutral), neutral(neutral), combine(combine) {}

    IterativeSegTree(const vector<T>& a, T neutral = 0, function<T(T, T)> combine = [](T a, T b) { return a + b; })
        : n(a.size()), tree(2 * a.size(), neutral), neutral(neutral), combine(combine) {
        for (int i = 0; i < n; i++) tree[n + i] = a[i];
        for (int i = n - 1; i > 0; i--) tree[i] = combine(tree[i << 1], tree[i << 1 | 1]);
    }

    void update(int p, T val) {
        // combine(LEFT, RIGHT) - do NOT write combine(tree[p], tree[p ^ 1]): when p is the right
        // child that reverses the arguments, which is wrong for any non-commutative combine
        // (matrix product, affine composition, "merge two structs").
        for (tree[p += n] = val; p > 1; p >>= 1)
            tree[p >> 1] = combine(tree[p & ~1], tree[p | 1]);
    }

    // Range query on [l, r] inclusive
    T query(int l, int r) {
        T resl = neutral, resr = neutral;
        for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
            if (l & 1) resl = combine(resl, tree[l++]);
            if (r & 1) resr = combine(tree[--r], resr);
        }
        return combine(resl, resr);
    }
};
