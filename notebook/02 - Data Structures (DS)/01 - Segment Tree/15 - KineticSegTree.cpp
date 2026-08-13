// KINETIC SEGMENT TREE (kinetic tournament) - each index i holds a LINE y = a[i]*T + b[i]; the
// tree answers "max over i in [l, r] of a[i]*T + b[i]" at a global time T that only MOVES FORWARD,
// and supports adding a constant to b over a range.
// WHEN: DP where the candidate values are linear in a parameter that sweeps monotonically -
// "max over j of dp[j] + (i-j)*c", scheduling with a moving deadline, CHT where the queries are
// sorted but the LINES also change over time (a Li Chao tree cannot delete or shift lines).
// If T is arbitrary (not monotone) use Li Chao / convex hull trick instead - this degenerates.
// COMPLEXITY: O(log n) per query and per range-add; advancing T is O(log^2 n) AMORTISED, and the
// total over a whole run is O((n + q) log^2 n). A single advance can cost O(n) in the worst case.
// KEY INVARIANT (the "certificate"): every node stores the winning line of its subtree AND melt =
// the earliest future time at which the winner of some node below it changes. Advancing to T
// recurses ONLY into nodes with melt <= T; everything else is provably still correct. Each
// recursion strictly changes a winner, and a winner can only change O(log n) times amortised per
// element because two lines cross at most once.
struct KineticSeg {
    int n;
    ll T = 0;
    vector<ll> a, b, lz, melt;                                 // a, b = the node's WINNING line
    KineticSeg(const vector<ll>& A, const vector<ll>& B)
        : n(A.size()), a(4 * n), b(4 * n), lz(4 * n, 0), melt(4 * n, llinf) {
        build(A, B, 1, 0, n - 1);
    }
    static ll fdiv(ll p, ll q) { return p / q - ((p % q != 0) && ((p < 0) != (q < 0))); }
    ll cross(int w, int o) {                                   // first time o strictly beats w
        if (a[o] <= a[w]) return llinf;                        // parallel or flatter: never
        return fdiv(b[w] - b[o], a[o] - a[w]) + 1;
    }
    void pull(int x) {
        int L = 2 * x, R = L + 1;
        int w = a[L] * T + b[L] >= a[R] * T + b[R] ? L : R, o = L + R - w;
        a[x] = a[w], b[x] = b[w];
        melt[x] = min({melt[L], melt[R], cross(w, o)});
    }
    void apply(int x, ll v) { b[x] += v, lz[x] += v; }         // uniform shift keeps every melt
    void push(int x) { if (lz[x]) apply(2 * x, lz[x]), apply(2 * x + 1, lz[x]), lz[x] = 0; }
    void build(const vector<ll>& A, const vector<ll>& B, int x, int lo, int hi) {
        lz[x] = 0, melt[x] = llinf;
        if (lo == hi) { a[x] = A[lo], b[x] = B[lo]; return; }
        int m = (lo + hi) / 2;
        build(A, B, 2 * x, lo, m), build(A, B, 2 * x + 1, m + 1, hi);
        pull(x);
    }
    void heaten(int x, int lo, int hi) {
        if (melt[x] > T || lo == hi) return;                   // certificate still valid
        int m = (lo + hi) / 2;
        push(x);
        heaten(2 * x, lo, m), heaten(2 * x + 1, m + 1, hi);
        pull(x);
    }
    void advance(ll t) { T = t; heaten(1, 0, n - 1); }         // t must be >= the current T
    void add(int x, int lo, int hi, int l, int r, ll v) {      // b[i] += v for i in [l, r]
        if (r < lo || hi < l) return;
        if (l <= lo && hi <= r) { apply(x, v); return; }
        int m = (lo + hi) / 2;
        push(x);
        add(2 * x, lo, m, l, r, v), add(2 * x + 1, m + 1, hi, l, r, v);
        pull(x);
    }
    void add(int l, int r, ll v) { add(1, 0, n - 1, l, r, v); }
    void set(int x, int lo, int hi, int i, ll na, ll nb) {     // replace the line at index i
        if (lo == hi) { a[x] = na, b[x] = nb; return; }
        int m = (lo + hi) / 2;
        push(x);
        i <= m ? set(2 * x, lo, m, i, na, nb) : set(2 * x + 1, m + 1, hi, i, na, nb);
        pull(x);
    }
    void set(int i, ll na, ll nb) { set(1, 0, n - 1, i, na, nb); }
    ll qry(int x, int lo, int hi, int l, int r) {              // max at the CURRENT T
        if (r < lo || hi < l) return -llinf;
        if (l <= lo && hi <= r) return a[x] * T + b[x];
        int m = (lo + hi) / 2;
        push(x);
        return max(qry(2 * x, lo, m, l, r), qry(2 * x + 1, m + 1, hi, l, r));
    }
    ll qry(int l, int r) { return qry(1, 0, n - 1, l, r); }
};
// MIN instead of max: negate both a and b, or flip the comparison in pull() and cross().
// ADDING TO THE SLOPES over a range destroys every certificate below and is NOT amortised - do it
// only at a single index (set()) or rebuild.
// SEGMENT TREE BEATS (04) is the same idea with a different certificate: recurse only where the
// tag cannot be applied wholesale, and bound the recursions by a potential. If you can phrase your
// update as "the answer only changes when two stored candidates swap order", this is the shape.
// A CHEAPER SPECIAL CASE: if the lines are only ever ADDED (never removed) and queries are sorted,
// a monotone CHT is O(1) amortised per op - reach for that first.
