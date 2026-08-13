// BLOCK LIST (chunked / unrolled list, a "sqrt rope") - an array that supports insert and erase in
// the middle, RANGE ASSIGN, RANGE REVERSE, and range statistics, all in O(sqrt n).
// WHEN: an implicit treap (07 - BBST/02) does the same in O(log n) and is usually the right answer.
// Reach for this instead when the per-block statistic is something a treap node cannot merge in
// O(1) - "how many elements >= x in this range" (each block keeps a SORTED copy), "the k-th
// smallest", "count of distinct" - or when you want a structure you can debug by printing.
// COMPLEXITY: O(sqrt n) per operation amortised, with a full rebuild every ~sqrt n operations to
// stop the blocks from fragmenting. Rebuild is O(n).
// KEY INVARIANT: the array is a LIST OF BLOCKS, each holding at most ~2B elements plus two lazy
// tags (assigned-value, reversed). Every range operation starts by SPLITTING at both endpoints, so
// afterwards the range is an exact run of whole blocks and each one can be tagged in O(1). A
// reverse then only has to flip each block's tag and reverse the ORDER OF THE BLOCK LIST itself.
struct BlockList {
    struct Block {
        vector<int> v, s;                                      // s = sorted copy of v
        int len = 0, asg = -1;                                 // asg >= 0: whole block is asg
        bool rev = false;
        void resort() { s = v, sort(all(s)); }
        void solid() {                                         // drop both tags
            if (asg >= 0) v.assign(len, asg), asg = -1, resort();
            if (rev) std::reverse(all(v)), rev = false;
        }
        int atLeast(int x) {                                   // how many elements >= x
            if (asg >= 0) return asg >= x ? len : 0;
            return s.end() - lower_bound(all(s), x);
        }
    };
    vector<Block> b;
    int n, B, ops = 0;
    BlockList(const vector<int>& a) : n(a.size()), B(max(1, (int)sqrt((double)n) + 1)) { chunk(a); }
    void chunk(const vector<int>& a) {
        b.clear();
        for (int i = 0; i < (int)a.size(); i += B) {
            Block x;
            x.v.assign(a.begin() + i, a.begin() + min((int)a.size(), i + B));
            x.len = x.v.size(), x.resort();
            b.push_back(x);
        }
    }
    void rebuild() {                                           // O(n), every ~B operations
        vector<int> a;
        a.reserve(n);
        for (auto& x : b) {
            x.solid();
            for (int y : x.v) a.push_back(y);
        }
        chunk(a);
    }
    int split(int i) {                                         // index of the block starting at i
        if (i >= n) return b.size();
        int cur = 0;
        for (int k = 0; k < (int)b.size(); k++) {
            if (i < cur + b[k].len) {
                if (i == cur) return k;
                int off = i - cur;
                b[k].solid();
                Block nb;
                nb.v.assign(b[k].v.begin() + off, b[k].v.end());
                nb.len = nb.v.size(), nb.resort();
                b[k].v.resize(off), b[k].len = off, b[k].resort();
                b.insert(b.begin() + k + 1, nb);
                return k + 1;
            }
            cur += b[k].len;
        }
        return b.size();
    }
    void tick() { if (++ops % B == 0) rebuild(); }             // call after every modification
    void assign(int l, int r, int x) {
        int p = split(l), q = split(r + 1);
        for (int k = p; k < q; k++) b[k].asg = x, b[k].rev = false;
        tick();
    }
    void reverse(int l, int r) {
        int p = split(l), q = split(r + 1);
        for (int k = p; k < q; k++) b[k].rev ^= 1;
        std::reverse(b.begin() + p, b.begin() + q);
        tick();
    }
    void insert(int i, int x) {
        int p = split(i);
        Block nb;
        nb.v = {x}, nb.len = 1, nb.resort();
        b.insert(b.begin() + p, nb), n++;
        tick();
    }
    void erase(int l, int r) {
        int p = split(l), q = split(r + 1);
        b.erase(b.begin() + p, b.begin() + q), n -= r - l + 1;
        tick();
    }
    int count(int l, int r, int x) {                           // how many a[i] >= x in [l, r]
        int p = split(l), q = split(r + 1), res = 0;
        for (int k = p; k < q; k++) res += b[k].atLeast(x);
        return res;
    }
};
// THE TWO TAGS INTERACT: assigning kills a pending reverse (all values are equal), reversing an
// assigned block is a no-op. solid() is the only place that has to get the order right.
// OTHER PER-BLOCK STATISTICS drop straight in: a frequency map (count of a value), a prefix sum
// (range sum with range assign), a max, a gcd. Anything you can rebuild in O(B) works.
// WITHOUT insert / erase / reverse, do not write this: 04 - BlockDecomposition is half the code.
// WITHOUT the sorted-copy statistic, an implicit treap is faster and shorter.
