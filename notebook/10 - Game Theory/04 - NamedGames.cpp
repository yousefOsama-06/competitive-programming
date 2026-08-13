/* ====== NAMED GAMES: the values you cannot re-derive during a contest ======
 Two different families live here: TAKE-AND-BREAK (octal) games, and COIN-TURNING games.
 They have different theory - octal games are the splitting rule below, coin-turning games are
 XOR of single-head values and, in 2-D, nim MULTIPLICATION. Do not mix them up.
 Every value on this card was brute-force verified.
 All are impartial, NORMAL play. Grundy of a sum = XOR of parts.

 SPLITTING RULE (the general take-and-break move):
   g(v) = mex{ g(a1) XOR g(a2) XOR ... : v -> (a1, a2, ...) }

 LASKER'S NIM  (take any amount from a pile, OR split a pile of size >= 2 into two non-empty)
   g(4k+1) = 4k+1     g(4k+2) = 4k+2     g(4k+3) = 4k+4     g(4k+4) = 4k+3

 KAYLES  (remove 1 or 2 adjacent pins, possibly splitting the row)
   Periodic with period 12 from n = 71 onward (Winning Ways says 72; 71 is tight).
     block for n >= 71:  7 4 1 2 8 1 4 7 2 1 8 2
     14 exceptions (n, G): (0,0)(3,3)(6,3)(9,4)(11,6)(15,7)(18,3)(21,4)(22,6)(28,5)(34,6)
                           (39,3)(57,4)(70,6)          last exception n = 70

 DAWSON'S CHESS  (octal game .137)
   Period 34 from n = 52 onward.
     block for n >= 52: 3 3 0 1 1 3 0 2 1 1 0 4 5 3 7 4 8 1 1 2 0 3 1 1 0 3 3 2 2 4 4 5 5 9
     7 exceptions (n, G): (0,0)(14,0)(16,2)(17,2)(31,2)(34,0)(51,2)     last exception n = 51
 DAWSON'S KAYLES (.07)  period 34 from n = 53, same block, 8 exceptions, last at n = 52.

 GRUNDY'S GAME  (split a pile into two UNEQUAL non-empty piles)
   Periodicity is an OPEN PROBLEM - more than 2*10^10 values computed and no period found.
   Do NOT assume a period here; compute the prefix you need.

 GUY-SMITH PERIODICITY THEOREM (how to PROVE a period, not just guess it)
   For an octal game whose largest non-zero code digit has index k: if
     G(n + p) = G(n) for all n with n0 <= n < 2*n0 + p + k
   then the period holds for ALL n >= n0. So checking up to 2*n0 + p + k is a proof.

 TURNING GAMES  (a position is a set of heads; the whole position's Grundy value is the XOR of
 the values of each single head considered alone)
   Twins       (turn exactly 2 coins), 0-indexed:  g(x) = x                    [plain nim]
   Mock Turtles(turn 1, 2 or 3 coins), 0-indexed:  g(x) = the x-th ODIOUS number
               (odious = odd popcount): 1, 2, 4, 7, 8, 11, 13, 14, 16, ...
               g(x) = 2x if 2x is odious, else 2x + 1
   Ruler       (turn any number of CONSECUTIVE coins), 1-indexed:
               g(x) = largest power of two dividing x = x & (-x)  -> 1,2,1,4,1,2,1,8,...
   TRAP: Ruler is 1-INDEXED while Twins and Mock Turtles are 0-INDEXED. Mixing them up gives a
   silently wrong answer.

 NIM WITH ONE PASS (Guy's model: the pass may be used once, at any NON-terminal position)
   1 heap:  G(0) = 0,  G(n) = n+1 for n odd,  G(n) = n-1 for n even >= 2.
   2 heaps: P-positions are exactly (0,0) and (a, a+1) with a ODD.
   3+ heaps: OPEN. No pattern; 34 P-positions with a<=b<=c<=13 and neither xor==0 nor xor==1
     comes close. Never guess.
   If the pass is ALSO legal at a terminal position (a weaker rule), the game collapses:
     G = (XOR of piles) XOR 1, so P-positions are exactly XOR == 1.
======================================================================================= */
bool odious(ll x) { return __builtin_parityll(x); }
ll mockTurtles(ll x) { return odious(2 * x) ? 2 * x : 2 * x + 1; }   // 0-indexed
ll ruler(ll x) { return x & -x; }                                    // 1-indexed
ll lasker(ll n) {                                    // g(0) = 0, NOT -1
    ll r = n % 4;
    if (r == 0) return n ? n - 1 : 0;
    return r == 3 ? n + 1 : n;
}
// Grundy of a sum of turning-game heads = XOR of the single-head values.
ll turningGameValue(const vector<ll>& heads, ll (*g)(ll)) {
    ll r = 0;
    for (ll h : heads) r ^= g(h);
    return r;
}
