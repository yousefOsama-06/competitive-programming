// BIT with RANGE UPDATE + RANGE QUERY (two BITs). 0-based, O(log n) per op.
// pre(i) = sum_{j<=i} a[j] = S1(i)*(i+1) - S2(i), where a range-add of v on [l,r] does
//   S1: +v at l, -v at r+1        S2: +v*l at l, -v*(r+1) at r+1
template <typename T = ll>
struct BIT2 {
    int n;
    vector<T> b1, b2;

    BIT2(int n = 0) : n(n), b1(n + 1, 0), b2(n + 1, 0) {}
    void add(vector<T>& b, int i, T v) { for (i++; i <= n; i += i & -i) b[i] += v; }
    T sum(const vector<T>& b, int i) const { T s = 0; for (i++; i > 0; i -= i & -i) s += b[i]; return s; }

    void upd(int l, int r, T v) {               // add v to [l, r]
        add(b1, l, v), add(b1, r + 1, -v);
        add(b2, l, v * l), add(b2, r + 1, -v * (r + 1));
    }
    T pre(int i) const { return i < 0 ? 0 : sum(b1, i) * (i + 1) - sum(b2, i); }
    T qry(int l, int r) const { return pre(r) - pre(l - 1); }
};
