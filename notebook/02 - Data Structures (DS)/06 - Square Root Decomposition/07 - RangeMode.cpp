// RANGE MODE - the most frequent value in a range, and its frequency. Both structures below have
// O(1) ADD and O(1) DELETE, so dropped into Mo's (01 - MoAlgorithm) they solve the problem in
// O(n sqrt n) total.
// WHEN: mode / "maximum multiplicity" queries. There is no known linear-space polylog-per-query
// solution, so sqrt IS the intended answer here, not a fallback. The mode is NOT decomposable -
// the mode of a union is not a function of the two modes - which is why a segment tree cannot do
// it and why the counter has to be maintained incrementally.
// KEY INVARIANT (both): adding one element raises some count by exactly one, so the running
// maximum can only go UP by one; deleting lowers one count by one, so the maximum can only go DOWN
// by one, and only when the deleted value was the SOLE holder of it. A count-of-counts array turns
// "is it the sole holder" into an O(1) test.

// --- max FREQUENCY only. O(1) everything, O(V) memory. Use this unless you need the value. ---
struct FreqCounter {
    vector<int> cnt, f;                                        // f[c] = #values with count == c
    int best = 0;
    FreqCounter(int V, int n) : cnt(V, 0), f(n + 2, 0) {}
    void add(int v) { f[cnt[v]]--, f[++cnt[v]]++, best = max(best, cnt[v]); }
    void del(int v) {
        f[cnt[v]]--;
        if (cnt[v] == best && !f[cnt[v]]) best--;
        f[--cnt[v]]++;
    }
    int query() { return best; }                               // the frequency of the mode
};
// --- the VALUE too: bucket the value axis and keep a max per bucket. O(sqrt V) query. ---
struct ModeCounter {
    int V, B, W;
    vector<int> cnt, bmx, bc;                                  // bc[b * W + c] flattened
    ModeCounter(int V, int n) : V(V), B(max(1, (int)sqrt((double)V) + 1)), W(n + 2),
                                cnt(V, 0), bmx(V / B + 2, 0), bc((V / B + 2) * W, 0) {}
    void add(int v) {
        int b = v / B;
        bc[b * W + cnt[v]]--, bc[b * W + ++cnt[v]]++;
        bmx[b] = max(bmx[b], cnt[v]);
    }
    void del(int v) {
        int b = v / B;
        if (bmx[b] == cnt[v] && bc[b * W + cnt[v]] == 1) bmx[b]--;
        bc[b * W + cnt[v]]--, bc[b * W + --cnt[v]]++;
    }
    pii query() {                                              // {value, count}, O(sqrt V)
        int best = 0;
        for (int x : bmx) best = max(best, x);
        for (int b = 0; b < (int)bmx.size(); b++)
            if (bmx[b] == best)
                for (int v = b * B; v < min(V, (b + 1) * B); v++)
                    if (cnt[v] == best) return {v, best};
        return {-1, 0};
    }
};
// MEMORY WARNING: bc is O(sqrt(V) * n) ints - about 160 MB at V = n = 2e5. Shrink it by passing
// the largest range length as n, by widening B (query cost O(V/B + B), memory O(V/B * n)), or by
// dropping to FreqCounter when the value itself is not needed.
// COORDINATE COMPRESS the values first so V is the number of DISTINCT values (01 - Misc/05).
// BECAUSE DELETE IS O(1) you do not need Mo's with rollback (06) - plain Mo's is enough.
// THE ONLINE VERSION, O(sqrt n) per query and no Mo's: cut the array into sqrt(n) blocks,
// precompute the mode of every block RANGE [i..j] and the prefix count of every value per block.
// A query starts from the precomputed mode of its maximal inner block range and folds in the
// O(sqrt n) leftover elements one at a time; each can only raise the answer, and the count of any
// value in the current range is (prefix count) + (leftovers seen so far), i.e. O(1). The same
// scaffolding answers any statistic that is monotone under adding one element.
// MEX WITH UPDATES is the same shape: keep cnt[] plus f[], and scan f[1], f[2], ... for the first
// zero. O(sqrt n) per query inside Mo's, O(1) amortised if elements are only ever added.
