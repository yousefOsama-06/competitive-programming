// PERMUTATION TREE (PQ-tree / substitution decomposition) - represents ALL "common intervals" of a
// permutation: the ranges [l, r] whose VALUES also form a contiguous range. There can be O(n^2) of
// them; this tree stores every one of them in O(n) space, in O(n log n) time.
// EVERY node has a contiguous span of POSITIONS and a contiguous range of VALUES, and is one of:
//   LEAF        - a single position.
//   JOIN node   - its children's value ranges are consecutive and monotone (increasing = type 1,
//                 decreasing = type 2). ANY consecutive run of >= 2 of its children is itself a
//                 common interval. This is where the O(n^2) of them hide.
//   CUT (prime) - no proper consecutive run of its children is a common interval; only the whole
//                 node is. A cut node always has >= 4 children.
// WHEN: "count subarrays whose values are consecutive integers", "the number of ways to split a
// permutation into monotone blocks", simple-permutation questions, and the smallest common
// interval containing a given [l, r]. If you only need to COUNT common intervals, the divide and
// conquer with min/max stacks is shorter - reach for the tree when you need the STRUCTURE.
// KEY INVARIANT: sweep r and keep, for every l, the quantity (max - min) - (r - l) over a[l..r],
// which is >= 0 and equals 0 exactly when [l, r] is a common interval. Monotone stacks maintain it
// under range add in O(1) amortised each, and a min segment tree finds the zeros. Whenever the new
// element extends an existing node's value range by one the node absorbs it (join), and whenever a
// zero appears further left a cut node is created over everything between.
struct PermTree {
    int n, id, root = 0;
    vector<int> p, ty, par, mn, lz;                            // ty: 0 cut, 1 up-join, 2 down-join
    vector<pii> val, span;                                     // value range, position range
    vector<vector<int>> ch;
    PermTree(const vector<int>& perm)                          // perm[1..n], a permutation of 1..n
        : n(perm.size() - 1), id(0), p(perm), ty(2 * perm.size(), 0), par(2 * perm.size(), -1),
          mn(8 * perm.size(), 0), lz(8 * perm.size(), 0), val(2 * perm.size()),
          span(2 * perm.size()), ch(2 * perm.size()) { build(); }
    void apply(int x, int v) { mn[x] += v, lz[x] += v; }
    void push(int x) { if (lz[x]) apply(2 * x, lz[x]), apply(2 * x + 1, lz[x]), lz[x] = 0; }
    void upd(int x, int b, int e, int l, int r, int v) {
        if (r < b || e < l) return;
        if (l <= b && e <= r) { apply(x, v); return; }
        int m = (b + e) / 2;
        push(x);
        upd(2 * x, b, m, l, r, v), upd(2 * x + 1, m + 1, e, l, r, v);
        mn[x] = min(mn[2 * x], mn[2 * x + 1]);
    }
    int qry(int x, int b, int e, int l, int r) {
        if (r < b || e < l || l > r) return inf;
        if (l <= b && e <= r) return mn[x];
        int m = (b + e) / 2;
        push(x);
        return min(qry(2 * x, b, m, l, r), qry(2 * x + 1, m + 1, e, l, r));
    }
    void link(int u, int v) { par[v] = u, ch[u].push_back(v); }
    bool adj(int i, int j) { return val[i].se == val[j].fi - 1; }     // j starts right after i
    int len(int i) { return val[i].se - val[i].fi + 1; }
    static pii un(pii a, pii b) { return {min(a.fi, b.fi), max(a.se, b.se)}; }
    void build() {
        id = n + 1;
        vector<int> mx{0}, mi{0}, st;                          // monotone stacks + node stack
        for (int i = 1; i <= n; i++) {
            while (mx.back() && p[mx.back()] < p[i]) {
                int r = mx.back();
                mx.pop_back();
                upd(1, 1, n, mx.back() + 1, r, p[i] - p[r]);
            }
            mx.push_back(i);
            while (mi.back() && p[mi.back()] > p[i]) {
                int r = mi.back();
                mi.pop_back();
                upd(1, 1, n, mi.back() + 1, r, p[r] - p[i]);
            }
            mi.push_back(i);
            val[i] = {p[i], p[i]}, span[i] = {i, i};
            int cur = i;
            while (true) {
                if (!st.empty() && (adj(st.back(), cur) || adj(cur, st.back()))) {
                    int b = st.back(), want = adj(b, cur) ? 1 : 2;
                    if (ty[b] == want) {                       // absorb into the existing join
                        link(b, cur);
                        val[b] = un(val[b], val[cur]), span[b] = un(span[b], span[cur]);
                        cur = b;
                    } else {                                   // start a new join node
                        ty[id] = want, link(id, b), link(id, cur);
                        val[id] = un(val[b], val[cur]), span[id] = un(span[b], span[cur]);
                        cur = id++;
                    }
                    st.pop_back();
                } else if (qry(1, 1, n, 1, i - len(cur)) == 0) {     // a cut node closes here
                    int L = len(cur);
                    pii v = val[cur], s = span[cur];
                    link(id, cur);
                    do {
                        int b = st.back();
                        L += len(b), v = un(v, val[b]), s = un(s, span[b]);
                        link(id, b), st.pop_back();
                    } while (v.se - v.fi + 1 != L);
                    reverse(all(ch[id]));
                    val[id] = v, span[id] = s, cur = id++;
                } else break;
            }
            st.push_back(cur);
            upd(1, 1, n, 1, i, -1);
        }
        root = st.back();
    }
};
// COUNTING COMMON INTERVALS: sum over join nodes with k children of k*(k-1)/2, plus n (the leaves),
// plus the number of cut nodes. That is the classic "count subarrays forming a value range".
// SMALLEST COMMON INTERVAL CONTAINING [l, r]: binary-lift from leaf l to the highest ancestor whose
// span still ends before r, take its parent u. If u is a cut node the answer is span[u]; if u is a
// join node the answer is the run of u's children from the one containing l to the one containing
// r (binary search over the children, whose spans are sorted).
// NODE COUNT is < 2n, so size every array 2n. Leaves are 1..n, internal nodes n+1..id-1.
// SIMPLE PERMUTATIONS are exactly those whose tree is a single cut node over n leaves.
// THIS IS MODULAR DECOMPOSITION specialised to permutation graphs - the same "join / cut" shape
// appears for cographs and for general graphs, where it is much harder to compute.
