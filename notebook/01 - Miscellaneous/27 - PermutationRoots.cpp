// CYCLE DECOMPOSITION AND WHAT IT ANSWERS: the k-th root of a permutation, the minimum number of
// transpositions (with the swaps), and the order.
// Convention: p is 0-indexed, p[i] is the image of i, (p . q)[i] = p[q[i]], p^k is p applied k
// times. All three routines are O(n) after the O(n) decomposition (the order pays a small
// factorisation on top).
vector<vector<int>> cycles(const vector<int>& p) {          // each cycle as i, p[i], p[p[i]], ...
    int n = sz(p); vector<vector<int>> c; vector<char> vis(n, 0);
    for (int i = 0; i < n; i++) if (!vis[i]) {
        c.pb({});
        for (int j = i; !vis[j]; j = p[j]) vis[j] = 1, c.back().pb(j);
    }
    return c;                                               // fixed points are cycles of length 1
}
vector<int> fromCycles(int n, const vector<vector<int>>& c) {
    vector<int> p(n);
    for (auto& v : c) for (int i = 0; i < sz(v); i++) p[v[i]] = v[(i + 1) % sz(v)];
    return p;
}
// --- K-TH ROOT: q with q^k = p, or {} if none exists. k >= 1 (k = 0 is not a question).
// DERIVATION. Raising to the k-th power splits a q-cycle of length m into gcd(m, k) cycles of
// length m / gcd(m, k) - so the q-cycle behind a group of p-cycles of length L must have length
// m = g*L where g is the group size, and it must satisfy g = gcd(gL, k). Divide that out: g = k
// is not required, the condition is exactly
//        g | k   AND   gcd(L, k / g) = 1.
// Let t = product over primes r dividing L of r^(exponent of r in k) - computed below by
// stripping gcd(L, .) off k until it is 1. Then k/t is coprime to L, every legal g is a multiple
// of t, and every g = t*z with z | (k/t) is legal. So the p-cycles of length L can be packed iff
// t divides their COUNT, and greedily taking groups of exactly t always works. Groups of size t
// also maximise the number of cycles of q, so this returns the root with the FEWEST
// transpositions. Existence therefore = "for every L, t(L, k) divides the number of cycles of
// length L". A cycle length that does not appear is vacuously fine.
// CONSTRUCTION: interleave the g cycles v_0..v_{g-1} into one array c of length m = g*L by
// c[(x + y*k) mod m] = v_x[y]. That is a bijection precisely because gcd(m, k) = g, and then
// stepping one place in c is q, so q^k moves v_x[y] to v_x[y+1] = p(v_x[y]).
vector<int> permRoot(const vector<int>& p, ll k) {
    int n = sz(p);
    vector<vector<vector<int>>> by(n + 1);                  // p-cycles bucketed by their length
    for (auto& v : cycles(p)) by[sz(v)].pb(v);
    vector<vector<int>> out;
    for (int L = 1; L <= n; L++) {
        if (by[L].empty()) continue;
        ll t = 1, rem = k, g;
        while ((g = gcd((ll)L, rem)) != 1) t *= g, rem /= g;        // t = minimal legal group
        if (sz(by[L]) % t) return {};                       // count not divisible: NO ROOT
        for (ll i = 0; i < sz(by[L]); i += t) {
            ll m = t * L, kk = k % m; vector<int> c(m);     // reduce k first: y*k can overflow
            for (ll x = 0; x < t; x++) for (ll y = 0; y < L; y++)
                c[(x + y * kk) % m] = by[L][i + x][y];      // the interleave
            out.pb(c);
        }
    }
    return fromCycles(n, out);                              // n == 0 returns {}, which is right
}
// --- MINIMUM TRANSPOSITIONS to sort p is n - (number of cycles), and no fewer: one swap changes
// the cycle count by exactly +-1 (inside a cycle it splits, across two it merges), and sorted
// means n cycles. To turn a into b it is n - cycles(b^-1 . a). Both are the SAME loop: drag the
// wanted value into place, which kills one cycle per swap.
vector<pii> swapsToMatch(vector<int> a, const vector<int>& b) {     // b = 0,1,..,n-1 means "sort"
    int n = sz(a); vector<int> pos(n); vector<pii> s;
    for (int i = 0; i < n; i++) pos[a[i]] = i;              // pos[v] = where value v sits now
    for (int i = 0; i < n; i++) if (a[i] != b[i]) {
        int j = pos[b[i]];                                  // j > i always, so one pass suffices
        s.pb({i, j}), pos[a[i]] = j, pos[a[j]] = i, swap(a[i], a[j]);
    }
    return s;                                               // sz(s) == n - cycles(b^-1 . a)
}
// --- ORDER of p = lcm of the cycle lengths. It OVERFLOWS EARLY: the max over permutations of n
// (Landau's g(n) ~ e^sqrt(n ln n)) passes 1e18 around n = 130, so never store it in an ll. Keep
// it factored, or reduce it mod M. Returns prime -> exponent; order mod M is then the product of
// q^e mod M over the map, and the exponents sum to at most log2(order).
map<ll, int> permOrder(const vector<int>& p) {
    map<ll, int> f;
    for (auto& v : cycles(p)) {
        int m = sz(v);
        for (ll q = 2; q * q <= m; q++) if (m % q == 0) {
            int e = 0; while (m % q == 0) m /= q, e++;
            f[q] = max(f[q], e);
        }
        if (m > 1) f[m] = max(f[m], 1);                     // the leftover prime factor
    }
    return f;                                               // identity / n == 0 gives {} = order 1
}
// VARIANTS / RELATED
// - p^k in O(n): inside each cycle of length L step by k, giving gcd(L, k) cycles of length
//   L / gcd(L, k). That is the root loop read backwards, and the cheap way to check a root.
// - p^k for huge k: reduce k modulo the cycle length of each element, not modulo the order.
// - SQUARE ROOT (k = 2): t = 2 for even L and 1 for odd L, so a root exists iff the cycles of
//   every even length come in pairs. Odd-length cycles always have a root alone.
// - Need a root with a prescribed PARITY of transposition count: once, merge 2t cycles of some
//   length instead of t (legal iff k/t is even and that length has >= 2t cycles). It changes the
//   cycle count by one, hence the parity.
// - MINIMUM ADJACENT swaps is a completely different quantity - the inversion count. See
//   14 - Inversions.cpp. Do not confuse it with n - cycles.
// - The permutation is even iff n - cycles is even; that is the sign, and it is what tells you
//   whether the 15-puzzle position is solvable.
