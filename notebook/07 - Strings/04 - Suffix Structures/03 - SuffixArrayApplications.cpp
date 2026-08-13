// WHAT YOU ACTUALLY EXTRACT FROM A SUFFIX ARRAY.
// Convention of 01 - SuffixArray.cpp: sa has n+1 entries (sa[0] = the empty suffix),
// and lcp[i] = LCP(sa[i], sa[i-1]).  Everything below assumes that.

// 1) NUMBER OF DISTINCT SUBSTRINGS = sum over i of (len of suffix i) - lcp with the previous.
ll distinctSubstrings(const SuffixArray& S) {
    ll n = S.S.size(), r = 0;
    for (ll i = 1; i <= n; i++) r += (n - S.sa[i]) - S.lcp[i];
    return r;
}
// 2) LONGEST REPEATED SUBSTRING = max lcp; the substring starts at sa[argmax].
pair<int, int> longestRepeated(const SuffixArray& S) {         // {length, start position}
    int n = S.S.size(), bi = 0, best = 0;
    for (int i = 1; i <= n; i++) if (S.lcp[i] > best) best = S.lcp[i], bi = S.sa[i];
    return {best, bi};
}
// 3) OCCURRENCES of a pattern = the contiguous SA block whose suffixes start with it.
//    Two binary searches, O(|P| log n).
pair<int, int> patternRange(const SuffixArray& S, const string& p) {   // [lo, hi], empty if lo>hi
    int n = S.S.size();
    auto cmp = [&](int idx, const string& q) { return S.S.compare(idx, q.size(), q) < 0; };
    int lo = 1, hi = n, L = n + 1, R = n;
    while (lo <= hi) { int m = (lo + hi) / 2; cmp(S.sa[m], p) ? lo = m + 1 : (L = m, hi = m - 1); }
    lo = L, hi = n, R = L - 1;                                 // R MUST be reset here:
                                                               // otherwise an absent pattern
                                                               // returns the range [L, n]
    while (lo <= hi) {
        int m = (lo + hi) / 2;
        S.S.compare(S.sa[m], p.size(), p) == 0 ? (R = m, lo = m + 1) : hi = m - 1;
    }
    return {L, R};                                             // count = R - L + 1
}
// 4) LONGEST COMMON SUBSTRING of two strings (named lcsViaSuffixArray to avoid clashing
//    with the SAM version in 02 - SuffixAutomaton.cpp): build SA of a + sep + b (sep smaller than every
//    real character), then take the max lcp over ADJACENT pairs coming from different sides.
int lcsViaSuffixArray(const string& a, const string& b) {
    string t = a + char(1) + b;
    SuffixArray S(t);
    int n = t.size(), best = 0, cut = a.size();
    auto side = [&](int p) { return p < cut ? 0 : (p == cut ? -1 : 1); };
    for (int i = 2; i <= n; i++) {
        int s1 = side(S.sa[i]), s2 = side(S.sa[i - 1]);
        if (s1 >= 0 && s2 >= 0 && s1 != s2) best = max(best, S.lcp[i]);
    }
    return best;
}
// 5) K-TH SMALLEST DISTINCT SUBSTRING: SA entry i contributes (n - sa[i]) - lcp[i] new
//    substrings, all sharing the prefix of length lcp[i]. Prefix-sum those counts and binary
//    search for k; inside the block the answer is a prefix of suffix sa[i].
// 6) LCS OF K STRINGS: concatenate with distinct separators, then slide a window over the SA
//    that contains at least one suffix of every string; the answer is the max over windows of
//    the minimum lcp inside (monotonic deque), O(n) after the SA.
// 7) COMPARE TWO SUBSTRINGS in O(1): let l = S.lce(i, j) (STRING positions - queryLcp takes
//    SUFFIX-ARRAY indices, do not mix them up); if l >= both
//    lengths they are equal, otherwise compare s[i+l] with s[j+l].
