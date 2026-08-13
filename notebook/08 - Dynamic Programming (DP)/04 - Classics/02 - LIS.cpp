// LIS in O(n log n), with reconstruction. STRICT increasing by default.
// Non-decreasing: change lower_bound -> upper_bound.
// Longest DEcreasing: negate the array. Longest non-increasing: negate + upper_bound.
vector<int> lis(const vector<ll>& a) {
    int n = a.size();
    vector<ll> tail;                             // tail[k] = smallest possible tail of an LIS of length k+1
    vector<int> pos(n), prv(n, -1), idx;
    for (int i = 0; i < n; i++) {
        int k = lower_bound(all(tail), a[i]) - tail.begin();
        if (k == (int)tail.size()) tail.push_back(a[i]), idx.push_back(i);
        else tail[k] = a[i], idx[k] = i;
        pos[i] = k;
        prv[i] = k ? idx[k - 1] : -1;
    }
    vector<int> res;
    for (int i = idx.empty() ? -1 : idx.back(); i >= 0; i = prv[i]) res.push_back(i);
    reverse(all(res));
    return res;                                  // indices of one LIS; length = tail.size()
}
// SAME SHAPE SOLVES: longest chain of pairs (sort by x, LIS on y); maximum non-overlapping
// intervals; "minimum number of non-increasing subsequences covering the array" = LIS length
// (Dilworth). For LIS with an extra weight, replace `tail` with a max-BIT over compressed values.
