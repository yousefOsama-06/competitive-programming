// Needs: 02 - Binary Indexed Tree (BIT)/01 - FenwickTree.cpp
// COUNT DISTINCT VALUES IN A RANGE, OFFLINE - the single most reused sweep in the whole notebook.
// O((n + q) log n), O(n) memory, ~20 lines. (SPOJ DQUERY and a hundred clones.)
// WHEN: any "how many different / how many values appear exactly once / sum of distinct values"
// question with all queries known up front. Mo's (06 - Square Root Decomposition/01) also does it
// but is O(n sqrt n); a persistent segment tree does it ONLINE at O(log n) per query with the very
// same invariant, so start here and upgrade only if the queries are forced online.
// KEY INVARIANT: sweep r left to right and keep the BIT holding a 1 at the LAST OCCURRENCE SO FAR
// of every distinct value and 0 everywhere else. Then the number of ones in [l, r] is exactly the
// number of distinct values in [l, r]: each value is counted at its rightmost occurrence, which
// lies in [l, r] if and only if the value occurs there at all. Moving r to a new position means
// clearing the previous occurrence of a[r] and setting the new one - two BIT updates.
struct OfflineDistinct {
    int n;
    vector<int> a;                                             // values already compressed to [0,V)
    vector<array<int, 3>> qs;                                  // {r, l, id}
    OfflineDistinct(const vector<int>& a) : n(a.size()), a(a) {}
    void ask(int l, int r, int id) { qs.push_back({r, l, id}); }
    vector<int> solve(int V) {
        sort(all(qs));
        FenwickTree<int> bit(n);
        vector<int> last(V, -1), res(qs.size());
        int at = 0;
        for (int r = 0; r < n; r++) {
            if (last[a[r]] >= 0) bit.add(last[a[r]], -1);
            bit.add(last[a[r]] = r, 1);
            while (at < (int)qs.size() && qs[at][0] == r)
                res[qs[at][2]] = bit.query(qs[at][1], r), at++;
        }
        return res;
    }
};
// VALUES APPEARING EXACTLY ONCE in [l, r]: keep TWO BITs, one with a 1 at the last occurrence and
// one with a 1 at the second-to-last; the answer is ones(last) - ones(secondLast) over [l, r].
// SUM of the distinct values: put a[r] instead of 1 at the last occurrence. Any per-value weight
// works the same way, including "count values whose multiplicity is >= k" (mark the k-th last).
// ONLINE version: build a persistent segment tree where version r is "the array of last
// occurrences after processing r", then the answer is a range count on version r. Same invariant,
// O(n log n) memory (01 - Segment Tree/03).
// UPDATES to the array as well: this sweep dies; use Mo's with updates (06/05) or a BIT of segment
// trees (05) keyed by "previous occurrence" - both are much heavier, so check first whether the
// problem really needs them.
