// Sparse Table (Range Minimum/Maximum Query) - O(N log N) build, O(1) query
template<typename T, typename F = function<T(T, T)>>
struct SparseTable {
    int n;
    vector<vector<T>> mat;
    F func;

    SparseTable() = default;
    SparseTable(const vector<T>& a, F f = [](T x, T y) { return max(x, y); }) : n(a.size()), func(f) {
        int k = n ? __lg(n) + 1 : 0;
        mat.assign(k, a);
        for (int j = 1; j < k; j++) {
            for (int i = 0; i + (1 << j) <= n; i++) {
                mat[j][i] = func(mat[j - 1][i], mat[j - 1][i + (1 << (j - 1))]);
            }
        }
    }

    T query(int l, int r) const {
        int j = __lg(r - l + 1);
        return func(mat[j][l], mat[j][r - (1 << j) + 1]);
    }
};