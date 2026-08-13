// Needs: Blossom (03 - Graph Theory/07 - Matching/04 - Blossom.cpp) for the matching part.
// Games that Sprague-Grundy does NOT cover: partisan games, scored games, draws.

// MINIMAX with ALPHA-BETA pruning. Returns the value in PLAYER 0's units (a fixed sign), not
// "the value for the player to move" - with maxing = false the result is still player-0 scored.
// `moves(state)` lists successors; `eval(state)` scores terminal/leaf states from player 0's view.
template <class S, class Moves, class Eval>
ll alphabeta(const S& s, int depth, ll alpha, ll beta, bool maxing, Moves moves, Eval eval) {
    auto nxt = moves(s);
    if (!depth || nxt.empty()) return eval(s);
    if (maxing) {
        ll best = LLONG_MIN;
        for (auto& t : nxt) {
            best = max(best, alphabeta(t, depth - 1, alpha, beta, false, moves, eval));
            alpha = max(alpha, best);
            if (beta <= alpha) break;                     // beta cutoff
        }
        return best;
    }
    ll best = LLONG_MAX;
    for (auto& t : nxt) {
        best = min(best, alphabeta(t, depth - 1, alpha, beta, true, moves, eval));
        beta = min(beta, best);
        if (beta <= alpha) break;                         // alpha cutoff
    }
    return best;
}
// Order moves best-first: pruning is only good if you try strong moves early.
// Memoise on (state) when states repeat; use iterative deepening when depth is unbounded.

// MATCHING GAME ON A GRAPH: a token sits on vertex s; players alternately move it along an
// edge to an unvisited vertex; a player who cannot move loses.
// THEOREM: the FIRST player wins iff s is in EVERY maximum matching, i.e. iff removing s
// decreases the size of the maximum matching.
// This works on ANY graph, and you get EVERY start vertex at once for the price of one matching.
// After a maximum matching is fixed, the vertices left exposed by SOME maximum matching are
// exactly those reachable from an already-exposed vertex by an EVEN-length alternating path -
// which is precisely the set an unsuccessful blossom search paints as OUTER. So run the matching,
// then one failing search from each exposed vertex, and the unpainted vertices are the wins.
// O(V^3) for the whole table, versus O(V^4) for "recompute the matching without s" per vertex.
// Needs Blossom (03 - Graph Theory/07 - Matching/04 - Blossom.cpp) - see the wrapper below.
vector<char> matchingGameWins(Blossom& B) {               // B already has all edges added
    B.solve();
    vector<char> miss(B.n, 0);
    for (int v = 0; v < B.n; v++) if (B.match[v] < 0) {
        B.augment(v);                                    // always fails; it is here for used[]
        for (int u = 0; u < B.n; u++) miss[u] |= B.used[u];
    }
    for (int v = 0; v < B.n; v++) miss[v] = !miss[v];     // in every maximum matching -> P1 wins
    return miss;
}
// A vertex with NO edges is exposed in every matching, so the first player loses there - which is
// right, they cannot move. ISOLATED vertices therefore need no special case.
// GAMES WITH SCORES (not just win/lose): plain minimax DP over states,
//   dp[state] = best over moves of (gain + (-dp[next]))  for a zero-sum symmetric game,
//   or keep two arrays (dp0, dp1) when the players' objectives differ.
// GAMES WITH DRAWS / CYCLES: retrograde BFS (see 01 - GrundyAndNim.cpp), never Grundy.
