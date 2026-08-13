// CENTROID DECOMPOSITION - recursively remove the centroid (the vertex whose largest remaining
// component is <= n/2), giving a decomposition tree of depth O(log n). Every path in the original
// tree passes through the centroid of some level, which turns path-counting problems into
// "for each centroid, combine the distance lists of its components" in O(n log n).
class CentroidDecomposition {
private:
    int n;
    vector<vector<int>> adj;
    vector<int> sz, par, nodes;
    vector<bool> vis;

    int dfsSz(int u, int p) {
        sz[u] = 1;
        for (int v : adj[u]) {
            if (vis[v] || v == p) continue;
            sz[u] += dfsSz(v, u);
        }
        return sz[u];
    }

    int getCentroid(int u, int p, int total_nodes) {
        for (int v : adj[u]) {
            if (vis[v] || v == p) continue;
            if (sz[v] * 2 > total_nodes) {
                return getCentroid(v, u, total_nodes);
            }
        }
        return u;
    }

    void collect(int u, int p) {
        nodes.push_back(u);
        for (int v : adj[u]) {
            if (vis[v] || v == p) continue;
            collect(v, u);
        }
    }

    void build(int u, int p) {
        int total_nodes = dfsSz(u, p);
        int centroid = getCentroid(u, p, total_nodes);
        
        if (p != -1) {
            par[centroid] = p;
        } else {
            par[centroid] = centroid;
        }
        
        vis[centroid] = true;

        // ==========================================
        //       PROBLEM SPECIFIC PROCESSING
        // ==========================================
        // Process paths going through the 'centroid' here
        for (int v : adj[centroid]) {
            if (vis[v]) continue;
            nodes.clear();
            collect(v, centroid);
            
            // e.g., Update answers using the nodes collected from this subtree branch
        }
        // ==========================================

        // Recursively decompose the remaining components
        for (int v : adj[centroid]) {
            if (vis[v]) continue;
            build(v, centroid);
        }
    }

public:
    // Initialize with 1-based indexing
    CentroidDecomposition(int nodes_count) : n(nodes_count) {
        adj.assign(n + 1, vector<int>());
        sz.assign(n + 1, 0);
        par.assign(n + 1, 0);
        vis.assign(n + 1, false);
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // Start decomposition, usually from node 1
    void decompose(int root = 1) {
        build(root, -1);
    }

    // Returns the parent of a node in the centroid tree
    int get_centroid_parent(int u) const {
        return par[u];
    }
};