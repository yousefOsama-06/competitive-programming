// Needs: Mint, determinant (06 - Math/04/02), phi (04 - NT/02/06), muSmall (09 - EulerTransform...).
// LGV LEMMA and the POLYA CYCLE INDEX - the two theorems that were stated but never coded.

// LINDSTROM-GESSEL-VIENNOT: with sources a_1..a_k and sinks b_1..b_k in a DAG,
//   det(M), M[i][j] = #paths a_i -> b_j
// counts NON-INTERSECTING path systems, PROVIDED the only permutation that admits a
// non-intersecting system is the identity (true for "sorted" sources/sinks in a grid).
// Grid instance: monotone lattice paths, so M[i][j] = C(dx + dy, dx).
Mint lgvGrid(const vector<pair<ll, ll>>& src, const vector<pair<ll, ll>>& dst) {
    int k = src.size();
    vector<vector<Mint>> M(k, vector<Mint>(k));
    for (int i = 0; i < k; i++)
        for (int j = 0; j < k; j++) {
            ll dx = dst[j].first - src[i].first, dy = dst[j].second - src[i].second;
            M[i][j] = (dx < 0 || dy < 0) ? Mint(0) : C(dx + dy, dx);
        }
    return determinant(M);                             // needs GaussianElimination.cpp
}
// USES: k non-crossing monotone paths, rhombus/domino tilings of a region, plane partitions,
// "k tokens walk on a grid and must never share a cell".

// POLYA CYCLE INDEX. Z(G)(a_1..a_n) = (1/|G|) sum_{g in G} prod_k a_k^(#k-cycles of g).
// The UNWEIGHTED orbit count is Z(G)(m, m, ..., m) for m colours - that is what necklaces()
// and bracelets() already give. The reason to carry the cycle index is the WEIGHTED case:
// "necklaces with exactly c_1 of colour 1, c_2 of colour 2, ..." which the numeric form cannot do.
//   Z(C_n) = (1/n) * sum_{d | n} phi(d) * a_d^(n/d)
//   Z(D_n) = (1/2) Z(C_n) + (1/2) a_1 a_2^((n-1)/2)                    [n odd]
//   Z(D_n) = (1/2) Z(C_n) + (1/4) ( a_1^2 a_2^((n-2)/2) + a_2^(n/2) )  [n even]
//   Z(S_n) = (1/n) * sum_{l=1..n} a_l * Z(S_(n-l)),  Z(S_0) = 1
// Weighted PET: substitute a_k -> (t_1^k + t_2^k + ... + t_m^k) and read off the coefficient
// of t_1^c1 * ... * t_m^cm.
// Necklaces with a PRESCRIBED colour multiset, n beads, counts c[] summing to n (rotations only):
Mint necklacesWithCounts(int n, const vector<int>& c) {
    Mint total = 0;
    for (int d = 1; d <= n; d++) {
        if (n % d) continue;                           // rotations of order n/d have d cycles
        bool ok = true;
        for (int x : c) if (x % (n / d)) ok = false;    // each colour must split evenly
        if (!ok) continue;
        Mint ways = fact[d];                           // multinomial over the d cycle slots
        for (int x : c) ways *= invFact[x / (n / d)];
        total += Mint(phi(n / d)) * ways;
    }
    return total * Mint(n).inv();
}
// APERIODIC necklaces (Lyndon words) with k colours: M(n,k) = (1/n) sum_{d|n} mu(d) k^(n/d).
Mint lyndonCount(ll n, ll k) {
    Mint s = 0;
    for (ll d = 1; d <= n; d++) if (n % d == 0) s += Mint(muSmall(d)) * Mint(k).pow(n / d);
    return s * Mint(n).inv();
}

// TRANSFER MATRIX: count length-L walks in a finite automaton. Build A[s][t] = #transitions,
// then (A^L)[s][t]. The generating function sum_L (A^L)[i][j] x^L = ((I - xA)^-1)[i][j] is
// RATIONAL with denominator det(I - xA) of degree <= #states, so the sequence satisfies a linear
// recurrence of that order - feed a prefix to Berlekamp-Massey and finish in O(k^2 log L)
// instead of O(states^3 log L).
