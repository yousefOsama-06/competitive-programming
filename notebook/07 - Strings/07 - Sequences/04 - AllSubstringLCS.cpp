// ALL-SUBSTRING LCS - after ONE O(n m) pass you can answer  lcs(s, t[i..j])  for EVERY one of the
// m^2 substrings of t. The classic DP gives one pair per O(n m); this gives all of them at once.
// INDEX CONVENTION: s and t are 1-indexed below; the returned array F is indexed 1..m and
//     lcs(s, t[i..j]) = #{ k in [i, j] : F[k] < i }.
// So one array of m integers encodes all m^2 answers, and a single query is a RANGE COUNT of
// values below a threshold - BIT offline, wavelet tree, or merge-sort tree, O(log m) each.
// WHEN: "for every substring of t, how similar is it to s", edit-distance-to-a-window sweeps,
// and problems asking for the shortest/longest window of t whose LCS with s reaches a target.
// WHY IT WORKS: for a fixed left end i the row of the LCS table is monotone with steps of 0/1;
// F[k] is the largest left end for which position k still contributes a step. f/g below track
// the row and column "critical points" of the table - that is the Alves-Caceres-Song theory.
vector<int> allSubstringLCS(const string& s, const string& t) {
    int n = s.size(), m = t.size();
    vector<int> f(m + 1), g(m + 1, 0), nf(m + 1, 0);
    for (int j = 0; j <= m; j++) f[j] = j;                     // row 0 of the table
    for (int i = 1; i <= n; i++) {
        nf[0] = 0;
        for (int j = 1; j <= m; j++) {
            int gp = g[j - 1], fp = f[j];                      // g[i][j-1] and f[i-1][j]
            if (s[i - 1] == t[j - 1]) nf[j] = gp, g[j] = fp;
            else nf[j] = max(fp, gp), g[j] = min(gp, fp);
        }
        swap(f, nf);
    }
    return f;                                                  // F[1..m], F[0] is unused
}
// DIRECT USE - all answers for one left end i in O(m):
//   int cnt = 0; for (int j = i; j <= m; j++) { if (F[j] < i) cnt++; /* lcs(s, t[i..j]) = cnt */ }
// MANY (i, j) QUERIES: sort by i, sweep i from m down to 1 adding position k when F[k] < i to a
// BIT, then the answer is a prefix-sum query on [i, j]. O((m + q) log m) total.
/* RELATED SHAPES
 ALL-SUBSTRING LCS OF s AGAINST ITS OWN SUBSTRINGS is the same call with t = s.
 THE TRANSPOSE (all substrings of s against the whole t) is the same routine with the arguments
   swapped - the construction is NOT symmetric, so swap deliberately.
 SEMI-LOCAL LCS is the full theory this is a corner of: it also answers "s[a..b] vs t[i..j]"
   using a permutation matrix and range counting, in O(n m log) preprocessing. Rarely needed.
 EDIT DISTANCE has no such compact all-substring encoding; only the +1/-1 step structure of LCS
   makes the answers a range count.
 SANITY CHECK: lcs(s, t[i..i]) is 1 exactly when t[i] appears in s, and F[k] < i is then the test
   "the first position of t[k]'s character in s is early enough" - verify on a tiny case before
   trusting the direction of the inequality. */
