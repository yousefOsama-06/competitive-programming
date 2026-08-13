// Concrete game solvers. All impartial games under NORMAL play (last move wins) unless stated.

// SUBTRACTION GAME with allowed move set S: grundy is eventually PERIODIC. Compute a prefix,
// detect the period, extrapolate to n up to 1e18. This is the standard route for huge n.
struct Periodic {
    vector<int> g;
    int start = -1, per = -1;
    Periodic(const vector<int>& S, int prefix = 2000) {
        g.assign(prefix, 0);
        for (int i = 1; i < prefix; i++) {
            vector<bool> seen(S.size() + 2, false);
            for (int s : S) if (s <= i && g[i - s] < (int)seen.size()) seen[g[i - s]] = true;
            while (g[i] < (int)seen.size() && seen[g[i]]) g[i]++;
        }
        detect();
    }
    void detect() {                                       // smallest (start, per) that fits
        int n = g.size();
        for (int p = 1; p <= n / 3; p++) {
            bool ok = true;
            for (int i = n / 3; i + p < n; i++) if (g[i] != g[i + p]) { ok = false; break; }
            if (!ok) continue;
            int st = 0;                                   // the LAST break, not the first match:
            for (int i = n - p - 1; i >= 0; i--)          // periodicity must hold from start ON
                if (g[i] != g[i + p]) { st = i + 1; break; }
            if (n - p - st < 2 * p + 20) continue;        // demand real evidence before believing
            start = st, per = p;
            return;
        }
    }
    int at(ll n) const {
        if (n < (ll)g.size()) return g[n];
        assert(per > 0);                                  // no period found: do NOT extrapolate
        return g[start + (n - start) % per];
    }
};
// S = {1..k}  =>  grundy(n) = n mod (k+1)  (no need to compute anything)

// WYTHOFF'S GAME (two piles; take any amount from one pile, or the SAME amount from both).
// Losing (P-)positions: (floor(k*phi), floor(k*phi^2)) for k >= 0, phi = (1+sqrt5)/2.
// Since phi^2 - phi = 1, the two coordinates differ by EXACTLY k, so k = b - a.
// Tested exactly, no floating point:  a == floor(k*phi)  <=>  d <= k*sqrt5 < d+2, d = 2a-k.
bool wythoffLosing(ll a, ll b) {
    if (a > b) swap(a, b);
    ll k = b - a, d = 2 * a - k;
    return d >= 0 && (__int128)d * d <= (__int128)5 * k * k
                  && (__int128)5 * k * k < (__int128)(d + 2) * (d + 2);
}
// P-positions: (0,0) (1,2) (3,5) (4,7) (6,10) (8,13) (9,15) (11,18) (12,20) (14,23) ...

// MOORE'S NIM_k: you may take from up to k piles in one move.
// Losing iff, for EVERY bit, the number of piles having that bit set is divisible by (k+1).
bool mooreLosing(const vector<ll>& piles, int k) {
    for (int b = 0; b < 63; b++) {
        int c = 0;
        for (ll x : piles) c += (x >> b) & 1;
        if (c % (k + 1)) return false;
    }
    return true;
}

// STAIRCASE NIM: coins on positions 0..n-1, a move slides some coins from i to i-1.
// INDEX 0 IS THE GROUND: coins there are DEAD and cannot move. Then only ODD indices matter
// (a move from an even index is reversible, so it is worth nothing).
// If your problem instead lets a coin at index 0 slide OFF the board, the ground is a virtual
// index -1 and you must XOR the EVEN indices. Brute force over 4 positions: the wrong convention
// is wrong on 96 of 256 states, e.g. (0,0,0,1).
bool staircaseLosing(const vector<ll>& cnt) {
    ll x = 0;
    for (size_t i = 1; i < cnt.size(); i += 2) x ^= cnt[i];
    return x == 0;
}

// GREEN HACKENBUSH (all edges neutral), grundy of a rooted tree = COLON PRINCIPLE:
//   g(v) = XOR over children c of (g(c) + 1)
int hackenbushTree(int u, int p, const vector<vector<int>>& adj) {
    int r = 0;
    for (int v : adj[u]) if (v != p) r ^= hackenbushTree(v, u, adj) + 1;
    return r;
}
// FUSION PRINCIPLE for graphs: all vertices of a cycle can be fused into one; an ODD cycle
// leaves a single loop (= one edge, grundy 1), an EVEN cycle leaves nothing (grundy 0).

// COIN TURNING GAMES: a position is a set of "heads". The grundy value of the whole position is
// the XOR of the grundy values of each single head considered alone (Mock Turtles, Ruler, ...).

// MISERE NIM (last to move LOSES): if EVERY pile is 1, the first player wins iff #piles is even;
// otherwise the normal-play rule (xor != 0 => win) applies unchanged.
