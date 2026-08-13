// 1D/1D DP OPTIMISATION - the self-referential case that divide & conquer structurally cannot do.
//     dp[i] = min over j < i of ( dp[j] + w(j, i) )
// D&C optimisation needs the LAYERED form dp[k][i] = min(dp[k-1][j] + w) so that the whole
// previous layer is already known. Here dp[j] is produced by the very loop that consumes it, so
// you must decide dp[i] before you may use it. That is what this file solves.
// PRECONDITION: w satisfies the QUADRANGLE INEQUALITY (Monge)
//     w(a,c) + w(b,d) <= w(a,d) + w(b,c)   for all a <= b <= c <= d
// checkable in the local form  w(a,c) + w(a+1,c+1) <= w(a+1,c) + w(a,c+1).
// Then the optimal j for i is NON-DECREASING in i, so the array of "who is currently best for
// each future i" is a sequence of INTERVALS, each owned by one j - and finalising dp[i] can only
// append one new interval and overwrite a suffix of the old ones. Keep them on a stack.
// O(n log n) (the binary search for the crossover); O(n) if the crossover is O(1) to find.
template <class F>
vector<ll> onedOned(int n, F w) {                              // w(j, i) for j < i
    vector<ll> dp(n + 1, llinf);
    dp[0] = 0;
    vector<array<int, 3>> st;                                  // {owner j, from, to} intervals
    st.push_back({0, 1, n});
    int head = 0;
    for (int i = 1; i <= n; i++) {
        while (head < (int)st.size() && st[head][2] < i) head++;
        dp[i] = dp[st[head][0]] + w(st[head][0], i);            // the owner of position i
        st[head][1] = i;
        // now insert i as a candidate owner for the suffix it wins
        auto better = [&](int a, int b, int x) {                // is a better than b at x?
            return dp[a] + w(a, x) <= dp[b] + w(b, x);
        };
        while (!st.empty() && st.back()[1] > i && better(i, st.back()[0], st.back()[1])) st.pop_back();
        if (st.empty() || head >= (int)st.size()) st.push_back({i, i + 1, n});
        else {
            auto [j, l, r] = st.back();
            if (better(i, j, r)) {                              // i wins part of this interval
                int lo = max(l, i + 1), hi = r, pos = r + 1;
                while (lo <= hi) {                              // first x where i beats j
                    int m = (lo + hi) / 2;
                    better(i, j, m) ? (pos = m, hi = m - 1) : lo = m + 1;
                }
                st.back()[2] = pos - 1;
                if (pos <= n) st.push_back({i, pos, n});
            } else if (r < n) st.push_back({i, r + 1, n});
        }
    }
    return dp;
}
/* WHEN YOU NEED WHICH OPTIMISATION - the whole family on one card
   dp[i] = min_j (dp[j] + w(j,i)),   w Monge, SELF-REFERENTIAL       -> THIS FILE, O(n log n)
   dp[k][i] = min_j (dp[k-1][j] + w(j,i)),  w Monge, LAYERED          -> D&C opt (01), O(n k log n)
   dp[i][j] = min_k (dp[i][k] + dp[k+1][j]) + w(i,j),  w Monge AND
              monotone on intervals                                   -> Knuth (06), O(n^2)
   dp[i] = min_j (m_j * x_i + b_j)                                    -> CHT (02) / Li Chao (04)
   "exactly k pieces", optimum convex in k                            -> Aliens (07)
   min-plus convolution of a CONVEX sequence with anything            -> SMAWK / D&C (11)
   the query point moves monotonically and lines are range-updated    -> kinetic segment tree
   VERIFY the quadrangle inequality numerically on random quadruples before trusting any of them:
   a DP optimisation applied without its precondition is a silent wrong answer, never a crash.
   COMMON MONGE COSTS: (S_j - S_i)^2; f(S_j - S_i) with f convex and a[] >= 0; c * (j - i);
   "number of equal pairs inside [i,j]"; a_i * b_j with a non-increasing and b non-decreasing.
   NOT MONGE: anything built from max, and anything with negative a[] where you also need the
   interval-monotonicity half (D&C only needs QI; Knuth needs both). */
