// RYSER'S FORMULA - permanent of an n x n matrix in O(2^n * n).
// perm(A) counts PERFECT MATCHINGS of a bipartite graph (weighted: sum over matchings of the
// product of weights). Use for n <= ~20: "count ways to assign n tasks to n people",
// "count systems of distinct representatives", "count permutations avoiding forbidden cells".
Mint permanent(const vector<vector<Mint>>& a) {
    int n = a.size();
    vector<Mint> row(n, 0);                            // row[i] = sum_{j in S} a[i][j]
    Mint total = 0;
    int prev = 0;
    for (int g = 1; g < (1 << n); g++) {               // Gray code: one column toggles per step
        int cur = g ^ (g >> 1), diff = cur ^ prev, j = __builtin_ctz(diff);
        if (cur & diff) for (int i = 0; i < n; i++) row[i] += a[i][j];   // column j entered S
        else for (int i = 0; i < n; i++) row[i] -= a[i][j];              // column j left S
        prev = cur;
        Mint p = 1;
        for (int i = 0; i < n; i++) p *= row[i];
        (__builtin_popcount(cur) & 1) ? total -= p : total += p;
    }
    return (n & 1) ? Mint(0) - total : total;          // perm = (-1)^n * sum_S (-1)^|S| prod_i rowSum
}
// PERMANENT vs DETERMINANT: identical formula without the signs. det is polynomial-time,
// perm is #P-hard - never expect an O(n^3) permanent.

// NEWTON'S IDENTITIES - convert between power sums p_k = sum x_i^k and elementary symmetric e_k.
// e_k = sum over k-subsets of the product; this is exactly "for every k, the sum over all
// k-subsets of the product of the chosen values".
//   e_0 = 1;   k * e_k = sum_{i=1..k} (-1)^(i-1) * e_(k-i) * p_i
vector<Mint> powerToElementary(const vector<Mint>& p, int n) {   // p[1..n] -> e[0..n]
    vector<Mint> e(n + 1, 0);
    e[0] = 1;
    for (int k = 1; k <= n; k++) {
        Mint s = 0;
        for (int i = 1; i <= k; i++) {
            Mint t = e[k - i] * p[i];
            (i & 1) ? s += t : s -= t;
        }
        e[k] = s * inv_[k];
    }
    return e;
}
//   p_k = (-1)^(k-1) * k * e_k + sum_{i=1..k-1} (-1)^(k-1+i) * e_(k-i) * p_i
vector<Mint> elementaryToPower(const vector<Mint>& e, int n) {   // e[0..n] -> p[1..n]
    vector<Mint> p(n + 1, 0);
    for (int k = 1; k <= n; k++) {
        Mint s = Mint(k) * e[k];
        if ((k - 1) & 1) s = Mint(0) - s;              // (-1)^(k-1) * k * e_k
        for (int i = 1; i < k; i++) {
            Mint t = e[k - i] * p[i];
            ((k - 1 + i) & 1) ? s -= t : s += t;
        }
        p[k] = s;
    }
    return p;
}
// e_k are also the coefficients of prod (x + x_i): prod_{i} (x + x_i) = sum_k e_k * x^(n-k).
// So "sum over k-subsets of the product" = coefficient extraction from that product, which you
// can also get in O(n log^2 n) by divide-and-conquer multiplication of the linear factors.
