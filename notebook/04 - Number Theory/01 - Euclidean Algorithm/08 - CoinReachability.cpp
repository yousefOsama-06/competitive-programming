// WHICH TOTALS ARE PAYABLE WITH COINS a[0..n-1]?  (all x_i >= 0, unbounded supply)
//   sum a_i x_i = k solvable?   how many solvable k in [l, r]?   largest unsolvable k?
// THE TRICK ("congruence shortest path" / Dijkstra on residues): fix A = min a_i and build a
// graph on the residues mod A with an edge u -> (u + a_i) mod A of weight a_i. Then
//   d[u] = the SMALLEST payable value congruent to u (mod A)
// and k is payable iff d[k mod A] <= k. So every residue class contributes the arithmetic
// progression d[u], d[u] + A, d[u] + 2A, ... and everything below d[u] in that class is not.
// Build O(A * n * log A) time, O(A) memory. Take A = min a_i (that is what bounds the table).
// PRECONDITIONS: all a_i >= 1. Values may be up to 1e18; A up to a few million is the real cap.
struct CoinReach {
    ll A;
    vector<ll> d;                                       // d[u] = min payable value = u (mod A)
    CoinReach(vector<ll> a) {
        A = *min_element(all(a));
        d.assign(A, llinf);
        d[0] = 0;
        priority_queue<pair<ll, ll>, vector<pair<ll, ll>>, greater<>> pq;
        pq.push({0, 0});
        while (!pq.empty()) {
            auto [val, u] = pq.top(); pq.pop();
            if (val != d[u]) continue;
            for (ll c : a) {
                ll nv = val + c, v = nv % A;
                if (nv < d[v]) d[v] = nv, pq.push({nv, v});
            }
        }
    }
    bool payable(ll k) { return k >= 0 && d[k % A] <= k; }
    ll count(ll l, ll r) {                              // #payable k in [l, r], l >= 0
        ll res = 0;
        for (ll u = 0; u < A; u++) {
            if (d[u] <= r) res += (r - d[u]) / A + 1;
            if (d[u] <= l - 1) res -= (l - 1 - d[u]) / A + 1;
        }
        return res;
    }
    ll frobenius() {                                    // largest UNpayable value, or -1
        ll mx = 0;                                      // requires gcd(a_i) = 1, else infinite
        for (ll u = 1; u < A; u++) mx = max(mx, d[u]);
        return mx == 0 ? -1 : mx - A;
    }
};
// FROBENIUS / CHICKEN McNUGGET closed forms exist only for two coins: with gcd(a,b) = 1 the
// largest unpayable value is a*b - a - b and exactly (a-1)(b-1)/2 values are unpayable.
// For three or more coins there is no formula - this table IS the algorithm.
// SANITY: if gcd of all a_i is g > 1, only multiples of g are ever payable; divide everything
// by g first, otherwise frobenius() is meaningless (infinitely many unpayable values).
// SAME TABLE ANSWERS: smallest payable value >= k (that is d[u] + ceil((k - d[u])/A)*A minimised
// over u), and "can I pay exactly k with at most/at least c coins" if you make d 2-dimensional.
