/* SPRAGUE-GRUNDY.  Every FINITE, ACYCLIC (progressively bounded) IMPARTIAL game under normal
   play (last to move wins) is equivalent to a nim pile of size grundy(position).
   ACYCLIC IS A HYPOTHESIS, NOT A DETAIL: on a graph with cycles the mex recursion is not
   well-founded and grundy() below returns silently wrong values - use retrograde BFS instead.
   grundy(p) = mex{ grundy(q) : q reachable from p }.
   A position is LOSING for the player to move  iff  grundy = 0.
   SUM of independent games: grundy = XOR of the parts.                                    */
int mex(const vector<int>& v) {
    vector<bool> seen(v.size() + 1, false);
    for (int x : v) if (x >= 0 && x <= (int)v.size()) seen[x] = true;
    int m = 0;
    while (m < (int)seen.size() && seen[m]) m++;
    return m;
}
// Grundy over a DAG of at most n positions, memoised.
vector<int> g_;
int grundy(int u, const vector<vector<int>>& nxt) {
    if (g_[u] != -1) return g_[u];
    g_[u] = 0;                                          // guard (DAG => no real cycles)
    vector<int> s;
    for (int v : nxt[u]) s.push_back(grundy(v, nxt));
    return g_[u] = mex(s);
}
/* NIM AND FRIENDS
   NIM (take any number from one pile). Losing iff XOR of pile sizes == 0.
     Winning move: find a pile with (x ^ X) < x where X is the total xor; reduce it to x ^ X.
   MISERE NIM (last to move LOSES). If every pile is size 1: first player wins iff #piles is EVEN.
     Otherwise identical to normal nim (win iff xor != 0).
   NIM_k / MOORE'S NIM (may take from up to k piles). Losing iff, for every bit, the number of
     piles with that bit set is divisible by (k+1).
   STAIRCASE NIM. INDEX 0 IS THE GROUND - coins there are DEAD and have no move.
     Losing iff XOR of the counts at ODD indices == 0.  (If instead a coin on index 0 may still
     slide OFF the board, the ground is a virtual index -1 and you must XOR the EVEN indices.
     Getting this backwards is wrong on 96 of the 256 four-position states.)
     Recognise it as "move something toward a sink, and moving it to an even slot is reversible".
   SUBTRACTION GAME, allowed set S: grundy is eventually PERIODIC. Compute a prefix, find the
     period, extrapolate - the standard route for n up to 1e9.
     S = {1..k}: grundy(n) = n mod (k+1).
   WYTHOFF (two piles, take from one or equally from both). Losing positions are
     (floor(k*phi), floor(k*phi^2)), phi = (1+sqrt5)/2, k >= 0.
   FIBONACCI NIM (first move any amount < pile; then at most 2x the previous move).
     First player loses iff the pile size is a Fibonacci number.
   TURNING / COIN GAMES: grundy of a set of coins = XOR of the grundy of each single coin.
   GREEN HACKENBUSH (all edges neutral): grundy of a path of length n is n; FUSION principle -
     a cycle of odd length fuses to a single edge, an even cycle to nothing; sum branches by XOR.
   COLON PRINCIPLE (trees): grundy of a node = XOR over children of (grundy(child) + 1).      */

// RETROGRADE ANALYSIS - when the game graph HAS CYCLES (draws possible) or the game is partisan,
// Grundy does not apply. BFS backwards from terminal positions counting out-degrees.
// win[u] = 1 first player to move wins, 0 loses, -1 draw.
vector<int> retrograde(int n, const vector<vector<int>>& radj, vector<int> deg,
                       const vector<int>& terminalLose) {
    vector<int> res(n, -1);
    queue<int> q;
    for (int u : terminalLose) res[u] = 0, q.push(u);
    while (!q.empty()) {
        int v = q.front(); q.pop();
        for (int u : radj[v]) {
            if (res[u] != -1) continue;
            if (res[v] == 0) res[u] = 1, q.push(u);      // can move to a losing position
            else if (--deg[u] == 0) res[u] = 0, q.push(u);   // all moves lead to wins
        }
    }
    return res;                                          // remaining -1 are draws
}
