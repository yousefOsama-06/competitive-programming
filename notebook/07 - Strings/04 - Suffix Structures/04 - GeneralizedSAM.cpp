// GENERALIZED SUFFIX AUTOMATON - one automaton accepting every substring of a SET of strings.
// Same size bound O(total length). The only change vs a plain SAM: when extending, the transition
// may ALREADY exist (a previous string walked here), so handle that case instead of blindly adding.
// Feeding strings one after another into a plain SAM is WRONG - it creates junk states.
struct GenSAM {
    struct S { int len = 0, link = -1; array<int, 26> nxt; S() { nxt.fill(-1); } };
    vector<S> st;
    GenSAM() { st.push_back(S()); }
    int clone(int q, int len) { int c = st.size(); st.push_back(st[q]), st[c].len = len; return c; }
    int extend(int last, int c) {                              // returns the new "last"
        if (st[last].nxt[c] != -1) {                           // the edge is already there
            int q = st[last].nxt[c];
            if (st[q].len == st[last].len + 1) return q;
            int cl = clone(q, st[last].len + 1);
            for (int p = last; p != -1 && st[p].nxt[c] == q; p = st[p].link) st[p].nxt[c] = cl;
            st[q].link = cl;
            return cl;
        }
        int cur = st.size();
        st.push_back(S()), st[cur].len = st[last].len + 1;
        int p = last;
        while (p != -1 && st[p].nxt[c] == -1) st[p].nxt[c] = cur, p = st[p].link;
        if (p == -1) st[cur].link = 0;
        else {
            int q = st[p].nxt[c];
            if (st[p].len + 1 == st[q].len) st[cur].link = q;
            else {
                int cl = clone(q, st[p].len + 1);
                for (; p != -1 && st[p].nxt[c] == q; p = st[p].link) st[p].nxt[c] = cl;
                st[q].link = st[cur].link = cl;
            }
        }
        return cur;
    }
    void add(const string& s) { int last = 0; for (char ch : s) last = extend(last, ch - 'a'); }
    vector<int> byLen() const {                                // states in increasing len
        int n = st.size(), mx = 0;
        for (auto& x : st) mx = max(mx, x.len);
        vector<int> c(mx + 2, 0), ord(n);
        for (auto& x : st) c[x.len]++;
        for (int i = 1; i <= mx; i++) c[i] += c[i - 1];
        for (int v = n - 1; v >= 0; v--) ord[--c[st[v].len]] = v;
        return ord;
    }
    // HOW MANY OF THE k STRINGS CONTAIN each substring: mark the states each string walks through,
    // then push the marks up the link tree. k <= 64 here; for larger k use small-to-large sets, or
    // a bitset<K>, or process the strings in batches of 64 and add up the popcounts.
    vector<int> countStringsContaining(const vector<string>& v) {   // requires v.size() <= 64
        vector<unsigned long long> mask(st.size(), 0);
        for (int i = 0; i < (int)v.size(); i++)
            for (int p = 0, j = 0; j < (int)v[i].size(); j++)
                p = st[p].nxt[v[i][j] - 'a'], mask[p] |= 1ULL << i;
        auto ord = byLen();
        for (int i = ord.size() - 1; i > 0; i--)
            mask[st[ord[i]].link] |= mask[ord[i]];
        vector<int> r(st.size());
        for (size_t i = 0; i < st.size(); i++) r[i] = __builtin_popcountll(mask[i]);
        return r;                                              // r[v] for every state v
    }
};
// LONGEST COMMON SUBSTRING OF k STRINGS: build the generalized SAM, take the deepest state with
// count == k; its longest string is len[v] (walk suffix links for the actual string).
// DISTINCT SUBSTRINGS OF THE UNION = sum over v != 0 of (len[v] - len[link[v]]), same formula.
// SUBSTRING PRESENT IN EXACTLY ONE STRING / IN AT LEAST k: filter by the same counts.
// ALTERNATIVE when the strings are few: concatenate with distinct separators and use one SUFFIX
// ARRAY + a sliding window over lcp. Shorter to write, O(n log n), but no online extension.
