// Needs: 07 - Strings/01 - String Matching/01 - KMP.cpp   (compute_pi)
// WHAT THE PREFIX FUNCTION ANSWERS once you have pi[] - every one of these is O(n) and every one
// of them is a contest problem on its own. INDEX CONVENTION: s is 0-indexed and
//     pi[i] = length of the longest PROPER border of the prefix s[0..i]  (so pi[0] = 0).
// A "border" is a string that is both a proper prefix and a proper suffix.
// WHEN: any question about periodicity, repetition, "shortest string whose k-th power is s",
// "how many times does every prefix occur", "which prefixes are also suffixes".

// --- ALL BORDERS of the whole string, longest first. The border of a border is a border, so the
// chain pi[n-1] -> pi[pi[n-1]-1] -> ... enumerates them ALL, in O(number of borders) <= O(n).
vector<int> allBorders(const vector<int>& pi) {
    vector<int> b;
    for (int k = pi.empty() ? 0 : pi.back(); k > 0; k = pi[k - 1]) b.push_back(k);
    return b;                                                  // lengths, decreasing
}
// --- PERIODS. p is a period of s (s[i] == s[i+p] wherever both exist) IFF n - p is a border.
// So the periods are exactly { n - b : b a border }, and the SMALLEST period is n - pi[n-1].
int smallestPeriod(int n, const vector<int>& pi) { return n - (n ? pi[n - 1] : 0); }
// s is a perfect power t^k IFF n % smallestPeriod == 0, and then k = n / smallestPeriod.
// (Careful: "aabaa" has period 3 but 5 % 3 != 0, so it is NOT a power - the test is required.)

// --- OCCURRENCE COUNT OF EVERY PREFIX inside s. cnt[len] = number of occurrences of s[0..len-1].
// Every position ends the prefix of length i+1, and an occurrence of a prefix implies an
// occurrence of all of ITS borders - so push the counts down the pi chain in reverse order.
vector<int> prefixOccurrences(const string& s, const vector<int>& pi) {
    int n = s.size();
    vector<int> cnt(n + 1, 0);
    for (int i = 0; i < n; i++) cnt[pi[i]]++;
    for (int i = n - 1; i > 0; i--) cnt[pi[i - 1]] += cnt[i];
    for (int i = 0; i <= n; i++) cnt[i]++;                     // the prefix itself
    cnt[0] = 0;
    return cnt;
}
// The same routine on  p + '#' + t  counts occurrences of every prefix of p inside t: read
// cnt[|p|] for the number of matches, and cnt[k] - (contribution inside p) for shorter prefixes.

// --- BORDER TREE: parent(i) = pi[i-1] over nodes 0..n (node i = the prefix of length i). Then
// "the borders of the prefix of length i" = the ancestors of i, so
//   is u a border of v?            -> u is an ancestor of v (Euler-tour interval test, O(1))
//   longest common border of two   -> LCA
//   count borders of prefix i      -> depth(i)
vector<int> borderTree(const vector<int>& pi) {
    int n = pi.size();
    vector<int> par(n + 1, 0);
    par[0] = -1;
    for (int i = 1; i <= n; i++) par[i] = pi[i - 1];
    return par;
}
/* MORE PREFIX-FUNCTION FACTS worth having on the page
 pi vs Z: z[i] = length of the lcp of s and s[i..]. Either converts to the other in O(n), but pi
   is the one that extends ONLINE (append a character, update in O(1) amortised) and the one that
   turns into an automaton; z is the one that answers "match at offset i" directly.
 AUTOMATON aut[i][c] (see 01 - KMP.cpp) makes the DP "count strings of length L avoiding p"
   trivial; without it that DP is O(L n |A|) with the pi chain walked per step.
 DISTINCT SUBSTRINGS incrementally: append c to s, run the prefix function of the REVERSED new
   string; the new distinct substrings number (i+1) - max(z-values) - O(n^2) total, and the right
   answer whenever the input arrives online and a suffix automaton is overkill.
 FINE AND WILF: if p and q are periods of s and p + q - gcd(p,q) <= n, then gcd(p,q) is a period.
   This is why the border lengths of any prefix form O(log n) ARITHMETIC PROGRESSIONS - the fact
   that makes "sum over all borders" DPs O(n log n) instead of O(n^2).
 COUNTING STRINGS with a given pi array, or with a given set of borders, is Fine and Wilf plus
   inclusion-exclusion over the progressions; do NOT try to enumerate borders one by one. */
