// STATIC TO DYNAMIC (the LOGARITHMIC METHOD) - makes ANY static structure accept insertions.
// If the static one builds in O(B(n)) and answers in O(Q(n)), you get insert in O(B(n)/n * log n)
// AMORTISED and query in O(Q(n) * log n) - you ask every bucket and combine.
// WHEN: you have a build-once structure (convex hull, sorted array, wavelet tree, suffix
// automaton, Aho-Corasick, KD-tree, sparse table) and the problem inserts items online. Do NOT
// use it when a natively dynamic structure exists and is fast enough - a BIT beats "buckets of
// sorted arrays" for prefix counting, and a balanced BST beats "buckets of sorted arrays" for
// order statistics.
// KEY INVARIANT: keep at most one instance of each size 2^0, 2^1, 2^2, ... - a BINARY COUNTER.
// Inserting sets the lowest empty bit and merges every full bucket below it, so a bucket of size
// 2^k is rebuilt only once per 2^k insertions; summed over k that is O(log n) rebuilds of
// amortised cost B(n)/n each. Because the buckets partition the items, ANY decomposable query
// (one whose answer over a union is a cheap combination of the answers over the parts: count, sum,
// min, max, "does there exist") can be answered by asking all O(log n) of them.
template <class Static, class Item>
struct StaticToDynamic {
    vector<vector<Item>> buf;                                  // buf[k] is empty or has 2^k items
    vector<Static> ds;
    void insert(Item x) {
        vector<Item> cur{x};
        int k = 0;
        while (k < (int)buf.size() && !buf[k].empty()) {
            for (auto& y : buf[k]) cur.push_back(y);
            buf[k].clear(), ds[k] = Static();
            k++;
        }
        while ((int)buf.size() <= k) buf.emplace_back(), ds.emplace_back();
        buf[k] = cur, ds[k] = Static(cur);                     // one rebuild, size 2^k
    }
    template <class F> void each(F f) {                        // f(instance) over every bucket
        for (int k = 0; k < (int)ds.size(); k++) if (!buf[k].empty()) f(ds[k]);
    }
};
// EXAMPLE static structure - a sorted array answering "how many items <= x".
struct SortedBucket {
    vector<ll> v;
    SortedBucket() {}
    SortedBucket(vector<ll> a) : v(a) { sort(all(v)); }
    int count(ll x) { return upper_bound(all(v), x) - v.begin(); }
};
// USAGE: StaticToDynamic<SortedBucket, ll> d; d.insert(5);
//        int c = 0; d.each([&](SortedBucket& b) { c += b.count(x); });
// DELETIONS, if the aggregate is INVERTIBLE (counts, sums): keep a SECOND instance holding the
// deleted items and answer query(all) - query(deleted). This is "deletion by counting" and it is
// how you get a fully dynamic convex-hull-trick or a deletable "count <= x".
// DELETIONS otherwise (min, max, "exists"): you cannot subtract - use offline dynamic connectivity
// style divide and conquer over the timeline (14) instead.
// WORST CASE: a single insertion can cost O(B(n)) (rebuilding the whole thing). If the problem has
// a per-query time limit rather than a total one, spread the rebuild over the next 2^k insertions
// (the "global rebuilding" / de-amortisation trick) - rarely needed in contests.
