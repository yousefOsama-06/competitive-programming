struct Edge {
    int to;
    int cost;
    int cap, flow, backEdge;
};

struct DecomposedPath {
    int amount;
    vector<int> vertices;
};

struct MCMF {
    const int inf = 1000000010;
    int n;
    vector<vector<Edge> > g;

    // store original forward edges
    vector<pair<int, int> > original;

    MCMF(int _n) {
        n = _n + 1;
        g.resize(n);
    }

    void addEdge(int u, int v, int cap, int cost) {
        Edge e1 = {v, cost, cap, 0, (int) g[v].size()};
        Edge e2 = {u, -cost, 0, 0, (int) g[u].size()};
        g[u].push_back(e1);
        g[v].push_back(e2);

        // remember where the original edge is
        original.push_back({u, (int) g[u].size() - 1});
    }

    pair<int, int> minCostMaxFlow(int s, int t) {
        int flow = 0;
        int cost = 0;
        vector<int> state(n), from(n), from_edge(n);
        vector<int> d(n);
        deque<int> q;

        while (true) {
            for (int i = 0 ; i < n ; i++)
                state[i] = 2, d[i] = inf, from[i] = -1;

            state[s] = 1;
            q.clear();
            q.push_back(s);
            d[s] = 0;

            while (!q.empty()) {
                int v = q.front();
                q.pop_front();
                state[v] = 0;

                for (int i = 0 ; i < (int) g[v].size() ; i++) {
                    Edge e = g[v][i];
                    if (e.flow >= e.cap || d[e.to] <= d[v] + e.cost)
                        continue;

                    int to = e.to;
                    d[to] = d[v] + e.cost;
                    from[to] = v;
                    from_edge[to] = i;

                    if (state[to] == 1) continue;
                    if (!state[to] || (!q.empty() && d[q.front()] > d[to]))
                        q.push_front(to);
                    else
                        q.push_back(to);

                    state[to] = 1;
                }
            }

            if (d[t] == inf) break;

            int it = t, addflow = inf;
            while (it != s) {
                addflow = min(addflow,
                              g[from[it]][from_edge[it]].cap -
                              g[from[it]][from_edge[it]].flow);
                it = from[it];
            }

            it = t;
            while (it != s) {
                g[from[it]][from_edge[it]].flow += addflow;
                g[it][g[from[it]][from_edge[it]].backEdge].flow -= addflow;
                cost += g[from[it]][from_edge[it]].cost * addflow;
                it = from[it];
            }

            flow += addflow;
        }

        return {cost, flow};
    }

    vector<DecomposedPath> extract_paths(int s, int t) {
        vector<int> rem(original.size(), 0);
        vector<vector<pair<int, int> > > adj(n);

        for (int id = 0 ; id < (int) original.size() ; id++) {
            auto [u, idx] = original[id];
            Edge &e = g[u][idx];
            if (e.flow > 0) {
                rem[id] = e.flow;
                adj[u].push_back({e.to, id});
            }
        }

        vector < DecomposedPath > res;
        vector<int> parV(n), parE(n);

        while (true) {
            fill(parV.begin(), parV.end(), -1);
            queue<int> q;
            q.push(s);
            parV[s] = s;

            while (!q.empty() && parV[t] == -1) {
                int u = q.front();
                q.pop();
                for (auto [v, id]: adj[u]) {
                    if (rem[id] > 0 && parV[v] == -1) {
                        parV[v] = u;
                        parE[v] = id;
                        q.push(v);
                    }
                }
            }

            if (parV[t] == -1) break;

            int add = inf;
            for (int v = t ; v != s ; v = parV[v]) {
                add = min(add, rem[parE[v]]);
            }

            vector<int> path;
            for (int v = t ; ; v = parV[v]) {
                path.push_back(v);
                if (v == s) break;
            }
            reverse(path.begin(), path.end());

            for (int v = t ; v != s ; v = parV[v]) {
                rem[parE[v]] -= add;
            }

            res.push_back({add, path});
        }

        return res;
    }

    vector<vector<int> > extract_paths_unit(int s, int t) {
        vector<vector<pair<int, int> > > adj(n);

        for (auto [u, idx]: original) {
            Edge &e = g[u][idx];
            if (e.flow > 0) {
                adj[u].push_back({e.to, idx});
            }
        }

        vector<int> vis(n), path;
        vector<vector<int> > paths;

        function<bool(int)> dfs = [&](int u) {
            path.push_back(u);

            if (u == t)
                return true;

            vis[u] = 1;

            for (auto [v, idx]: adj[u]) {
                Edge &e = g[u][idx];

                if (e.flow > 0 && !vis[v]) {
                    e.flow--; // consume this unit edge

                    if (dfs(v))
                        return true;

                    e.flow++;
                }
            }

            path.pop_back();
            return false;
        };

        while (true) {
            fill(vis.begin(), vis.end(), 0);
            path.clear();

            if (!dfs(s))
                break;

            paths.push_back(path);
        }

        return paths;
    }
};
