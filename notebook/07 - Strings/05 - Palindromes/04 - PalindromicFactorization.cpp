// Needs: 07 - Strings/05 - Palindromes/02 - PalindromicTree.cpp
// COUNTING PALINDROMIC FACTORISATIONS - the number of ways to cut s into palindromic pieces,
// in O(n log n). The tree file already has min_partitions (the MINIMUM number of pieces); this is
// the same series-link machinery with (min, +1) replaced by (sum, *1), plus the even-length
// variant that CF 932G needs. Doing it naively is O(n^2): "for each i, for each palindromic
// suffix" - and a string like a^n has n palindromic suffixes at every position.
// WHY IT IS O(n log n): the palindromic suffixes of any prefix fall into O(log n) ARITHMETIC
// PROGRESSIONS by length (Fine and Wilf). diff[v] = len[v] - len[link[v]] is the common
// difference of v's progression and slink[v] jumps to the previous progression, so walking
// v = cur, slink[v], slink[slink[v]], ... visits O(log n) nodes. series[v] aggregates the whole
// progression of v in O(1) by reusing series[link[v]] from the PREVIOUS position - that reuse is
// the entire trick and is why series[] must be indexed by NODE and not by position.
// INDEX CONVENTION: dp is 1-indexed over prefixes, dp[i] = answer for s[0..i-1], dp[0] = 1.
ll countPalindromicFactorizations(const string& s, bool evenLengthPiecesOnly = false) {
    int n = s.size();
    PalindromeTree t;
    t.init(n);
    vector<ll> dp(n + 1, 0), series(n + 2, 0);
    dp[0] = 1;
    for (int i = 0; i < n; i++) {
        t.add(s, i);
        for (int v = t.cur; t.len[v] > 0; v = t.slink[v]) {
            series[v] = dp[i + 1 - (t.len[t.slink[v]] + t.diff[v])];
            int sl = t.suflink[v];
            if (t.diff[v] == t.diff[sl]) series[v] = (series[v] + series[sl]) % MOD;
            dp[i + 1] = (dp[i + 1] + series[v]) % MOD;
        }
        if (evenLengthPiecesOnly && (i + 1) % 2) dp[i + 1] = 0;   // an odd prefix cannot be filled
    }
    return dp[n];
}
/* THE VARIANTS THIS UNLOCKS - all the same loop, only the semiring changes
 MINIMUM number of pieces: (min, +1) - already in 02 - PalindromicTree.cpp as min_partitions.
 NUMBER of factorisations: (sum, *1) - the code above.
 FACTORISATIONS INTO AN EVEN NUMBER OF PIECES, or with a parity constraint: carry two dp arrays
   dp[i][0], dp[i][1] and one series[] per parity, exactly as min_partition does for (even, odd).
 CF 932G "PALINDROME PARTITION" (split s into k pieces with p_1 = p_k, p_2 = p_{k-1}, ...):
   build t = s[0] s[n-1] s[1] s[n-2] ... and count factorisations of t into EVEN-LENGTH
   palindromes - that is the call with evenLengthPiecesOnly = true. n must be even.
 WEIGHTED pieces (cost per palindrome length): the series aggregate must then be a min/sum over
   the progression WITH the weight, which only stays O(1) if the weight is affine in the length -
   otherwise you are back to O(n^2) and should look for a different decomposition.
 SANITY: countPalindromicFactorizations("aaa") = 4 (aaa | a+aa | aa+a | a+a+a), and any string of
   distinct characters gives exactly 1. */
