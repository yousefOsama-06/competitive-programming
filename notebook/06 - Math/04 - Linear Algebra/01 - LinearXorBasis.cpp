// XOR basis (vector space over GF(2)). insert / query O(B).
// span size = 2^sz. Use MERGE to keep a basis in each segment-tree node -> max-xor over a RANGE.
struct Basis {
    static const int B = 62;      // 60 silently drops bits 61-62 of an ll up to 9e18
    ll b[B + 1] = {};                                   // b[i] has leading bit i
    int sz = 0;

    bool insert(ll x) {                                 // false if x already in the span
        for (int i = B; i >= 0; i--) if (x >> i & 1) {
            if (!b[i]) { b[i] = x, sz++; return true; }
            x ^= b[i];
        }
        return false;
    }
    bool contains(ll x) const {
        for (int i = B; i >= 0; i--) if (x >> i & 1) { if (!b[i]) return false; x ^= b[i]; }
        return true;
    }
    ll maxXor(ll r = 0) const { for (int i = B; i >= 0; i--) r = max(r, r ^ b[i]); return r; }
    ll minXor(ll r = 0) const { for (int i = B; i >= 0; i--) if (r >> i & 1) r ^= b[i]; return r; }
    void reduce() {                                     // reduced row echelon form
        for (int i = B; i >= 0; i--) if (b[i])
            for (int j = i - 1; j >= 0; j--) if (b[i] >> j & 1) b[i] ^= b[j];
    }
    ll kth(ll k) {                                      // k-th SMALLEST value in the span, 0-indexed
        reduce();                                       // kth(0) == 0; -1 if k >= 2^sz
        ll r = 0;
        for (int i = 0; i <= B; i++) if (b[i]) { if (k & 1) r ^= b[i]; k >>= 1; }
        return k ? -1 : r;
    }
    ll spanSize() const { return sz >= 63 ? -1 : 1LL << sz; }
    friend Basis merge(Basis a, const Basis& c) {
        for (int i = 0; i <= B; i++) if (c.b[i]) a.insert(c.b[i]);
        return a;
    }
};
