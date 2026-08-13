// MO'S ON TWO INTERVALS ("4D Mo") - each query is a PAIR of ranges [l1, r1] and [l2, r2] and the
// answer depends on the multiset union of both. Four pointers instead of two.
// COMPLEXITY: with block size B the cost is O(q * B) for the three bucketed pointers plus
// O(n^4 / B^3) for the monotone one; B ~ n^(3/4) balances them at O(n^(7/4)) when q ~ n.
// WHEN: "compare two subtrees / two subarrays" - e.g. the most common value in the union of two
// subtrees (CF 1767F). Both ranges must be flattened to intervals first: for subtrees use the
// Euler tour (09 - Trees/06), which is what makes a tree query a pair of intervals.
// Do NOT reach for this before checking whether the two ranges can be merged into one, or whether
// the answer is decomposable (then a segment tree merge, 16 - SegTreeMerging, is far faster).
// KEY INVARIANT: the same amortisation as ordinary Mo's, one level deeper. Sort by
// (l1/B, r1/B, l2/B, r2): the first three keys change O(B) per query each because they are
// bucketed, and the last one is monotone inside every fixed triple of buckets, so it sweeps the
// array once per triple. The state must support ADD and DELETE in O(1) - it is shared by both
// intervals, so an element can be inserted twice and that must be meaningful.
struct Mo2 {
    struct Q { int l1, r1, l2, r2, id; };
    int n, B;
    vector<Q> qs;
    Mo2(int n, int q) : n(n), B(max(1, (int)pow((double)n, 0.75))) { qs.reserve(q); }
    void addQuery(int l1, int r1, int l2, int r2, int id) { qs.push_back({l1, r1, l2, r2, id}); }
    // add(i) / del(i) update the shared state with position i; get() reads the answer.
    template <class A, class D, class G>
    void run(A add, D del, G get) {
        int Bl = B;
        sort(all(qs), [&](const Q& a, const Q& b) {
            array<int, 7> x{a.l1 / Bl, a.r1 / Bl, a.l2 / Bl, a.r2, a.l1, a.r1, a.l2};
            array<int, 7> y{b.l1 / Bl, b.r1 / Bl, b.l2 / Bl, b.r2, b.l1, b.r1, b.l2};
            return x < y;
        });
        int a = 0, b = -1, c = 0, d = -1;                      // [a, b] and [c, d], both empty
        for (auto& q : qs) {
            while (b < q.r1) add(++b);
            while (a > q.l1) add(--a);
            while (b > q.r1) del(b--);
            while (a < q.l1) del(a++);
            while (d < q.r2) add(++d);
            while (c > q.l2) add(--c);
            while (d > q.r2) del(d--);
            while (c < q.l2) del(c++);
            get(q.id);
        }
    }
};
// THE ORDER OF THE FOUR LOOPS MATTERS: always GROW before you SHRINK, exactly as in 1D Mo's,
// or the two ranges can transiently cross into an invalid state (negative counts).
// OVERLAPPING RANGES: if the two intervals may intersect, an element gets added twice. Decide up
// front whether the state should count it twice (multiset union) or once (set union) - for set
// union keep a per-position "times present" counter and only touch the state on 0 <-> 1.
// TREE PAIRS: l1 = tin[u], r1 = tout[u], l2 = tin[v], r2 = tout[v]. If u is an ancestor of v the
// intervals nest, so the "counted twice" question above is not hypothetical.
// 3+ INTERVALS: the same sort with more keys works but the exponent gets ugly fast - at that point
// look for a completely different decomposition.
