// INTERVAL SET / "Chtholly tree" (ODT). Keeps the array as maximal runs of equal value.
// assign(l, r, v) is amortised near-O(log n) WHEN the data has many range-assigns (random or
// adversarial-but-assign-heavy); each assign destroys the intervals it covers, so the total
// number of intervals created is O(q).
// THIS is the standard way to support range bitwise-AND / OR / MOD / assign together with
// range sum, because those operations rapidly make long stretches constant.
struct ODT {
    map<int, pair<int, ll>> mp;                      // l -> {r, value}, covering [l, r]

    ODT(int n, ll v = 0) { mp[0] = {n - 1, v}; }

    // split so that an interval starts exactly at p; returns the iterator to it
    auto split(int p) {
        auto it = prev(mp.upper_bound(p));
        if (it->first == p) return it;
        auto [r, v] = it->second;
        it->second.first = p - 1;
        return mp.emplace(p, make_pair(r, v)).first;
    }
    void assign(int l, int r, ll v) {
        auto itr = split(r + 1), itl = split(l);
        mp.erase(itl, itr);
        mp[l] = {r, v};
    }
    // Generic scan over [l, r]: f(l, r, value) is called once per run. O(#runs touched).
    template <class F> void forEach(int l, int r, F f) {
        auto itr = split(r + 1), itl = split(l);
        for (auto it = itl; it != itr; ++it) f(it->first, it->second.first, it->second.second);
    }
    ll sum(int l, int r) {
        ll s = 0;
        forEach(l, r, [&](int a, int b, ll v) { s += v * (b - a + 1); });
        return s;
    }
    // RANGE ADD: touch each run once. Runs are not merged, so adds alone do NOT keep the
    // amortised bound - this is only cheap when assigns keep collapsing the structure.
    void add(int l, int r, ll x) {
        auto itr = split(r + 1), itl = split(l);
        for (auto it = itl; it != itr; ++it) it->second.second += x;
    }
    // RANGE AND / OR / MOD follow the same loop, applying the op to each run's value.
    // They are fast because every value can only decrease O(log V) times before it stops
    // changing, at which point neighbouring runs become equal and can be merged.
    template <class Op> void apply(int l, int r, Op op) {
        auto itr = split(r + 1), itl = split(l);
        for (auto it = itl; it != itr; ++it) it->second.second = op(it->second.second);
        merge(l, r);
    }
    void merge(int l, int r) {                       // glue neighbouring runs of equal value
        auto it = split(l);
        while (it != mp.end() && it->first <= r) {
            auto nx = next(it);
            if (nx != mp.end() && nx->second.second == it->second.second &&
                it->second.first + 1 == nx->first) {
                it->second.first = nx->second.first;
                mp.erase(nx);
            } else it = nx;
        }
    }
};
// Careful: worst case WITHOUT assigns is O(n) per operation. If the problem has only adds and
// queries, use a segment tree instead.
