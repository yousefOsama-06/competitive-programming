// VENICE TECHNIQUE - a multiset with an O(1) "add x to EVERY element".
// Keep a global offset; store values shifted by it. 15 lines, and it turns a whole family of
// "everyone loses X health, remove the dead" problems into a one-liner.
struct Venice {
    multiset<ll> s;
    ll water = 0;                                      // global offset

    void add(ll v) { s.insert(v + water); }            // insert the true value v
    void erase(ll v) { auto it = s.find(v + water); if (it != s.end()) s.erase(it); }
    void addAll(ll v) { water -= v; }                  // O(1): add v to every element
    ll getMin() const { return *s.begin() - water; }
    ll getMax() const { return *s.rbegin() - water; }
    int size() const { return s.size(); }
    // Pop everything whose TRUE value is <= t (e.g. everyone who died).
    template <class F> void popLE(ll t, F onPop) {
        while (!s.empty() && *s.begin() - water <= t) { onPop(*s.begin() - water); s.erase(s.begin()); }
    }
};
// SAME IDEA elsewhere: a binary trie with a global XOR mask ("xor every element by x"),
// a BIT with a global additive offset, a lazy "add to all" on a heap.

// SWAG (Sliding Window Aggregation) for ANY monoid - non-commutative and non-invertible ops
// included: matrix product, affine composition, gcd, "merge two structs".
// Amortised O(1) push/pop/fold. This generalises 09/02 - TwoStackQueue.cpp, which is hardwired
// to min/max. Prefix-product-and-divide does NOT work without an inverse - this does.
template <class T, class Op>
struct SWAG {
    vector<pair<T, T>> front_, back_;                  // {value, aggregate from here to the bottom}
    T id;
    Op op;
    SWAG(T id, Op op) : id(id), op(op) {}

    T foldFront() const { return front_.empty() ? id : front_.back().second; }
    T foldBack() const { return back_.empty() ? id : back_.back().second; }
    T fold() const { return op(foldFront(), foldBack()); }        // front first: order matters
    void push(T v) { back_.push_back({v, op(foldBack(), v)}); }
    void pop() {
        if (front_.empty())
            while (!back_.empty()) {
                T v = back_.back().first; back_.pop_back();
                front_.push_back({v, op(v, foldFront())});        // reversed accumulation
            }
        if (!front_.empty()) front_.pop_back();
    }
    int size() const { return front_.size() + back_.size(); }
};
// USAGE: SWAG<ll, function<ll(ll,ll)>> q(0, [](ll a, ll b){ return a + b; });
// For non-commutative ops keep the argument order exactly as written above, or the window
// product comes out reversed.
