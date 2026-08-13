// SUBSET SUM IN O(S sqrt S) WITH RECONSTRUCTION, where S is the sum of all values (or the target,
// whichever is smaller). The bitset DP is O(n S / 64) and usually wins, but it does NOT
// reconstruct the subset and it degrades when n is large; this one is O(S sqrt S) REGARDLESS of
// n and hands back the item indices.
// THE COUNTING ARGUMENT that gives sqrt: only DISTINCT values matter, and if the values sum to S
// there are at most O(sqrt S) distinct ones. For a fixed value v with multiplicity c, "reachable
// with at most c copies of v" is computed in ONE O(S) pass: walk each residue class mod v and
// carry a counter of how many copies are still available - a reachable cell refills the counter
// to c, an unreachable cell consumes one copy if any remain. That is the bounded-knapsack
// feasibility trick without a deque.
// PRECONDITION: all values > 0 (a zero value makes the residue walk spin).
// RETURNS the indices of a subset summing to exactly `target`, or an empty vector if impossible.
vector<int> subsetSumSqrt(const vector<int>& a, int target) {
    if (target < 0) return {};
    vector<char> dp(target + 1, 0);
    vector<int> last(target + 1, -1), cnt(target + 1, 0);
    vector<vector<int>> id(target + 1);
    for (int i = 0; i < (int)a.size(); i++)
        if (a[i] > 0 && a[i] <= target) id[a[i]].push_back(i), cnt[a[i]]++;
    dp[0] = 1;
    for (int v = 1; v <= target; v++) {
        if (!cnt[v]) continue;
        for (int r = 0; r < v; r++) {
            int left = 0;                                      // copies of v still usable
            for (int k = r; k <= target; k += v) {
                if (dp[k]) left = cnt[v];                      // reachable without any copy here
                else if (left) dp[k] = 1, left--, last[k] = id[v][left];
            }
        }
    }
    if (!dp[target]) return {};
    vector<int> res;
    for (int s = target; s > 0; s -= a[last[s]]) res.push_back(last[s]);
    return res;
}
/* WHEN TO USE WHAT
 n small, S small           -> plain O(n S) DP, easiest to modify.
 feasibility only           -> bitset  dp |= dp << a[i],  O(n S / 64). Fastest by far.
 need the SUBSET back       -> THIS FILE, or the bitset plus O(n) "peel one item at a time" with
                               a per-item bitset snapshot (O(n S / 64) memory - usually too much).
 many equal values          -> THIS FILE is optimal: it is O(S) per DISTINCT value.
 counting the number of subsets, not feasibility -> the residue walk does not apply; use prefix
                               sums per residue class (bounded) or plain O(n S) (unbounded).
 CLASSIC USES: "split the array into two halves with a given sum", partitioning tree levels
   (CF 1481F), and any "choose a subset of the multiset of subtree sizes" argument - those all
   have sum of values = n, hence O(n sqrt n).
 THE SAME sqrt ARGUMENT gives the O(n sqrt n / 64) bound for "subset sums of an array whose
   values sum to n" with bitsets - group equal values, binary-group each group, and the total
   number of groups is O(sqrt n log n). */
