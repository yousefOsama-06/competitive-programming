// Needs: Mint (06 - Math/01) and AhoCorasick (01 - AhoCorasick.cpp).
// DP OVER THE AHO-CORASICK AUTOMATON - the high-value use of Aho, more than matching itself.
// After build(), nxt[] is a total transition function, so the automaton is a DFA over states
// 0..S-1 and you can run any DP on (position, state, extra).
// "bad" marks states that contain a forbidden pattern as a suffix; propagate it along exit links.
struct AhoDP {
    static const int A = 26;                       // trie fan-out
    int alpha = 26;                                // alphabet the DP counts over (set smaller!)
    vector<array<int, A>> nxt;
    vector<int> link, term;
    vector<char> bad;
    vector<int> order;                                 // BFS order of states

    AhoDP() { newNode(); }
    int newNode() { nxt.push_back({}); nxt.back().fill(0); link.push_back(0); term.push_back(0); bad.push_back(0); return nxt.size() - 1; }
    void insert(const string& p, int id = 1) {
        int u = 0;
        for (char c : p) {
            int v = c - 'a';
            if (!nxt[u][v]) nxt[u][v] = newNode();
            u = nxt[u][v];
        }
        term[u] = id, bad[u] = 1;
    }
    void build() {
        queue<int> q;
        for (int c = 0; c < A; c++) if (nxt[0][c]) q.push(nxt[0][c]);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            order.push_back(u);
            bad[u] = bad[u] || bad[link[u]];           // a state is bad if any SUFFIX is a pattern
            for (int c = 0; c < A; c++) {
                int v = nxt[u][c];
                if (v) link[v] = nxt[link[u]][c], q.push(v);
                else nxt[u][c] = nxt[link[u]][c];      // total transition function
            }
        }
    }
    // #strings of length L over the first `alpha` letters containing NO pattern. O(L*S*alpha).
    Mint countAvoiding(int L) {
        int S = nxt.size();
        vector<Mint> dp(S, 0), nd;
        dp[0] = 1;
        for (int i = 0; i < L; i++) {
            nd.assign(S, 0);
            for (int u = 0; u < S; u++) if (!bad[u] && dp[u].v)
                for (int c = 0; c < alpha; c++) {
                    int v = nxt[u][c];
                    if (!bad[v]) nd[v] += dp[u];
                }
            dp = nd;
        }
        Mint r = 0;
        for (int u = 0; u < S; u++) if (!bad[u]) r += dp[u];
        return r;
    }
    // #occurrences of every pattern in a text: walk the text, count visits per state, then push
    // the counts DOWN the suffix-link tree (reverse BFS order).
    vector<ll> occurrences(const string& s) {
        vector<ll> cnt(nxt.size(), 0);
        int u = 0;
        for (char c : s) u = nxt[u][c - 'a'], cnt[u]++;
        for (int i = order.size() - 1; i >= 0; i--) cnt[link[order[i]]] += cnt[order[i]];
        return cnt;                                    // cnt[state of pattern p] = #occurrences
    }
};
// SHORTEST / LEXICOGRAPHICALLY SMALLEST string avoiding all patterns: BFS over (state) with
// the alphabet in order. If a cycle among good states is reachable, arbitrarily long strings
// exist - detect it with a cycle check on the good-state subgraph.
