// PARALLEL BINARY SEARCH - binary search the answer for MANY queries at once, sweeping the
// shared structure once per level instead of once per query.
// PRECONDITION: test(i) must be MONOTONE in time - once it is true it stays true. If updates
// can also undo things, parallel binary search does not apply; use offline divide & conquer over
// the timeline with a rollback DSU (02 - Data Structures/14) instead.
// Total: O(log(maxT) * (cost of one full sweep)).
// Shape: "after which update does query q first become satisfiable?"
//   lo[q], hi[q] track each query's remaining range; every round, bucket the queries by their
//   midpoint, replay the updates in order, and test each query when the sweep reaches its mid.
template <class Apply, class Reset, class Test>
vector<int> parallelBinarySearch(int q, int T, Apply applyUpdate, Reset reset, Test test) {
    // applyUpdate(t): apply update t to the shared structure
    // reset(): clear the structure
    // test(i): is query i satisfied by the current structure?
    vector<int> lo(q, 0), hi(q, T);                    // answer in [lo, hi]; hi == T means "never"
    while (true) {
        vector<vector<int>> at(T + 1);
        bool active = false;
        for (int i = 0; i < q; i++)
            if (lo[i] < hi[i]) at[(lo[i] + hi[i]) / 2].push_back(i), active = true;
        if (!active) break;
        reset();
        for (int t = 0; t <= T; t++) {
            if (t) applyUpdate(t - 1);                 // now updates 0..t-1 are applied
            for (int i : at[t]) { if (test(i)) hi[i] = t; else lo[i] = t + 1; }
        }
    }
    return lo;                                         // lo[i] == T  =>  never satisfied
}
// CONCRETE USES
//  - "for each query, the first moment a path/edge set connects u and v"  (structure = DSU)
//  - k-th smallest in a range, offline (structure = BIT over values, updates = insert a value)
//  - "smallest capacity so that a flow/matching of size k exists"
//  - anything where a single query's binary search would need its own O(T) replay.
// If the structure supports ROLLBACK, prefer divide-and-conquer over the timeline instead
// (see OfflineDynamicConnectivity) - it is one pass instead of log passes.
