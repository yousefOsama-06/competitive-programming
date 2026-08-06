struct DSURollback {
    vector<int> par, sz;
    vector<pair<int, int>> history; // Stores {child_node, old_size_of_parent}

    DSURollback(int n) : par(n), sz(n, 1) { 
        iota(par.begin(), par.end(), 0); 
    }

    // Path compression is removed to preserve tree structure for rollbacks
    int find(int x) {
        if(x == par[x]) return x;
        return find(par[x]);
    }

    bool same(int x, int y) { return find(x) == find(y); }

    bool join(int x, int y) {
        x = find(x);
        y = find(y);
        
        if (x == y) {
            // Push dummy state to keep the rollback count 1:1 with join calls
            history.push_back({-1, -1}); 
            return false;
        }
        
        if (sz[x] < sz[y]) swap(x, y);
        
        // Record the node that becomes the child and the parent's current size
        history.push_back({y, sz[x]}); 
        
        sz[x] += sz[y];
        par[y] = x;
        return true;
    }

    int size(int x) { return sz[find(x)]; }

    // Undoes the last join operation
    void rollback() {
        if (history.empty()) return;
        
        auto [y, old_sz_x] = history.back();
        history.pop_back();
        
        if (y != -1) {
            int x = par[y];
            sz[x] = old_sz_x;
            par[y] = y; // Restore the child to be its own parent
        }
    }

    // Returns the current number of operations performed
    int get_state() {
        return history.size();
    }

    // Rolls back to a specific saved state
    void rollback_to(int state) {
        while (history.size() > state) {
            rollback();
        }
    }
};
