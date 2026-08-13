// BLUE-RED HACKENBUSH ON A TREE - the one PARTISAN game you can actually compute. The board is a
// tree rooted at the ground; every edge is Blue (only Left may delete it) or Red (only Right may
// delete it), and deleting an edge also deletes everything no longer connected to the root. The
// value of such a position is a DYADIC RATIONAL (a surreal NUMBER, never a nimber), and the usual
// number rules then decide everything: value > 0 means Left wins whoever moves, value < 0 means
// Right wins whoever moves, value == 0 means the SECOND player wins.
// THE RECURSION. For a stalk (a path from the ground) the value is easy and worth remembering:
// the first run of m equal colours is worth +-m, and every edge after it is worth +-1/2, +-1/4,
// halving each time. For a tree, evaluate each subtree, then push its value v through the edge
// that joins it to its parent:  Blue edge -> (v + p) / 2^(p-1) with p the smallest positive
// integer making v + p > 1;  Red edge -> (v - p) / 2^(p-1) with p smallest making v - p < -1.
// Sum the results over the children. That is the "one edge above a game" simplification, and it
// is what makes the whole tree computable in near-linear time.
// WHEN: exactly the CodeChef GERALD08 shape - a tree of black/white edges, two players delete
// their own colour, print who wins moving first and who wins moving second. More generally, any
// partisan game whose components are trees of forced-order moves.
// COMPLEXITY: O(n log^2 n) with small-to-large merging of the fraction maps. The value needs
// Theta(n) bits in the worst case, which is why it is stored as an integer part plus a map of
// set fraction bits rather than as a long double - a double loses the answer at depth 53.
// DEGENERATE: a single vertex (no edges) has value 0, so the second player wins. All-Blue trees
// have value equal to the number of edges' contribution and are a Left win; the routine handles
// them with no special case. EXACT - all arithmetic is integer bit shuffling.
struct Surreal {                                        // value = val + sum of frac bits, in [0,1)
    int val = 0, w = 0;                                 // absolute bit position = key + w
    map<int, int> frac;                                 // only positions < 0 survive normalisation
    void clear() { frac.clear(), val = w = 0; }
    void add(int x) {                                   // set bit at key x, then carry upward
        frac[x]++;
        auto it = frac.find(x);
        while (it != frac.end() && it->se != 1) {
            if (it->se > 1) frac[it->fi + 1] += it->se >> 1, it->se &= 1;
            auto d = it++;
            if (!d->se) frac.erase(d);
        }
    }
    void divide(int x) {                                // value /= 2^x
        for (int i = 0; i < x; i++) {
            if (!val) break;
            if (val & 1) add(i - w);                    // bit i of val lands at position i - x
            val >>= 1;
        }
        w -= x;
    }
    void operator+=(Surreal& o) {
        val += o.val;
        for (auto& [k, b] : o.frac) if (b) add(k + o.w - w);
        while (!frac.empty() && frac.rbegin()->fi + w >= 0) {           // promote carries into val
            if (frac.rbegin()->se) val += 1 << (frac.rbegin()->fi + w);
            frac.erase(prev(frac.end()));
        }
    }
    void blueEdge() {                                   // v -> (v + p) / 2^(p-1)
        int p = max(1, 1 - val);
        if (frac.empty() && p + val == 1) p++;           // v + p > 1 is STRICT
        val += p, divide(p - 1);
    }
    void redEdge() {                                    // v -> (v - p) / 2^(p-1)
        int p = max(1, 1 + val);
        if (val - p == -1) p++;
        val -= p, divide(p - 1);
    }
    int sign() { return val > 0 ? 1 : val < 0 ? -1 : !frac.empty(); }
};
// g[u] = {v, colour}, colour 0 = Blue (Left / first player's edges), 1 = Red. Tree rooted at 0.
// Returns the sign of the value: +1 Left wins always, -1 Right wins always, 0 second player wins.
int hackenbush(int n, const vector<vector<pii>>& g) {
    vector<Surreal> a(n);
    vector<int> id(n), vis(n, 0);
    iota(all(id), 0);
    function<void(int)> dfs = [&](int u) {
        vis[u] = 1;
        for (auto& [v, c] : g[u]) {
            if (vis[v]) continue;
            dfs(v);
            c ? a[id[v]].redEdge() : a[id[v]].blueEdge();
            if (sz(a[id[v]].frac) > sz(a[id[u]].frac)) swap(id[v], id[u]);   // small to large
            a[id[u]] += a[id[v]], a[id[v]].clear();
        }
    };
    dfs(0);
    return a[id[0]].sign();
}
// READING THE ANSWER: sign > 0 -> Left wins moving first AND second; sign < 0 -> Right wins both;
// sign == 0 -> whoever moves second wins. There is no "first player wins whoever they are" case,
// because a Hackenbush tree value is always a number and numbers are never fuzzy with 0.
// GREEN HACKENBUSH (every edge deletable by both) is impartial and is a NIMBER instead: the
// colon principle, grundy(u) = XOR over children of (grundy(child) + 1) - see 02 - GameAlgorithms.
// A tree with edges of all three colours has no closed form; you are back to computing surreal
// values of general games, which is exponential.
// THE PARTISAN TOOLKIT this file is the concrete instance of (05 - NimbersAndSums has the rest):
// 0 = second player wins, * = {0|0} = first player wins, up = {0|*} beats every 0 but loses to
// every positive number, a SWITCH {a|b} with a > b is hot, TEMPERATURE measures how much the move
// is worth, SIMPLICITY RULE picks the simplest number strictly between the Left and Right options,
// and NUMBER AVOIDANCE says never move in a number component while a non-number one exists.
