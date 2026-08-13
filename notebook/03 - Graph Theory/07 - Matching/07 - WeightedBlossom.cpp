// MAXIMUM WEIGHT MATCHING IN A GENERAL GRAPH - blossom with DUAL VARIABLES, O(n^3), O(n^2) memory.
// 1-INDEXED (vertices 1..n; index 0 is the "none" sentinel and 0 weight means "no edge").
// This is the weighted counterpart of 04 - Blossom. It returns the maximum TOTAL WEIGHT over all
// matchings (NOT necessarily a maximum-cardinality one) together with how many edges it used.
// Reach for it when the pairs are not bipartite and the pairing has a value: "pair up the people
// to maximise total compatibility", Chinese postman (08), "minimum cost to make every degree even",
// and any problem where Hungarian would work if only the graph had two sides.
// RECIPES
//   MINIMUM weight PERFECT matching: add a big constant B to every edge (B >= sum of |w|) and
//   negate, i.e. addEdge(u, v, B - w); a perfect matching then dominates any smaller one, and the
//   answer is B * n/2 - result. Verify solve().second * 2 == n before trusting it.
//   MAXIMUM CARDINALITY first, weight second: use w * (n + 1) + 1 per edge.
// The duals lab[] are Edmonds' y-values; a blossom b keeps lab[b] = 2 * (its dual). Every step
// either tightens an edge (slack 0), contracts a blossom, or expands one with zero dual.
struct WeightedBlossom {
    struct E { int u, v; ll w; };
    int n, nx, tk = 0;
    vector<vector<E>> g;
    vector<vector<int>> ff, flower;                            // ff = which sub-blossom x came from
    vector<int> match, slack, st, pa, S, vis;
    vector<ll> lab;
    deque<int> q;
    WeightedBlossom(int n) : n(n), nx(n), g(2 * n + 1, vector<E>(2 * n + 1)),
        ff(2 * n + 1, vector<int>(2 * n + 1, 0)), flower(2 * n + 1), match(2 * n + 1, 0),
        slack(2 * n + 1, 0), st(2 * n + 1, 0), pa(2 * n + 1, 0), S(2 * n + 1, 0),
        vis(2 * n + 1, 0), lab(2 * n + 1, 0) {
        for (int u = 1; u <= 2 * n; u++) for (int v = 1; v <= 2 * n; v++) g[u][v] = {u, v, 0};
    }
    void addEdge(int u, int v, ll w) { g[u][v].w = g[v][u].w = max(g[u][v].w, w); }
    ll d(const E& e) { return lab[e.u] + lab[e.v] - g[e.u][e.v].w * 2; }   // slack
    void upSlack(int u, int x) { if (!slack[x] || d(g[u][x]) < d(g[slack[x]][x])) slack[x] = u; }
    void setSlack(int x) {
        slack[x] = 0;
        for (int u = 1; u <= n; u++)
            if (g[u][x].w > 0 && st[u] != x && S[st[u]] == 0) upSlack(u, x);
    }
    void qPush(int x) { if (x <= n) q.push_back(x); else for (int y : flower[x]) qPush(y); }
    void setSt(int x, int b) { st[x] = b; if (x > n) for (int y : flower[x]) setSt(y, b); }
    int getPr(int b, int xr) {
        int pr = find(all(flower[b]), xr) - flower[b].begin();
        if (pr & 1) {
            reverse(flower[b].begin() + 1, flower[b].end());
            return (int)flower[b].size() - pr;
        }
        return pr;
    }
    void setMatch(int u, int v) {
        match[u] = g[u][v].v;
        if (u <= n) return;
        E e = g[u][v];
        int xr = ff[u][e.u], pr = getPr(u, xr);
        for (int i = 0; i < pr; i++) setMatch(flower[u][i], flower[u][i ^ 1]);
        setMatch(xr, v);
        rotate(flower[u].begin(), flower[u].begin() + pr, flower[u].end());
    }
    void augment(int u, int v) {
        int nv = st[match[u]];
        setMatch(u, v);
        if (!nv) return;
        setMatch(nv, st[pa[nv]]), augment(st[pa[nv]], nv);
    }
    int getLca(int u, int v) {
        for (++tk; u || v; swap(u, v)) {
            if (!u) continue;
            if (vis[u] == tk) return u;
            vis[u] = tk, u = st[match[u]];
            if (u) u = st[pa[u]];
        }
        return 0;
    }
    void addBlossom(int u, int lca, int v) {                   // contract the odd cycle u..lca..v
        int b = n + 1;
        while (b <= nx && st[b]) ++b;
        if (b > nx) ++nx;
        lab[b] = 0, S[b] = 0, match[b] = match[lca];
        flower[b].assign(1, lca);
        for (int x = u, y; x != lca; x = st[pa[y]])
            flower[b].push_back(x), flower[b].push_back(y = st[match[x]]), qPush(y);
        reverse(flower[b].begin() + 1, flower[b].end());
        for (int x = v, y; x != lca; x = st[pa[y]])
            flower[b].push_back(x), flower[b].push_back(y = st[match[x]]), qPush(y);
        setSt(b, b);
        for (int x = 1; x <= nx; x++) g[b][x].w = g[x][b].w = 0;
        for (int x = 1; x <= n; x++) ff[b][x] = 0;
        for (int xs : flower[b]) {
            for (int x = 1; x <= nx; x++)
                if (g[b][x].w == 0 || d(g[xs][x]) < d(g[b][x]))
                    g[b][x] = g[xs][x], g[x][b] = g[x][xs];
            for (int x = 1; x <= n; x++) if (ff[xs][x]) ff[b][x] = xs;
        }
        setSlack(b);
    }
    void expandBlossom(int b) {                                // only if S[b] == 1, lab[b] == 0
        for (int x : flower[b]) setSt(x, x);
        int xr = ff[b][g[b][pa[b]].u], pr = getPr(b, xr);
        for (int i = 0; i < pr; i += 2) {
            int xs = flower[b][i], xns = flower[b][i + 1];
            pa[xs] = g[xns][xs].u, S[xs] = 1, S[xns] = 0;
            slack[xs] = 0, setSlack(xns), qPush(xns);
        }
        S[xr] = 1, pa[xr] = pa[b];
        for (int i = pr + 1; i < (int)flower[b].size(); i++) S[flower[b][i]] = -1,
            setSlack(flower[b][i]);
        st[b] = 0;
    }
    bool onFound(const E& e) {
        int u = st[e.u], v = st[e.v];
        if (S[v] == -1) {
            pa[v] = e.u, S[v] = 1;
            int nu = st[match[v]];
            slack[v] = slack[nu] = 0, S[nu] = 0, qPush(nu);
        } else if (S[v] == 0) {
            int lca = getLca(u, v);
            if (!lca) return augment(u, v), augment(v, u), true;
            addBlossom(u, lca, v);
        }
        return false;
    }
    bool matching() {                                          // one augmentation, or false
        fill(S.begin(), S.begin() + nx + 1, -1);
        fill(slack.begin(), slack.begin() + nx + 1, 0);
        q.clear();
        for (int x = 1; x <= nx; x++) if (st[x] == x && !match[x]) pa[x] = 0, S[x] = 0, qPush(x);
        if (q.empty()) return false;
        for (;;) {
            while (!q.empty()) {
                int u = q.front(); q.pop_front();
                if (S[st[u]] == 1) continue;
                for (int v = 1; v <= n; v++) if (g[u][v].w > 0 && st[u] != st[v]) {
                    if (d(g[u][v]) == 0) { if (onFound(g[u][v])) return true; }
                    else upSlack(u, st[v]);
                }
            }
            ll dt = llinf;                                     // dual update step
            for (int b = n + 1; b <= nx; b++) if (st[b] == b && S[b] == 1) dt = min(dt, lab[b] / 2);
            for (int x = 1; x <= nx; x++) if (st[x] == x && slack[x]) {
                if (S[x] == -1) dt = min(dt, d(g[slack[x]][x]));
                else if (S[x] == 0) dt = min(dt, d(g[slack[x]][x]) / 2);
            }
            for (int u = 1; u <= n; u++) {
                if (S[st[u]] == 0) { if (lab[u] <= dt) return false; lab[u] -= dt; }
                else if (S[st[u]] == 1) lab[u] += dt;
            }
            for (int b = n + 1; b <= nx; b++) if (st[b] == b) {
                if (S[st[b]] == 0) lab[b] += dt * 2;
                else if (S[st[b]] == 1) lab[b] -= dt * 2;
            }
            q.clear();
            for (int x = 1; x <= nx; x++)
                if (st[x] == x && slack[x] && st[slack[x]] != x && d(g[slack[x]][x]) == 0)
                    if (onFound(g[slack[x]][x])) return true;
            for (int b = n + 1; b <= nx; b++)
                if (st[b] == b && S[b] == 1 && lab[b] == 0) expandBlossom(b);
        }
    }
    pair<ll, int> solve() {                                    // {total weight, #matched edges}
        fill(match.begin(), match.begin() + n + 1, 0);
        nx = n;
        ll ans = 0, wmax = 0;
        int cnt = 0;
        for (int u = 0; u <= n; u++) st[u] = u, flower[u].clear();
        for (int u = 1; u <= n; u++) for (int v = 1; v <= n; v++)
            ff[u][v] = (u == v ? u : 0), wmax = max(wmax, g[u][v].w);
        for (int u = 1; u <= n; u++) lab[u] = wmax;
        while (matching()) ++cnt;
        for (int u = 1; u <= n; u++) if (match[u] && match[u] < u) ans += g[u][match[u]].w;
        return {ans, cnt};                                     // match[u] = partner of u, or 0
    }
};
// WEIGHTS MUST BE NON-NEGATIVE here (an edge of weight 0 is indistinguishable from a non-edge);
// shift them if needed. Everything is integral - with real weights use long double and an eps.
// FASTER ALTERNATIVES: if n <= 20, a bitmask DP over matched vertices is 5 lines and beats this.
// If the graph is bipartite, use Hungarian (03) - O(n^3) too but with a far smaller constant.
// TUTTE MATRIX: for the UNWEIGHTED decision "is there a perfect matching", a random skew-symmetric
// matrix over a large field has full rank with high probability iff one exists - O(n^3) and 10
// lines, and rank/2 is the maximum matching size.
