// PERSISTENT MELDABLE HEAP - push / pop / meld that RETURN a new heap and leave the old ones
// intact. O(log n) time and O(log n) new nodes per operation.
// WHEN: an algorithm needs many heaps that share most of their contents. The canonical one is
// EPPSTEIN's k shortest walks: every vertex's candidate heap is its parent's heap plus its own
// out-edges, so you meld into a COPY and keep the parent's version alive. Also "k best paths in a
// DAG", "k-th best subset", and persistent priority queues in offline simulations.
// If nothing needs the old version, use 19 - MergeableHeap (destructive, no allocation per op).
// KEY INVARIANT: leftist heap - every node stores d = its null-path length and keeps the LONGER
// path on the left, so the right spine is O(log n) and meld only ever walks right spines. Making
// it persistent then costs nothing extra: COPY each node you touch on that spine instead of
// mutating it, which is exactly O(log n) copies.
struct PHeap {
    struct Node { int l = 0, r = 0, d = 0; ll key = 0; };
    vector<Node> t;
    PHeap() : t(1) {}                                          // node 0 is the empty heap, d = 0
    int meld(int a, int b) {                                   // returns a NEW root, min-heap
        if (!a || !b) return a | b;
        if (t[a].key > t[b].key) swap(a, b);
        int c = t.size();
        t.push_back(t[a]);                                     // copy, never mutate a
        int r = meld(t[c].r, b);
        t[c].r = r;
        if (t[t[c].l].d < t[r].d) swap(t[c].l, t[c].r);
        t[c].d = t[t[c].r].d + 1;
        return c;
    }
    int push(int a, ll key) {
        t.push_back({0, 0, 1, key});
        return meld(a, (int)t.size() - 1);
    }
    ll top(int a) { return t[a].key; }                         // a != 0
    int pop(int a) { return meld(t[a].l, t[a].r); }
    bool empty(int a) { return !a; }
};
// MEMORY: O((n + q) log n) nodes. If a heap is only ever pushed into (never melded), a persistent
// BINARY heap over a persistent array (20 - PersistentAndUndo) is smaller but has no meld.
// A LAZY ADD-TO-ALL tag does NOT combine with persistence for free: a tag is a mutation, so store
// the offset OUTSIDE the heap (carry "this version's keys are all shifted by s" in the caller) -
// that is what Eppstein and the directed-MST contraction actually do.
// PERSISTENT QUEUE, if that is what you came for, is much cheaper: keep a persistent ARRAY plus
// two indices (head, tail) per version. push = write at tail and store {head, tail+1}; pop = read
// at head and store {head+1, tail}. O(log n) per op, no heap needed.
// PERSISTENT STACK is a linked list: every version is just a node pointer. O(1) per op.
