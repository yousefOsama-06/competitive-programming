// RANGE-PARALLEL DSU ("DSU on a sparse table") - merge two whole RANGES elementwise in O(log n).
// The operation is: "a[u..u+len-1] must equal a[v..v+len-1] elementwise", i.e. union u+i with v+i
// for every i < len. Doing that naively is O(len) per constraint; this does it in O(log n).
// IDEA: keep LOGN separate DSUs. par[k] answers "these two blocks of length 2^k are identical".
// A range constraint of length len is covered by TWO blocks of length 2^k (k = floor(log2 len)),
// exactly like a sparse-table query - so one union at level k, twice. Afterwards, PUSH DOWN:
// if two blocks of length 2^k are equal then their two halves of length 2^(k-1) are equal too.
// After the push-down, level 0 holds the true elementwise DSU.
// USES: "count the strings/arrays satisfying these equal-substring constraints" (the answer is
// alphabet^(#components at level 0)), grid/string pattern unification, and any problem that says
// "these two intervals must match".
struct RangeDSU {
    int n, LOG;
    vector<vector<int>> par;
    RangeDSU(int n) : n(n), LOG(__lg(max(n, 1)) + 1), par(LOG, vector<int>(n)) {
        for (int k = 0; k < LOG; k++) iota(all(par[k]), 0);
    }
    int find(int k, int u) { return par[k][u] == u ? u : par[k][u] = find(k, par[k][u]); }
    void join(int k, int u, int v) {
        u = find(k, u), v = find(k, v);
        if (u != v) par[k][u] = v;
    }
    // a[u .. u+len-1] equals a[v .. v+len-1], elementwise. O(log n) amortised.
    void equalRanges(int u, int v, int len) {
        if (len <= 0) return;
        int k = __lg(len);
        join(k, u, v);
        join(k, u + len - (1 << k), v + len - (1 << k));
    }
    // Call ONCE after all the constraints. Then find(0, i) is the real elementwise component.
    void pushDown() {
        for (int k = LOG - 1; k > 0; k--) {
            int half = 1 << (k - 1);
            for (int i = 0; i + (1 << k) <= n; i++) {
                int j = find(k, i);
                if (j == i) continue;
                join(k - 1, i, j);                                 // both halves inherit it
                join(k - 1, i + half, j + half);
            }
        }
    }
    int components() {                                             // at level 0, after pushDown
        int c = 0;
        for (int i = 0; i < n; i++) if (find(0, i) == i) c++;
        return c;
    }
};
// The same "cover a range with two power-of-two blocks, then push down" trick works for any
// idempotent relation, not just equality - it is the DSU analogue of a sparse table.
// If instead you need to union a range of CONSECUTIVE elements to each other ("a[l..r] all equal"),
// that is much simpler: a "next unvisited pointer" DSU where find(i) jumps to the next index not
// yet merged, giving O(n alpha) total over all range-unions.
