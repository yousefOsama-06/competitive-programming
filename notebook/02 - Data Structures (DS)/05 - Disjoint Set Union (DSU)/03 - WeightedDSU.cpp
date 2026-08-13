// DSU with POTENTIALS: maintains pot[v] = value(v) - value(root), so you can answer
// "what is value(u) - value(v)" and detect contradictions.
// Group is (Z, +) here; for parity/bipartiteness use XOR: change ALL FOUR of pot[x] += p -> ^=,
// pu - pv == w -> (pu ^ pv) == w, pot[rv] = pu - pv - w -> pu ^ pv ^ w, and DELETE the w = -w line
// (negation is not the inverse in GF(2)), or Z_k for mod-k relations.
struct WDSU {
    vector<int> par;
    vector<ll> pot;                              // potential relative to the parent
    vector<int> sz;

    WDSU(int n = 0) : par(n), pot(n, 0), sz(n, 1) { iota(all(par), 0); }
    pair<int, ll> find(int x) {                  // {root, value(x) - value(root)}
        if (par[x] == x) return {x, 0};
        auto [r, p] = find(par[x]);
        par[x] = r, pot[x] += p;                 // path compression accumulates
        return {r, pot[x]};
    }
    // assert value(u) - value(v) == w. false = CONTRADICTION with what we already know.
    bool join(int u, int v, ll w) {
        auto [ru, pu] = find(u);
        auto [rv, pv] = find(v);
        if (ru == rv) return pu - pv == w;
        if (sz[ru] < sz[rv]) swap(ru, rv), swap(pu, pv), w = -w;
        par[rv] = ru, pot[rv] = pu - pv - w, sz[ru] += sz[rv];
        return true;
    }
    // value(u) - value(v), or nullopt if they are not related yet
    bool diff(int u, int v, ll& out) {
        auto [ru, pu] = find(u);
        auto [rv, pv] = find(v);
        if (ru != rv) return false;
        out = pu - pv;
        return true;
    }
};
// BIPARTITENESS under edge additions: use XOR potentials with w = 1 per edge; join() returning
// false means an odd cycle appeared. Same structure answers "are u and v the same colour".
