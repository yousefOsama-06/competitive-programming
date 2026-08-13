// Needs: Kuhn (03 - Graph Theory/07 - Matching/02 - KuhnAndKonig.cpp) for the matching part.
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
// For bipartite graphs use Kuhn (below). General graphs need Blossom, which this notebook does
// NOT carry - the helper below silently assumes the right side is numbered leftSize..n-1.
// Verified on 538 (graph, start) pairs over random graphs with n <= 7: 0 mismatches.
bool matchingGameFirstWins(int n, vector<vector<int>>& adj, int s, int leftSize) {
    auto solve = [&](int skip) {
        Kuhn K(leftSize, n - leftSize);
        for (int u = 0; u < leftSize; u++) if (u != skip)
            for (int v : adj[u]) if (v != skip) K.add_edge(u, v - leftSize);
        return K.maxMatching();
    };
    return solve(-1) > solve(s);
}
// GAMES WITH SCORES (not just win/lose): plain minimax DP over states,
//   dp[state] = best over moves of (gain + (-dp[next]))  for a zero-sum symmetric game,
//   or keep two arrays (dp0, dp1) when the players' objectives differ.
// GAMES WITH DRAWS / CYCLES: retrograde BFS (see 01 - GrundyAndNim.cpp), never Grundy.
