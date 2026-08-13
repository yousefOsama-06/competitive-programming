// MAXIMUM SUBARRAY (Kadane) and its dimensional relatives. All exact, all one pass per dimension.
// KADANE: the best subarray ending at i either extends the best one ending at i-1 or restarts.
// O(n). Returns {sum, l, r} with the segment NON-EMPTY (if every element is negative the answer
// is the largest single element - the empty-segment convention gives 0 instead, so decide which
// one the problem wants BEFORE writing the loop; that is the only bug this ever has).
array<ll, 3> maxSubarray(const vector<ll>& a) {
    int n = a.size();
    ll best = LLONG_MIN, cur = 0;
    int bl = 0, br = 0, start = 0;
    for (int i = 0; i < n; i++) {
        if (cur <= 0) cur = a[i], start = i;                   // restart: the prefix only hurts
        else cur += a[i];
        if (cur > best) best = cur, bl = start, br = i;
    }
    return {best, bl, br};
}
// CIRCULAR (the array wraps): the answer is either an ordinary subarray, or the complement of the
// MINIMUM subarray. Guard the all-negative case - "total - min" would return the empty segment.
ll maxCircularSubarray(const vector<ll>& a) {
    ll total = 0, mx = LLONG_MIN, mn = LLONG_MAX, cx = 0, cn = 0;
    for (ll x : a) {
        total += x;
        cx = max(x, cx + x), mx = max(mx, cx);
        cn = min(x, cn + x), mn = min(mn, cn);
    }
    return mx < 0 ? mx : max(mx, total - mn);
}
// MAXIMUM SUM SUBMATRIX, O(rows^2 * cols): fix the top and bottom rows, compress the columns
// between them into a 1D array by prefix sums, and run Kadane on it. Make ROWS the smaller side.
ll maxSubmatrix(const vector<vector<ll>>& g) {
    int n = g.size(), m = n ? g[0].size() : 0;
    ll best = LLONG_MIN;
    for (int top = 0; top < n; top++) {
        vector<ll> col(m, 0);
        for (int bot = top; bot < n; bot++) {
            for (int j = 0; j < m; j++) col[j] += g[bot][j];
            best = max(best, maxSubarray(col)[0]);
        }
    }
    return best;
}
/* THE VARIANTS, and which structure each one needs
 LENGTH AT LEAST k: prefix sums S, answer = max over i >= k of S[i] - min(S[0..i-k]) - one running
   minimum, O(n).
 LENGTH AT MOST k: S[i] - min over a SLIDING WINDOW of S, so a monotone deque, O(n).
 AT MOST k NEGATIVE ELEMENTS / at most k "skips": dp[i][j] = best ending at i having used j skips.
 MAXIMUM AVERAGE subarray (with a length constraint): binary search the average and run the
   "length at least k" version on a[i] - mid, or the tangent trick in 01 - DP Opt/09.
 MAXIMUM PRODUCT subarray: carry BOTH the running max and the running min (a negative times a
   negative flips them), O(n).
 K-TH LARGEST subarray sum: binary search the value plus a two-pointer/BIT count of pairs.
 MAXIMUM SUBARRAY WITH UPDATES / on a range: segment tree storing (total, best prefix, best
   suffix, best inside) - see 02 - Data Structures; this file is the static case.
 ALL-NEGATIVE ARRAYS are the standard trap in every one of these. */
