// DYNAMIC (ONLINE) AHO-CORASICK - the pattern set grows while queries arrive. A plain
// Aho-Corasick must be rebuilt from scratch after every insertion, which is O(total length) per
// insert; here the patterns are kept in O(log P) STATIC automata of sizes 1, 2, 4, 8, ... and
// inserting merges the small ones the way binary counter increments carry.
// AMORTISED O(|p| log P) per insert, O(|s| log P) per query (log P <= ~20 automata to feed).
// WHEN: "add a word, then count occurrences of all words in this text", online dictionary
// problems (CF 710F), and any offline-impossible interleaving of insert and query.
// DELETION: keep a SECOND identical structure of deleted patterns and subtract its answer.
// INDEX CONVENTION: node 0 is the root; cnt[v] is already the SUM over the whole suffix-link
// chain, so a query adds cnt[cur] once per text position and never walks exit links.
struct StaticAho {                                             // rebuilt wholesale, never patched
    vector<array<int, 26>> nxt;
    vector<int> link, cnt;
    StaticAho() { clear(); }
    void clear() { nxt.assign(1, {}), nxt[0].fill(0), link.assign(1, 0), cnt.assign(1, 0); }
    void insert(const string& p) {
        int u = 0;
        for (char ch : p) {
            int c = ch - 'a';
            if (!nxt[u][c]) {
                nxt[u][c] = nxt.size();
                nxt.push_back({}), nxt.back().fill(0), link.push_back(0), cnt.push_back(0);
            }
            u = nxt[u][c];
        }
        cnt[u]++;
    }
    void build() {                                             // goto-automaton + cumulative cnt
        queue<int> q;
        for (int c = 0; c < 26; c++) if (nxt[0][c]) q.push(nxt[0][c]);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            cnt[u] += cnt[link[u]];
            for (int c = 0; c < 26; c++) {
                int v = nxt[u][c];
                if (v) link[v] = nxt[link[u]][c], q.push(v);
                else nxt[u][c] = nxt[link[u]][c];
            }
        }
    }
    ll count(const string& s) const {                          // total occurrences of all patterns
        ll r = 0;
        int u = 0;
        for (char ch : s) u = nxt[u][ch - 'a'], r += cnt[u];
        return r;
    }
};
struct DynamicAho {
    static const int B = 20;                                   // supports 2^20 patterns
    vector<string> words[B];
    StaticAho aho[B];
    void insert(const string& p) {
        int lvl = 0;
        while (lvl < B && !words[lvl].empty()) lvl++;           // the carry of a binary counter
        words[lvl].push_back(p);
        for (int i = 0; i < lvl; i++) {
            for (auto& w : words[i]) words[lvl].push_back(w);
            words[i].clear(), aho[i].clear();
        }
        aho[lvl].clear();
        for (auto& w : words[lvl]) aho[lvl].insert(w);
        aho[lvl].build();
    }
    ll count(const string& s) {                                // occurrences of all patterns in s
        ll r = 0;
        for (int i = 0; i < B; i++) if (!words[i].empty()) r += aho[i].count(s);
        return r;
    }
};
/* NOTES AND VARIANTS
 WHY THE COST IS AMORTISED: a pattern is copied into a bigger automaton at most log P times, so
   the total rebuild work is O(sum |p| * log P) - exactly the same argument as binary lifting on
   a knapsack, or as the "logarithmic method" for making any static structure insert-only.
 THE SAME WRAPPER makes ANY static structure dynamic (suffix arrays, convex hulls, kd-trees):
   keep buckets of size 2^i and merge on carry. That is the reusable idea here.
 PER-PATTERN counts instead of a total: store an id per terminal and walk the exit-link chain, or
   put the query positions on the fail tree of each bucket - the total stays O(matches log P).
 MEMORY: 26 ints per node in every bucket. With big alphabets use a hash map per node or the
   sorted-children trick, otherwise 20 rebuilt copies will not fit.
 IF ALL PATTERNS ARE KNOWN OFFLINE, do not use this - build one static automaton. */
