// DYNAMIC CONVEX HULL TRICK (UPPER hull -> MAXIMUM of m*x + b). Lines may be inserted in ANY order
// and queried at any x, O(log n) each. If your slopes AND queries are both monotone, the deque
// version at the bottom of this file is O(1) amortised and much shorter - prefer it.
// For a MINIMUM, insert (-m, -b) and negate the answer.
// To maximise over all x in [l, r], query only l and r: the upper envelope is convex.
struct Line {
    ll m, b;
    mutable function<const Line *()> succ;

    bool operator<(const Line &other) const {
        return m < other.m;
    }

    bool operator<(const ll &x) const {
        const Line *s = succ();
        if (!s)
            return 0;
        return (lll)(b - s->b) < (lll)(s->m - m) * x;   // __int128: |dm|*|x| overflows ll
    }
};

// will maintain upper hull for maximum
struct HullDynamic : public multiset<Line, less<>> {
    bool bad(iterator y) {
        auto z = next(y);
        if (y == begin()) {
            if (z == end())
                return 0;
            return y->m == z->m && y->b <= z->b;
        }
        auto x = prev(y);
        if (z == end())
            return y->m == x->m && y->b <= x->b;
        // __int128, NOT long double: each factor reaches ~2e18, the product needs ~123 bits,
        // and this test is consulted exactly when three lines are nearly collinear.
        return (lll)(x->b - y->b) * (z->m - y->m) >= (lll)(y->b - z->b) * (y->m - x->m);
    }

    void insert_line(ll m, ll b) {
//         for minimum
//        m *= -1;
//        b *= -1;
        auto y = insert({m, b});
        y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
        if (bad(y)) {
            erase(y);
            return;
        }
        while (next(y) != end() && bad(next(y)))
            erase(next(y));
        while (y != begin() && bad(prev(y)))
            erase(prev(y));
    }

    ll query(ll x) {                                 // UB if empty - insert at least one line first
        auto l = *lower_bound(x);
//        for minimum
//        return -(l.m * x + l.b);
        return l.m * x + l.b;
    }
};
// ---------------------------------------------------------------------------------------------
// MONOTONE CHT (deque) - the version you actually want when the DP feeds lines in slope order.
// MINIMISES m*x + b. Requires slopes added in DECREASING order; queries in increasing x are O(1)
// amortised (move the head), arbitrary x is O(log n) by binary search on the hull.
// For a MAXIMUM, negate m and b on the way in and negate the answer on the way out.
struct MonoCHT {
    vector<ll> M, B;
    int head = 0;
    bool bad(int a, int b, int c) {                  // is line b unnecessary between a and c?
        return (lll)(B[c] - B[a]) * (M[a] - M[b]) <= (lll)(B[b] - B[a]) * (M[a] - M[c]);
    }
    void add(ll m, ll b) {                           // m must be <= the last slope added
        M.push_back(m), B.push_back(b);
        while (M.size() >= 3 && bad(M.size() - 3, M.size() - 2, M.size() - 1))
            M.erase(M.end() - 2), B.erase(B.end() - 2);
        head = min(head, (int)M.size() - 1);
    }
    ll f(int i, ll x) { return M[i] * x + B[i]; }
    ll queryInc(ll x) {                              // x non-decreasing across calls
        while (head + 1 < (int)M.size() && f(head + 1, x) <= f(head, x)) head++;
        return f(head, x);
    }
    ll query(ll x) {                                 // any x, O(log n)
        int lo = 0, hi = M.size() - 1;
        while (lo < hi) { int m = (lo + hi) / 2; f(m + 1, x) <= f(m, x) ? lo = m + 1 : hi = m; }
        return f(lo, x);
    }
};
// WHEN EACH ONE: monotone slopes + monotone queries -> MonoCHT::queryInc, O(n).
//                monotone slopes, arbitrary queries  -> MonoCHT::query, O(n log n).
//                arbitrary slopes                    -> HullDynamic above, or Li Chao (04),
//                                                       which needs no ordering at all.
//                lines inserted and removed          -> CHT with rollback (03) or Li Chao merge.