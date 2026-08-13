// ALL-PAIR OCCURRENCE RELATION between the patterns themselves: for every ordered pair (i, j),
// does pattern i occur as a SUBSTRING of pattern j? That relation is a partial order (a poset),
// and problems then ask for its longest chain, its maximum antichain (Dilworth = minimum path
// cover = k - maximum bipartite matching, see 03 - Graph Theory), or its transitive reduction.
// CF 590E "Birthday" is the canonical use: pick the largest subset with no containments.
// NAIVE is O(k^2 * L) with a matcher per pair. This is O(sum |p| * A) for the automaton plus
// O(k^3 / 64) for the closure, and the closure is the whole trick:
// during one pass over p_j you record ONLY the NEAREST terminal suffix at each position (the
// exit link). Everything else follows by TRANSITIVITY, because "occurs in" is transitive.
// INDEX CONVENTION: patterns are 0-indexed in insertion order and must be DISTINCT (dedup first,
// otherwise p_i occurs in p_j and p_j in p_i and the poset degenerates).
// Needs: 07 - Strings/03 - Aho Corasick/01 - AhoCorasick.cpp
template <int K>                                    // K = compile-time bound on the #patterns
vector<bitset<K>> containment(const vector<string>& p) {
    Aho<26, Mapper> ac;
    vector<int> node(p.size());
    for (size_t i = 0; i < p.size(); i++) node[i] = ac.insert(p[i]);
    ac.build();
    int k = p.size();
    vector<bitset<K>> in(k);                                   // in[j][i] = p_i occurs in p_j
    for (int j = 0; j < k; j++) {
        int u = 0;
        for (char c : p[j]) {
            u = ac.advance(u, c);
            // the nearest terminal suffix - skipping p_j itself, which ends at its own last
            // position and would otherwise hide the shorter terminal suffixes underneath it
            int tu = ac.t[u].terminal_idx;
            int v = (tu != -1 && tu != j) ? u : ac.t[u].exit_link;
            if (v && ac.t[v].terminal_idx != -1 && ac.t[v].terminal_idx != j)
                in[j][ac.t[v].terminal_idx] = 1;
        }
    }
    vector<int> ord(k);                                        // shorter patterns first: a proper
    iota(ord.begin(), ord.end(), 0);                           // substring is strictly shorter
    sort(ord.begin(), ord.end(), [&](int a, int b) { return p[a].size() < p[b].size(); });
    for (int j : ord) {                                        // transitive closure
        auto cur = in[j];
        for (int i = 0; i < k; i++) if (cur[i]) in[j] |= in[i];
    }
    return in;
}
/* USING THE POSET
 LARGEST SUBSET WITH NO CONTAINMENT (maximum antichain) = k - (minimum chain cover), and by
   Dilworth on the TRANSITIVELY CLOSED relation, minimum chain cover = k - maximum matching in
   the bipartite graph "i on the left, j on the right, edge if p_i occurs in p_j". Recover the
   actual antichain from the Konig vertex cover: keep the vertices NOT in the cover.
 LONGEST CHAIN = longest path in the DAG = the usual O(k^2) DP on the closed relation.
 COUNTING occurrences instead of deciding: replace the bitset with a fail-tree subtree sum -
   push +1 at every visited node of p_j and read the subtree sum of each terminal, O((L + k) log).
 MEMORY: k * K / 8 bytes. k = 750 (the CF 590E bound) is 70 KB; k = 1e5 is not viable, and at
   that size the question is always something the fail tree answers directly.
 IF THE PATTERNS REPEAT, dedup and remember the multiplicity; the poset is on DISTINCT strings. */
