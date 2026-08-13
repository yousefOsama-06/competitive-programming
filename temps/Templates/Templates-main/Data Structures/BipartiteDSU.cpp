struct BipartiteDSU {
    vector<int> sz,bipartite;
    vector<pair<int, int>>par;

    BipartiteDSU(int n) : par(n), sz(n, 1),bipartite(n) {
        for (int i = 0; i < n; ++i) {
            par[i] = {i,0};
        }
    }
    
    pair<int, int> find(int u) {
        if (u == par[u].fi)return {u, 0};
        int parity = par[u].se;
        par[u] = find(par[u].first);
        par[u].se ^= parity;
        return par[u];
    }

    bool same(int x, int y) { return find(x).first == find(y).first; }
    
    bool join(int u, int v) {
        pair<int,int>pu = find(u);
        pair<int,int>pv = find(v);
        u = pu.first;
        v = pv.first;
        int x = pu.second,y = pv.second;
        if (u == v) {
            if(x == y)
                bipartite[u] = false;
            return false;
        }
        if (sz[u] < sz[v])
            swap(u, v);
        par[v] = {u, x ^ y ^ 1};
        bipartite[u] &= bipartite[v];
        sz[u] += sz[v];
        return true;
    }

    int size(int x) { return sz[find(x).first]; }
};