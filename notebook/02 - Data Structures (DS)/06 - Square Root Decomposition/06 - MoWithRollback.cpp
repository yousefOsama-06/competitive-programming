// MO'S WITH ROLLBACK - for structures where ADD is easy but REMOVE is hard or impossible
// (running maximum, DSU, "best pair so far", knapsack-ish state).
// Per block of left endpoints: sort by r, keep a persistent right part that only grows, and
// rebuild the short left part from scratch for every query, rolling back afterwards.
// O((n + q) sqrt n) adds, ZERO removes.
struct MoRollback {
    struct Q { int l, r, idx; };
    int n, B;
    vector<ll> a;
    vector<Q> qs;

    // ---- problem state: max (value * count) seen; only add() and a snapshot/rollback ----
    vector<int> cnt;
    ll best = 0;
    vector<pair<int, ll>> hist;                       // {value index, previous best}
    void add(int v, bool rec) {
        if (rec) hist.push_back({v, best});
        cnt[v]++;
        best = max(best, (ll)cnt[v] * v);
    }
    void rollback(size_t to) {
        while (hist.size() > to) {
            auto [v, b] = hist.back(); hist.pop_back();
            cnt[v]--, best = b;
        }
    }
    void clearAll() { fill(all(cnt), 0), best = 0, hist.clear(); }
    ll answer() const { return best; }
    // -------------------------------------------------------------------------------------

    MoRollback(vector<ll> arr, int maxVal) : n(arr.size()), a(arr), cnt(maxVal + 1, 0) {
        B = max(1, (int)sqrtl((ld)max(1, n)));
    }
    void addQuery(int l, int r) { qs.push_back({l, r, (int)qs.size()}); }

    vector<ll> solve() {
        sort(all(qs), [&](const Q& x, const Q& y) {
            int bx = x.l / B, by = y.l / B;
            return bx != by ? bx < by : x.r < y.r;
        });
        vector<ll> res(qs.size());
        size_t i = 0;
        while (i < qs.size()) {
            int b = qs[i].l / B, rEnd = min(n - 1, (b + 1) * B - 1);
            size_t j = i;
            while (j < qs.size() && qs[j].l / B == b) j++;
            // pass 1: queries contained in this block - answer each from a CLEAN state
            for (size_t k2 = i; k2 < j; k2++) if (qs[k2].r <= rEnd) {
                clearAll();
                for (int k = qs[k2].l; k <= qs[k2].r; k++) add(a[k], false);
                res[qs[k2].idx] = answer();
            }
            // pass 2: long queries - right part grows monotonically, left part is rolled back
            clearAll();
            int R = rEnd;
            for (size_t k2 = i; k2 < j; k2++) if (qs[k2].r > rEnd) {
                while (R < qs[k2].r) add(a[++R], false);             // grow-only, not recorded
                size_t snap = hist.size();
                for (int k = qs[k2].l; k <= rEnd; k++) add(a[k], true);   // short left part
                res[qs[k2].idx] = answer();
                rollback(snap);
            }
            i = j;
        }
        return res;
    }
};
