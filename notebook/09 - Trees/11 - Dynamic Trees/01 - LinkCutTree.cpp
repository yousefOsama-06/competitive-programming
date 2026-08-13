// LINK-CUT TREE - a FOREST that changes shape, with path queries. O(log n) amortised per op.
// This is dynamic HLD: link, cut, re-root, and aggregate over the path u..v, all online.
// If the tree is STATIC use HLD (09 - Trees/03) - it is faster and far shorter. Reach for an LCT
// only when edges are added/removed, or when you need makeRoot.
// HOW IT WORKS: each vertex has at most one "preferred" child; preferred paths are stored as
// splay trees keyed by DEPTH; a non-preferred edge is a path-parent pointer the splay does not
// know about. access(v) makes root..v one preferred path, and the same heavy/light potential that
// bounds HLD bounds the number of preferred-child changes, hence the O(log n) amortised.
struct LCT {
    struct Node { int ch[2] = {0, 0}, p = 0; ll val = 0, sum = 0; bool rev = 0; };
    vector<Node> t;
    LCT(int n) : t(n + 1) {}                                   // 1-indexed; node 0 is the null
    bool isRoot(int x) { return t[t[x].p].ch[0] != x && t[t[x].p].ch[1] != x; }
    void pull(int x) { t[x].sum = t[t[x].ch[0]].sum ^ t[x].val ^ t[t[x].ch[1]].sum; }
    void flip(int x) { if (x) swap(t[x].ch[0], t[x].ch[1]), t[x].rev ^= 1; }
    void push(int x) { if (t[x].rev) flip(t[x].ch[0]), flip(t[x].ch[1]), t[x].rev = 0; }
    void rot(int x) {
        int p = t[x].p, g = t[p].p, d = t[p].ch[1] == x;
        if (!isRoot(p)) t[g].ch[t[g].ch[1] == p] = x;
        t[x].p = g;
        t[p].ch[d] = t[x].ch[!d], t[t[x].ch[!d]].p = p;
        t[x].ch[!d] = p, t[p].p = x;
        pull(p), pull(x);
    }
    void splay(int x) {
        vector<int> st{x};
        for (int y = x; !isRoot(y); y = t[y].p) st.push_back(t[y].p);
        for (int i = st.size() - 1; i >= 0; i--) push(st[i]);
        while (!isRoot(x)) {
            int p = t[x].p, g = t[p].p;
            if (!isRoot(p)) rot((t[g].ch[1] == p) == (t[p].ch[1] == x) ? p : x);
            rot(x);
        }
    }
    int access(int x) {                                        // root..x becomes one splay tree
        int last = 0;
        for (int y = x; y; y = t[y].p) splay(y), t[y].ch[1] = last, pull(y), last = y;
        splay(x);
        return last;                                           // = the LCA on the previous access
    }
    void makeRoot(int x) { access(x), flip(x), push(x); }
    int findRoot(int x) { access(x); while (t[x].ch[0]) push(x), x = t[x].ch[0]; splay(x); return x; }
    bool connected(int x, int y) { return findRoot(x) == findRoot(y); }
    void link(int x, int y) { makeRoot(x); if (findRoot(y) != x) t[x].p = y; }
    void cut(int x, int y) {                                   // no-op if the edge is absent
        makeRoot(x);
        if (findRoot(y) == x && t[y].p == x && !t[y].ch[0]) t[y].p = t[x].ch[1] = 0, pull(x);
    }
    ll query(int x, int y) { makeRoot(x), access(y); return t[y].sum; }    // path aggregate
    void update(int x, ll v) { access(x), t[x].val = v, pull(x); }
    int lca(int r, int x, int y) { makeRoot(r), access(x); return access(y); }
};
// THE AGGREGATE is XOR here purely because it is self-inverse and needs no reversal handling.
// For SUM/MIN/MAX just change pull(); for a NON-COMMUTATIVE aggregate (matrix product, max
// prefix sum) you must store BOTH directions in the node and swap them inside flip().
// EDGE WEIGHTS: give each edge its own vertex (n + edgeId) with the weight, and link both ends
// to it. Vertex values then stay 0 and every path query still works.
// SUBTREE aggregates need "virtual subtree" bookkeeping: keep a second sum over the non-preferred
// children, updated in access() when you swap t[y].ch[1]. That doubles the code - if the tree is
// static, an Euler tour (09 - Trees/06) is O(log n) and ten lines.
/* WHAT LCT UNLOCKS
   ONLINE DYNAMIC CONNECTIVITY ON A FOREST: connected() in O(log n). For general graphs (cycles)
     you need the offline segment-tree-on-time trick (02 - Data Structures/14) or Holm-de
     Lichtenberg-Thorup at O(log^2 n).
   DYNAMIC MST: to add edge (u,v,w), if u and v are connected find the MAXIMUM edge on the path;
     if it exceeds w, cut it and link the new edge. O(log n) per update.
   MAXIMUM FLOW (Dinic with link-cut) improves the blocking-flow step to O(VE log V) - almost
     never needed at contest sizes.
   OFFLINE "PAINT THE ROOT PATH": access() itself is the operation "recolour root..v with a new
     colour"; the number of colour changes is O(n log n) total, so if each change costs a segment
     tree update you get O(n log^2 n). This is the standard trick for "count distinct substrings
     over all substrings of s" and for tree-path-colouring problems. */
