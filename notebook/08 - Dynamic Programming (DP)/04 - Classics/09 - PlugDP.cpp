// PLUG DP WITH CONNECTIVITY (the bracket / minimum-representation profile DP). This is the
// heavyweight cousin of 04 - BrokenProfileDP.cpp: broken profile only remembers WHICH frontier
// cells are occupied, which is enough for tilings; here the frontier must also remember WHICH
// PARTIAL PATHS ARE CONNECTED TO WHICH, so that closing a loop early can be forbidden.
// ENCODING: the frontier has m + 1 "plugs". Each plug is 0 (nothing), 1 (a LEFT bracket) or
// 2 (a RIGHT bracket), packed two bits per plug. Because the paths live in a planar grid they
// can never cross, so the connection pattern is a balanced bracket sequence - matching brackets
// = "these two plug ends belong to the same partial path". THAT is why 3 states per plug suffice
// instead of a general set partition of the frontier.
// COMPLEXITY: O(n * m * S) where S = number of reachable states, empirically a few thousand for
// m <= 12 (the Motzkin-like count, far below 3^(m+1)); keep the states in a hash map.
// INDEX CONVENTION (get this wrong and nothing works): cells are 0-based and scanned row by row.
// BEFORE processing cell (i, j) the state holds, in plug index order 0..m:
//   indices < j  : the BOTTOM edges of the cells (i, 0..j-1) already placed in this row
//   index    j   : the LEFT edge of (i, j)          <- p below
//   index    j+1 : the TOP edge of (i, j)           <- q below
//   indices > j+1: the TOP edges of (i, j+1..m-1), inherited from the previous row
// AFTER processing, plug j becomes the BOTTOM edge of (i, j) and plug j+1 its RIGHT edge, which
// is exactly the invariant for cell (i, j+1). At the end of a row plug m must be 0 (no path may
// leave the grid) and the state is SHIFTED LEFT by one plug. Plug k uses bits 2k and 2k+1.
// THE PROBLEM SOLVED HERE: count HAMILTONIAN CYCLES in a grid with blocked cells (URAL 1519
// "Formula 1"). Every other plug-DP problem is this loop with different accept/reject rules.
struct PlugDP {
    int n, m;
    vector<string> g;                                          // '.' free, '*' blocked
    PlugDP(const vector<string>& grid) : g(grid) { n = g.size(), m = n ? g[0].size() : 0; }
    static int get(ll s, int i) { return (s >> (2 * i)) & 3; }
    static ll set(ll s, int i, int v) { return (s & ~(3LL << (2 * i))) | ((ll)v << (2 * i)); }
    int matchRight(ll s, int i) {                              // find the ')' matching '(' at i
        int cnt = 0;
        for (int j = i; j <= m; j++) {
            int v = get(s, j);
            if (v == 1) cnt++;
            else if (v == 2 && --cnt == 0) return j;
        }
        return -1;
    }
    int matchLeft(ll s, int i) {                               // find the '(' matching ')' at i
        int cnt = 0;
        for (int j = i; j >= 0; j--) {
            int v = get(s, j);
            if (v == 2) cnt++;
            else if (v == 1 && --cnt == 0) return j;
        }
        return -1;
    }
    ll solve() {
        int li = -1, lj = -1;                                  // the LAST free cell
        for (int i = 0; i < n; i++)
            for (int j = 0; j < m; j++) if (g[i][j] == '.') li = i, lj = j;
        if (li < 0) return 0;
        ll ans = 0;
        map<ll, ll> cur, nxt;
        cur[0] = 1;
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < m; j++) {
                nxt.clear();
                for (auto& [s, w] : cur) {
                    int p = get(s, j), q = get(s, j + 1);
                    if (g[i][j] == '*') {                      // blocked: no plug may touch it
                        if (!p && !q) nxt[s] += w;
                        continue;
                    }
                    if (!p && !q) {                            // open a new path: down and right
                        if (i + 1 < n && j + 1 < m) nxt[set(set(s, j, 1), j + 1, 2)] += w;
                    } else if (!p || !q) {                     // one plug: continue down or right
                        int v = p ? p : q;
                        if (i + 1 < n) nxt[set(set(s, j, v), j + 1, 0)] += w;    // go down
                        if (j + 1 < m) nxt[set(set(s, j, 0), j + 1, v)] += w;    // go right
                    } else if (p == 1 && q == 1) {             // two '(' : merge, retag the match
                        int k = matchRight(s, j + 1);
                        nxt[set(set(set(s, k, 1), j, 0), j + 1, 0)] += w;
                    } else if (p == 2 && q == 2) {             // two ')' : merge, retag the match
                        int k = matchLeft(s, j);
                        nxt[set(set(set(s, k, 2), j, 0), j + 1, 0)] += w;
                    } else if (p == 2 && q == 1) {             // different paths: just join them
                        nxt[set(set(s, j, 0), j + 1, 0)] += w;
                    } else if (i == li && j == lj) {           // p == 1 && q == 2: closes a loop
                        ll t = set(set(s, j, 0), j + 1, 0);    // legal ONLY at the last free cell
                        if (t == 0) ans += w;
                    }
                }
                cur.swap(nxt);
            }
            nxt.clear();                                       // end of row: shift the profile
            for (auto& [s, w] : cur) if (!get(s, m)) nxt[s << 2] += w;
            cur.swap(nxt);
        }
        return ans;
    }
};
/* ADAPTING IT - the five cases above never change, only what you do with them
 COUNT HAMILTONIAN PATHS (not cycles): allow the "close" case at any cell but require exactly one
   open path at the end, or add a virtual start/end cell.
 SIMPLE CYCLE THAT NEED NOT COVER EVERY CELL (max weight cycle): let a free cell be SKIPPED (the
   `!p && !q` case also allows keeping the state unchanged), and accept the closure at any cell,
   taking the max instead of the sum.
 PARTITION INTO CLOSED LOOPS: accept every closure and multiply, no "last cell" restriction.
 THE THREE-STATE ENCODING ONLY WORKS FOR NON-CROSSING (planar) connections. For general
   connectivity (Steiner-tree profile DPs, spanning forests) you need MINIMUM REPRESENTATION:
   store, per frontier cell, the id of its component normalised to 0,1,2,... left to right - same
   loop, bigger state, and a canonicalisation step after every move.
 ALWAYS make m the SMALLER dimension (transpose if needed) - the state count is exponential in m.
 DEBUGGING: a 4x4 empty grid has 6 Hamiltonian cycles, 6x6 has 1072, 8x8 has 425;  a grid with an
   odd number of free cells has none. Check those before running anything bigger. */
