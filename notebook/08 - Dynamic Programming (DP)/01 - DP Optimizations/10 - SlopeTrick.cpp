// SLOPE TRICK - maintain a CONVEX piecewise-linear f(x) as two heaps of its breakpoints.
// L = left slopes (max-heap), R = right slopes (min-heap), minf = current minimum value.
// Every operation below is O(log n). Classic use: "minimum total |change| to make an array
// non-decreasing", scheduling with earliness/tardiness penalties, and any DP where
//   dp_i(x) = min over y<=x of dp_{i-1}(y)  + |x - a_i|.
struct SlopeTrick {
    priority_queue<ll> L;                                    // left breakpoints (max-heap)
    priority_queue<ll, vector<ll>, greater<ll>> R;           // right breakpoints (min-heap)
    ll minf = 0, addL = 0, addR = 0;                         // lazy shifts of each side

    ll topL() { return L.top() + addL; }
    ll topR() { return R.top() + addR; }
    void pushL(ll x) { L.push(x - addL); }
    void pushR(ll x) { R.push(x - addR); }

    void addConst(ll c) { minf += c; }
    void addRampR(ll a) {                                    // f(x) += max(0, x - a)
        if (!L.empty()) minf += max(0LL, topL() - a);
        pushL(a);
        pushR(topL()); L.pop();
    }
    void addRampL(ll a) {                                    // f(x) += max(0, a - x)
        if (!R.empty()) minf += max(0LL, a - topR());
        pushR(a);
        pushL(topR()); R.pop();
    }
    void addAbs(ll a) { addRampL(a), addRampR(a); }          // f(x) += |x - a|
    void prefixMin() { while (!R.empty()) R.pop(); }         // f(x) = min_{y<=x} f(y)
    void suffixMin() { while (!L.empty()) L.pop(); }         // f(x) = min_{y>=x} f(y)
    void shift(ll dl, ll dr) { addL += dl, addR += dr; }     // f(x) -> min over [x-dr, x-dl]
    ll minVal() const { return minf; }
};
// RECIPE for "make a[] non-decreasing with min sum |b_i - a_i|":
//   for each a_i:  st.prefixMin();  st.addAbs(a_i);   answer = st.minVal()
// For "strictly increasing", first replace a_i by a_i - i.
