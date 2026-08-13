template<typename flow_t = int, typename cost_t = int>
struct mcSFlow {
    struct Edge {
        cost_t c;
        flow_t f;
        int to, rev;

        Edge(int _to, cost_t _c, flow_t _f, int _rev) : c(_c), f(_f), to(_to), rev(_rev) {
        }
    };

    struct DecomposedPath {
        flow_t amount;
        vector<int> vertices;
    };

    static constexpr cost_t INFCOST = numeric_limits<cost_t>::max() / 2;
    cost_t eps;
    int N, S, T;
    vector<vector<Edge> > G;
    vector<unsigned int> isq, cur;
    vector<flow_t> ex;
    vector<cost_t> h;

    mcSFlow(int _N, int _S, int _T) : eps(0), N(_N), S(_S), T(_T), G(_N) {
    }

    // store original forward edges
    vector<pair<int, int> > original;

    void add_edge(int a, int b, cost_t cost, flow_t cap) {
        assert(cap >= 0);
        assert(a >= 0 && a < N && b >= 0 && b < N);
        if (a == b) {
            assert(cost >= 0);
            return;
        }
        cost *= N;
        eps = max(eps, abs(cost));
        original.push_back({a, (int) G[a].size()});
        G[a].emplace_back(b, cost, cap, (int) G[b].size());
        G[b].emplace_back(a, -cost, 0, (int) G[a].size() - 1);
    }

    void add_flow(Edge &e, flow_t f) {
        Edge &back = G[e.to][e.rev];
        if (!ex[e.to] && f)
            hs[h[e.to]].push_back(e.to);
        e.f -= f;
        ex[e.to] += f;
        back.f += f;
        ex[back.to] -= f;
    }

    vector<vector<int> > hs;
    vector<int> co;

    flow_t max_flow() {
        ex.assign(N, 0);
        h.assign(N, 0);
        hs.resize(2 * N);
        co.assign(2 * N, 0);
        cur.assign(N, 0);
        h[S] = N;
        ex[T] = 1;
        co[0] = N - 1;
        for (auto &e: G[S]) add_flow(e, e.f);
        if (hs[0].size())
            for (int hi = 0 ; hi >= 0 ;) {
                int u = hs[hi].back();
                hs[hi].pop_back();
                while (ex[u] > 0) {
                    if (cur[u] == (int) G[u].size()) {
                        h[u] = 1e9;
                        for (unsigned int i = 0 ; i < G[u].size() ; ++i) {
                            auto &e = G[u][i];
                            if (e.f && h[u] > h[e.to] + 1) {
                                h[u] = h[e.to] + 1, cur[u] = i;
                            }
                        }
                        if (++co[h[u]], !--co[hi] && hi < N)
                            for (int i = 0 ; i < N ; ++i)
                                if (hi < h[i] && h[i] < N) {
                                    --co[h[i]];
                                    h[i] = N + 1;
                                }
                        hi = h[u];
                    } else if (G[u][cur[u]].f && h[u] == h[G[u][cur[u]].to] + 1)
                        add_flow(G[u][cur[u]], min(ex[u], G[u][cur[u]].f));
                    else ++cur[u];
                }
                while (hi >= 0 && hs[hi].empty()) --hi;
            }
        return -ex[S];
    }

    void push(Edge &e, flow_t amt) {
        if (e.f < amt) amt = e.f;
        e.f -= amt;
        ex[e.to] += amt;
        G[e.to][e.rev].f += amt;
        ex[G[e.to][e.rev].to] -= amt;
    }

    void relabel(int vertex) {
        cost_t newHeight = -INFCOST;
        for (unsigned int i = 0 ; i < G[vertex].size() ; ++i) {
            Edge const &e = G[vertex][i];
            if (e.f && newHeight < h[e.to] - e.c) {
                newHeight = h[e.to] - e.c;
                cur[vertex] = i;
            }
        }
        h[vertex] = newHeight - eps;
    }

    static constexpr int scale = 2;

    pair<flow_t, cost_t> minCostMaxFlow() {
        cost_t retCost = 0;
        for (int i = 0 ; i < N ; ++i)
            for (Edge &e: G[i])
                retCost += e.c * (e.f);

        flow_t retFlow = max_flow();
        h.assign(N, 0);
        ex.assign(N, 0);
        isq.assign(N, 0);
        cur.assign(N, 0);
        queue<int> q;

        for (; eps ; eps >>= scale) {
            fill(cur.begin(), cur.end(), 0);
            for (int i = 0 ; i < N ; ++i)
                for (auto &e: G[i])
                    if (h[i] + e.c - h[e.to] < 0 && e.f) push(e, e.f);

            for (int i = 0 ; i < N ; ++i) {
                if (ex[i] > 0) {
                    q.push(i);
                    isq[i] = 1;
                }
            }

            while (!q.empty()) {
                int u = q.front();
                q.pop();
                isq[u] = 0;
                while (ex[u] > 0) {
                    if (cur[u] == (int) G[u].size())
                        relabel(u);

                    for (unsigned int &i = cur[u], max_i = G[u].size() ; i < max_i ; ++i) {
                        Edge &e = G[u][i];
                        if (h[u] + e.c - h[e.to] < 0) {
                            push(e, ex[u]);
                            if (ex[e.to] > 0 && isq[e.to] == 0) {
                                q.push(e.to);
                                isq[e.to] = 1;
                            }
                            if (ex[u] == 0) break;
                        }
                    }
                }
            }

            if (eps > 1 && eps >> scale == 0) {
                eps = 1 << scale;
            }
        }

        for (int i = 0 ; i < N ; ++i)
            for (Edge &e: G[i])
                retCost -= e.c * (e.f);

        return make_pair(retFlow, retCost / 2 / N);
    }

    flow_t getFlow(Edge const &e) {
        return G[e.to][e.rev].f;
    }

    vector<DecomposedPath> extract_st_paths() {
        vector<flow_t> rem(original.size(), 0);
        vector<vector<pair<int, int> > > adj(N);

        for (int id = 0 ; id < (int) original.size() ; ++id) {
            auto [u, idx] = original[id];
            Edge &e = G[u][idx];
            flow_t f = G[e.to][e.rev].f; // flow on the original edge
            if (f > 0) {
                rem[id] = f;
                adj[u].push_back({e.to, id});
            }
        }

        vector < DecomposedPath > res;
        vector<int> parV(N), parE(N), vis(N);

        while (true) {
            fill(vis.begin(), vis.end(), 0);
            queue<int> q;
            q.push(S);
            vis[S] = 1;
            parV[S] = -1;

            while (!q.empty() && !vis[T]) {
                int u = q.front();
                q.pop();
                for (auto [v, id]: adj[u]) {
                    if (rem[id] > 0 && !vis[v]) {
                        vis[v] = 1;
                        parV[v] = u;
                        parE[v] = id;
                        q.push(v);
                        if (v == T) break;
                    }
                }
            }

            if (!vis[T]) break;

            flow_t add = numeric_limits<flow_t>::max();
            vector<int> path;
            for (int v = T ; v != S ; v = parV[v]) {
                add = min(add, rem[parE[v]]);
            }
            for (int v = T ; ; v = parV[v]) {
                path.push_back(v);
                if (v == S) break;
            }
            reverse(path.begin(), path.end());

            for (int v = T ; v != S ; v = parV[v]) {
                rem[parE[v]] -= add;
            }

            res.push_back({add, path});
        }

        return res;
    }

    vector<DecomposedPath> extract_st_paths_unit() {
        vector<int> rem(original.size(), 0);
        vector<vector<pair<int, int> > > adj(N);

        // Build graph of edges that carry positive flow
        for (int id = 0 ; id < (int) original.size() ; ++id) {
            auto [u, idx] = original[id];
            Edge &e = G[u][idx];
            flow_t f = G[e.to][e.rev].f; // flow on original edge
            if (f > 0) {
                rem[id] = f; // for unit capacities this is 0/1
                adj[u].push_back({e.to, id});
            }
        }

        vector < DecomposedPath > res;
        vector<int> parV(N), parE(N);

        while (true) {
            fill(parV.begin(), parV.end(), -1);

            queue<int> q;
            q.push(S);
            parV[S] = S;

            while (!q.empty() && parV[T] == -1) {
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

            if (parV[T] == -1) break;

            vector<int> path;
            for (int v = T ; ; v = parV[v]) {
                path.push_back(v);
                if (v == S) break;
            }
            reverse(path.begin(), path.end());

            // unit capacities => path amount is always 1
            for (int v = T ; v != S ; v = parV[v]) {
                rem[parE[v]] = 0;
            }

            res.push_back({1, path});
        }

        return res;
    }
};
