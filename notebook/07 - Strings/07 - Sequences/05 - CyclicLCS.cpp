// CYCLIC LCS - max over all n cyclic rotations of s of lcs(rotation, t), in O(n m) TOTAL.
// The naive answer is n separate LCS computations, O(n^2 m). WHEN: the two sequences are
// necklaces / circular arrangements and the cut point is yours to choose.
// HOW: build the LCS table of s+s against t once, then RE-ROOT it n times. Deleting the first
// row of the table only ever cancels ONE unit of the answer, and the path that loses it can be
// followed in O(m) - so all n rotations cost O(n m) together, not O(n^2 m).
// INDEX CONVENTION: rows 1..2n of dp read the character s[(row - 1) % n]; dp[i][j] is the LCS of
// the first i characters of the doubled s with t[0..j-1]. Rotation r reads dp[r + n][m].
// from[i][j] records WHICH transition was taken: 0 = came from the left, 1 = diagonal match,
// 2 = came from above. The re-rooting walk needs it, so it is not optional bookkeeping.
int cyclicLCS(const string& s, const string& t) {
    int n = s.size(), m = t.size();
    if (!n || !m) return 0;
    vector<vector<int>> dp(2 * n + 1, vector<int>(m + 1, 0));
    vector<vector<char>> from(2 * n + 1, vector<char>(m + 1, 0));
    auto eq = [&](int i, int j) { return s[(i - 1) % n] == t[j - 1]; };
    for (int i = 0; i <= 2 * n; i++)
        for (int j = 0; j <= m; j++) {
            dp[i][j] = 0, from[i][j] = 0;
            if (j && dp[i][j - 1] > dp[i][j]) dp[i][j] = dp[i][j - 1], from[i][j] = 0;
            if (i && j && eq(i, j) && dp[i - 1][j - 1] + 1 > dp[i][j])
                dp[i][j] = dp[i - 1][j - 1] + 1, from[i][j] = 1;
            if (i && dp[i - 1][j] > dp[i][j]) dp[i][j] = dp[i - 1][j], from[i][j] = 2;
        }
    int res = 0;
    for (int r = 0; r < n; r++) {
        res = max(res, dp[r + n][m]);                          // rotation starting at s[r]
        int x = r + 1, y = 0;                                  // now delete row r + 1
        while (y <= m && from[x][y] == 0) y++;                 // find where that row is used
        for (; y <= m && x <= 2 * n; x++) {
            from[x][y] = 0, dp[x][m]--;
            if (x == 2 * n) break;
            for (; y <= m; y++) {                              // follow the path downward
                if (from[x + 1][y] == 2) break;
                if (y + 1 <= m && from[x + 1][y + 1] == 1) { y++; break; }
            }
        }
    }
    return res;
}
/* WHAT THIS DOES AND DOES NOT COMPUTE
 IT ROTATES ONLY s. That is the standard definition of CLCS and it is enough: rotating BOTH
   strings is a strictly larger maximum in general, and needs n * m separate cut pairs - if a
   problem really wants both, rotate t in an outer loop (O(n m^2)) or think again.
 MEMORY is O(n m) for two tables - at n = m = 2000 that is ~32 MB for dp plus ~8 MB for from.
   Make s the SHORTER string; the doubled dimension is the expensive one.
 THE ANSWER IS NOT MONOTONE in the rotation, so binary searching the cut point is wrong.
 CYCLIC EDIT DISTANCE is the same re-rooting idea (Maes' algorithm) but is O(n m log n) via
   divide & conquer on non-crossing optimal paths - do not expect this code to adapt.
 SANITY: cyclicLCS(s, s) == |s| and cyclicLCS(s, t) >= lcs(s, t) always. */
