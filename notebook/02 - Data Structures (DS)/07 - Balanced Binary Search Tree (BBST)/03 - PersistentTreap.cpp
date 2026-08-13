// PERSISTENT IMPLICIT TREAP - a persistent ARRAY that supports insert / erase / concatenate / cut
// a block in the middle, with every version staying queryable. O(log n) time and O(log n) new
// nodes per operation.
// WHEN: "version v of the text, then insert this string at position k", copy-paste editors, and
// the killer application: COPY A RANGE OF AN OLD VERSION AND PASTE IT ANYWHERE - two splits of an
// old root share all their nodes with it, so the copy is free. A persistent segment tree
// (01 - Segment Tree/03) cannot do this because its index set is fixed; 02 - ImplicitTreap can,
// but destroys the old version.
// KEY INVARIANT: COPY ON WRITE. split and merge only ever touch one root-to-leaf path, so cloning
// each node before modifying it costs O(log n) extra nodes and leaves every earlier root pointing
// at a consistent tree. Priorities are copied too, so the shape (and hence the depth bound) is
// unchanged by cloning.
struct PTreap {
    struct Node { int l = 0, r = 0, sz = 0; unsigned pr = 0; ll val = 0, sum = 0; };
    vector<Node> t;
    PTreap() : t(1) {}                                         // node 0 = the empty tree
    unsigned rnd() {
        static unsigned s = 88172645u;
        return s ^= s << 13, s ^= s >> 17, s ^= s << 5;
    }
    int cp(int x) { t.push_back(t[x]); return t.size() - 1; }
    void pull(int x) {
        t[x].sz = t[t[x].l].sz + 1 + t[t[x].r].sz;
        t[x].sum = t[t[x].l].sum + t[x].val + t[t[x].r].sum;
    }
    pair<int, int> split(int x, int k) {                       // first k elements go left
        if (!x) return {0, 0};
        int c = cp(x);
        if (t[t[c].l].sz < k) {
            auto [a, b] = split(t[c].r, k - t[t[c].l].sz - 1);
            t[c].r = a, pull(c);
            return {c, b};
        }
        auto [a, b] = split(t[c].l, k);
        t[c].l = b, pull(c);
        return {a, c};
    }
    int merge(int a, int b) {
        if (!a || !b) return a | b;
        if (t[a].pr > t[b].pr) {
            int c = cp(a), r = merge(t[c].r, b);
            t[c].r = r, pull(c);
            return c;
        }
        int c = cp(b), l = merge(a, t[c].l);
        t[c].l = l, pull(c);
        return c;
    }
    int mk(ll v) { t.push_back({0, 0, 1, rnd(), v, v}); return t.size() - 1; }
    int build(const vector<ll>& a) {                           // O(n log n), fine for a one-off
        int r = 0;
        for (ll v : a) r = merge(r, mk(v));
        return r;
    }
    int insert(int root, int pos, ll v) {                      // returns the new root
        auto [a, b] = split(root, pos);
        return merge(merge(a, mk(v)), b);
    }
    int erase(int root, int l, int r) {                        // drop positions [l, r]
        auto [a, b] = split(root, l);
        auto [m, c] = split(b, r - l + 1);
        return merge(a, c);
    }
    int cut(int root, int l, int r) {                          // the block [l, r] as its own tree
        auto [a, b] = split(root, l);
        return split(b, r - l + 1).first;
    }
    int paste(int root, int pos, int piece) {                  // splice another tree in
        auto [a, b] = split(root, pos);
        return merge(merge(a, piece), b);
    }
    ll sum(int x, int l, int r) {                              // read-only: allocates nothing
        if (!x || l > r || r < 0 || l >= t[x].sz) return 0;
        if (l <= 0 && r >= t[x].sz - 1) return t[x].sum;
        int L = t[t[x].l].sz;
        return sum(t[x].l, l, min(r, L - 1)) + (l <= L && L <= r ? t[x].val : 0) +
               sum(t[x].r, max(0, l - L - 1), r - L - 1);
    }
    ll get(int root, int i) { return sum(root, i, i); }
};
// LAZY TAGS (range add, range reverse) DO work, but push() must CLONE both children before
// pushing into them - otherwise you mutate a node an old version still owns. That is one extra
// line in push and doubles the node count; write it only if the problem needs it.
// MEMORY is the binding constraint: O(q log n) nodes at ~32 bytes. reserve() and, if a version is
// provably dead, do not bother freeing - contests end first.
// IF YOU ONLY NEED THE LATEST VERSION use 02 - ImplicitTreap (no allocation per split).
// IF THE OPERATIONS ARE POSITION-STABLE (no insert / erase, only updates) a persistent segment
// tree is 3x faster and half the memory.
