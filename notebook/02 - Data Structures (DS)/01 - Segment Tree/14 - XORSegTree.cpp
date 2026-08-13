// XOR SEGMENT TREE - sum of a[p ^ x] over l <= p <= r, for an x given per query, plus point
// updates. O(log n) both, and NO extra memory over an ordinary segment tree.
// WHEN: "the array is permuted by xor-ing every index with x" - subset/bit problems where a query
// asks about a[l..r] of a xor-shifted array, e.g. CF 1654F. Also the fast way to combine two
// arrays under an xor convolution restricted to a range.
// KEY INVARIANT: an ALIGNED block [b, b + 2^k) has its low k bits free, so xor-ing every element
// of it with x maps the block ONTO the aligned block starting at b ^ (x >> k << k) - a block goes
// to a block, never to a straddling range. The canonical decomposition of [l, r] is O(log n)
// aligned blocks, so the answer is O(log n) node lookups; the only work is finding each partner
// node, which at level k is just xor-ing the node's position-within-level by x >> k.
struct XorSeg {                                    // n MUST be a power of two; queries use x < n
    int n;
    vector<ll> t;
    XorSeg(const vector<ll>& a) : n(a.size()), t(2 * n) {
        copy(all(a), t.begin() + n);
        for (int i = n - 1; i; i--) t[i] = t[2 * i] + t[2 * i + 1];
    }
    void add(int i, ll v) { for (t[i += n] += v; i >>= 1;) t[i] = t[2 * i] + t[2 * i + 1]; }
    ll query(int l, int r, int x) {                // sum of a[p ^ x] over l <= p <= r
        ll res = 0;
        for (int lo = l + n, hi = r + n + 1, b = n; lo < hi; lo >>= 1, hi >>= 1, b >>= 1, x >>= 1) {
            if (lo & 1) res += t[((lo - b) ^ x) + b], lo++;    // b = number of nodes on this level
            if (hi & 1) res += t[((--hi - b) ^ x) + b];        //   and also the level's index base
        }
        return res;
    }
};
// THE AGGREGATE must be COMMUTATIVE (sum, xor, min, gcd): xor-ing reorders the elements inside
// each block, so anything order-sensitive (max prefix sum, matrix product) breaks.
// PAD n up to a power of two with identity elements - the alignment argument needs it.
// THE SAME TRICK, no data structure: if you only need it once, enumerate the O(log n) blocks of
// [l, r] yourself and answer each with a prefix sum of the ORIGINAL array. That is the version to
// write when there are no updates.
// RANGE UPDATES do not survive this: a lazy tag would have to travel to the xor-partner subtree.
// If you need them, keep the tag OUTSIDE (a separate BIT of "add v to a[l..r]" offsets) and add
// the contribution of the covered blocks analytically.
