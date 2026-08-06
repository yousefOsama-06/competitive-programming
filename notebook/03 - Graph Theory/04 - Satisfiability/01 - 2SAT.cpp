struct TwoSat {
    int N;
    vector<pair<int, int>> edges;

    TwoSat(int _N = 0) {
        init(_N);
    }

    void init(int _N = 0) {
        N = _N;
    }

    int addVar() { return N++; }

    // x or y, edges will be refined in the end
    void either(int x, int y) {
        x = max(2 * x, -1 - 2 * x);
        y = max(2 * y, -1 - 2 * y);
        edges.pb({x, y});
    }

    void implies(int x, int y) {
        either(~x, y);
    }

    void must(int x) {
        either(x, x);
    }

    void XOR(int x, int y) {
        either(x, y);
        either(~x, ~y);
    }

    vector<bool> solve(int _N = -1) {
        if (_N != -1) N = _N;
        SCC scc;
        scc.init(2 * N);
        for (auto e: edges) {
            scc.addEdge(e.st ^ 1, e.nd);
            scc.addEdge(e.nd ^ 1, e.st);
        }
        scc.gen();
        for (int i = 0; i < 2 * N; ++i) {
            if (scc.compOf[i] == scc.compOf[i ^ 1])return {};
        }
        vector<vector<int>> comps = scc.comps;
        reverse(all(comps));
        vector<int> compOf(2 * N);
        for (int i = 0; i < comps.size(); ++i) {
            for (auto e: comps[i])
                compOf[e] = i;
        }
        vector<int> tmp(comps.size());
        for (int i = 0; i < comps.size(); ++i) {
            if (!tmp[i]) {
                tmp[i] = 1;
                for (auto e: comps[i])
                    tmp[compOf[e ^ 1]] = -1;
            }
        }
        vector<bool> ans(N);
        for (int i = 0; i < N; ++i)
            ans[i] = tmp[compOf[2 * i]] == 1;
        return ans;
    }

    void atMostOne(const vector<int> &li) {
        if (li.size() <= 1) return;
        int last = ~li[1];
        for (int i = 2; i < li.size(); i++) {
            int next = addVar();
            implies(li[i], last);
            either(last, next);
            implies(li[i], next);
            last = ~next;
        }
        implies(li[0], last);
    }
};