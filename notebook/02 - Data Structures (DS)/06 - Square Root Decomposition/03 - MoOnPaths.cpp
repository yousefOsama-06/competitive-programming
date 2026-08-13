// MO'S ON TREE PATHS - the Euler tour trick where a vertex appearing TWICE in the range cancels out,
// plus the LCA added back by hand. Answers path queries offline in O((n + q) sqrt n) with no HLD.
class MoTreePath {
private:
    int n, LOG, block_size;
    vector<vector<int>> adj, anc;
    vector<int> depth, in, out, flat;
    vector<long long> up, a;
    vector<bool> exist;

    // --- PROBLEM SPECIFIC STATE ---
    long long ans;
    vector<deque<int>> dep; // Tracks nodes by depth

    void add(int idx) {
        if (!dep[depth[idx]].empty()) {
            ans += 1ll * a[dep[depth[idx]].back()] * a[idx];
        }
        dep[depth[idx]].push_back(idx);
    }

    void remove(int idx) {
        if (dep[depth[idx]].size() == 2) {
            ans -= a[*dep[depth[idx]].begin()] * a[*next(dep[depth[idx]].begin())];
        }
        if (dep[depth[idx]].front() == idx) {
            dep[depth[idx]].pop_front();
        } else {
            dep[depth[idx]].pop_back();
        }
    }

    void update(int idx) {
        int node = flat[idx];
        if (exist[node]) {
            remove(node);
        } else {
            add(node);
        }
        exist[node] = !exist[node];
    }

    inline long long get_answer() const {
        return ans;
    }
    // ------------------------------

    void dfs(int u, int p = -1) {
        if (p != -1) {
            anc[u][0] = p;
            up[u] = a[u] * a[u] + up[p];
        } else {
            depth[u] = 0;
            up[u] = a[u] * a[u];
        }
        in[u] = flat.size();
        flat.push_back(u);
        
        for (int v : adj[u]) {
            if (v == p) continue;
            depth[v] = depth[u] + 1;
            dfs(v, u);
        }
        
        out[u] = flat.size();
        flat.push_back(u); // Push again for Eulerian tour
    }

    void build_lca() {
        for (int k = 1; k < LOG; k++) {
            for (int i = 1; i <= n; i++) {
                anc[i][k] = anc[anc[i][k - 1]][k - 1];
            }
        }
    }

    int lift(int x, int k) {
        for (int bit = 0; bit < LOG; bit++) {
            if ((1 << bit) & k) x = anc[x][bit];
        }
        return x;
    }

    int lca(int u, int v) {
        if (depth[u] < depth[v]) swap(u, v);
        u = lift(u, depth[u] - depth[v]);
        if (u == v) return u;
        for (int bit = LOG - 1; bit >= 0; bit--) {
            if (anc[u][bit] != anc[v][bit]) {
                u = anc[u][bit];
                v = anc[v][bit];
            }
        }
        return anc[u][0];
    }

public:
    struct Query {
        int l, r, idx, x = -1;
        int bnd;

        bool operator<(const Query& other) const {
            if (bnd != other.bnd) return bnd < other.bnd;
            return (bnd & 1) ? (r < other.r) : (r > other.r);
        }
    };

    MoTreePath(int n_nodes, const vector<long long>& node_values) 
        : n(n_nodes), a(node_values), LOG(log2(n_nodes) + 2) {
        
        adj.assign(n + 1, vector<int>());
        anc.assign(n + 1, vector<int>(LOG, 0));
        depth.assign(n + 1, 0);
        in.assign(n + 1, 0);
        out.assign(n + 1, 0);
        up.assign(n + 1, 0);
        exist.assign(n + 1, false);
        dep.assign(n + 1, deque<int>());
        ans = 0;
    }

    void add_edge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    // PRIMARY ENTRY POINT: give it the paths as vertex pairs and it builds the queries itself.
    // (in[], out[] and lca() are only valid AFTER the dfs, so you cannot build them yourself.)
    vector<long long> solve(int root, const vector<pair<int, int>>& paths) {
        flat.clear();
        dfs(root);
        build_lca();
        vector<Query> queries;
        for (int i = 0; i < (int)paths.size(); i++) {
            int u = paths[i].first, v = paths[i].second;
            if (in[u] > in[v]) swap(u, v);
            int w = lca(u, v);
            // u inside v's subtree -> plain range; otherwise use out[u]..in[v] and add back the LCA
            if (w == u) queries.push_back({in[u], in[v], i, -1, 0});
            else queries.push_back({out[u], in[v], i, w, 0});
        }
        return solveRanges(queries);
    }
    vector<long long> solveRanges(vector<Query>& queries) {
        int q = queries.size();
        if (q == 0) return {};
        
        block_size = max(1, (int)(flat.size() / sqrt(q)));
        
        for (auto& qry : queries) {
            qry.bnd = qry.l / block_size;
        }

        sort(queries.begin(), queries.end());

        vector<long long> answers(q);
        int cur_l = 0, cur_r = -1;

        for (const Query& qry : queries) {
            while (cur_l > qry.l) update(--cur_l);       // GROW first, then shrink - the other
            while (cur_r < qry.r) update(++cur_r);       // order can pass through an invalid window
            while (cur_l < qry.l) update(cur_l++);
            while (cur_r > qry.r) update(cur_r--);

            answers[qry.idx] = get_answer() + (qry.x != -1 ? up[qry.x] : 0);
        }
        return answers;
    }
};