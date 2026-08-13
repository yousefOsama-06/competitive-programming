// LARGEST ALL-ZERO (or all-one) RECTANGLE in a binary grid, O(n*m).
// Row by row, keep the histogram of consecutive zeros ending at this row, then run the
// largest-rectangle-in-histogram monotonic stack.
ll maximalRectangle(const vector<vector<int>>& g, int target = 0) {
    int n = g.size(), m = n ? g[0].size() : 0;
    vector<int> h(m, 0);
    ll best = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) h[j] = (g[i][j] == target) ? h[j] + 1 : 0;
        vector<int> st;                                // indices, increasing height
        for (int j = 0; j <= m; j++) {
            int cur = (j == m) ? -1 : h[j];
            while (!st.empty() && h[st.back()] >= cur) {
                int ht = h[st.back()]; st.pop_back();
                int left = st.empty() ? -1 : st.back();
                best = max(best, (ll)ht * (j - left - 1));
            }
            st.push_back(j);
        }
        st.clear();
    }
    return best;
}
// LARGEST SQUARE instead: dp[i][j] = min(dp[i-1][j], dp[i][j-1], dp[i-1][j-1]) + 1 when free.
// COUNTING all all-zero submatrices: for each row, for each column j keep `run[j]` = the number
// of rectangles ending at (i, j); with a monotonic stack, run[j] = run[prevSmaller] + h[j] *
// (j - prevSmaller), and the total is the sum of run[j] over all cells. O(n*m).
ll countAllZeroSubmatrices(const vector<vector<int>>& g) {
    int n = g.size(), m = n ? g[0].size() : 0;
    vector<ll> h(m, 0), run(m, 0);
    ll total = 0;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) h[j] = g[i][j] == 0 ? h[j] + 1 : 0;
        vector<int> st;
        for (int j = 0; j < m; j++) {
            while (!st.empty() && h[st.back()] >= h[j]) st.pop_back();
            int p = st.empty() ? -1 : st.back();
            run[j] = (p < 0 ? 0 : run[p]) + h[j] * (j - p);
            total += run[j];
            st.push_back(j);
        }
    }
    return total;
}
