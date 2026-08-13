// MO'S WITH UPDATES (3D Mo). Queries {l, r, t} where t = #updates applied before this query.
// Block size n^(2/3) gives O(n^(5/3)) total. Use when the array changes between queries and no
// online structure fits.
struct MoUpd {
    struct Q { int l, r, t, idx; };
    struct U { int pos, oldV, newV; };

    int n, B;
    vector<int> a, rec;                               // a = state at time 0; rec = while recording
    vector<Q> qs;
    vector<U> us;

    // ---- problem state: #distinct in the window ----
    vector<int> cnt;
    int cur = 0;
    void add(int v) { if (cnt[v]++ == 0) cur++; }
    void rem(int v) { if (--cnt[v] == 0) cur--; }
    int answer() const { return cur; }
    // ------------------------------------------------

    MoUpd(vector<int> arr, int maxVal) : n(arr.size()), a(arr), rec(arr), cnt(maxVal + 1, 0) {
        B = max(1, (int)pow((double)max(n, 1), 2.0 / 3));
    }
    void addQuery(int l, int r) { qs.push_back({l, r, (int)us.size(), (int)qs.size()}); }
    void addUpdate(int pos, int newV) { us.push_back({pos, rec[pos], newV}); rec[pos] = newV; }

    void applyU(int i, int l, int r) {                // step over update i, forwards or backwards
        auto& u = us[i];
        if (l <= u.pos && u.pos <= r) rem(a[u.pos]), add(u.newV);
        a[u.pos] = u.newV;
        swap(u.oldV, u.newV);                          // makes the operation its own inverse
    }
    vector<int> solve() {
        sort(all(qs), [&](const Q& x, const Q& y) {
            int bx = x.l / B, by = y.l / B;
            if (bx != by) return bx < by;
            int cx = x.r / B, cy = y.r / B;
            if (cx != cy) return (bx & 1) ? cx > cy : cx < cy;
            return (cx & 1) ? x.t > y.t : x.t < y.t;
        });
        vector<int> res(qs.size());
        int L = 0, R = -1, T = 0;
        for (auto& q : qs) {
            while (T < q.t) applyU(T++, L, R);
            while (T > q.t) applyU(--T, L, R);
            while (R < q.r) add(a[++R]);
            while (L > q.l) add(a[--L]);
            while (R > q.r) rem(a[R--]);
            while (L < q.l) rem(a[L++]);
            res[q.idx] = answer();
        }
        return res;
    }
};
// NOTE: `a` stays at time 0; `rec` tracks values while you record updates, so applyU() can
// step forwards AND backwards through time using the same swap-based inverse.
