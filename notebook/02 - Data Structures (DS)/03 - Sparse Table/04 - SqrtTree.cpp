// SQRT TREE - O(1) range query for ANY associative operation, WITH point updates in O(sqrt n).
// Build O(n log log n), memory O(n log log n).
// WHEN: the operation is associative but NOT idempotent and NOT invertible (matrix product, "max
// suffix sum", min-plus, string hash concat), so a sparse table (01) is wrong and a BIT is
// impossible - and you need O(1) queries or updates on top. If the op is idempotent use
// 01 - SparseTable; if there are no updates use 12 - DisjointSparseTable (O(n log n) build, O(1)
// query, 20 lines); if O(log n) queries are fine just use a segment tree.
// KEY INVARIANT: cut the array into sqrt(n) blocks of sqrt(n). Store prefix and suffix aggregates
// inside every block and a table `between[i][j]` for whole blocks i..j. A query then costs one
// suffix + one between + one prefix. Blocks are handled RECURSIVELY by the same structure, which
// is why the depth is log log n: n -> sqrt(n) -> n^(1/4) -> ... The top-level `between` table is
// itself a Sqrt Tree over the sqrt(n) block aggregates, so an update rebuilds only one block per
// layer plus one entry of the index structure.
struct SqrtTree {
    typedef ll T;
    static T op(T a, T b) { return a + b; }                    // ANY associative op
    int n, lg, isz;
    vector<T> v;
    vector<int> clz, layers, onLayer;
    vector<vector<T>> pref, suf, betw;
    void buildBlock(int k, int l, int r) {
        pref[k][l] = v[l];
        for (int i = l + 1; i < r; i++) pref[k][i] = op(pref[k][i - 1], v[i]);
        suf[k][r - 1] = v[r - 1];
        for (int i = r - 2; i >= l; i--) suf[k][i] = op(v[i], suf[k][i + 1]);
    }
    void buildBetween(int k, int lb, int rb, int off) {
        int sl = (layers[k] + 1) >> 1, cl = layers[k] >> 1, bs = 1 << sl;
        int bc = (rb - lb + bs - 1) >> sl;
        for (int i = 0; i < bc; i++) {
            T cur = T();
            for (int j = i; j < bc; j++) {
                T add = suf[k][lb + (j << sl)];
                cur = i == j ? add : op(cur, add);
                betw[k - 1][off + lb + (i << cl) + j] = cur;
            }
        }
    }
    void build(int k, int lb, int rb, int off) {
        if (k >= (int)layers.size()) return;
        int bs = 1 << ((layers[k] + 1) >> 1);
        for (int l = lb; l < rb; l += bs) {
            int r = min(l + bs, rb);
            buildBlock(k, l, r), build(k + 1, l, r, off);
        }
        if (k) buildBetween(k, lb, rb, off);
        else {                                                 // layer 0: rebuild the index array
            int sl = (lg + 1) >> 1;
            for (int i = 0; i < isz; i++) v[n + i] = suf[0][i << sl];
            build(1, n, n + isz, (1 << lg) - n);
        }
    }
    void update(int k, int lb, int rb, int off, int x) {
        if (k >= (int)layers.size()) return;
        int sl = (layers[k] + 1) >> 1, bs = 1 << sl, bi = (x - lb) >> sl;
        int l = lb + (bi << sl), r = min(l + bs, rb);
        buildBlock(k, l, r);
        if (k) buildBetween(k, lb, rb, off);
        else {
            v[n + bi] = suf[0][bi << ((lg + 1) >> 1)];
            update(1, n, n + isz, (1 << lg) - n, n + bi);
        }
        update(k + 1, l, r, off, x);
    }
    void update(int x, T val) { v[x] = val, update(0, 0, n, 0, x); }
    T query(int l, int r, int off, int base) {                 // inclusive [l, r]
        if (l == r) return v[l];
        if (l + 1 == r) return op(v[l], v[r]);
        int k = onLayer[clz[(l - base) ^ (r - base)]];
        int sl = (layers[k] + 1) >> 1, cl = layers[k] >> 1;
        int lb = (((l - base) >> layers[k]) << layers[k]) + base;
        int lblk = ((l - lb) >> sl) + 1, rblk = ((r - lb) >> sl) - 1;
        T res = suf[k][l];
        if (lblk <= rblk)
            res = op(res, k ? betw[k - 1][off + lb + (lblk << cl) + rblk]
                            : query(n + lblk, n + rblk, (1 << lg) - n, n));
        return op(res, pref[k][r]);
    }
    T query(int l, int r) { return query(l, r, 0, 0); }
    SqrtTree(const vector<T>& a) : n(a.size()), lg(0), v(a) {
        while ((1 << lg) < n) lg++;
        clz.assign(1 << lg, 0), onLayer.assign(lg + 1, 0);
        for (int i = 1; i < (1 << lg); i++) clz[i] = clz[i >> 1] + 1;
        for (int t = lg; t > 1; t = (t + 1) >> 1) onLayer[t] = layers.size(), layers.push_back(t);
        for (int i = lg - 1; i >= 0; i--) onLayer[i] = max(onLayer[i], onLayer[i + 1]);
        int sl = (lg + 1) >> 1, bs = 1 << sl;
        isz = (n + bs - 1) >> sl;
        v.resize(n + isz);
        pref.assign(layers.size(), vector<T>(n + isz));
        suf.assign(layers.size(), vector<T>(n + isz));
        betw.assign(max(0, (int)layers.size() - 1), vector<T>((1 << lg) + bs));
        build(0, 0, n, 0);
    }
};
// NON-COMMUTATIVE ops are fine and are the whole point - op(a, b) is always applied left to right.
// T MUST have a sensible default constructor; it is never used as an identity, only as a filler.
// THE O(1)-QUERY / O(sqrt n)-UPDATE trade is unusual. Its real niche is "answer 1e6 queries of a
// non-invertible op with occasional updates". If updates outnumber queries, a segment tree wins.
// THE TWO-LEVEL VERSION (one layer of blocks, no recursion) is 20 lines and gives O(1) query with
// O(n) build only if you can afford a sqrt(n) x sqrt(n) between-table; recursion is what makes the
// update cheap. Write the two-level one first if the array is static and small.
