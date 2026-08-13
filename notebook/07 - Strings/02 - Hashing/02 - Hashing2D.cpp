// 2D HASHING - O(1) hash of any submatrix, for grid pattern matching.
// 2D prefix hash with random bases b1 (down) and b2 (right), single 62-bit modulus via __int128.
ll HB2D1 = 0, HB2D2 = 0;                             // ONE base pair per RUN, at file scope.
struct Hash2D {                                      // Per-object bases would make two grids
    static const ll M = (ll)1e18 + 9;                // incomparable - and "find this pattern in
    int n, m;                                        // that grid" needs exactly two objects.
    ll b1, b2;
    vector<ll> P1, P2;
    vector<vector<ll>> h;

    static ll mul(ll a, ll b) { return (ll)((__int128)a * b % M); }
    Hash2D(const vector<string>& g) : n(g.size()), m(n ? g[0].size() : 0),
                                      P1(n + 1, 1), P2(m + 1, 1), h(n + 1, vector<ll>(m + 1, 0)) {
        if (!HB2D1) {
            mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
            HB2D1 = 256 + rng() % (M - 300), HB2D2 = 256 + rng() % (M - 300);
        }
        b1 = HB2D1, b2 = HB2D2;
        for (int i = 1; i <= n; i++) P1[i] = mul(P1[i - 1], b1);
        for (int j = 1; j <= m; j++) P2[j] = mul(P2[j - 1], b2);
        for (int i = 1; i <= n; i++)
            for (int j = 1; j <= m; j++) {
                __int128 v = (__int128)mul(h[i - 1][j], b1) + mul(h[i][j - 1], b2)
                           - mul(mul(h[i - 1][j - 1], b1), b2) + g[i - 1][j - 1];
                h[i][j] = (ll)((v % M + M) % M);
            }
    }
    // hash of the submatrix with top-left (r, c) and size dr x dc, 0-based
    ll get(int r, int c, int dr, int dc) const {
        int r2 = r + dr, c2 = c + dc;
        __int128 v = (__int128)h[r2][c2] - mul(h[r][c2], P1[dr]) - mul(h[r2][c], P2[dc])
                   + mul(mul(h[r][c], P1[dr]), P2[dc]);
        return (ll)((v % M + M) % M);
    }
};
// USES: count occurrences of a pattern grid · largest common square submatrix (binary search the
// side, hash all windows of both grids) · count distinct k x k submatrices.
// CHEAPER when the pattern is fixed: 1D-hash each row of the pattern, then run KMP /
// Aho-Corasick over the rows treated as single symbols.
