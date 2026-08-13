// Needs: Mint, buildFact, C (06 - Math/01 - Modular Arithmetic/01 - Mint.cpp).
// PARTIAL SUMS OF A BINOMIAL ROW:  f(n, k) = C(n,0) + C(n,1) + ... + C(n,k), for q queries.
// There is NO closed form for f(n, k), but both coordinates move in O(1):
//   f(n+1, k) = 2*f(n, k) - C(n, k)        f(n, k+1) = f(n, k) + C(n, k+1)
//   f(n-1, k) = (f(n, k) + C(n-1, k)) / 2  f(n, k-1) = f(n, k) - C(n, k)
// so treat the queries as points (n, k) and run MO'S ALGORITHM on them: O((n + q) sqrt n) with a
// tiny constant, versus O(n) per query naively.
// REACH FOR IT when a problem asks "probability that at most k of n coins came up heads" or
// "number of subsets of size <= k" for many (n, k) pairs offline.
// PRECONDITIONS: MOD prime, buildFact() called, all n < N. Queries must be answered OFFLINE.
vector<Mint> binomPrefixQueries(vector<pair<int, int>> qs) {
    int q = qs.size(), B = max(1, (int)(N / max(1.0, sqrt((double)q))));
    vector<int> ord(q);
    iota(all(ord), 0);
    sort(all(ord), [&](int x, int y) {
        int bx = qs[x].first / B, by = qs[y].first / B;
        if (bx != by) return bx < by;
        return (bx & 1) ? qs[x].second > qs[y].second : qs[x].second < qs[y].second;
    });
    vector<Mint> ans(q);
    Mint cur = 1, inv2 = Mint((MOD + 1) / 2);           // f(1, 0) = C(1, 0) = 1
    int n = 1, k = 0;
    for (int i : ord) {
        auto [L, R] = qs[i];
        while (n < L) cur = cur * 2 - C(n, k), n++;
        while (n > L) --n, cur = (cur + C(n, k)) * inv2;
        while (k < R) ++k, cur += C(n, k);
        while (k > R) cur -= C(n, k), k--;
        ans[i] = cur;
    }
    return ans;
}
// EXACT VALUES you can use as identities instead, when they apply:
//   f(n, n) = 2^n;  f(n, k) = 2^n - f(n, n-k-1);  f(2m, m) = (2^(2m) + C(2m, m)) / 2.
// A SINGLE query with n up to 1e18 and k small is just Cbig summed k times.
// The same Mo trick works for any quantity with O(1) moves in both indices - partial sums of
// C(n,i)*x^i, of Catalan-like rows, or of a fixed row of any Pascal-style triangle.
