// BROKEN PROFILE DP - sweep the grid cell by cell carrying a bitmask of the "broken" frontier.
// O(n * m * 2^m); always make m the SMALLER dimension.
// mask bit j = "cell (i, j) is already covered by something placed earlier".

// Count tilings of an n x m grid by 1x2 dominoes.
ll dominoTilings(int n, int m) {
    if (m > n) swap(n, m);
    if (((ll)n * m) & 1) return 0;
    int M = 1 << m;
    vector<ll> cur(M, 0), nxt;
    cur[0] = 1;
    for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
            nxt.assign(M, 0);
            for (int mask = 0; mask < M; mask++) if (cur[mask]) {
                if (mask >> j & 1) nxt[mask ^ (1 << j)] += cur[mask];           // already covered
                else {
                    if (i + 1 < n) nxt[mask | (1 << j)] += cur[mask];           // vertical domino
                    if (j + 1 < m && !(mask >> (j + 1) & 1))
                        nxt[mask | (1 << (j + 1))] += cur[mask];                // horizontal domino
                }
            }
            cur = nxt;
        }
    return cur[0];
}
// BLOCKED CELLS: keep a grid `bad[i][j]`; a blocked cell must arrive already "covered"
// (skip it by clearing bit j and forbidding any placement on it).
// OTHER PIECES: add one transition per shape; the mask must record every cell the shape
// occupies in a LATER position of the sweep.
// COUNTING INDEPENDENT SETS / no-two-adjacent colourings: mask = which cells of the previous
// row are used; transition over masks with (a & b) == 0 and no two adjacent bits in b.
// AS WRITTEN this is O(n * 4^m) (nested mask loops), not O(n * m * 2^m): at m = 14 that is 2.7e8
// per row. To get O(n * 2^m), note sum over a with (a & b) == 0 of cur[a] = zetaSub(cur)[~b],
// so one subset-zeta pass per row replaces the inner loop (see 05 - Combinatorics/07).
ll independentSets(int n, int m) {                     // #ways to pick cells, none orthogonally adjacent
    if (m > n) swap(n, m);
    int M = 1 << m;
    vector<ll> cur(M, 0), nxt;
    for (int b = 0; b < M; b++) if (!(b & (b << 1))) cur[b] = 1;
    for (int i = 1; i < n; i++) {
        nxt.assign(M, 0);
        for (int a = 0; a < M; a++) if (cur[a])
            for (int b = 0; b < M; b++) if (!(b & (b << 1)) && !(a & b)) nxt[b] += cur[a];
        cur = nxt;
    }
    ll r = 0;
    for (ll x : cur) r += x;
    return r;
}
// CONNECTED-COMPONENT DP ("plug DP") is the heavy version: the state stores, for each frontier
// cell, WHICH connected component its plug belongs to (a bracket-like labelling). Use it for
// "count Hamiltonian cycles / single-closed-loop tilings"; it is long - only write it if the
// problem clearly needs connectivity, not just coverage.
