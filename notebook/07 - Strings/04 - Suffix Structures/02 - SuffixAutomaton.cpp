// SUFFIX AUTOMATON - O(n) states/transitions, accepts exactly the substrings of s.
// st[v].len   = longest string in v's endpos class      link = suffix link (v's parent)
// cnt[v]      = |endpos(v)| = #occurrences of every string in v
// distinct substrings = sum over v != root of (len[v] - len[link[v]])
struct SAM {
    struct S { int len = 0, link = -1; array<int, 26> nxt; S() { nxt.fill(-1); } };
    vector<S> st;
    vector<ll> cnt;                                  // occurrence counts (after countOcc)
    vector<int> firstPos;                            // end position of the first occurrence
    int last = 0;
    bool counted = false;                            // countOcc() is idempotent

    SAM(int reserve = 0) { st.reserve(2 * reserve), st.push_back(S()), cnt.push_back(0), firstPos.push_back(0); }

    void extend(char ch) {
        int c = ch - 'a', cur = st.size();
        st.push_back(S()), cnt.push_back(1), firstPos.push_back(0);
        st[cur].len = st[last].len + 1, firstPos[cur] = st[cur].len - 1;
        int p = last;
        while (p != -1 && st[p].nxt[c] == -1) st[p].nxt[c] = cur, p = st[p].link;
        if (p == -1) st[cur].link = 0;
        else {
            int q = st[p].nxt[c];
            if (st[p].len + 1 == st[q].len) st[cur].link = q;
            else {
                int cl = st.size();
                st.push_back(st[q]), cnt.push_back(0), firstPos.push_back(firstPos[q]);
                st[cl].len = st[p].len + 1;
                while (p != -1 && st[p].nxt[c] == q) st[p].nxt[c] = cl, p = st[p].link;
                st[q].link = st[cur].link = cl;
            }
        }
        last = cur;
    }
    void build(const string& s) { for (char c : s) extend(c); }

    vector<int> byLen() const {                      // states sorted by len (counting sort)
        int n = st.size(), mx = st[0].len;
        for (auto& x : st) mx = max(mx, x.len);
        vector<int> c(mx + 2, 0), ord(n);
        for (auto& x : st) c[x.len]++;
        for (int i = 1; i <= mx; i++) c[i] += c[i - 1];
        for (int v = n - 1; v >= 0; v--) ord[--c[st[v].len]] = v;
        return ord;                                  // increasing len; reverse for link-tree order
    }
    void countOcc() {                                // push counts down the suffix-link tree
        if (counted) return;                         // NOT idempotent without this guard
        counted = true;
        auto ord = byLen();
        for (int i = (int)ord.size() - 1; i > 0; i--) {
            int v = ord[i];
            if (st[v].link >= 0) cnt[st[v].link] += cnt[v];
        }
        cnt[0] = 0;
    }
    ll distinctSubstrings() const {                  // number of DISTINCT non-empty substrings
        ll r = 0;
        for (int v = 1; v < (int)st.size(); v++) r += st[v].len - st[st[v].link].len;
        return r;
    }
    ll totalOccurrences() {                          // sum over distinct substrings of #occurrences
        countOcc();
        ll r = 0;
        for (int v = 1; v < (int)st.size(); v++) r += cnt[v] * (st[v].len - st[st[v].link].len);
        return r;
    }
    int countOf(const string& p) {                   // #occurrences of p in s (call countOcc first)
        int v = 0;
        for (char ch : p) { v = st[v].nxt[ch - 'a']; if (v < 0) return 0; }
        return cnt[v];
    }
    // K-th SMALLEST distinct substring (k is 1-indexed). Needs `paths` = #substrings from v.
    vector<ll> paths;
    void buildPaths() {
        paths.assign(st.size(), 1);                  // 1 counts the empty continuation
        auto ord = byLen();
        for (int i = (int)ord.size() - 1; i >= 0; i--) {
            int v = ord[i];
            paths[v] = 1;
            for (int c = 0; c < 26; c++) if (st[v].nxt[c] >= 0) paths[v] += paths[st[v].nxt[c]];
        }
    }
    string kthSubstring(ll k) {                      // "" if fewer than k distinct substrings
        string r;
        int v = 0;
        while (k > 0) {
            int c = 0;
            for (; c < 26; c++) {
                int u = st[v].nxt[c];
                if (u < 0) continue;
                if (k <= paths[u]) { r += char('a' + c), v = u, k--; break; }
                k -= paths[u];
            }
            if (c == 26) break;
        }
        return r;
    }
};
// LONGEST COMMON SUBSTRING of s and t: build the SAM of s, then walk t keeping the current
// length; on a missing transition follow suffix links and clamp the length to st[v].len.
int longestCommonSubstring(SAM& A, const string& t) {
    int v = 0, l = 0, best = 0;
    for (char ch : t) {
        int c = ch - 'a';
        while (v && A.st[v].nxt[c] < 0) v = A.st[v].link, l = A.st[v].len;
        if (A.st[v].nxt[c] >= 0) v = A.st[v].nxt[c], l++;
        best = max(best, l);
    }
    return best;
}
// GENERALISED SAM (several strings): reset `last = 0` before each string and, in extend(),
// if a transition already exists reuse/clone it instead of creating a new state.
