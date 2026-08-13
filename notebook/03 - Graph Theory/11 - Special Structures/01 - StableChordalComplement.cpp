// THREE GRAPH TOOLS THAT ARE NOT ABOUT PATHS OR FLOW.

// --- GALE-SHAPLEY STABLE MARRIAGE, O(n^2). ---
// n proposers, n receivers, each with a full preference ranking. A matching is STABLE if no pair
// mutually prefer each other to their assigned partners. One always exists, and this finds the
// PROPOSER-OPTIMAL one: every proposer gets the best partner they could have in ANY stable
// matching, and simultaneously every receiver gets their worst. (Swap the roles to flip that.)
// pref[i] = the proposers' preference lists; rank[j][i] = receiver j's ranking of proposer i.
vector<int> stableMarriage(const vector<vector<int>>& pref, const vector<vector<int>>& rank) {
    int n = pref.size();
    vector<int> match(n, -1), nxt(n, 0);                       // match[receiver] = proposer
    vector<int> free_(n);
    iota(all(free_), 0);
    while (!free_.empty()) {
        int m = free_.back(); free_.pop_back();
        int w = pref[m][nxt[m]++];                             // propose down the list
        if (match[w] < 0) match[w] = m;
        else if (rank[w][m] < rank[w][match[w]]) free_.push_back(match[w]), match[w] = m;
        else free_.push_back(m);
    }
    return match;
}
// HOSPITALS/RESIDENTS (one side has capacity c) is the same loop with a heap of accepted
// proposers per hospital. STABLE ROOMMATES (one pool, no sides) may have NO stable matching -
// Irving's algorithm decides it, and it is a different, longer algorithm.

// --- TRAVERSAL OF THE COMPLEMENT GRAPH in O(n + m), never materialising the O(n^2) edges. ---
// Keep the not-yet-visited vertices in a linked structure; from u, walk that set and step to
// every w that is NOT a neighbour of u. Each vertex leaves the set once, and each REAL edge is
// inspected once, so the total is O(n + m).
// Reach for it whenever the input describes the NON-edges: "connected components of the graph of
// pairs that do NOT conflict", complement colouring, complement BFS distances.
vector<int> complementComponents(int n, const vector<vector<int>>& adj) {
    vector<char> isAdj(n, 0);
    set<int> unvis;
    for (int i = 0; i < n; i++) unvis.insert(i);
    vector<int> comp(n, -1);
    int c = 0;
    while (!unvis.empty()) {
        int s = *unvis.begin();
        unvis.erase(unvis.begin());
        vector<int> st{s};
        comp[s] = c;
        while (!st.empty()) {
            int u = st.back(); st.pop_back();
            for (int v : adj[u]) isAdj[v] = 1;
            vector<int> take;
            for (int w : unvis) if (!isAdj[w]) take.push_back(w);
            for (int w : take) unvis.erase(w), comp[w] = c, st.push_back(w);
            for (int v : adj[u]) isAdj[v] = 0;
        }
        c++;
    }
    return comp;                                               // comp[v] = component id
}
// The complement of a DISCONNECTED graph is connected, so a complement graph almost always has
// very few components - that is why this is fast in practice as well as in theory.

// --- CHORDAL GRAPHS: maximum cardinality search gives a PERFECT ELIMINATION ORDERING. ---
// A graph is CHORDAL if every cycle of length >= 4 has a chord. Equivalently it has a PEO: an
// order in which every vertex's LATER neighbours form a clique.
// Why you care: on a chordal graph, MAXIMUM CLIQUE, CHROMATIC NUMBER, MAXIMUM INDEPENDENT SET
// and MINIMUM CLIQUE COVER are all POLYNOMIAL - the four problems that are NP-hard in general.
// Interval graphs and trees are chordal, so "these intervals conflict" problems land here.
vector<int> mcsOrder(int n, const vector<vector<int>>& adj) {   // reverse of a PEO if chordal
    vector<int> w(n, 0), ord;
    vector<char> used(n, 0);
    for (int it = 0; it < n; it++) {
        int b = -1;
        for (int v = 0; v < n; v++) if (!used[v] && (b < 0 || w[v] > w[b])) b = v;
        used[b] = 1, ord.push_back(b);
        for (int u : adj[b]) if (!used[u]) w[u]++;
    }
    reverse(all(ord));
    return ord;                                                // this IS a PEO iff chordal
}
bool isChordal(int n, const vector<vector<int>>& adj) {
    auto ord = mcsOrder(n, adj);
    vector<int> pos(n);
    for (int i = 0; i < n; i++) pos[ord[i]] = i;
    vector<char> mark(n, 0);
    for (int i = 0; i < n; i++) {                              // v's later neighbours must be
        int v = ord[i], par = -1;                              // a clique; it is enough to check
        for (int u : adj[v]) if (pos[u] > i && (par < 0 || pos[u] < pos[par])) par = u;
        if (par < 0) continue;
        for (int u : adj[par]) mark[u] = 1;
        for (int u : adj[v]) if (pos[u] > i && u != par && !mark[u]) {
            for (int x : adj[par]) mark[x] = 0;
            return false;
        }
        for (int u : adj[par]) mark[u] = 0;
    }
    return true;
}
// GIVEN A PEO: greedy colouring in PEO order uses exactly omega colours (so chi = omega); the
// maximum clique is the largest "v plus its later neighbours"; the maximum independent set is the
// greedy "take v, delete its later neighbours" over the PEO.
