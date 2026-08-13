// KINETIC SEGMENT TREE ("segment tree beats" for lines). Each position i owns a LINE
// f_i(x) = a_i * x + b_i and a current parameter x; the tree supports
//     addX(l, r, d)   : x += d for every position in [l, r]   (d >= 0, MONOTONE)
//     queryMax(l, r)  : max over the range of a_i * x_i + b_i
//     setLine(i, a, b): replace one line
// in AMORTISED O(log^2 n) per operation (O(n log^2 n) total), and that amortisation is the point:
// a Li Chao tree answers "many lines, one query point" while THIS answers "one line per position,
// query points that drift". 12 - OneDOneD.cpp lists it as the tool for "the query point moves
// monotonically and the lines are range-updated" - this is that tool.
// PRECONDITION FOR THE COMPLEXITY (not for correctness): x only ever INCREASES. Each node keeps
// `melt` = how much further x may advance before the argmax inside it changes; a decrease would
// invalidate every melt and destroy the potential-function argument (the total number of argmax
// changes over the whole run is O(n log^2 n) only for monotone x).
// INDEX CONVENTION: 0-based positions, inclusive ranges, x starts at 0 for every position.
struct KineticSegTree {
    struct Node { ll val, slope, melt; };                      // melt = INF when nothing can flip
    int n;
    vector<Node> t;
    vector<ll> lz;                                             // pending x-advance for a subtree
    KineticSegTree(const vector<ll>& a, const vector<ll>& b) {
        n = a.size(), t.assign(4 * n, {0, 0, 0}), lz.assign(4 * n, 0);
        build(1, 0, n - 1, a, b);
    }
    Node merge(const Node& L, const Node& R) {
        const Node &w = L.val >= R.val ? L : R, &o = L.val >= R.val ? R : L;
        ll cross = llinf;
        if (o.slope > w.slope) cross = (w.val - o.val) / (o.slope - w.slope) + 1;
        return {w.val, w.slope, min({L.melt, R.melt, cross})};
    }
    void build(int v, int l, int r, const vector<ll>& a, const vector<ll>& b) {
        if (l == r) { t[v] = {b[l], a[l], llinf}; return; }
        int m = (l + r) / 2;
        build(2 * v, l, m, a, b), build(2 * v + 1, m + 1, r, a, b);
        t[v] = merge(t[2 * v], t[2 * v + 1]);
    }
    void apply(int v, ll d) {                                  // legal only when d < t[v].melt
        t[v].val += t[v].slope * d;
        if (t[v].melt != llinf) t[v].melt -= d;
        lz[v] += d;
    }
    void push(int v) {
        if (!lz[v]) return;
        apply(2 * v, lz[v]), apply(2 * v + 1, lz[v]), lz[v] = 0;
    }
    void addX(int v, int l, int r, int ql, int qr, ll d) {
        if (qr < l || r < ql) return;
        if (ql <= l && r <= qr && d < t[v].melt) { apply(v, d); return; }
        if (l == r) { t[v].val += t[v].slope * d; return; }    // a leaf never "melts"
        push(v);
        int m = (l + r) / 2;
        addX(2 * v, l, m, ql, qr, d), addX(2 * v + 1, m + 1, r, ql, qr, d);
        t[v] = merge(t[2 * v], t[2 * v + 1]);                  // the RECURSION is the beats step
    }
    ll query(int v, int l, int r, int ql, int qr) {
        if (qr < l || r < ql) return LLONG_MIN;
        if (ql <= l && r <= qr) return t[v].val;
        push(v);
        int m = (l + r) / 2;
        return max(query(2 * v, l, m, ql, qr), query(2 * v + 1, m + 1, r, ql, qr));
    }
    void setLine(int v, int l, int r, int i, ll a, ll b, ll x) {
        if (l == r) { t[v] = {a * x + b, a, llinf}; return; }
        push(v);
        int m = (l + r) / 2;
        i <= m ? setLine(2 * v, l, m, i, a, b, x) : setLine(2 * v + 1, m + 1, r, i, a, b, x);
        t[v] = merge(t[2 * v], t[2 * v + 1]);
    }
    void addX(int l, int r, ll d) { addX(1, 0, n - 1, l, r, d); }
    ll queryMax(int l, int r) { return query(1, 0, n - 1, l, r); }
    void setLine(int i, ll a, ll b, ll x) { setLine(1, 0, n - 1, i, a, b, x); }
};
/* USING IT
 THE MODEL PROBLEM: process items left to right; item i contributes a_i * (how long it has been
   open) + b_i, and you repeatedly ask for the best open item. Every "time step" is one range
   addX, every answer one queryMax.
 SLOPE TRICK / APERIODIC DPs of the form dp[i] = max_j (dp[j] + w_j * (i - j)) become: insert a
   line at position j when it becomes available, addX(0, n-1, 1) each step, query the max. That
   is the self-referential case D&C optimisation cannot do (see 12 - OneDOneD.cpp).
 THE MELT FORMULA is integer division; with real-valued slopes use long double and a small
   epsilon, or scale to integers. `cross` must be the FIRST x at which the loser strictly wins,
   which is why it is (diff / dslope) + 1 - dropping the +1 makes the tree do nothing and the
   answers silently stale.
 DECREASING x, or arbitrary jumps, still give CORRECT answers (the recursion just triggers more
   often) but the complexity degrades to O(n) per operation in the worst case.
 SIMPLER ALTERNATIVES FIRST: if all lines are inserted once and queries are offline-sortable, use
   CHT (02) or Li Chao (04). Reach for this only when positions are updated in RANGES. */
