// Needs: 05 - Disjoint Set Union (DSU)/02 - DSURollback.cpp
// QUEUE UNDO TRICK - turns a structure that can only "undo the LAST operation" into one that can
// also "undo the OLDEST operation", i.e. a QUEUE of operations instead of a stack.
// O(log n) amortised extra factor on top of the underlying structure.
// WHEN: a sliding window over operations - "for each l, the largest r such that edges l..r keep
// the graph bipartite / acyclic / with <= k components" (two pointers over edges), or any online
// problem where insertions and deletions are FIFO. Deletions in arbitrary order need
// 14 - OfflineDynamicConnectivity (offline) or a link-cut tree (forests only).
// KEY INVARIANT: the undo stack is kept partitioned into a "to be deleted soon" prefix and the
// rest. To pop the front, undo a suffix of the stack, then re-push it with the OLDEST elements
// nearest the top; the rebuild always processes the SMALLER of the two groups, which is what makes
// the potential argument work - each operation is re-pushed O(log n) times over its lifetime.
// The underlying structure must support: apply(op) and undo-last, both cheap; nothing else.
struct QueueUndo {
    struct Op { int u, v; bool old; };                         // old = "already re-pushed once"
    DSURollback d;
    vector<Op> st;                                             // mirrors the undo stack exactly
    int bot = 0;                                               // first index that is not yet `old`
    QueueUndo(int n) : d(n) {}
    void push(Op o) { d.join(o.u, o.v), st.push_back(o); }
    void push(int u, int v) { push({u, v, false}); }
    void pop() {                                               // remove the OLDEST operation
        vector<Op> a[2];
        while (bot < (int)st.size() && st[bot].old) bot++;
        if (bot == (int)st.size()) {                           // no old ops left: flip everything
            while (!st.empty()) a[0].push_back(st.back()), st.pop_back(), d.rollback();
            for (auto& o : a[0]) o.old = true, push(o);
            a[0].clear(), bot = 0;
        }
        do {                                                   // pull back until the front is up
            a[st.back().old].push_back(st.back());
            st.pop_back(), d.rollback();
        } while (a[1].size() < a[0].size() && (int)st.size() > bot);
        for (int i : {0, 1}) {
            reverse(all(a[i]));
            for (auto& o : a[i]) push(o);
        }
        st.pop_back(), d.rollback();                           // the front is now on top
    }
};
// THE STRUCTURE UNDERNEATH can be anything with rollback: a DSU (connectivity, bipartiteness by
// storing the parity to the parent, number of components), a XOR basis, a max-stack, a knapsack
// with rollback. Replace push()/pop() bodies and keep the stack discipline identical.
// SLIDING WINDOW RECIPE: for l = 1..m, push edges while the invariant holds, record the answer,
// then pop() the edge at l. Total O(m log m * alpha).
// CHEAPER OPTIONS FIRST: if the problem is offline, the segment tree over the timeline
// (14 - OfflineDynamicConnectivity) is shorter and has a better constant. If the deletions are
// LIFO, plain rollback is enough. This file is only for FIFO + online.
