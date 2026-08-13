// Needs: 07 - Strings/04 - Suffix Structures/05 - SuffixArraySAIS.cpp   (saIs, kasai)
// PARAMETERIZED / ISOMORPHIC SUFFIX ARRAY. Two strings are ISOMORPHIC (p-match) when some
// BIJECTION of the alphabet turns one into the other: "abab" ~ "cdcd" ~ "baba", but not "aabb".
// WHEN: "how many substrings of s are isomorphic to s[l..r]", detecting renamed patterns,
// register-allocation style matching. Plain hashing cannot do this - the equality is not on
// characters but on the PATTERN OF REPEATS.
// THE ENCODING that makes it work: a[i] = (i - previous position of s[i]) + 1, or 1 when s[i] is
// the first occurrence of its character. Two substrings are isomorphic iff their encodings match
// AFTER CAPPING: at offset L inside a substring the value is a[x] if a[x] <= L + 1, else 1 (a
// reference that points outside the window becomes "first occurrence"). The cap is why this is
// not just an ordinary suffix array on a[].
// INDEX CONVENTION: sa has n entries, no empty suffix; sa[i] = start of the i-th smallest suffix
// UNDER ISOMORPHISM, and lcp[i] = longest common ISOMORPHIC prefix of sa[i] and sa[i+1] (note:
// indexed by the LEFT of the pair, unlike 01 - SuffixArray.cpp - one entry per adjacent pair).
// COMPLEXITY: O(n log n * A) worst case (A = alphabet), because one comparison may restart at
// every distinct character; in practice each comparison is a couple of jumps.
struct IsoSuffixArray {
    int n, LG;
    vector<int> a, sa, rnk, lcp, hsa, hrnk, hlcp, lg;
    vector<vector<int>> sp;                                    // sparse table over hlcp
    IsoSuffixArray(const string& s, int A = 26, char base = 'a') {
        n = s.size(), a.resize(n);
        vector<int> last(A, -1);
        for (int i = 0; i < n; i++)
            a[i] = last[s[i] - base] < 0 ? 1 : i - last[s[i] - base] + 1, last[s[i] - base] = i;
        hsa = saIs(a, n + 1);                                  // helper: plain SA of the encoding
        hrnk.resize(n);
        for (int i = 0; i < n; i++) hrnk[hsa[i]] = i;
        hlcp.assign(n, 0);
        for (int i = 0, k = 0; i < n; i++) {                   // Kasai on the integer array
            if (!hrnk[i]) { k = 0; continue; }
            int j = hsa[hrnk[i] - 1];
            while (i + k < n && j + k < n && a[i + k] == a[j + k]) k++;
            hlcp[hrnk[i]] = k;
            if (k) k--;
        }
        lg.assign(n + 1, 0);
        for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;
        LG = lg[n] + 1;
        sp.assign(LG, vector<int>(n, 0));
        sp[0] = hlcp;
        for (int k = 1; k < LG; k++)
            for (int i = 0; i + (1 << k) <= n; i++)
                sp[k][i] = min(sp[k - 1][i], sp[k - 1][i + (1 << (k - 1))]);
        sa.resize(n), iota(sa.begin(), sa.end(), 0);
        sort(sa.begin(), sa.end(), [&](int i, int j) { return cmp(i, j); });
        rnk.resize(n);
        for (int i = 0; i < n; i++) rnk[sa[i]] = i;
        lcp.assign(max(0, n - 1), 0);
        for (int i = 0; i + 1 < n; i++) lcp[i] = lce(sa[i], sa[i + 1]);
    }
    int rawLce(int i, int j) {                                 // lcp of a[i..] and a[j..]
        if (i == j) return n - i;
        int l = hrnk[i], r = hrnk[j];
        if (l > r) swap(l, r);
        int k = lg[r - l];
        return min(sp[k][l + 1], sp[k][r - (1 << k) + 1]);
    }
    int lce(int i, int j) {                                    // longest common ISOMORPHIC prefix
        for (int L = 0;;) {
            int x = i + L, y = j + L;
            if (x >= n || y >= n) return L;
            int vx = a[x] > L + 1 ? 1 : a[x], vy = a[y] > L + 1 ? 1 : a[y];
            if (vx != vy) return L;
            L += max(1, rawLce(x, y));                         // raw equality implies iso equality
        }
    }
    bool cmp(int i, int j) {
        int L = lce(i, j);
        if (i + L >= n) return true;
        if (j + L >= n) return false;
        int vx = a[i + L] > L + 1 ? 1 : a[i + L], vy = a[j + L] > L + 1 ? 1 : a[j + L];
        return vx < vy;
    }
    // Number of substrings isomorphic to s[p .. p+len-1]: the SA block of suffixes whose
    // isomorphic prefix of that length matches. O(log^2 n) with the lcp array as a min-sparse
    // table; the linear scan below is fine when len is large or queries are few.
    pii occurrences(int p, int len) {
        int lo = rnk[p], hi = rnk[p];
        while (lo > 0 && lcp[lo - 1] >= len) lo--;
        while (hi + 1 < n && lcp[hi] >= len) hi++;
        return {lo, hi};                                       // count = hi - lo + 1
    }
};
/* NOTES
 THE OTHER ENCODINGS you may meet: "previous occurrence index" (0 if none) is the same idea;
   "order of first appearance" (abcab -> 01201) is NOT enough for substrings, because a substring
   restarts the numbering - that is exactly what the cap fixes here.
 HASHING ALTERNATIVE: hash the capped encoding with a segment tree over "distance to previous
   occurrence" and answer isomorphism of two substrings in O(log n) - shorter if you only need
   EQUALITY tests and not an order.
 STRUCTURAL / ORDER-ISOMORPHISM (the same RELATIVE ORDER of numbers, not equality pattern) is a
   different problem: encode each position by (rank among the previous window) and use a similar
   suffix structure, or hash the pair (previous smaller, previous larger) positions.
 THE COMPARATOR IS NOT TRANSITIVE-SAFE if the cap is wrong; test on "aabb" vs "bbaa" (isomorphic)
   and "aabb" vs "abab" (not) before trusting a modified version. */
