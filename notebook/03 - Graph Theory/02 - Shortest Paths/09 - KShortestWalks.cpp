// EPPSTEIN'S ALGORITHM - the k SHORTEST s-t WALKS (repeated vertices and edges allowed), in
// O(m log m + k log k). Directed, 0-indexed, non-negative weights. Returns the k lengths in
// non-decreasing order, padding with -1 when fewer than k walks exist.
// Reach for it for "the k cheapest routes", "the k-th smallest path length", "is there a path of
// length exactly L" (enumerate until you pass L), and for problems that sum over the k best plans.
// THE IDEA. Fix the shortest path TREE towards t, with d[v] = dist(v, t). Any walk is described by
// the SIDETRACK edges it takes - the edges it uses that are not tree edges - and its length is
// d[s] + (sum of the sidetracks' reduced costs w + d[to] - d[from]), each of which is >= 0. So the
// k shortest walks = the k cheapest ANTICHAINS in a heap-ordered structure, and a best-first search
// over that structure emits them one at a time.
// H[v] is a PERSISTENT leftist heap holding the sidetracks available at v and at all its ancestors
// in the tree; persistence is what makes "inherit the parent's heap" O(log n) instead of O(n).
struct KShortestWalks {
    struct Node { Node *l = 0, *r = 0; int sz = 1, to; ll val; };
    Node* meld(Node* a, Node* b) {
        if (!a) return b;
        if (!b) return a;
        if (a->val > b->val) swap(a, b);
        Node* c = new Node(*a);                                // copy-on-write: keeps old versions
        c->r = meld(c->r, b);
        if (!c->l || c->l->sz < c->r->sz) swap(c->l, c->r);
        c->sz = (c->r ? c->r->sz : 0) + 1;
        return c;
    }
    Node* push(Node* h, ll val, int to) {
        Node* c = new Node();
        c->val = val, c->to = to;
        return meld(h, c);
    }
    // e = {u, v, w} directed arcs. Returns the k shortest s -> t walk lengths.
    vector<ll> solve(int n, const vector<array<ll, 3>>& e, int s, int t, int k) {
        vector<vector<array<ll, 3>>> fwd(n), rev(n);           // {other endpoint, weight, edge id}
        for (int i = 0; i < (int)e.size(); i++) {
            fwd[e[i][0]].push_back({e[i][1], e[i][2], (ll)i});
            rev[e[i][1]].push_back({e[i][0], e[i][2], (ll)i});
        }
        vector<ll> d(n, llinf);
        vector<int> par(n, -1), tid(n, -1);                    // tid[v] = v's tree edge id
        priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
        d[t] = 0, pq.push({0, t});
        while (!pq.empty()) {                                  // Dijkstra on the REVERSED graph
            auto [dd, u] = pq.top(); pq.pop();
            if (dd > d[u]) continue;
            for (auto [v, w, id] : rev[u]) if (dd + w < d[v])
                d[v] = dd + w, par[v] = u, tid[v] = id, pq.push({d[v], (int)v});
        }
        vector<ll> ans;
        if (d[s] == llinf) return vector<ll>(k, -1);
        vector<vector<int>> kids(n);
        for (int v = 0; v < n; v++) if (par[v] >= 0) kids[par[v]].push_back(v);
        vector<Node*> H(n, nullptr);
        vector<int> bfs{t};                                    // build H down the tree from t
        for (int qi = 0; qi < (int)bfs.size(); qi++) {
            int u = bfs[qi];
            if (par[u] >= 0) H[u] = meld(H[u], H[par[u]]);
            for (auto [v, w, id] : fwd[u])
                if ((int)id != tid[u] && d[v] < llinf) H[u] = push(H[u], w + d[v] - d[u], (int)v);
            for (int v : kids[u]) bfs.push_back(v);
        }
        auto cmp = [](const pair<ll, Node*>& a, const pair<ll, Node*>& b) {
            return a.first > b.first;
        };
        priority_queue<pair<ll, Node*>, vector<pair<ll, Node*>>, decltype(cmp)> Q(cmp);
        Node* root = new Node();
        root->val = d[s], root->to = s;
        Q.push({d[s], root});
        while (!Q.empty() && (int)ans.size() < k) {
            auto [w, cur] = Q.top(); Q.pop();
            ans.push_back(w);
            if (cur->l) Q.push({w + cur->l->val - cur->val, cur->l});   // swap the last sidetrack
            if (cur->r) Q.push({w + cur->r->val - cur->val, cur->r});
            if (H[cur->to]) Q.push({w + H[cur->to]->val, H[cur->to]});     // append a new sidetrack
        }
        while ((int)ans.size() < k) ans.push_back(-1);
        return ans;
    }
};
// WALKS, NOT PATHS. The second-shortest walk may repeat a vertex. If the problem insists on SIMPLE
// paths you need YEN'S ALGORITHM: keep the best path, then for each prefix of it (the "spur"), ban
// the edges that would recreate an already-found path, run one Dijkstra from the spur vertex, and
// keep the best candidate. O(k * n * (m + n log n)) - much slower, but it is the only correct
// answer for "k shortest SIMPLE paths". On a DAG walks and paths coincide, so use this file.
// K-TH SHORTEST PATH WITH A COUNT LIMIT PER VERTEX: a plain Dijkstra that pops each vertex up to k
// times is O(k m log) and far simpler - reach for that first if k is small (<= a few hundred).
// SECOND SHORTEST PATH ONLY: just take ans[1] here, or do the two-array trick (best and
// second-best distance per vertex) in one Dijkstra.
