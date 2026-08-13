// EDIT DISTANCE (Levenshtein): fewest single-character insertions, deletions and substitutions
// turning a into b. dp[i][j] = distance between the prefixes a[0..i-1] and b[0..j-1], so the
// table is (n+1) x (m+1) and the answer is dp[n][m]. O(n m) time, O(min(n,m)) memory here.
// WHEN: fuzzy matching, "minimum operations to make equal", DNA-style alignment. If you only need
// insert+delete (no substitution) the answer is n + m - 2*LCS(a, b) - use the LCS DP instead.
int editDistance(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<int> prv(m + 1), cur(m + 1);
    iota(prv.begin(), prv.end(), 0);
    for (int i = 1; i <= n; i++) {
        cur[0] = i;
        for (int j = 1; j <= m; j++)
            cur[j] = min({prv[j] + 1, cur[j - 1] + 1, prv[j - 1] + (a[i - 1] != b[j - 1])});
        swap(prv, cur);
    }
    return prv[m];
}
// --- BANDED: when you only care whether the distance is <= k. Cells off the diagonal by more
// than k can never be on an optimal path (each step off the diagonal costs >= 1), so evaluate
// only |i - j| <= k. O(n k). Returns k + 1 as "more than k".
int editDistanceAtMost(const string& a, const string& b, int k) {
    int n = a.size(), m = b.size(), BIG = k + 1;
    if (abs(n - m) > k) return BIG;
    vector<int> prv(m + 2, BIG), cur(m + 2, BIG);
    for (int j = 0; j <= min(m, k); j++) prv[j] = j;               // row 0
    for (int i = 1; i <= n; i++) {
        int lo = max(0, i - k), hi = min(m, i + k);
        cur[lo] = lo ? min({prv[lo] + 1, prv[lo - 1] + (a[i - 1] != b[lo - 1]), BIG}) : min(i, BIG);
        for (int j = lo + 1; j <= hi; j++)                         // cur[lo-1] is OUTSIDE the band
            cur[j] = min({prv[j] + 1, cur[j - 1] + 1, prv[j - 1] + (a[i - 1] != b[j - 1]), BIG});
        if (hi + 1 <= m) cur[hi + 1] = BIG;                        // the cell the next row reads
        swap(prv, cur);
    }
    return min(prv[m], BIG);
}
// --- DAMERAU-LEVENSHTEIN, RESTRICTED (optimal string alignment): also allows swapping two
// ADJACENT characters at cost 1, but forbids editing a substring twice. This is the version
// contests mean; the unrestricted one needs the "last row/column per character" table.
int damerau(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    vector<vector<int>> dp(n + 1, vector<int>(m + 1));
    for (int i = 0; i <= n; i++) dp[i][0] = i;
    for (int j = 0; j <= m; j++) dp[0][j] = j;
    for (int i = 1; i <= n; i++)
        for (int j = 1; j <= m; j++) {
            dp[i][j] = min({dp[i - 1][j] + 1, dp[i][j - 1] + 1,
                            dp[i - 1][j - 1] + (a[i - 1] != b[j - 1])});
            if (i > 1 && j > 1 && a[i - 1] == b[j - 2] && a[i - 2] == b[j - 1])
                dp[i][j] = min(dp[i][j], dp[i - 2][j - 2] + 1);
        }
    return dp[n][m];
}
// --- MYERS' BIT-VECTOR ALGORITHM: exact Levenshtein in O(n * m / 64), the escape hatch when the
// O(n m) table is 1e10. It stores the VERTICAL differences dp[i][j] - dp[i-1][j] (always in
// {-1,0,+1}) as two bitmasks Pv/Mv over the rows, and advances a whole column with 12 word ops.
// Here a is the "pattern" and must satisfy |a| <= 64; loop over blocks of 64 rows for longer a.
int myersEditDistance(const string& a, const string& b) {
    int m = a.size();
    if (!m) return b.size();
    unsigned long long peq[256] = {};
    for (int i = 0; i < m; i++) peq[(unsigned char)a[i]] |= 1ULL << i;
    unsigned long long Pv = ~0ULL, Mv = 0, high = 1ULL << (m - 1);
    int score = m;
    for (char ch : b) {
        unsigned long long Eq = peq[(unsigned char)ch];
        unsigned long long Xv = Eq | Mv;
        unsigned long long Xh = (((Eq & Pv) + Pv) ^ Pv) | Eq;
        unsigned long long Ph = Mv | ~(Xh | Pv), Mh = Pv & Xh;
        if (Ph & high) score++;
        if (Mh & high) score--;
        Ph = (Ph << 1) | 1;                                    // dp[0][j] = j  (GLOBAL alignment)
        Mh <<= 1;
        Pv = Mh | ~(Xv | Ph);
        Mv = Ph & Xv;
    }
    return score;
}
/* VARIANTS AND TRAPS
 SHIFT IN 0 INSTEAD OF 1 (`Ph <<= 1`) and Myers computes, for every j, the distance from a to the
   BEST-MATCHING substring of b ending at j - i.e. approximate pattern matching, k-differences.
 RECONSTRUCTING the alignment needs the table; with O(n) memory use Hirschberg
   (08 - DP/04 - Classics), which is the same divide & conquer for any such 2-string DP.
 WEIGHTED costs (ins != del != sub) work in the plain DP but break Myers and the LCS identity.
 LONGEST COMMON SUBSTRING (contiguous) is NOT this DP: use hashing + binary search, a suffix
   automaton, or dp[i][j] = dp[i-1][j-1] + 1 with the answer read as a max over all cells.
 EDIT DISTANCE TO A REGULAR LANGUAGE / to any string of a trie: same DP with the automaton state
   as the second dimension, O(n * states).
 LOWER BOUND: |n - m| <= dist <= max(n, m), and dist >= (number of characters whose counts
   differ) / 2 style bounds - cheap filters before running the real DP on many pairs. */
