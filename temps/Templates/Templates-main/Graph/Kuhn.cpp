struct Kuhn {
    int n, m;
    vector<int> leftMatch,rightMatch;
    vector<bool> vis;
    vector<vector<int>> g;

    Kuhn(int n = 101, int m = 101) : n(n), m(m) {
        vis.resize(n);
        g.resize(n + 1);
        leftMatch.assign(m,-1);
        rightMatch.assign(n,-1);
    }

    void addEdge(int u, int v) {
        g[u].push_back(v);
    }

    bool match(int u) {
        if (vis[u])
            return false;
        vis[u] = true;
        for (auto v: g[u]) {
            if (leftMatch[v] == -1 || match(leftMatch[v])) {
                leftMatch[v] = u;
                rightMatch[u] = v;
                return true;
            }
        }
        return false;
    }

    int maxMatch() {
        vector<bool> used(n);
        for (int i = 0; i < n; ++i) {
            for (auto v: g[i]) {
                if (leftMatch[v] == -1) {
                    used[i] = true;
                    rightMatch[i] = v;
                    leftMatch[v] = i;
                    break;
                }
            }
        }

        for (int i = 0; i < n; i++) {
            if (used[i])continue;
            fill(vis.begin(), vis.end(), 0);
            match(i);
        }

        int sol = 0;
        for (int i = 0; i < m; i++)
            sol += leftMatch[i] != -1;

        return sol;
    }
};
