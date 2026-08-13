// PERSISTENCE AND UNDO - three ways to travel in a structure's history, and when each applies.
//   ROLLBACK  (05 - DSU/02): undo the last op. Cheapest. Needs a stack discipline (LIFO).
//   OFFLINE DELETION (14 - OfflineDynamicConnectivity): segment tree over time + rollback.
//     Handles arbitrary insert/delete, but you must know every operation in advance.
//   PERSISTENCE (this file): branch the history. The only one that works ONLINE with deletions,
//     and the only one that lets you query a past version after moving on.

// --- PERSISTENT ARRAY: a segment tree over indices. get/set in O(log n), O(log n) new nodes. ---
struct PArray {
    struct Node { int l = 0, r = 0; ll v = 0; };
    vector<Node> t;
    int n;
    PArray(int n) : t(1), n(n) {}                              // node 0 is the shared all-zero
    int build(const vector<ll>& a, int lo, int hi) {
        int id = t.size();
        t.push_back({});
        if (lo == hi) { t[id].v = a[lo]; return id; }
        int m = (lo + hi) / 2, L = build(a, lo, m), R = build(a, m + 1, hi);
        t[id].l = L, t[id].r = R;
        return id;
    }
    int build(const vector<ll>& a) { return build(a, 0, n - 1); }
    ll get(int u, int i, int lo, int hi) {
        if (lo == hi) return t[u].v;
        int m = (lo + hi) / 2;
        return i <= m ? get(t[u].l, i, lo, m) : get(t[u].r, i, m + 1, hi);
    }
    ll get(int u, int i) { return get(u, i, 0, n - 1); }
    int set(int u, int i, ll v, int lo, int hi) {
        int id = t.size();
        t.push_back(t[u]);
        if (lo == hi) { t[id].v = v; return id; }
        int m = (lo + hi) / 2;
        if (i <= m) t[id].l = set(t[u].l, i, v, lo, m);
        else t[id].r = set(t[u].r, i, v, m + 1, hi);
        return id;
    }
    int set(int u, int i, ll v) { return set(u, i, v, 0, n - 1); }
};
// --- PERSISTENT DSU on top of it. O(log^2 n) per operation. ---
// The catch that makes it work: NO PATH COMPRESSION (that would mutate an old version), so you
// MUST union by size/rank to keep the height O(log n).
struct PDSU {
    PArray par, siz;
    PDSU(int n) : par(n), sz(n) {}
    pair<int, int> init(int n) {                               // returns {parRoot, szRoot}
        vector<ll> p(n), s(n, 1);
        iota(all(p), 0);
        return {par.build(p), siz.build(s)};
    }
    int find(int pr, int x) { ll p = par.get(pr, x); return p == x ? x : find(pr, p); }
    pair<int, int> join(int pr, int sr, int a, int b) {
        a = find(pr, a), b = find(pr, b);
        if (a == b) return {pr, sr};
        if (siz.get(sr, a) < siz.get(sr, b)) swap(a, b);
        return {par.set(pr, b, a), siz.set(sr, a, siz.get(sr, a) + siz.get(sr, b))};
    }
};
/* THE OTHER TWO TIME-TRAVEL TRICKS, as recipes (code: 21 - QueueUndoTrick, 25 - StaticToDynamic)
 QUEUE UNDO. You have a structure that supports only "undo the last op" (rollback DSU), but the
   deletions arrive in FIFO order (a sliding window). Keep the undo stack partitioned so the
   OLDEST element is near the top; when it is not, pop a prefix and re-push it in the other order,
   always rebuilding the SMALLER half. O(log n) amortised extra. This is the online alternative to
   the offline segment-tree-on-time trick, and it is what "sliding window connectivity" needs.
 STATIC TO DYNAMIC (the logarithmic method). Any STATIC structure with O(B(n)) build and O(Q(n))
   query becomes insert-only: keep O(log n) instances of sizes 2^0, 2^1, 2^2, ...; inserting is
   binary-counter carrying, so you rebuild an instance of size 2^k only every 2^k insertions.
   Insert becomes O(B(n)/n * log n) amortised and query O(Q(n) log n) (ask all instances).
   Use it to make a convex hull / a wavelet tree / an Aho-Corasick automaton accept insertions.
 PARTIALLY vs FULLY PERSISTENT: partially = you may query any past version but only modify the
   latest - for a DSU that is 05 - DSU/05 - PartiallyPersistentDSU, O(n) memory instead of this
   file's O(n log n); fully = you may modify any version, branching the history into a tree.
   The code above is fully persistent.
 WHEN NOT TO BOTHER: if the problem is offline and only needs "delete", the segment tree over the
   timeline with a rollback DSU is shorter, faster, and has a smaller constant than any of this. */
