class DSUOnTree {
private:
    struct Query {
        int idx, x;
    };

    int n;
    vector<vector<int>> adj;
    vector<int> sz, depth;
    vector<bool> big;
    vector<vector<Query>> queries;
    vector<int> ans;

    // ==========================================
    //       PROBLEM SPECIFIC STATE VARIABLES
    // ==========================================
    vector<char> c; // Example: Array of characters per node
    
    // Example state variables
    // int distinct_count = 0;
    // vector<int> freq;

    void add(int u, int p) {
        // --- ADD LOGIC HERE ---
        // Example: freq[c[u] - 'a']++;

        for (int v : adj[u]) {
            if (v == p || big[v]) continue; // Skip parent and heavy child[cite: 4]
            add(v, u);
        }
    }

    void remove(int u, int p) {
        // --- REMOVE LOGIC HERE ---
        // Example: freq[c[u] - 'a']--;

        for (int v : adj[u]) {
            if (v == p || big[v]) continue; // Skip parent and heavy child[cite: 4]
            remove(v, u);
        }
    }

    int get_answer(int u, int x = 0) {
        // --- QUERY ANSWER LOGIC HERE ---
        return 0; 
    }
    // ==========================================

    void pre(int u, int p = 0) {
        sz[u] = 1;
        depth[u] = depth[p] + 1;
        for (int v : adj[u]) {
            if (v == p) continue;
            pre(v, u);
            sz[u] += sz[v]; // Compute subtree size[cite: 4]
        }
    }

    void dfs(int u, int p = 0, bool keep = false) {
        int mx = -1, bigChild = -1;
        
        // Find the heavy child[cite: 4]
        for (int v : adj[u]) {
            if (v == p) continue;
            if (sz[v] > mx) {
                mx = sz[v];
                bigChild = v;
            }
        }

        // Process light children and clear them[cite: 4]
        for (int v : adj[u]) {
            if (v == p || v == bigChild) continue;
            dfs(v, u, false);
        }

        // Process heavy child and keep it[cite: 4]
        if (bigChild != -1) {
            dfs(bigChild, u, true);
            big[bigChild] = true; 
        }

        // Add current node and its light children[cite: 4]
        add(u, p);
        
        // Answer all queries for the current node[cite: 4]
        for (const auto& q : queries[u]) {
            ans[q.idx] = get_answer(u, q.x);
        }

        // Reset the big child flag[cite: 4]
        if (bigChild != -1) {
            big[bigChild] = false;
        }

        // Clear state if this node is not meant to be kept[cite: 4]
        if (!keep) {
            remove(u, p);
        }
    }

public:
    // Initialize with 1-based indexing in mind
    DSUOnTree(int nodes, const vector<char>& chars) : n(nodes), c(chars) {
        adj.assign(n + 1, vector<int>());
        sz.assign(n + 1, 0);
        depth.assign(n + 1, 0);
        big.assign(n + 1, false);
        queries.assign(n + 1, vector<Query>());
        // Initialize problem-specific arrays here (e.g., freq.assign(26, 0);)
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    void add_query(int node, int query_idx, int x = 0) {
        queries[node].push_back({query_idx, x});
    }

    vector<int> solve(int root, int num_queries) {
        ans.assign(num_queries, 0);
        pre(root, 0); // Precalculate sizes and depths[cite: 4]
        dfs(root, 0, false); // Run the sack algorithm[cite: 4]
        return ans;
    }
};