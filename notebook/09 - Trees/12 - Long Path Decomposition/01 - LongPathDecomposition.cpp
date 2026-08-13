// LONG PATH DECOMPOSITION - split the tree into vertical paths by always following the child with
// the DEEPEST subtree (HLD follows the child with the BIGGEST subtree instead). 0-indexed, rooted.
// TWO THINGS IT BUYS YOU THAT HLD DOES NOT.
//  1. O(1) K-TH ANCESTOR after O(n log n) preprocessing. Binary lifting is O(log n) per query;
//     this is the standard constant-time answer, and it is only ~30 lines.
//  2. DEPTH-INDEXED TREE DP IN O(n) TOTAL ("dp[v][d] = something about the vertices at depth d
//     below v"). Give each path head an array of its length and let every vertex on the path SHARE
//     that array with an offset - the heavy child inherits the parent's array for free, and only
//     the O(sqrt n)-ish path heads allocate. The classic use is "for each v, the most common depth
//     in its subtree" and "count vertices at distance k below v" for all v.
// The number of paths equals the number of LEAVES, and the crucial property is: the path containing
// v has length at least (height of v), so jumping to the head of v's path already covers a lot.
// K-TH ANCESTOR IN O(1). For every path head h of length L, store the L ancestors above h as well
// as the L vertices of the path itself ("up" and "down" tables of size 2L, total O(n)). To answer
// kth(v, k): let j = highest power of two <= k, jump v to its 2^j-th ancestor with a lifting table
// (one step), and the remaining k - 2^j < 2^j <= (height of the new vertex), so the rest of the
// jump lands inside the new vertex's own path table - one array lookup.
struct LongPath {
    int n, B;
    vector<int> par, dep, hgt, heavy, head;
    vector<vector<int>> jmp, tab;                              // tab[h] = ladder of head h
    vector<int> posInTab, headOf, logs;
    LongPath(int n, const vector<vector<int>>& adj, int root = 0)
        : n(n), B(1), par(n, -1), dep(n, 0), hgt(n, 0), heavy(n, -1), head(n, -1),
          posInTab(n, 0), headOf(n, -1), logs(n + 1, 0) {
        while ((1 << B) <= n) B++;
        for (int i = 2; i <= n; i++) logs[i] = logs[i / 2] + 1;
        vector<int> ord;                                       // iterative DFS -> preorder
        vector<int> st{root};
        par[root] = -1;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            ord.push_back(u);
            for (int v : adj[u]) if (v != par[u]) par[v] = u, dep[v] = dep[u] + 1, st.push_back(v);
        }
        for (int i = n - 1; i >= 0; i--) {                     // heights, bottom up
            int u = ord[i];
            for (int v : adj[u]) if (v != par[u] && hgt[v] + 1 > hgt[u])
                hgt[u] = hgt[v] + 1, heavy[u] = v;
        }
        jmp.assign(B, vector<int>(n, -1));
        for (int u = 0; u < n; u++) jmp[0][u] = par[u];
        for (int k = 1; k < B; k++) for (int u = 0; u < n; u++)
            jmp[k][u] = jmp[k - 1][u] < 0 ? -1 : jmp[k - 1][jmp[k - 1][u]];
        for (int u : ord) head[u] = (par[u] >= 0 && heavy[par[u]] == u) ? head[par[u]] : u;
        tab.assign(n, {});
        for (int h = 0; h < n; h++) if (head[h] == h) {        // build the ladder of this path
            int L = hgt[h] + 1;
            vector<int> down;
            for (int v = h; v >= 0; v = heavy[v]) down.push_back(v);
            vector<int> upv;                               // L ancestors above h, if they exist
            for (int x = par[h], c = 0; x >= 0 && c < L; x = par[x], c++) upv.push_back(x);
            reverse(all(upv));
            for (int v : down) upv.push_back(v);               // [far ancestors ... h ... deepest]
            tab[h] = upv;
            int off = (int)tab[h].size() - (int)down.size();
            for (int i = 0; i < (int)down.size(); i++)
                posInTab[down[i]] = off + i, headOf[down[i]] = h;
        }
    }
    int kth(int v, int k) const {                              // k-th ancestor, -1 if too high
        if (k == 0) return v;
        if (k > dep[v]) return -1;
        int j = logs[k];
        v = jmp[j][v], k -= 1 << j;                            // one lifting step
        int h = headOf[v];
        return tab[h][posInTab[v] - k];                        // the rest is inside the ladder
    }
};
// WHY THE LADDER IS LONG ENOUGH: after the single 2^j jump, the remainder k' < 2^j and the vertex
// we are on has height >= 2^j - 1, so its path (and hence its ladder's upper half) covers k'.
// MEMORY is O(n log n) for the lifting table and O(n) for the ladders (each path of length L keeps
// 2L entries, and the path lengths sum to n).
// LEVEL ANCESTOR can also be done in O(n) preprocessing (Euler tour + the ladder trick without the
// lifting table), but the version above is the one you can write from memory.
// SMALL-TO-LARGE ALTERNATIVE for depth-indexed DP: merge maps/multisets child by child, O(n log^2)
// - shorter but slower; use it when the DP state is not a plain array indexed by depth.
