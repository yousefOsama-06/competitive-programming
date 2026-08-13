// HISTORIC-MAXIMUM SEGMENT TREE - range add, range max of the CURRENT values, and range max of
// the HISTORIC value hmax[i] = max over every past moment of a[i]. O(log n) per operation.
// WHEN: "what is the largest this cell ever reached", "peak stock price / peak queue length /
// high-water mark over a range", and as the engine behind offline problems that sweep an index
// while range-adding. If you only need the current max, use 02 - LazySegmentTree.
// KEY INVARIANT: the tag is a PAIR (add, mx) = "the total of this batch of adds" and "the largest
// PARTIAL SUM (prefix) any moment inside the batch reached". That pair is closed under
// composition: applying t and then u gives {t.add + u.add, max(t.mx, t.add + u.mx)}. So a whole
// run of pending updates can sit on a node and still be replayed exactly for the historic max.
// Careful: mx is a max over prefixes INCLUDING the empty one, so it is always >= 0.
struct HistoricSeg {
    struct Tag { ll add = 0, mx = 0; };
    int n;
    vector<ll> cur, his;                                       // current max, historic max
    vector<Tag> lz;
    HistoricSeg(int n) : n(n), cur(4 * n, 0), his(4 * n, 0), lz(4 * n) {}
    void apply(int x, Tag t) {
        his[x] = max(his[x], cur[x] + t.mx);                   // peak inside this batch
        cur[x] += t.add;
        lz[x] = {lz[x].add + t.add, max(lz[x].mx, lz[x].add + t.mx)};
    }
    void push(int x) {
        if (!lz[x].add && !lz[x].mx) return;
        apply(2 * x, lz[x]), apply(2 * x + 1, lz[x]);
        lz[x] = {};
    }
    void pull(int x) {
        cur[x] = max(cur[2 * x], cur[2 * x + 1]);
        his[x] = max(his[2 * x], his[2 * x + 1]);
    }
    void build(const vector<ll>& a, int x, int lo, int hi) {
        lz[x] = {};
        if (lo == hi) { cur[x] = his[x] = a[lo]; return; }
        int m = (lo + hi) / 2;
        build(a, 2 * x, lo, m), build(a, 2 * x + 1, m + 1, hi);
        pull(x);
    }
    void build(const vector<ll>& a) { build(a, 1, 0, n - 1); }
    void upd(int x, int lo, int hi, int l, int r, ll v) {
        if (r < lo || hi < l) return;
        if (l <= lo && hi <= r) { apply(x, {v, max(v, 0LL)}); return; }
        int m = (lo + hi) / 2;
        push(x);
        upd(2 * x, lo, m, l, r, v), upd(2 * x + 1, m + 1, hi, l, r, v);
        pull(x);
    }
    void upd(int l, int r, ll v) { upd(1, 0, n - 1, l, r, v); }
    // hist = false -> max of current values; hist = true -> max of the all-time values
    ll qry(int x, int lo, int hi, int l, int r, bool hist) {
        if (r < lo || hi < l) return -llinf;
        if (l <= lo && hi <= r) return hist ? his[x] : cur[x];
        int m = (lo + hi) / 2;
        push(x);
        return max(qry(2 * x, lo, m, l, r, hist), qry(2 * x + 1, m + 1, hi, l, r, hist));
    }
    ll qry(int l, int r, bool hist) { return qry(1, 0, n - 1, l, r, hist); }
};
// HISTORIC MIN: mirror everything (track the smallest prefix of the batch).
// HISTORIC SUM (sum over time of every past value, i.e. integrate the array) needs a bigger tag:
// keep sum, historic sum, and a count of elapsed time steps, and let the tag carry (add, time)
// with hsum += sum * dt. Same idea, one more field - only write it if the problem forces it.
// RANGE ASSIGN + historic max is the same tag with "assign" replacing "add"; compose so the later
// assign wins and mx tracks the largest value ever assigned.
// THE OFFLINE ALTERNATIVE, almost always shorter: if the queries are known in advance, sweep time
// and answer with an ordinary max segment tree plus a BIT for "was this cell touched after t".
