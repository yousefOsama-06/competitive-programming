/*
========================================================================================
   CATALOGUE: Lengauer-Tarjan Dominator Tree (O((N + M) log N))
========================================================================================
   DESCRIPTION:
   Finds the "bottlenecks" in any directed graph. A node 'u' dominates 'v' if
   EVERY path from the root to 'v' is forced to go through 'u'.

   USAGE:
   1. Build graph: Create a 2D vector `adj` (1-based) containing the directed edges.
   2. Initialize:  DominatorTree dt(n, root, adj);
                   (Automatically builds the tree on instantiation)

   EXTRACTING ANSWERS:
   - dt.dom[u]:       The Immediate Dominator of 'u' (the closest bottleneck to 'u').
   - dt.id[u]:        If dt.id[u] == 0 after building, 'u' is UNREACHABLE from the root.
========================================================================================
*/
struct DominatorTree {
    int T = 0, n;
    vector<vector<int>> rg, bucket, adj;
    vector<int> dsu, par, sdom, idom, dom, label, id, rev;

    DominatorTree(int _n, int r, vector<vector<int>>& adj):
        n(_n + 1), rg(n), bucket(n), adj(adj), dsu(n),
        par(n), sdom(n), idom(n), dom(n), label(n), id(n), rev(n) { dfs(r); build(r); }

    int find(int u, int x = 0) {
        if (u == dsu[u]) return x ? -1 : u;
        int v = find(dsu[u], x + 1);
        if (v < 0) return u;
        if (sdom[label[dsu[u]]] < sdom[label[u]]) label[u] = label[dsu[u]];
        dsu[u] = v;
        return x ? v : label[u];
    }

    void dfs(int u) {
        id[u] = ++T, rev[T] = u;
        label[T] = sdom[T] = dsu[T] = T;
        for (int& w : adj[u]) {
            if (!id[w]) {
                dfs(w);
                par[id[w]] = id[u];
            }
            rg[id[w]].push_back(id[u]);
        }
    }

    void build(int r) {
        // FIXED: Loop MUST go down to i >= 1 to process the root's bucket!
        for (int i = T; i >= 1; i--) {
            for (int& u : rg[i]) sdom[i] = min(sdom[i], sdom[find(u)]);
            if (i > 1) bucket[sdom[i]].push_back(i);

            for (int& w : bucket[i]) {
                int v = find(w);
                idom[w] = sdom[v] == sdom[w] ? sdom[w] : v;
            }

            if (i > 1) dsu[i] = par[i];
        }

        for (int i = 2; i <= T; i++) {
            if (idom[i] != sdom[i]) idom[i] = idom[idom[i]];
        }

        for (int u = 1; u <= T; ++u) {
            dom[rev[u]] = rev[idom[u]];
        }
    }
};

void testCase() {
    int n, m, s;
    cin >> n >> m >> s;
    s++; // Convert to 1-based indexing

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; i++) {
        int u, v;
        cin >> u >> v;
        adj[u + 1].push_back(v + 1);
    }

    // Automatically builds upon instantiation
    DominatorTree dt(n, s, adj);

    for (int i = 1; i <= n; i++) {
        if (i == s) {
            // pS is S
            cout << s - 1 << (i == n ? "" : " ");
        } else if (!dt.id[i]) {
            // If we can not reach i from S, print -1
            cout << -1 << (i == n ? "" : " ");
        } else {
            // Print the 0-based parent (which is now guaranteed to be calculated)
            cout << dt.dom[i] - 1 << (i == n ? "" : " ");
        }
    }
    cout << "\n";
}
