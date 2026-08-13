// Needs: 07 - Strings/04 - Suffix Structures/02 - SuffixAutomaton.cpp
// NUMBER OF DISTINCT SUBSTRINGS OF s[l..r], for q queries, OFFLINE in O((n + q) log n).
// For the whole string this is one line off a suffix automaton or a suffix array; for an
// arbitrary WINDOW it is a genuinely hard problem, and this is the standard solution
// (HackerRank "How Many Substrings", CF gym 102129I).
// INDEX CONVENTION: 0-based inclusive queries {l, r}; internally everything is 1-based over
// positions 1..n, and LCT node id = SAM state id + 1 (id 0 is the null node).
// THE INVARIANT: sweep r from left to right and keep, in a Fenwick with range add and range sum,
//     val[p] = number of distinct substrings whose LAST occurrence in s[1..r] STARTS at p.
// Then the answer for [l, r] is sum of val[l..r] - every distinct substring of the window is
// counted at the start position of its last occurrence, which is inside the window exactly when
// the substring occurs in the window at all.
// MAINTAINING IT is the trick: appending s[r] creates r new occurrences (the suffixes of the
// prefix), so add +1 to every position 1..r, and then REMOVE the previous last-occurrence entries
// of exactly those substrings. Those substrings are the ones on the suffix-link path from the
// state of prefix r to the root - and their old "last occurrence" positions are constant along
// each PREFERRED PATH of a link-cut tree. So maintain the suffix-link tree in an LCT where
// access(state, r) PAINTS the path with the colour r: every preferred-child change reports one
// maximal group (a range of lengths that shared an old colour), which is one range update.
// That "access as painting" pattern is reusable: any "assign a colour to a root path, and pay
// only for the O(log n) amortised colour groups you destroy" problem is this code.
struct DistinctSubstringsInRange {
    int n = 0, sz = 0;
    vector<int> par, lz, col, len;
    vector<array<int, 2>> ch;
    vector<pii> mods;                                          // {segment top len, old colour}
    vector<ll> b1, b2;                                         // range-add range-sum Fenwick
    void bAdd(vector<ll>& b, int i, ll v) { for (; i <= n; i += i & -i) b[i] += v; }
    ll bSum(const vector<ll>& b, int i) const {
        ll r = 0;
        for (; i > 0; i -= i & -i) r += b[i];
        return r;
    }
    void rangeAdd(int l, int r, ll v) {                        // add v to positions [l, r]
        if (l > r) return;
        bAdd(b1, l, v), bAdd(b2, l, v * (l - 1));
        if (r + 1 <= n) bAdd(b1, r + 1, -v), bAdd(b2, r + 1, -v * r);
    }
    ll pref(int i) const { return bSum(b1, i) * i - bSum(b2, i); }
    ll rangeSum(int l, int r) const { return l > r ? 0 : pref(r) - pref(l - 1); }
    void mark(int x, int v) { lz[x] = col[x] = v; }
    void push(int x) {
        if (!lz[x]) return;
        if (ch[x][0]) mark(ch[x][0], lz[x]);
        if (ch[x][1]) mark(ch[x][1], lz[x]);
        lz[x] = 0;
    }
    bool isRoot(int x) { return ch[par[x]][0] != x && ch[par[x]][1] != x; }
    void rotate(int x) {
        int y = par[x], z = par[y], k = ch[y][1] == x, w = ch[x][!k];
        if (!isRoot(y)) ch[z][ch[z][1] == y] = x;
        ch[x][!k] = y, ch[y][k] = w;
        if (w) par[w] = y;
        par[y] = x, par[x] = z;
    }
    void splay(int x) {
        static vector<int> st;
        st.clear(), st.push_back(x);
        for (int i = x; !isRoot(i); i = par[i]) st.push_back(par[i]);
        while (!st.empty()) push(st.back()), st.pop_back();
        while (!isRoot(x)) {
            int y = par[x], z = par[y];
            if (!isRoot(y)) rotate((ch[y][1] == x) == (ch[z][1] == y) ? y : x);
            rotate(x);
        }
    }
    void access(int x, int v) {                                // paint root..x with colour v
        mods.clear();
        for (int z = 0; x; z = x, x = par[x]) {
            splay(x);
            mods.push_back({len[x - 1], col[x]});              // this preferred group dies now
            ch[x][1] = z;
            mark(x, v);
        }
    }
    vector<ll> solve(const string& s, const vector<pii>& queries) {
        n = s.size();
        SAM sam(n);
        vector<int> pos(n + 1, 1);
        for (int i = 0; i < n; i++) sam.extend(s[i]), pos[i + 1] = sam.last + 1;
        sz = sam.st.size();
        par.assign(sz + 1, 0), lz.assign(sz + 1, 0), col.assign(sz + 1, 0);
        ch.assign(sz + 1, {0, 0}), len.assign(sz, 0);
        for (int i = 0; i < sz; i++) len[i] = sam.st[i].len;
        for (int i = 1; i <= sz; i++) par[i] = sam.st[i - 1].link + 1;
        b1.assign(n + 2, 0), b2.assign(n + 2, 0);
        vector<vector<pii>> byR(n + 1);                        // {l, query id}, 1-based r
        for (int i = 0; i < (int)queries.size(); i++)
            byR[queries[i].second + 1].push_back({queries[i].first + 1, i});
        vector<ll> ans(queries.size(), 0);
        for (int r = 1; r <= n; r++) {
            rangeAdd(1, r, 1);                                 // every suffix of s[1..r] is new
            access(pos[r], r);
            int prevLen = 0;
            for (int j = (int)mods.size() - 1; j >= 1; j--) {  // root-most group first
                auto [topLen, oldCol] = mods[j];
                if (!topLen) continue;                         // the root state (empty string)
                if (oldCol) rangeAdd(oldCol - topLen + 1, oldCol - prevLen, -1);
                prevLen = topLen;
            }
            for (auto [l, id] : byR[r]) ans[id] = rangeSum(l, r);
        }
        return ans;
    }
};
/* NOTES
 WHY THE LENGTHS LINE UP: a preferred group is a set of suffix-link-tree nodes whose strings all
   had the same last-occurrence end position `oldCol`, and the group covers exactly the lengths
   (prevLen, topLen]. Those substrings therefore started at positions oldCol - topLen + 1 ..
   oldCol - prevLen, which is the single range you cancel. Get the direction of prevLen wrong and
   the answers drift slowly - test against a brute force on n <= 12 before using it.
 THE SAM IS BUILT FIRST, in full, and the LCT is built over the FINAL suffix-link tree; no links
   or cuts ever happen, only access(). Clones created later do not break the sweep because the
   path from the state of prefix r to the root always lists exactly the suffixes of that prefix.
 COMPLEXITY: O(n log n) amortised for the accesses (the number of preferred-child changes is
   O(n log n)), each producing one O(log n) Fenwick range update.
 ONLINE variant: replace the Fenwick with a PERSISTENT one, keeping a root per r; queries then
   need no sorting. Memory becomes O(n log^2 n).
 RELATED, MUCH EASIER: distinct substrings of the WHOLE string (sum of len - len[link] over SAM
   states), and distinct VALUES in a range (the classic BIT-over-last-occurrence sweep, which is
   this same idea with one position per value instead of a whole path). */
