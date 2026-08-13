// LOGARITHMIC SUBARRAY AGGREGATOR - every value of op(a[l..r]) over every subarray, in O(n log A).
// Fix r. As l walks from r down to 0 the aggregate op(a[l..r]) only ever moves one way, and every
// strict move is a big jump, so only O(log A) DISTINCT values occur and equal ones form contiguous
// runs of l. subarrayAgg calls cb(r, runs) for every r (0-indexed) with runs = {v, lo, hi} meaning
// op(a[l..r]) == v for exactly the l in [lo, hi]; the runs partition [0, r] and are listed from
// l = r downwards, so runs[0].hi == r and runs.back().lo == 0.
// WHEN: "count / find the subarrays whose gcd (or / and / min) is X", "how many subarrays have
// gcd > 1", "longest subarray with or <= k". Replaces a sparse table plus a binary search per l,
// and it is shorter and exact.
// PRECONDITION for correctness: op associative, and op(x, y) monotone in the same direction for
// every x (so equal values are adjacent and the merge below is legal).
// PRECONDITION for the O(log A) run count: every STRICT change must shrink a potential of size A -
// gcd at least halves, and / or turn at least one bit off / on, lcm capped at LIM at least doubles.
// gcd, and, or, capped lcm all qualify. min and max are monotone so the code is CORRECT for them,
// but their run count is O(n), not O(log A) (a[i] = i gives n runs at r = n-1) - use a monotonic
// stack there. sum, xor and product do not qualify at all: nothing forces values to repeat.
// COMPLEXITY: O(n log A) time, O(log A) live memory plus whatever cb stores.
// DEGENERATE: empty a calls cb zero times. n == 1 gives one run {a[0], 0, 0}. All-equal gives one
// run per r. Zeros are fine for gcd (gcd(0, x) == x), so an all-zero array reports v == 0.
// EXACTNESS: exact on integers, the aggregate never leaves the value type. Counts of subarrays
// need ll (n(n+1)/2 overflows int at n = 65536), the run endpoints stay int.
template <class T> struct Run { T v; int lo, hi; };      // op(a[l..r]) == v for every l in [lo, hi]
template <class T, class Op, class F>
void subarrayAgg(const vector<T>& a, Op op, F cb) {
    vector<Run<T>> cur, nxt;                                // runs for r-1, then the ones for r
    for (int r = 0; r < sz(a); r++) {
        nxt.clear();
        nxt.pb({a[r], r, r});                               // l == r: the element on its own
        for (auto& e : cur) {                               // associativity: op(a[l..r-1], a[r])
            T v = op(e.v, a[r]);
            if (v == nxt.back().v) nxt.back().lo = e.lo;    // unchanged: swallow the whole old run
            else nxt.pb({v, e.lo, e.hi});                   // changed: at most O(log A) times
        }
        cur.swap(nxt);
        cb(r, cur);
    }
}
// --- CONSUMER 1: cnt[x] = number of subarrays whose gcd is exactly x. O(n log A (log A + log n)).
// std::gcd (C++17) and not __gcd: __gcd is a libstdc++ internal and rejects signed types on clang.
map<ll, ll> gcdHistogram(const vector<ll>& a) {
    map<ll, ll> cnt;
    subarrayAgg(a, [](ll x, ll y) { return gcd(x, y); }, [&](int, auto& v) {
        for (auto& e : v) cnt[e.v] += e.hi - e.lo + 1;
    });
    return cnt;
}
// --- CONSUMER 2: number of subarrays with or(a[l..r]) >= k, and the set of distinct subarray ORs
// (which has size O(n log A), not O(n^2) - that bound is the reason the question gets asked).
ll orAtLeast(const vector<ll>& a, ll k, set<ll>* distinct = nullptr) {
    ll c = 0;
    subarrayAgg(a, [](ll x, ll y) { return x | y; }, [&](int, auto& v) {
        for (auto& e : v) {                                 // v is sorted by value, so you could
            if (e.v >= k) c += e.hi - e.lo + 1;             // binary search - but it has O(log A)
            if (distinct) distinct->insert(e.v);            // entries, a scan is already free
        }
    });
    return c;
}
// VARIANTS
//  - LONGEST / SHORTEST subarray with a target property: the runs are sorted by l, so per r the
//    best l is an endpoint of the first / last run that satisfies it. O(n log A) overall.
//  - "gcd of a[l..r] == 1" for all pairs: gcdHistogram(a)[1].
//  - LCM needs a cap or it overflows instantly: g = gcd(x, y), then x / g > LIM / y ? LIM :
//    x / g * y. Capping keeps monotonicity, so the run structure survives.
//  - Two-pointer twin: for or / and / gcd the aggregate is monotone in l, so "count subarrays with
//    op == k" also falls to two pointers plus a sliding structure, but that needs undo; this does
//    not and generalises to any associative monotone op in the same 10 lines.
//  - The same list carried from the LEFT (fix l, grow r) works identically; pick the end your
//    query is indexed by.
