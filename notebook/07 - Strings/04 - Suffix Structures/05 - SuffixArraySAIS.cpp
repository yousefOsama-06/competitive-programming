// SUFFIX ARRAY IN O(n) - SA-IS. Use it instead of 01 - SuffixArray.cpp (O(n log n) doubling)
// when n is 1e6 or more, when the input is a vector<int> with a huge alphabet (doubling needs a
// radix sort per round), or when the doubling version is the bottleneck of a heavy solution.
// It is ~4x faster than doubling at n = 1e6 and needs no sorting of pairs at all.
// INDEX CONVENTION - DIFFERENT FROM 01 - SuffixArray.cpp, and mixing them up is a wrong answer:
//   sa has EXACTLY n entries (no empty suffix); sa[i] = start of the i-th smallest suffix.
//   lcp has n entries with lcp[0] = 0 and lcp[i] = LCP(sa[i-1], sa[i]).
// HOW IT WORKS (enough to debug it): classify every position as S-type (its suffix is smaller
// than the next one) or L-type. The LMS positions (an S right after an L) split the string into
// blocks; sort those blocks by recursing on a shorter string, then INDUCE the whole order from
// them with two linear scans. The recursion halves the length, so the total is O(n).
vector<int> saIs(const vector<int>& s, int up) {               // values in [0, up], up >= 0
    int n = s.size();
    if (n == 0) return {};
    if (n == 1) return {0};
    if (n == 2) return s[0] < s[1] ? vector<int>{0, 1} : vector<int>{1, 0};
    vector<int> sa(n);
    vector<char> ls(n, 0);                                     // 1 = S-type
    for (int i = n - 2; i >= 0; i--) ls[i] = s[i] == s[i + 1] ? ls[i + 1] : s[i] < s[i + 1];
    vector<int> sumL(up + 2, 0), sumS(up + 2, 0);
    for (int i = 0; i < n; i++) ls[i] ? sumL[s[i] + 1]++ : sumS[s[i]]++;
    for (int i = 0; i <= up; i++) { sumS[i] += sumL[i]; if (i < up) sumL[i + 1] += sumS[i]; }
    auto induce = [&](const vector<int>& lms) {
        fill(sa.begin(), sa.end(), -1);
        vector<int> buf = sumS;
        for (int d : lms) if (d != n) sa[buf[s[d]]++] = d;
        buf = sumL;
        sa[buf[s[n - 1]]++] = n - 1;
        for (int i = 0; i < n; i++) {
            int v = sa[i];
            if (v >= 1 && !ls[v - 1]) sa[buf[s[v - 1]]++] = v - 1;
        }
        buf = sumL;
        for (int i = n - 1; i >= 0; i--) {
            int v = sa[i];
            if (v >= 1 && ls[v - 1]) sa[--buf[s[v - 1] + 1]] = v - 1;
        }
    };
    vector<int> lmsMap(n + 1, -1), lms;
    int m = 0;
    for (int i = 1; i < n; i++) if (!ls[i - 1] && ls[i]) lmsMap[i] = m++, lms.push_back(i);
    induce(lms);
    if (m) {
        vector<int> sorted;
        for (int v : sa) if (lmsMap[v] != -1) sorted.push_back(v);
        vector<int> rec(m);
        int upRec = 0;
        rec[lmsMap[sorted[0]]] = 0;
        for (int i = 1; i < m; i++) {
            int l = sorted[i - 1], r = sorted[i];
            int el = lmsMap[l] + 1 < m ? lms[lmsMap[l] + 1] : n;
            int er = lmsMap[r] + 1 < m ? lms[lmsMap[r] + 1] : n;
            bool same = el - l == er - r;
            if (same) {
                while (l < el && s[l] == s[r]) l++, r++;
                if (l == n || s[l] != s[r]) same = false;
            }
            if (!same) upRec++;
            rec[lmsMap[sorted[i]]] = upRec;
        }
        vector<int> recSa = saIs(rec, upRec);
        for (int i = 0; i < m; i++) sorted[i] = lms[recSa[i]];
        induce(sorted);
    }
    return sa;
}
vector<int> suffixArray(const string& s) {                     // n entries, no empty suffix
    vector<int> a(s.begin(), s.end());
    for (int& x : a) x = (unsigned char)x;
    return saIs(a, 255);
}
// KASAI: lcp[i] = LCP(suffix sa[i-1], suffix sa[i]), lcp[0] = 0. O(n).
vector<int> kasai(const string& s, const vector<int>& sa) {
    int n = s.size(), k = 0;
    vector<int> rnk(n), lcp(n, 0);
    for (int i = 0; i < n; i++) rnk[sa[i]] = i;
    for (int i = 0; i < n; i++) {
        if (!rnk[i]) { k = 0; continue; }
        int j = sa[rnk[i] - 1];
        while (i + k < n && j + k < n && s[i + k] == s[j + k]) k++;
        lcp[rnk[i]] = k;
        if (k) k--;
    }
    return lcp;
}
/* NOTES
 GENERAL ALPHABETS: call saIs directly on a vector<int> after coordinate-compressing the values
   to [0, up]; every value must be >= 0 and the LAST element does NOT have to be a sentinel -
   this implementation handles the end of string internally.
 CONCATENATING MANY STRINGS (generalised suffix array): join with distinct separators SMALLER
   than every real character, or - better with SA-IS - compress to ints and use 0, 1, 2, ... as
   separators, shifting real characters up.
 EVERYTHING ELSE (distinct substrings, longest repeated, pattern range, LCE sparse table) is in
   03 - SuffixArrayApplications.cpp; shift its formulas by one because that file's sa[0] is the
   empty suffix and this one has no empty suffix.
 MEMORY is ~3n ints at the top level plus the recursion, which sums to < 2x that. Doubling needs
   5 arrays of n and a radix sort - SA-IS also wins on memory.
 DC3 / SKEW is the other linear construction; it is shorter to write but slower in practice and
   needs 3n extra memory, so SA-IS is the one to carry. */
