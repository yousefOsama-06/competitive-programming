// ARITHMETIC-PROGRESSION SEGMENT TREE - "add a, a+d, a+2d, ... to a[l..r]", range sum, O(log n).
// WHEN: updates whose value DEPENDS ON THE POSITION inside the range - "add i-l+1 to a[l..r]",
// "add w to every node on a path, weighted by depth", any linear ramp. A plain lazy segment tree
// cannot do it because its tag must be one number.
// KEY INVARIANT: a progression restricted to a sub-range is still a progression, so the tag stays
// two numbers (a, d) and stays CLOSED under composition: two tags add componentwise. The tag is
// ANCHORED at the node's left endpoint, so pushing to the right child skips (m+1-b) terms.
struct APSeg {
    struct Tag { ll a = 0, d = 0; };                           // a, a+d, a+2d, ... from node's left
    int n;
    vector<ll> s;
    vector<Tag> lz;
    APSeg(int n) : n(n), s(4 * n, 0), lz(4 * n) {}
    static ll tot(Tag t, ll len) { return t.a * len + t.d * (len * (len - 1) / 2); }
    static Tag skip(Tag t, ll k) { return {t.a + k * t.d, t.d}; }          // drop the first k terms
    void apply(int x, Tag t, int len) { s[x] += tot(t, len), lz[x].a += t.a, lz[x].d += t.d; }
    void push(int x, int b, int e) {
        if (!lz[x].a && !lz[x].d) return;
        int m = (b + e) / 2;
        apply(2 * x, lz[x], m - b + 1), apply(2 * x + 1, skip(lz[x], m + 1 - b), e - m);
        lz[x] = {};
    }
    void build(const vector<ll>& a, int x, int b, int e) {
        lz[x] = {};
        if (b == e) { s[x] = a[b]; return; }
        int m = (b + e) / 2;
        build(a, 2 * x, b, m), build(a, 2 * x + 1, m + 1, e);
        s[x] = s[2 * x] + s[2 * x + 1];
    }
    void build(const vector<ll>& a) { build(a, 1, 0, n - 1); }
    void upd(int x, int b, int e, int l, int r, Tag t) {       // t is anchored at index l
        if (r < b || e < l) return;
        if (l <= b && e <= r) { apply(x, skip(t, b - l), e - b + 1); return; }
        push(x, b, e);
        int m = (b + e) / 2;
        upd(2 * x, b, m, l, r, t), upd(2 * x + 1, m + 1, e, l, r, t);
        s[x] = s[2 * x] + s[2 * x + 1];
    }
    void upd(int l, int r, ll a, ll d) { upd(1, 0, n - 1, l, r, {a, d}); } // a[l]+=a, a[l+1]+=a+d..
    ll qry(int x, int b, int e, int l, int r) {
        if (r < b || e < l) return 0;
        if (l <= b && e <= r) return s[x];
        push(x, b, e);
        int m = (b + e) / 2;
        return qry(2 * x, b, m, l, r) + qry(2 * x + 1, m + 1, e, l, r);
    }
    ll qry(int l, int r) { return qry(1, 0, n - 1, l, r); }
};
// OVERFLOW: tot() is O(len^2 * d) - with n = 2e5 and d = 1e9 that is 2e19, past ll. Work mod p
// (replace /2 by * inv2) or bound d.
// DECREASING ramps: pass a negative d; "add a, a-d, a-2d" to [l,r] read right-to-left is the same
// tag anchored at r, so mirror the array or negate d and anchor at l with a' = a + (r-l)*d.
// LIGHTER ALTERNATIVES, use these when they fit:
//   Range AP add + POINT query only: one BIT over the second difference b[i] = a[i]-2a[i-1]+a[i-2];
//     an AP add touches 4 cells, and a[i] is a double prefix sum. 5 lines (02 - BIT).
//   OFFLINE with all updates before all queries: apply the second-difference trick to a plain
//     array and prefix-sum twice at the end. O(n) total.
// GENERAL RULE: any tag family closed under composition and restriction works. Polynomials of
// degree k need k+1 coefficients; affine "a[i] = p*a[i] + q" needs (p, q) composed as function
// composition (that one is NOT commutative - compose in the right order).
