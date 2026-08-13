// HIRSCHBERG - reconstruct the ANSWER of a two-sequence DP (LCS, alignment, edit script) in
// O(n m) time but only O(min(n, m)) memory instead of the O(n m) table. WHEN: n = m = 1e5 and you
// need the actual subsequence, not its length - the table would be 1e10 cells.
// THE IDEA (divide & conquer on the ROW, not on the value): the last row of the DP for the top
// half of a, and the last row of the REVERSED DP for the bottom half, meet at the middle row.
// The column k maximising L[k] + R[m - k] is a point the optimal path passes through, so the
// problem splits into (top half of a vs b[0..k)) and (bottom half of a vs b[k..m)).
// COST: T(n, m) = O(n m) + T(n/2, k) + T(n/2, m - k) = O(n m) total - the halves of the second
// level together cost half of the first level, so the series is 2 n m.
// PRECONDITION: the DP must be reversible - dp computed left-to-right on the reversed strings
// must equal the suffix DP. True for LCS, edit distance and affine-gap alignment; NOT true for
// DPs whose transition depends on the direction (e.g. "no two chosen positions adjacent" is fine,
// but anything reading a prefix STATE such as a running remainder is not).
vector<int> lcsLastRow(const string& a, const string& b) {      // O(|b|) memory
    int m = b.size();
    vector<int> prv(m + 1, 0), cur(m + 1, 0);
    for (char ca : a) {
        for (int j = 1; j <= m; j++)
            cur[j] = ca == b[j - 1] ? prv[j - 1] + 1 : max(prv[j], cur[j - 1]);
        swap(prv, cur);
    }
    return prv;
}
string hirschbergLCS(const string& a, const string& b) {
    int n = a.size(), m = b.size();
    if (n == 0 || m == 0) return "";
    if (n == 1) return b.find(a[0]) != string::npos ? string(1, a[0]) : "";
    int i = n / 2;
    string a1 = a.substr(0, i), a2 = a.substr(i);
    string ra(a2.rbegin(), a2.rend()), rb(b.rbegin(), b.rend());
    vector<int> L = lcsLastRow(a1, b), R = lcsLastRow(ra, rb);
    int best = -1, k = 0;
    for (int j = 0; j <= m; j++) if (L[j] + R[m - j] > best) best = L[j] + R[m - j], k = j;
    return hirschbergLCS(a1, b.substr(0, k)) + hirschbergLCS(a2, b.substr(k));
}
/* ADAPTING IT
 EDIT DISTANCE / ALIGNMENT: replace lcsLastRow with the edit-distance row (initialised to
   0,1,2,... instead of zeros) and take the MINIMUM of L[j] + R[m-j] instead of the maximum. The
   split column is then a cell the optimal alignment passes through; emit the operations.
 AFFINE GAP COSTS need three rows (match / gap-in-a / gap-in-b) and the split must record WHICH
   of the three states the path is in at the middle row - otherwise the two halves disagree.
 MEMORY: O(m) for the rows plus O(m log n) for the substrings taken at each recursion level; pass
   index ranges instead of substrings if that matters.
 RECURSION DEPTH is O(log n) - safe.
 IF YOU ONLY NEED THE LENGTH, do not use this: one rolling row is enough (and for LCS, the
   bit-parallel version in 07 - Strings/07 - Sequences/03 is O(n m / 64)).
 THE SAME TRICK reconstructs paths in any layered DAG DP where a middle layer is cheap to
   evaluate from both ends - it is the DP analogue of "meet in the middle". */
