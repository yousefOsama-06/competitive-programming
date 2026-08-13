// INCREMENTAL DAG REACHABILITY (Italiano) - answer "is there a path u -> v?" while ARCS ARE BEING
// ADDED, in O(1) per query and O(n) amortised per insertion. Directed, 0-indexed, O(n^2) memory.
// addEdge(s, t) refuses (and returns false) any arc that would create a CYCLE, so the structure
// doubles as an online "would this arc break acyclicity?" oracle - which is the usual reason to
// reach for it: "process the constraints in order and report the first one that is contradictory",
// online topological ordering, incremental dependency graphs.
// STRUCTURE. For every vertex s it keeps a REACHABILITY TREE of everything s can reach, stored as
// par[s][v] = v's parent in that tree. Adding s -> t only ever ADDS vertices to those trees, and a
// vertex is added to a tree at most once, so the total work over all insertions is O(n^2) per
// source - i.e. O(n) amortised per update. Nothing is ever removed, hence "incremental only".
struct IncrementalDAG {
    int n;
    vector<vector<int>> par;                                   // par[root][v], -1 = not reachable
    vector<vector<vector<int>>> ch;                            // ch[root][v] = children in the tree
    IncrementalDAG(int n) : n(n), par(n, vector<int>(n, -1)), ch(n, vector<vector<int>>(n)) {}
    bool reachable(int s, int t) const { return s == t || par[s][t] >= 0; }
    void meld(int root, int sub, int u, int v) {               // graft v (and its subtree) under u
        par[root][v] = u, ch[root][u].push_back(v);
        for (int c : ch[sub][v]) if (!reachable(root, c)) meld(root, sub, v, c);
    }
    bool addEdge(int s, int t) {                               // false = it would close a cycle
        if (reachable(t, s)) return false;
        if (reachable(s, t)) return true;                      // nothing changes
        for (int p = 0; p < n; p++)
            if (reachable(p, s) && !reachable(p, t)) meld(p, t, s, t);
        return true;
    }
};
// FULLY DYNAMIC reachability (with deletions) has no simple solution; if the updates are known in
// advance, run OFFLINE divide and conquer over the time axis instead.
// DENSE ALTERNATIVE: bitset transitive closure, n/64 words per vertex, rebuilt from scratch in
// O(n * m / 64) - simpler and often faster for n <= 2000 when the number of queries is small.
// TRANSITIVE CLOSURE of a static DAG: process in reverse topological order and OR the successors'
// bitsets - O(n m / 64).
// COUNTING PAIRS (u, v) with u reaching v is the same bitset sweep; there is no better general
// algorithm (it is equivalent to boolean matrix multiplication).
