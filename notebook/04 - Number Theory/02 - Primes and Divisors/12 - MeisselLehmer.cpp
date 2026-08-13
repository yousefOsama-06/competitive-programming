// MEISSEL-LEHMER PRIME COUNTING - pi(n) in about O(n^(2/3) / log n), well under a second at
// n = 1e12 and a few seconds at 1e14. Use it when you need pi(n) MANY times, or for n past the
// comfort zone of Lucy's O(n^(3/4)) method (04 - Theorems/03 - PrimeCounting.cpp) - but Lucy
// stays the better choice when you also want the SUM of the primes, which this cannot give.
// phi(n, k) = count of i <= n free of the first k primes; the recursion phi(n, k) =
// phi(n, k-1) - phi(n/p_k, k-1) is memoised in a table for small arguments, and Lehmer's
// identity peels off the numbers with exactly two or three prime factors.
// CALL init() ONCE. Memory: LIM ints for the prefix table plus PN*PK ints (about 30 MB with the
// constants below) - shrink PN/PK if the limit is tight, raise LIM if n is huge.
namespace PCF {
    const int LIM = 5000000, PN = 40000, PK = 60;       // sieve bound; memo table dimensions
    vector<int> primes, pref;                           // pref[i] = pi(i) for i < LIM
    vector<vector<int>> memo;
    ll isqrt(ll x) { ll r = (ll)sqrtl((ld)x); while (r * r > x) r--; while ((r+1)*(r+1) <= x) r++;
                     return r; }
    ll icbrt(ll x) { ll r = (ll)cbrtl((ld)x); while (r * r * r > x) r--;
                     while ((r+1)*(r+1)*(r+1) <= x) r++; return r; }
    void init() {
        vector<bool> comp(LIM, false);
        pref.assign(LIM, 0);
        for (int i = 2; i < LIM; i++) {
            if (!comp[i]) { primes.push_back(i);
                            for (ll j = (ll)i * i; j < LIM; j += i) comp[j] = 1; }
            pref[i] = primes.size();
        }
        memo.assign(PN, vector<int>(PK));
        for (int i = 0; i < PN; i++) memo[i][0] = i;
        for (int k = 1; k < PK; k++)
            for (int i = 0; i < PN; i++)
                memo[i][k] = memo[i][k - 1] - memo[i / primes[k - 1]][k - 1];
    }
    ll phi(ll n, int k) {                          // count i <= n free of the first k primes
        if (n < PN && k < PK) return memo[n][k];
        if (k == 1) return (n + 1) >> 1;
        if (primes[k - 1] >= n) return 1;
        return phi(n, k - 1) - phi(n / primes[k - 1], k - 1);
    }
    ll legendre(ll n) {                                 // simplest version, O(n^(2/3) log n)-ish
        if (n < LIM) return pref[n];
        int k = pref[isqrt(n)];
        return phi(n, k) + k - 1;
    }
    ll lehmer(ll n) {                                   // the fast one
        if (n < LIM) return pref[n];
        ll b = isqrt(n), c = lehmer(icbrt(n)), a = lehmer(isqrt(b));
        b = lehmer(b);
        ll res = phi(n, a) + (b + a - 2) * (b - a + 1) / 2;
        for (ll i = a; i < b; i++) {
            ll w = n / primes[i];
            res -= lehmer(w);
            if (i <= c) {
                ll lim = lehmer(isqrt(w));
                for (ll j = i; j < lim; j++) res += j - lehmer(w / primes[j]);
            }
        }
        return res;
    }
}
// SANITY VALUES: pi(1e6) = 78498, pi(1e9) = 50847534, pi(1e12) = 37607912018,
//                pi(1e13) = 346065536839, pi(1e15) = 29844570422669.
// pi(n) ~ n / ln n, and li(n) is a much better estimate; the k-th prime is ~ k(ln k + ln ln k).
// NEXT STEPS: the Lagarias-Miller-Odlyzko and Deleglise-Rivat refinements reach n^(2/3)/log^2 and
// are what a real prime-counting program uses - not worth writing during a contest.
