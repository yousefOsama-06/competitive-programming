// DSU with Rollback - No path compression to allow backtracking
// Time: O(log N) per operation, O(1) rollback
struct DSURollback {
    vector<int> par, siz;
    vector<pair<int, int>> history; // {child_node, old_parent_size}

    DSURollback(int n = 0) : par(n), sz(n, 1) {
        iota(all(par), 0);
    }

    int find(int x) {
        return par[x] == x ? x : find(par[x]);
    }

    bool same(int x, int y) {
        return find(x) == find(y);
    }

    bool join(int x, int y) {
        x = find(x);
        y = find(y);
        if (x == y) {
            history.eb(-1, -1);
            return false;
        }
        if (siz[x] < siz[y]) swap(x, y);
        history.eb(y, siz[x]);
        siz[x] += siz[y];
        par[y] = x;
        return true;
    }

    int size(int x) {
        return siz[find(x)];
    }

    void rollback() {
        if (history.empty()) return;
        auto [y, old_sz_x] = history.back();
        history.pop_back();
        if (y != -1) {
            siz[par[y]] = old_sz_x;
            par[y] = y;
        }
    }

    int get_state() const {
        return history.size();
    }

    void rollback_to(int state) {
        while ((int)history.size() > state) rollback();
    }
};
