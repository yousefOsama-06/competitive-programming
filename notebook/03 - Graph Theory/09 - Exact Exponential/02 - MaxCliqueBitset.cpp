// MAXIMUM CLIQUE FOR n UP TO ~150 (about 1 s at n = 155, much faster on sparse graphs). 0-indexed.
// 01 - CliqueColouringMIS is capped at 32 vertices by its unsigned mask; this is the same problem
// with a bitset adjacency matrix and a far stronger pruning rule, so it is the one to paste when
// n is in the 40..200 range. MAXIMUM INDEPENDENT SET = run it on the COMPLEMENT graph.
// The bound: greedily colour the candidate set (vertices of the same colour are pairwise
// non-adjacent, so at most one of them can join the clique). If |current| + #colours <= |best|,
// prune. The extra trick is the ADAPTIVE re-sorting - it recomputes the degree order in a
// sub-branch only when that branch has been visited often enough to pay for it (the `limit`).
// PRECONDITION: symmetric matrix, NO self edges. Set MV to the smallest power-of-two-ish bound
// above n; a bitset<MV> is scanned in MV/64 words, so an over-large MV costs real time.
const int MV = 160;                                            // <-- adjust to your n
struct MaxClique {
    typedef vector<bitset<MV>> Graph;
    struct Vertex { int i, d = 0; };
    double limit = 0.025, pk = 0;
    Graph e;
    vector<Vertex> V;
    vector<vector<int>> C;
    vector<int> qmax, q, S, old;
    MaxClique(Graph g) : e(g), C(g.size() + 1), S(C.size()), old(S) {
        for (int i = 0; i < (int)e.size(); i++) V.push_back({i});
    }
    void init(vector<Vertex>& r) {                             // order by degree inside r
        for (auto& v : r) v.d = 0;
        for (auto& v : r) for (auto& j : r) v.d += e[v.i][j.i];
        sort(all(r), [](Vertex a, Vertex b) { return a.d > b.d; });
        int mxD = r[0].d;
        for (int i = 0; i < (int)r.size(); i++) r[i].d = min(i, mxD) + 1;
    }
    void expand(vector<Vertex>& R, int lev = 1) {
        S[lev] += S[lev - 1] - old[lev], old[lev] = S[lev - 1];
        while (R.size()) {
            if ((int)q.size() + R.back().d <= (int)qmax.size()) return;     // colour bound
            q.push_back(R.back().i);
            vector<Vertex> T;
            for (auto v : R) if (e[R.back().i][v.i]) T.push_back({v.i});
            if (T.size()) {
                if (S[lev]++ / ++pk < limit) init(T);
                int j = 0, mxk = 1, mnk = max((int)qmax.size() - (int)q.size() + 1, 1);
                C[1].clear(), C[2].clear();
                for (auto v : T) {                             // greedy colouring
                    int k = 1;
                    auto f = [&](int i) { return e[v.i][i]; };
                    while (any_of(all(C[k]), f)) k++;
                    if (k > mxk) mxk = k, C[mxk + 1].clear();
                    if (k < mnk) T[j++].i = v.i;
                    C[k].push_back(v.i);
                }
                if (j > 0) T[j - 1].d = 0;
                for (int k = mnk; k <= mxk; k++) for (int i : C[k]) T[j].i = i, T[j++].d = k;
                expand(T, lev + 1);
            } else if (q.size() > qmax.size()) qmax = q;
            q.pop_back(), R.pop_back();
        }
    }
    vector<int> solve() { init(V), expand(V); return qmax; }   // the clique itself
};
// MAXIMUM INDEPENDENT SET: build h with h[i][j] = (i != j && !g[i][j]) and run this on h.
// MINIMUM VERTEX COVER = n - MIS.  MINIMUM CLIQUE COVER of G = chromatic number of the complement.
// WEIGHTED maximum clique needs a different bound (sum of the per-colour maxima); the unweighted
// colour count above is not valid there.
// MAXIMUM CLIQUE ON A PERFECT GRAPH (bipartite, chordal, interval, comparability) is POLYNOMIAL -
// check the structure before reaching for exponential search.
// COUNTING all maximal cliques instead: Bron-Kerbosch with pivoting (01), O(3^(n/3)) of them.
