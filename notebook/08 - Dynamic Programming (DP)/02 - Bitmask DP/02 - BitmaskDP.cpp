// BITMASK DP - the n <= 20 toolkit.
// MEMORY: vector<vector<ll>> dp(1<<20, vector<ll>(20)) is ~193 MB (24 B of vector header per row).
// Flatten to ONE vector<ll> of size (1<<n)*n (168 MB) or vector<int> (84 MB), or iterate masks in
// increasing popcount and keep only two layers. At n = 20, 2^n * n^2 = 4e8 - usually not intended.

// TSP: shortest Hamiltonian cycle from 0 back to 0. O(2^n * n^2).
ll tsp(const vector<vector<ll>>& d) {
    int n = d.size(), N = 1 << n;
    vector<vector<ll>> dp(N, vector<ll>(n, llinf));
    dp[1][0] = 0;
    for (int m = 1; m < N; m++)
        for (int u = 0; u < n; u++) if ((m >> u & 1) && dp[m][u] < llinf)
            for (int v = 0; v < n; v++) if (!(m >> v & 1) && d[u][v] < llinf)
                dp[m | 1 << v][v] = min(dp[m | 1 << v][v], dp[m][u] + d[u][v]);
    ll best = llinf;
    for (int u = 1; u < n; u++) if (dp[N - 1][u] < llinf && d[u][0] < llinf)
        best = min(best, dp[N - 1][u] + d[u][0]);
    return n == 1 ? 0 : best;                        // drop the +d[u][0] for a Hamiltonian PATH
}

// MINIMUM SET COVER / partition into the fewest valid groups. O(3^n) via submask enumeration.
// ok[m] = true if the set m is a legal single group.
int minPartition(int n, const vector<char>& ok) {
    int N = 1 << n;
    vector<int> dp(N, inf);
    dp[0] = 0;
    for (int m = 1; m < N; m++) {
        int low = m & -m;                            // fix the lowest element: kills the n! factor
        for (int s = m; s; s = (s - 1) & m) if ((s & low) && ok[s])
            dp[m] = min(dp[m], dp[m ^ s] + 1);
    }
    return dp[N - 1];
}
// COUNTING the partitions instead: same loop with dp[m] += dp[m ^ s].
// If groups are UNORDERED, fixing the lowest element (above) already prevents double counting.

// ASSIGNMENT (n tasks to n workers, minimum cost), O(2^n * n). dp[m] = best cost using the
// first popcount(m) workers on exactly the task set m.
ll assignment(const vector<vector<ll>>& c) {
    int n = c.size(), N = 1 << n;
    vector<ll> dp(N, llinf);
    dp[0] = 0;
    for (int m = 0; m < N - 1; m++) if (dp[m] < llinf) {
        int i = __builtin_popcount(m);               // next worker
        for (int j = 0; j < n; j++) if (!(m >> j & 1))
            dp[m | 1 << j] = min(dp[m | 1 << j], dp[m] + c[i][j]);
    }
    return dp[N - 1];                                // n > 20 -> use the Hungarian algorithm
}

// COUNTING HAMILTONIAN PATHS / permutations with adjacency constraints: same shape as tsp but
// dp[m][u] += dp[m ^ (1<<u)][v] over allowed v.
// SUBSET-SUM STYLE: when the state is "which items used" AND the order does not matter, prefer
// dp over masks; when only a total matters, use a knapsack - 2^n is not always necessary.
// MEET IN THE MIDDLE: n <= 40 -> split in half, enumerate 2^20 each side, sort + two pointers.
