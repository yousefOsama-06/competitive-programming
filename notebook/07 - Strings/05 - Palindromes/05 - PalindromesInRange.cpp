// Needs: 07 - Strings/05 - Palindromes/01 - Manacher.cpp
// HOW MANY PALINDROMIC SUBSTRINGS LIE ENTIRELY INSIDE s[l..r]? Offline, O((n + q) log n).
// The whole-string count is one Manacher pass; the RANGE version is the hard part, because a
// centre's contribution is CLIPPED by both ends of the query.
// INDEX CONVENTION (0-based, inclusive): with the interleaved radii p[] of 01 - Manacher.cpp,
//   d1[i] = (p[2i + 1] + 1) / 2 = number of odd palindromes centred AT i  (longest = 2*d1-1)
//   d2[i] = p[2i] / 2           = number of even palindromes centred BETWEEN i-1 and i
// Then the answer for [l, r] is
//   sum over i in [l, r]     of min(d1[i], i - l + 1, r - i + 1)
// + sum over i in [l+1, r]   of min(d2[i], i - l,     r - i + 1).
// SPLIT AT THE MIDPOINT so that only ONE of the two clips can bind: on the left half the binding
// clip is the distance to l, on the right half it is the distance to r. Each half becomes
//   sum over i in a range of ( key[i] >= T ? affine(i) : v[i] )
// with key[i] = v[i] - i (left) or v[i] + i (right) - a threshold sweep with three Fenwicks.
struct PalinRange {
    int n;
    vector<ll> d1, d2;
    PalinRange(const string& s) {
        Manacher M(s);
        n = s.size(), d1.assign(n, 0), d2.assign(n, 0);
        for (int i = 0; i < n; i++) d1[i] = (M.p[2 * i + 1] + 1) / 2, d2[i] = M.p[2 * i] / 2;
    }
    struct BIT {
        vector<ll> f;
        BIT(int n = 0) : f(n + 1, 0) {}
        void add(int i, ll v) { for (i++; i < (int)f.size(); i += i & -i) f[i] += v; }
        ll qr(int i) { ll r = 0; for (i++; i > 0; i -= i & -i) r += f[i]; return r; }
        ll rng(int l, int r) { return l > r ? 0 : qr(r) - qr(l - 1); }
    };
    // sum over i in [ql, qr] of ( key[i] >= T ? c0 + c1 * i : v[i] ), for many queries at once.
    vector<ll> sweep(const vector<ll>& v, const vector<ll>& key,
                     const vector<array<ll, 5>>& qs) {          // {ql, qr, T, c0, c1}
        int q = qs.size();
        vector<ll> res(q, 0), pre(n + 1, 0);
        for (int i = 0; i < n; i++) pre[i + 1] = pre[i] + v[i];
        vector<int> ord(n), qo(q);
        iota(ord.begin(), ord.end(), 0), iota(qo.begin(), qo.end(), 0);
        sort(ord.begin(), ord.end(), [&](int a, int b) { return key[a] > key[b]; });
        sort(qo.begin(), qo.end(), [&](int a, int b) { return qs[a][2] > qs[b][2]; });
        BIT cnt(n), sIdx(n), sVal(n);
        int ptr = 0;
        for (int id : qo) {
            auto [ql, qr, T, c0, c1] = qs[id];
            while (ptr < n && key[ord[ptr]] >= T) {
                int i = ord[ptr++];
                cnt.add(i, 1), sIdx.add(i, i), sVal.add(i, v[i]);
            }
            if (ql > qr) continue;
            ll c = cnt.rng(ql, qr), si = sIdx.rng(ql, qr), sv = sVal.rng(ql, qr);
            res[id] = c0 * c + c1 * si + (pre[qr + 1] - pre[ql] - sv);
        }
        return res;
    }
    vector<ll> solve(const vector<pii>& queries) {              // {l, r} 0-based inclusive
        int q = queries.size();
        vector<ll> km(n), kp(n), res(q, 0);
        vector<array<ll, 5>> a(q), b(q), c(q), d(q);
        for (int i = 0; i < n; i++) km[i] = d1[i] - i, kp[i] = d1[i] + i;
        for (int j = 0; j < q; j++) {
            ll l = queries[j].first, r = queries[j].second, m = (l + r) / 2;
            a[j] = {l, m, 1 - l, 1 - l, 1};                     // odd, left half:  i - l + 1
            b[j] = {m + 1, r, r + 1, r + 1, -1};                // odd, right half: r - i + 1
        }
        auto ra = sweep(d1, km, a), rb = sweep(d1, kp, b);
        for (int i = 0; i < n; i++) km[i] = d2[i] - i, kp[i] = d2[i] + i;
        for (int j = 0; j < q; j++) {
            ll l = queries[j].first, r = queries[j].second, m = (l + r + 1) / 2;
            c[j] = {l + 1, m, -l, -l, 1};                       // even, left half:  i - l
            d[j] = {m + 1, r, r + 1, r + 1, -1};                // even, right half: r - i + 1
        }
        auto rc = sweep(d2, km, c), rd = sweep(d2, kp, d);
        for (int j = 0; j < q; j++) res[j] = ra[j] + rb[j] + rc[j] + rd[j];
        return res;
    }
};
/* NOTES
 WHOLE-STRING COUNT is just sum(d1) + sum(d2) - no sweep needed, and the number of DISTINCT
   palindromic substrings is the node count of an eertree (05 - Palindromes/02).
 ONLINE queries: replace the sweep with a WAVELET TREE (02 - DS/13) that also stores prefix SUMS
   per node, or with a merge-sort tree - same formulas, O(log^2 n) per query, no sorting of
   queries. The offline version above is shorter and faster; use it unless the queries are forced
   to be online.
 PALINDROMES STARTING IN [l, r] (rather than contained in it) is a different and easier sweep:
   each centre contributes a contiguous range of start positions.
 COUNTING DISTINCT palindromes inside a range is much harder (offline eertree + BIT over the
   "last occurrence" trick, like distinct values in a range); do not confuse the two.
 CHECK THE SPLIT POINT: the odd half uses m = (l+r)/2 and the even half m = (l+r+1)/2. Both come
   from solving i - l + 1 <= r - i + 1 and i - l <= r - i + 1; an off-by-one there double counts
   the centre and is the only bug this code ever has. */
