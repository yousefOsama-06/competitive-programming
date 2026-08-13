// MERGEABLE (MELDABLE) HEAP - a priority queue that also supports MERGING two heaps in O(log n),
// which std::priority_queue cannot do. This is a skew heap: merge by walking the two right spines
// and swapping children, which keeps the amortised depth logarithmic with no stored rank.
// Supports a lazy "add v to every key in this heap" tag, because that is what the two algorithms
// that need a mergeable heap actually want.
// USES: Chu-Liu/Edmonds directed MST in O(E log V) (each contracted cycle merges its in-edge
// heaps and subtracts the chosen edge's weight from the whole heap); Dijkstra-style k-shortest
// paths (Eppstein); "merge the two children's candidate sets" DP on trees; leftist-heap solutions
// to scheduling problems (repeatedly merge and pop the largest, e.g. minimise total lateness).
struct MHeap {
    struct Node { ll key, lz = 0; Node *l = 0, *r = 0; };
    static void down(Node* t) {                                // apply the lazy tag downwards
        if (!t || !t->lz) return;
        t->key += t->lz;
        if (t->l) t->l->lz += t->lz;
        if (t->r) t->r->lz += t->lz;
        t->lz = 0;
    }
    static Node* merge(Node* a, Node* b) {                     // O(log n) amortised, MIN-heap
        if (!a || !b) return a ? a : b;
        down(a), down(b);
        if (a->key > b->key) swap(a, b);
        a->r = merge(a->r, b);
        swap(a->l, a->r);                                      // the "skew" step
        return a;
    }
    static Node* push(Node* t, ll key) { return merge(t, new Node{key}); }
    static ll top(Node* t) { return down(t), t->key; }
    static Node* pop(Node* t) { down(t); return merge(t->l, t->r); }
    static void addAll(Node* t, ll v) { if (t) t->lz += v; }   // O(1) add to every key
};
// TO GET A MAX-HEAP: negate the keys, or flip the comparison in merge.
// LEFTIST HEAP is the same interface with a stored `dist` (null-path length) and the rule "keep the
// larger dist on the left"; it gives WORST-CASE O(log n) instead of amortised. Skew is shorter and
// fine unless you need the worst-case bound inside another amortised argument.
// PAIRING HEAP has O(1) merge and decrease-key in practice and is what you want if you need
// decrease-key (Dijkstra on very dense graphs); in contests a binary heap with lazy deletion wins.
// LAZY DELETION is usually enough: push (key, id), and pop until the top is still valid. That
// removes the need for decrease-key entirely and is 5 lines.
// SMALL-TO-LARGE with std::priority_queue also "merges" heaps: repeatedly pop from the smaller and
// push into the larger, O(n log^2 n) total. Shorter than this file - use it unless the log^2 hurts.
