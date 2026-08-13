// INTERVAL DP - dp over [l, r], processed by increasing length. O(n^3) (O(n^2) with Knuth).
// This is the base pattern that Knuth's optimisation speeds up; write this first, optimise later.

// 1) SPLIT form: dp[l][r] = min over k of dp[l][k] + dp[k+1][r] + cost(l, r)
//    matrix chain multiplication, merging stones, optimal BST, "cost to combine a range"
template <class Cost> ll intervalSplit(int n, Cost cost) {
    vector<vector<ll>> dp(n, vector<ll>(n, 0));
    for (int len = 2; len <= n; len++)
        for (int l = 0, r = len - 1; r < n; l++, r++) {
            dp[l][r] = llinf;
            for (int k = l; k < r; k++) dp[l][r] = min(dp[l][r], dp[l][k] + dp[k + 1][r]);
            dp[l][r] += cost(l, r);
        }
    return dp[0][n - 1];
}

// 2) ENDPOINT form: decide about a[l] or a[r] and shrink. Palindromes, "remove from either end".
// Minimum insertions to make s a palindrome:
int minInsertPalindrome(const string& s) {
    int n = s.size();
    vector<vector<int>> dp(n, vector<int>(n, 0));
    for (int len = 2; len <= n; len++)
        for (int l = 0, r = len - 1; r < n; l++, r++)
            dp[l][r] = s[l] == s[r] ? dp[l + 1][r - 1] : 1 + min(dp[l + 1][r], dp[l][r - 1]);
    return n ? dp[0][n - 1] : 0;
}

// 3) "BURST BALLOONS" form: choose the LAST element removed in the range, so the neighbours are
//    the (still intact) boundaries l-1 and r+1.
ll burst(vector<ll> a) {
    int n = a.size();
    a.insert(a.begin(), 1), a.push_back(1);
    vector<vector<ll>> dp(n + 2, vector<ll>(n + 2, 0));
    for (int len = 1; len <= n; len++)
        for (int l = 1, r = len; r <= n; l++, r++)
            for (int k = l; k <= r; k++)
                dp[l][r] = max(dp[l][r], dp[l][k - 1] + a[l - 1] * a[k] * a[r + 1] + dp[k + 1][r]);
    return dp[1][n];
}
// WHEN KNUTH APPLIES (form 1): cost is monotone on intervals and satisfies the quadrangle
// inequality; then opt[l][r-1] <= opt[l][r] <= opt[l+1][r] and the k-loop becomes amortised O(1).
// COUNTING variants (e.g. #ways to parenthesise) replace min with + and cost with 1.
