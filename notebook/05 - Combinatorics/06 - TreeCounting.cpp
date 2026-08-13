// Needs: Mint (06 - Math/01) and determinant (06 - Math/04 - Linear Algebra/02).
// COUNTING TREES AND GRAPHS.

// PRUFER CODE: bijection between labelled trees on n>=2 vertices and sequences in [0,n)^(n-2).
// Immediate corollaries: Cayley n^(n-2) labelled trees; with prescribed degrees d_i the count is
// (n-2)! / prod (d_i - 1)!  (because vertex i appears exactly d_i - 1 times in the code).
vector<int> toPrufer(const vector<vector<int>>& adj) {     // repeatedly strip the SMALLEST leaf
    int n = adj.size();
    if (n <= 2) return {};
    vector<int> deg(n), code;
    vector<bool> gone(n, false);
    priority_queue<int, vector<int>, greater<int>> q;
    for (int i = 0; i < n; i++) if ((deg[i] = adj[i].size()) == 1) q.push(i);
    for (int it = 0; it < n - 2; it++) {
        int leaf = q.top(); q.pop();
        gone[leaf] = true;
        int nb = -1;
        for (int v : adj[leaf]) if (!gone[v]) nb = v;
        code.push_back(nb);
        if (--deg[nb] == 1) q.push(nb);
    }
    return code;
}
vector<pair<int, int>> fromPrufer(vector<int> code) {
    int n = code.size() + 2;
    vector<int> deg(n, 1);
    for (int x : code) deg[x]++;
    priority_queue<int, vector<int>, greater<int>> q;
    for (int i = 0; i < n; i++) if (deg[i] == 1) q.push(i);
    vector<pair<int, int>> e;
    for (int x : code) {
        int leaf = q.top(); q.pop();
        e.push_back({leaf, x});
        if (--deg[x] == 1) q.push(x);
    }
    int a = q.top(); q.pop();
    e.push_back({a, q.top()});
    return e;
}

// MATRIX-TREE THEOREM: #spanning trees of a graph = any cofactor of the Laplacian L = D - A.
// Multigraphs work (put the multiplicity in A). Needs determinant() from GaussianElimination.
Mint spanningTrees(int n, const vector<pair<int, int>>& edges) {
    if (n == 1) return 1;
    vector<vector<Mint>> L(n, vector<Mint>(n, 0));
    for (auto [u, v] : edges) {
        L[u][u] += 1, L[v][v] += 1;
        L[u][v] -= 1, L[v][u] -= 1;
    }
    vector<vector<Mint>> M(n - 1, vector<Mint>(n - 1));    // delete row/col 0
    for (int i = 1; i < n; i++) for (int j = 1; j < n; j++) M[i - 1][j - 1] = L[i][j];
    return determinant(M);
}
// DIRECTED version (Tutte): #arborescences rooted at r = cofactor of (Dout - A) deleting row/col r
// for in-trees, or (Din - A) for out-trees.
// WEIGHTED version: put edge weights in A and the weighted degree in D -> sum over spanning
// trees of the product of weights (useful mod p for "sum over all spanning trees" problems).

// EXPONENTIAL FORMULA: if C(x) is the EGF of CONNECTED structures then e^(C(x)) is the EGF of
// all structures.  Concretely, with g_n = #labelled graphs on n nodes = 2^C(n,2):
//   c_n = g_n - sum_{k=1}^{n-1} C(n-1, k-1) * c_k * g_{n-k}      (connected labelled graphs)
vector<Mint> connectedLabelledGraphs(int n) {
    vector<Mint> g(n + 1), c(n + 1, 0);
    for (int i = 0; i <= n; i++) g[i] = Mint(2).pow((ll)i * (i - 1) / 2);
    for (int i = 1; i <= n; i++) {
        c[i] = g[i];
        for (int k = 1; k < i; k++) c[i] -= C(i - 1, k - 1) * c[k] * g[i - k];
    }
    return c;
}
