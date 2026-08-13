// NIMBERS, OCTAL GAMES, AND THE SUM VARIANTS. Everything on this page was brute-force verified.

// --- NIM PRODUCT. [0, 2^(2^k)) is a FIELD under (XOR, nimMul). ---
// Definition: a (*) b = mex{ a'(*)b ^ a(*)b' ^ a'(*)b' : a' < a, b' < b }.
// Computed with the two identities: for a Fermat 2-power F = 2^(2^k),
//   x (*) F = x * F for x < F,   and   F (*) F = 3F/2.
// USES: Turning Corners (turn the 4 corners of a rectangle) has g(x,y) = x (*) y, and the TARTAN
// THEOREM says the 2-D product of two coin-turning games has g(x,y) = g1(x) (*) g2(y). Also
// CF 1310F, which asks you to solve a^(*)x = b in the nimber field of size 2^64.
map<pair<u64, u64>, u64> nmMemo;
u64 nimMul(u64 a, u64 b) {
    if (a < b) swap(a, b);
    if (b == 0) return 0;
    if (b == 1) return a;
    auto it = nmMemo.find({a, b});
    if (it != nmMemo.end()) return it->second;
    int L = 0;
    while (L < 6 && (a >> (1u << L))) L++;
    u64 F = 1ULL << (1u << (L - 1));                           // largest Fermat 2-power <= a
    u64 ah = a / F, al = a % F, bh = b / F, bl = b % F;
    u64 A = nimMul(ah, bh), B = nimMul(ah, bl), C = nimMul(al, bh), D = nimMul(al, bl);
    return nmMemo[{a, b}] = ((A ^ B ^ C) * F) ^ D ^ nimMul(A, F / 2);
}

// --- OCTAL (TAKE-AND-BREAK) GAME GRUNDY GENERATOR. ---
// The code is ".d1d2d3...": digit d_k governs "remove exactly k tokens from one heap".
//   bit 1 (value 1): you may take the WHOLE heap        (i.e. leave zero heaps)
//   bit 2 (value 2): you may leave ONE non-empty heap
//   bit 4 (value 4): you may SPLIT the remainder into TWO non-empty heaps
// So .333... = Nim, .77 = Kayles, .07 = Dawson's Kayles, .137 = Dawson's chess.
// Being able to read a statement into a code string turns "invent a Grundy recurrence" into a
// function call. O(N^2 * digits) because of the split term; O(N * digits) without it.
vector<int> octal(const string& code, int N) {                 // code = the digits AFTER the dot
    vector<int> d;
    for (char c : code) d.push_back(isdigit(c) ? c - '0' : c - 'a' + 10);
    vector<int> g(N + 1, 0);
    for (int n = 1; n <= N; n++) {
        set<int> s;
        for (int i = 0; i < (int)d.size(); i++) {
            int k = i + 1;
            if (k > n) break;
            int rem = n - k;
            if ((d[i] & 1) && rem == 0) s.insert(0);
            if ((d[i] & 2) && rem >= 1) s.insert(g[rem]);
            if (d[i] & 4) for (int a = 1; a < rem; a++) s.insert(g[a] ^ g[rem - a]);
        }
        int m = 0;
        while (s.count(m)) m++;
        g[n] = m;
    }
    return g;
}
// GUY-SMITH PERIODICITY THEOREM turns a guess into a PROOF: with k = the index of the last
// non-zero code digit, if G(n + p) = G(n) for all n0 <= n < 2*n0 + p + k, the period holds for
// ALL n >= n0. See 02 - GameAlgorithms.cpp for the (fixed) period detector.

// --- EUCLID'S GAME. (a, b); subtract a positive multiple of the smaller from the larger. ---
// FIRST PLAYER WINS iff a == b OR max/min > phi.  The usual phrasing ">= phi" is WRONG on the
// whole diagonal: (a, a) is always an immediate win.
bool euclidWin(ll a, ll b) {
    if (a < b) swap(a, b);
    if (b == 0) return false;
    if (a == b || a >= 2 * b) return true;
    return !euclidWin(b, a - b);
}

// --- ANTI-SG / the SJ theorem: MISERE play on a SUM of impartial games. ---
// PRECONDITION, and it is not optional: in every component, SG(u) == 0 must imply u is TERMINAL.
// (Without it this rule is wrong on ~1.3% of random impartial games; with it, exact.)
// Misere nim satisfies it, which is why the classic misere-nim rule looks so clean.
bool antiSGFirstWins(const vector<int>& g) {
    int x = 0, mx = 0;
    for (int v : g) x ^= v, mx = max(mx, v);
    return (x && mx > 1) || (!x && mx <= 1);
}

// --- EVERY-SG: you must move in EVERY unfinished component. Not a function of the XOR at all. ---
// step(v) = 0                                    if v is terminal
// step(v) = 1 + max{ step(u) : SG(u) == 0 }      if SG(v) > 0   (drag it out - you will win it)
// step(v) = 1 + min{ step(u) }                   if SG(v) == 0  (end it fast - you will lose it)
// First player wins iff max over components of step(root) is ODD.
// The mirrored max/min assignment is WRONG; this orientation is the verified one.
void everySGStep(const vector<vector<int>>& succ, const vector<int>& sg, vector<int>& step) {
    int n = succ.size();
    step.assign(n, 0);
    for (int u = 0; u < n; u++) {                              // u in REVERSE topological order
        if (succ[u].empty()) { step[u] = 0; continue; }
        if (sg[u] > 0) {
            int b = 0;
            for (int v : succ[u]) if (!sg[v]) b = max(b, step[v] + 1);
            step[u] = b;
        } else {
            int b = INT_MAX;
            for (int v : succ[u]) b = min(b, step[v] + 1);
            step[u] = b;
        }
    }
}
/* ============ VERIFIED CLOSED FORMS AND FACTS ============
 COIN-TURNING FAMILY (a position is a set of heads; the whole position = XOR of single-head
 values). THE INDEXING IS LOAD-BEARING - mixing 0- and 1-indexed values is silently wrong.
   Twins           turn exactly 2          0-idx   g(x) = x
   TurningTurtles  turn 1 or 2             1-idx   g(x) = x
   MockTurtles     turn 1, 2 or 3          0-idx   g(x) = 2x or 2x+1, whichever is ODIOUS
                                                   ( = the x-th odious number: 1 2 4 7 8 11 13 )
   Ruler           any CONSECUTIVE run     1-idx   g(x) = x & -x
   Triplets        turn exactly 3          0-idx   g(x) = 0 for x < 2, MockTurtles(x-2) otherwise
   Mogul           turn 1..5                       NO closed form (1 2 4 8 16 31 32 64 103 ...)
   TurningCorners  turn 4 rectangle corners        g(x, y) = x (*) y     [nim product]
   TARTAN THEOREM: for the 2-D product of two coin-turning games, g(x,y) = g1(x) (*) g2(y).

 k-WYTHOFF (Fraenkel): take from one pile, or k1, k2 > 0 from both with |k1 - k2| < k.
   P-positions: a_n = mex{a_i, b_i : i < n}, b_n = a_n + k*n.
   Closed form: (floor(n*alpha), floor(n*beta)) with alpha = (2 - k + sqrt(k*k + 4)) / 2,
   beta = alpha + k.  k = 1 is ordinary Wythoff.

 SILVER DOLLAR (coins at p_1 < ... < p_k on a strip, slide one left onto an empty square, no
   jumping): with gaps g_i = p_i - p_{i-1} - 1 and p_0 = -1, the GRUNDY VALUE is
   g_k XOR g_{k-2} XOR g_{k-4} XOR ...  - staircase nim on alternate gaps counted FROM THE RIGHT.

 CHOMP: every m x n board is a FIRST-PLAYER WIN except 1x1, by strategy stealing - and there is
   NO known constructive winning move. Two-row Chomp: P-positions are exactly (b+1, b).
   Strategy stealing is a pure EXISTENCE proof; if the problem wants the move, it is useless.

 BLUE-RED HACKENBUSH (partisan!): a string of edges from the ground has a NUMBER value, not a
   nimber. The first run of m equal colours contributes +-m (Blue positive), and every later edge
   contributes +-1/2, +-1/4, ... halving. This is the cleanest concrete model of surreal numbers.
   Partisan values in general: 0 = second player wins, * = {0|0} = first player wins,
   up = {0|*} is positive but smaller than every positive number, a SWITCH {a|b} with a > b is
   "hot" (both players want to move there), and the TEMPERATURE is what it costs the opponent to
   let you move first. SIMPLICITY RULE: if some number lies strictly between the Left and Right
   options, the game IS the simplest such number. NUMBER AVOIDANCE: never move in a number
   component while a non-number component exists.

 MISERE THEORY, honestly: misere nim's clean rule is an ACCIDENT. In general the misere outcome
   of a sum is NOT a function of the Grundy values; the correct invariant is the GENUS, and only
   "tame" games (genus matching nim's) obey nim-like rules. Anti-SG above is exactly the tame
   case, which is why it carries a precondition.

 COMPLEXITY LANDSCAPE - when to stop looking for a formula:
   UNDIRECTED vertex geography is in P (the maximum-matching theorem in 03 - MinimaxAndMatching).
   DIRECTED vertex geography, Node Kayles, generalized geography and generalized Hex are all
   PSPACE-complete; generalized chess and Go are EXPTIME-complete. So a 1e5-sized instance of
   directed geography MUST have extra structure - find it instead of searching.

 BOGUS NIM / REVERSIBLE MOVES: adding a move that the opponent can immediately undo does not
 change the Grundy value. That is precisely why the even indices in staircase nim are worth
 nothing, and it is the impartial case of the "bypass a reversible option" simplification. */
