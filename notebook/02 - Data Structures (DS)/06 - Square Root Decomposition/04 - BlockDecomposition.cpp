// SQRT / BLOCK DECOMPOSITION. Split [0,n) into blocks of B ~ sqrt(n) and keep one aggregate per
// block. Point update O(1..B), range query O(sqrt n). A BIT/segtree beats it for sum, so reach for
// this when the aggregate is NOT a group / has no lazy tag: block-sorted arrays, block frequency
// tables, "count values > x in [l,r]", range-assign + weird query, or a per-block rebuild that is
// cheap. General shape below with sum; swap `agg` and the two combine lines for anything else.
struct Blocks {
    int n, B;
    vector<ll> a, agg;                                         // agg[b] = aggregate of block b
    Blocks(const vector<ll>& v) : n(v.size()), B(max(1, (int)sqrt(n) + 1)), a(v),
                                  agg((n + B - 1) / B, 0) {
        for (int i = 0; i < n; i++) agg[i / B] += a[i];
    }
    void set(int i, ll v) { agg[i / B] += v - a[i], a[i] = v; } // O(1); for non-group aggregates
                                                               // rebuild the block instead, O(B)
    ll query(int l, int r) {                                   // inclusive, O(n/B + B)
        ll s = 0;
        int bl = l / B, br = r / B;
        if (bl == br) { for (int i = l; i <= r; i++) s += a[i]; return s; }
        for (int i = l; i < (bl + 1) * B; i++) s += a[i];
        for (int b = bl + 1; b < br; b++) s += agg[b];
        for (int i = br * B; i <= r; i++) s += a[i];
        return s;
    }
};
// RANGE UPDATE, RANGE QUERY without lazy propagation: keep add[b] per block. A partial block is
// updated element-wise and its aggregate rebuilt; a full block only bumps add[b]. Query adds
// add[b] * (elements taken from b).
// BLOCK-SORTED ARRAY ("sqrt tree"): store each block also as a sorted copy. Then
//   count of values <= x in [l,r] = partial blocks scanned + binary search inside full blocks,
//   O(sqrt(n) log n) per query, O(B log B) per point update (re-sort one block). This is the
//   offline-free alternative to a wavelet tree / merge-sort tree when updates are present.
// REBUILD TRICK: when an operation is expensive but rare (e.g. inserting into the middle), buffer
// changes and rebuild the whole decomposition every sqrt(q) operations -> O((n+q) sqrt q) total.
// CHOOSING B: query cost n/B + B*c is minimised at B = sqrt(n*c) where c is the per-element cost;
// if updates are much more frequent than queries, take bigger blocks, and vice versa.
