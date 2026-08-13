// TWO SMALL STRUCTURES THAT KEEP SHOWING UP, and are always retyped badly under pressure.

// --- LAZY-DELETION HEAP: a priority_queue that supports erase(x), O(log n) amortised. ---
// Keep a second heap of "things already deleted"; before reading the top, pop matching pairs.
// This is why you almost never need decrease-key: push the new value and lazily drop the old one.
template <class T, class Cmp = less<T>>
struct LazyHeap {
    priority_queue<T, vector<T>, Cmp> in, del;
    void push(T x) { in.push(x); }
    void erase(T x) { del.push(x); }                           // x must actually be present
    void clean() { while (!del.empty() && in.top() == del.top()) in.pop(), del.pop(); }
    bool empty() { clean(); return in.empty(); }
    int size() { return (int)in.size() - (int)del.size(); }
    T top() { clean(); return in.top(); }
    void pop() { clean(); in.pop(); }
};
// A multiset does the same thing with erase(find(x)) and is shorter - reach for THIS when you
// need heap performance (no red-black overhead) or when the values are not comparable-unique.
// TRAP: erase(x) must be matched by an earlier push(x), or `size()` and `clean()` drift.

// --- TOP-K SUM: maintain the sum of the k largest elements under insert and erase. ---
// Two multisets with a fixed frontier: `hi` holds the k largest (and its sum), `lo` the rest.
// After every operation, move elements across the frontier until |hi| == min(k, total).
// USES: "sum of the k best so far" while sweeping, sliding-window k-th order statistics, and
// scheduling/greedy problems where you keep the k cheapest and pay for the rest.
struct TopK {
    int k;
    ll sum = 0;                                                // sum of the k largest
    multiset<ll> hi, lo;                                       // hi = the chosen k, lo = the rest
    TopK(int k) : k(k) {}
    void fix() {
        while ((int)hi.size() < k && !lo.empty()) {
            auto it = prev(lo.end());
            sum += *it, hi.insert(*it), lo.erase(it);
        }
        while ((int)hi.size() > k) {
            auto it = hi.begin();
            sum -= *it, lo.insert(*it), hi.erase(it);
        }
        while (!hi.empty() && !lo.empty() && *hi.begin() < *prev(lo.end())) {
            auto a = hi.begin(), b = prev(lo.end());
            sum += *b - *a;
            hi.insert(*b), lo.insert(*a), hi.erase(a), lo.erase(b);
        }
    }
    void insert(ll x) { lo.insert(x), fix(); }
    void erase(ll x) {
        auto it = hi.find(x);
        if (it != hi.end()) sum -= x, hi.erase(it);
        else lo.erase(lo.find(x));
        fix();
    }
    int size() const { return hi.size() + lo.size(); }
};
// SMALLEST k instead: negate on the way in and out, or mirror the two sets.
// If k CHANGES too, just set k and call fix() - the frontier moves in O(|dk| log n).
// If you only ever ADD (no erase), a single size-k min-heap is shorter: push, and pop while the
// size exceeds k, keeping a running sum.
