// TREE ISOMORPHISM (AHU canonical form). O(n log n) with a map, O(n) with per-level bucketing.
// Gives every rooted subtree a canonical ID: two subtrees are isomorphic iff their IDs match.
// Exact - unlike random subtree hashing, which can collide.
struct TreeIso {
    map<vector<int>, int> id;                          // canonical child-multiset -> id
    // canonical id of the subtree rooted at u
    int rootedId(int u, int p, const vector<vector<int>>& adj) {
        vector<int> ch;
        for (int v : adj[u]) if (v != p) ch.push_back(rootedId(v, u, adj));
        sort(all(ch));
        auto it = id.find(ch);
        if (it != id.end()) return it->second;
        int nid = id.size();
        return id[ch] = nid;
    }
    // CENTERS of a tree: 1 or 2 vertices (Jordan). Root there to canonicalise an UNROOTED tree.
    vector<int> centers(int n, const vector<vector<int>>& adj) {
        if (n == 1) return {0};
        vector<int> deg(n), leaves;
        for (int i = 0; i < n; i++) if ((deg[i] = adj[i].size()) <= 1) leaves.push_back(i);
        int removed = leaves.size();
        while (removed < n) {
            if (leaves.empty()) break;                 // guard: not a tree / disconnected
            vector<int> nxt;
            for (int u : leaves)
                for (int v : adj[u]) if (--deg[v] == 1) nxt.push_back(v);
            leaves = nxt, removed += leaves.size();
        }
        return leaves;                                 // 1 or 2 vertices, adjacent if 2
    }
    // Canonical id of an UNROOTED tree: min over its centers.
    int unrootedId(int n, const vector<vector<int>>& adj) {
        auto c = centers(n, adj);
        int best = rootedId(c[0], -1, adj);
        if (c.size() == 2) best = min(best, rootedId(c[1], -1, adj));
        return best;
    }
};
// Two unrooted trees are isomorphic iff unrootedId is equal (use ONE shared TreeIso object so
// the id space is common). Two rooted trees: compare rootedId at their roots.
// USES: "how many distinct tree shapes", dedupe trees, count isomorphism classes, group
// identical subtrees for a DP over shapes.
// JORDAN: a tree has exactly one or two centers; if two, they are adjacent.
