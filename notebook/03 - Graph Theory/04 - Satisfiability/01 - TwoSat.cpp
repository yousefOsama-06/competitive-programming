// 2-SAT via Kosaraju - O(n + m). Self-contained, 0-based variables.
// Literal encoding: 2*i = (x_i false), 2*i+1 = (x_i true).
// Satisfiable iff x and !x are never in the same SCC; then take the literal whose SCC is LATER
// in topological order.
struct TwoSat {
    int n;
    vector<vector<int>> adj, radj;
    vector<int> ord, comp;
    vector<bool> used, val;

    TwoSat(int n = 0) : n(n), adj(2 * n), radj(2 * n) {}
    int lit(int i, bool t) { return 2 * i + t; }
    int add_var() { n++, adj.resize(2 * n), radj.resize(2 * n); return n - 1; }

    void imply(int a, int b) {                          // literal a  =>  literal b  (and contrapositive)
        adj[a].push_back(b), radj[b].push_back(a);
        adj[b ^ 1].push_back(a ^ 1), radj[a ^ 1].push_back(b ^ 1);
    }
    void either(int a, int b) { imply(a ^ 1, b); }      // (a OR b)
    void force(int a) { imply(a ^ 1, a); }              // a must be true
    void nand_(int a, int b) { either(a ^ 1, b ^ 1); }  // not both
    void eq(int a, int b) { imply(a, b), imply(b, a); } // a <=> b
    void xor_(int a, int b) { eq(a, b ^ 1); }

    // AT MOST ONE of `lits` is true, with O(k) edges instead of O(k^2) (prefix/"lightbulb" trick):
    // aux_i means "someone at index <= i was chosen".
    void at_most_one(const vector<int>& lits) {
        int prev = -1;
        for (int c : lits) {
            int cur = lit(add_var(), true);
            imply(c, cur);
            if (prev != -1) imply(prev, cur), imply(c, prev ^ 1);
            prev = cur;
        }
    }

    void dfs1(int u) { used[u] = true; for (int v : adj[u]) if (!used[v]) dfs1(v); ord.push_back(u); }
    void dfs2(int u, int c) { comp[u] = c; for (int v : radj[u]) if (comp[v] < 0) dfs2(v, c); }

    bool solve() {
        used.assign(2 * n, false), comp.assign(2 * n, -1), val.assign(n, false), ord.clear();
        for (int i = 0; i < 2 * n; i++) if (!used[i]) dfs1(i);
        int c = 0;
        for (int i = 2 * n - 1; i >= 0; i--) if (comp[ord[i]] < 0) dfs2(ord[i], c++);
        for (int i = 0; i < n; i++) {
            if (comp[2 * i] == comp[2 * i + 1]) return false;
            val[i] = comp[2 * i + 1] > comp[2 * i];
        }
        return true;                                    // read val[0..base_n-1]; aux vars are padding
    }
};
// RANGE CLAUSE ("if x then ALL of [l,r]") in O(log n) edges: build a segment tree over the
// variables, give every internal node an aux variable, add parent => both children, then
// imply(x, node) for the O(log n) covering nodes. Mirror the tree (child => parent) for
// "if any of [l,r] then x".
