// Named sequences that keep showing up, with what each one counts. Needs Mint + buildFact().

// MOTZKIN M(n): lattice paths (0,0)->(n,0) with steps U/D/FLAT staying >= 0; also the number of
// ways to draw non-crossing chords on n points on a circle. M: 1,1,2,4,9,21,51,127
vector<Mint> motzkin(int n) {
    vector<Mint> m(n + 1, 0);
    m[0] = 1;
    if (n >= 1) m[1] = 1;
    for (int i = 2; i <= n; i++) {                        // M(i) = M(i-1) + sum M(k)M(i-2-k)
        m[i] = m[i - 1];
        for (int k = 0; k <= i - 2; k++) m[i] += m[k] * m[i - 2 - k];
    }
    return m;
}
// NARAYANA N(n,k): Dyck paths of length 2n with exactly k peaks. sum_k N(n,k) = Catalan(n).
Mint narayana(int n, int k) { return k < 1 || k > n ? Mint(0) : C(n, k) * C(n, k - 1) * inv_[n]; }
// SCHRODER: large R(n) counts lattice paths (0,0)->(n,n) with steps E/N/NE staying below y=x.
// R: 1,2,6,22,90,394.  R(n) = R(n-1) + sum_{k} R(k) R(n-1-k)  (little schroeder s(n) = R(n)/2 for n >= 1 only (s(0) = 1, not 1/2))
vector<Mint> schroder(int n) {
    vector<Mint> r(n + 1, 0);
    r[0] = 1;
    for (int i = 1; i <= n; i++) { r[i] = r[i - 1]; for (int k = 0; k < i; k++) r[i] += r[k] * r[i - 1 - k]; }
    return r;
}
// EULERIAN A(n,k): permutations of n with exactly k ascents.
// A(n,k) = (k+1) A(n-1,k) + (n-k) A(n-1,k-1).   sum_k A(n,k) = n!
vector<vector<Mint>> eulerian(int n) {
    vector<vector<Mint>> a(n + 1, vector<Mint>(n + 1, 0));
    a[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int k = 0; k < i; k++)
            a[i][k] = Mint(k + 1) * a[i - 1][k] + Mint(i - k) * (k ? a[i - 1][k - 1] : Mint(0));
    return a;
}
// q-BINOMIAL (Gaussian) coefficient: #k-dim subspaces of an n-dim space over F_q; also
// #partitions fitting in a k x (n-k) box, weighted by q^area.
// PRECONDITION: ord_p(q) > k. The denominator is prod (q^i - 1), which vanishes in F_p
// whenever ord_p(q) <= k - NOT only at q == 1. (q = -1 mod 998244353 breaks every k >= 2.)
// Root-of-unity-safe fallback, O(nk): Pascal's rule [n,k]_q = [n-1,k-1]_q + q^k [n-1,k]_q.
Mint qBinom(int n, int k, Mint q) {                       // prod_{i=1..k} (q^(n-k+i)-1)/(q^i-1)
    if (k < 0 || k > n) return 0;
    if (q == Mint(1)) return C(n, k);                     // q=1 is the 0/0 limit
    Mint num = 1, den = 1;
    for (int i = 1; i <= k; i++) num *= q.pow(n - k + i) - 1, den *= q.pow(i) - 1;
    return num / den;
}
// HOOK LENGTH FORMULA: #standard Young tableaux of shape lambda = n! / prod(hook lengths).
// hook(i,j) = (arm to the right) + (leg below) + 1.
Mint youngTableaux(const vector<int>& lam) {
    int n = 0;
    for (int x : lam) n += x;
    vector<int> conj(lam.empty() ? 0 : lam[0], 0);
    for (int i = 0; i < (int)lam.size(); i++) for (int j = 0; j < lam[i]; j++) conj[j]++;
    Mint den = 1;
    for (int i = 0; i < (int)lam.size(); i++)
        for (int j = 0; j < lam[i]; j++) den *= Mint(lam[i] - j + conj[j] - i - 1);
    return fact[n] / den;
}
// LGV LEMMA: for sources a_1..a_k and sinks b_1..b_k in a DAG, the number of NON-INTERSECTING
// path systems equals det(M) with M[i][j] = #paths a_i -> b_j (when only the identity
// permutation can be non-intersecting). Counts plane partitions, rhombus tilings, etc.
