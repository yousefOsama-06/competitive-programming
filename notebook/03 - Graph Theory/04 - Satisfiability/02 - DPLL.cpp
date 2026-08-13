// GENERAL SAT (3-SAT and beyond) by DPLL - Davis-Putnam-Logemann-Loveland. 0-indexed variables.
// Exponential in the worst case, but with unit propagation and pure-literal elimination it decides
// most contest-sized instances (n in the hundreds, even thousands) instantly.
// 2-SAT is polynomial (01 - TwoSat) - always check whether the clauses really need three literals.
// Reach for this when a problem reduces to "assign booleans so that all these constraints hold" and
// some clause has three or more literals: exact cover with few options, graph colouring encoded as
// clauses, puzzle solvers, "is this configuration reachable" searches with heavy pruning.
// LITERAL ENCODING: the positive literal of variable v is v, the negative one is ~v (= -v-1).
// A clause is a vector of literals meaning their OR. Internally literal u lives at index u + n.
// THE THREE MOVES.
//   UNIT PROPAGATION: a clause with all but one literal already false forces that last literal.
//   PURE LITERAL: a variable that only ever appears with one sign can be set that way for free.
//   BRANCH: pick the unassigned variable with the MOST remaining occurrences, try it, backtrack.
struct SAT {
    int n;
    vector<int> occ, pos, neg, unitS, pureS;
    vector<vector<int>> g, cls, level;                         // g[lit] = clauses containing lit
    vector<char> val;                                          // val[u + n] = "literal u is true"
    SAT(int n) : n(n), occ(2 * n, 0), g(2 * n) {}
    void addClause(const vector<int>& c) {
        for (int u : c) g[u + n].push_back(cls.size()), occ[u + n]++;
        cls.push_back(c);
    }
    void set(int u) {                                          // assign literal u = true
        val[u + n] = 1, level.back().push_back(u);
        for (int i : g[u + n]) if (pos[i]++ == 0) for (int w : cls[i]) --occ[w + n];
        for (int i : g[~u + n]) { ++neg[i]; if (pos[i] == 0) unitS.push_back(i); }
    }
    void undo() {
        int u = level.back().back();
        level.back().pop_back(), val[u + n] = 0;
        for (int i : g[u + n]) if (--pos[i] == 0) for (int w : cls[i]) ++occ[w + n];
        for (int i : g[~u + n]) --neg[i];
    }
    bool reduce() {                                            // propagate until nothing is forced
        while (!unitS.empty() || !pureS.empty()) {
            if (!pureS.empty()) {
                int u = pureS.back(); pureS.pop_back();
                if (occ[u + n] == 1 && occ[~u + n] == 0) set(u);
            } else {
                int i = unitS.back(); unitS.pop_back();
                if (pos[i] > 0) continue;                      // clause already satisfied
                if (neg[i] == (int)cls[i].size()) return false; // clause already violated
                if (neg[i] + 1 == (int)cls[i].size()) {
                    int w = 0;
                    bool found = false;
                    for (int u : cls[i]) if (!val[u + n] && !val[~u + n]) w = u, found = true;
                    if (!found) return false;
                    set(w);
                }
            }
        }
        return true;
    }
    bool solve() {                                             // val[v + n] = the model afterwards
        val.assign(2 * n, 0);
        pos.assign(cls.size(), 0), neg.assign(cls.size(), 0);
        level.assign(1, {}), unitS.clear(), pureS.clear();
        for (;;) {
            if (reduce()) {
                int s = -1, bc = 0;                            // branch on the most constrained
                for (int u = 0; u < n; u++) {                  // UNASSIGNED variable
                    if (val[u + n] || val[~u + n]) continue;
                    int c = occ[u + n] + occ[~u + n];
                    if (c > bc) bc = c, s = u;
                }
                if (s < 0) return true;                        // every clause is satisfied
                level.push_back({}), set(s);
            } else {
                if (level.back().empty()) return false;
                int s = level.back()[0];
                while (!level.back().empty()) undo();
                level.pop_back();
                if (level.empty()) return false;
                set(~s);                                       // the other branch
            }
        }
    }
    bool value(int v) const { return val[v + n]; }
};
// EXAMPLE. (x0 OR NOT x1) AND (NOT x1) AND (NOT x1 OR x3 OR x5) is
//   addClause({0, ~1});  addClause({~1});  addClause({~1, 3, 5});
// WATCHED LITERALS + CLAUSE LEARNING (CDCL) is what real solvers add on top; it is far more code
// and rarely needed in a contest - if DPLL is too slow, the intended solution is not SAT.
// EXACTLY-ONE OF k VARIABLES: one clause (v1 OR ... OR vk) plus C(k,2) clauses (NOT vi OR NOT vj);
// for large k use the commander/sequential encoding to keep the clause count linear.
// AT MOST ONE with 2-SAT structure? Only for k = 2. Beyond that you are in this file.
// MAX-SAT and #SAT are harder still; for #SAT on n <= 20 variables, use inclusion-exclusion or a
// subset DP over the clause set instead.
