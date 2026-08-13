// REROOTING DP - compute f(v as root) for EVERY v in O(n).
// Four hooks:  E() identity of merge · self(v) contribution of v itself ·
//              merge(a,b) associative+commutative · lift(a, child) push a child's value
//              across one edge into the parent's frame.
// Shipped example: ans[v].first = sum of distances from v to every other node.
struct Reroot {
    typedef pair<ll, ll> T;                      // {sum of distances, #nodes}
    static T E() { return {0, 0}; }
    static T self(int v) { return {0, 1}; }
    static T merge(T a, T b) { return {a.first + b.first, a.second + b.second}; }
    static T lift(T a, int child) { return {a.first + a.second, a.second}; }

    int n;
    vector<vector<int>> adj;
    vector<T> down, up, ans;
    vector<int> par, ord;

    Reroot(int n) : n(n), adj(n), down(n, E()), up(n, E()), ans(n, E()), par(n, -1) {}
    void add_edge(int u, int v) { adj[u].push_back(v), adj[v].push_back(u); }

    void build(int root = 0) {
        ord.clear(), ord.reserve(n);
        vector<int> st = {root};
        par[root] = -1;
        while (!st.empty()) {                    // iterative order: no recursion depth limit
            int u = st.back(); st.pop_back(); ord.push_back(u);
            for (int v : adj[u]) if (v != par[u]) par[v] = u, st.push_back(v);
        }
        for (int i = (int)ord.size() - 1; i >= 0; i--) {   // pass 1: leaves -> root
            int u = ord[i];
            down[u] = self(u);
            for (int v : adj[u]) if (v != par[u]) down[u] = merge(down[u], lift(down[v], v));
        }
        up[root] = E();
        for (int u : ord) {                      // pass 2: root -> leaves
            vector<int> ch;
            for (int v : adj[u]) if (v != par[u]) ch.push_back(v);
            int k = ch.size();
            vector<T> pre(k + 1, E()), suf(k + 1, E());
            for (int i = 0; i < k; i++) pre[i + 1] = merge(pre[i], lift(down[ch[i]], ch[i]));
            for (int i = k - 1; i >= 0; i--) suf[i] = merge(suf[i + 1], lift(down[ch[i]], ch[i]));
            ans[u] = merge(down[u], up[u]);
            for (int i = 0; i < k; i++)
                up[ch[i]] = lift(merge(self(u), merge(up[u], merge(pre[i], suf[i + 1]))), u);
        }
    }
};
// The prefix/suffix trick ("aggregate over all children EXCEPT one") is reusable on its own:
// use it whenever a DP needs "everything but this branch" and merge has no inverse.
// OTHER INSTANTIATIONS: max depth from each node (T = ll, lift = a+1, self = 0);
// count of nodes at even distance; product of subtree answers; tree diameter through each node.
