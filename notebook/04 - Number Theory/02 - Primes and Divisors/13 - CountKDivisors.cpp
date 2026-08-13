// Needs: PCF::lehmer, PCF::primes (12 - MeisselLehmer.cpp).
// HOW MANY x <= n HAVE EXACTLY k DIVISORS?  n up to ~1e12, k up to a few thousand.
// A number with exactly k divisors has the shape p1^(e1-1) * p2^(e2-1) * ... where e1*e2*... = k
// and (choosing the exponents in decreasing order) the primes can be taken increasing. So we
// backtrack over the FACTORISATIONS of k into factors > 1, assigning a prime to each factor.
// Two things make it fast: (a) the case k = 2 is just "count the primes <= n", answered by
// Meissel-Lehmer, and (b) a single remaining factor x means "count primes p with p^(x-1) <= n",
// answered by a binary search over the prime list.
// PRECONDITIONS: call PCF::init() first, and prep() once for the k range you need. Sieve limit
// LIM in PCF must exceed n^(1/2) for the binary searches to be safe (5e6 covers n <= 2.5e13).
// COMPLEXITY: hard to state - it is a search whose branching is bounded by the divisors of k;
// in practice a handful of milliseconds per query for n = 1e12.
namespace KDiv {
    const int MK = 2100;                                // max k + 1
    vector<vector<int>> dv;                             // dv[k] = divisors of k that are > 1, desc
    vector<int> minexp;                                 // minexp[k] = sum (p-1) over primes p | k
    void prep() {
        dv.assign(MK, {}), minexp.assign(MK, 0);
        for (int i = 2; i < MK; i++) {
            for (int j = i; j < MK; j += i) dv[j].push_back(i);
            for (int p : PCF::primes) {
                if (p > i) break;
                int cur = i;
                while (cur % p == 0) cur /= p, minexp[i] += p - 1;
            }
        }
        for (int i = 2; i < MK; i++) reverse(all(dv[i]));   // try the big exponents first
    }
    ll powCap(ll b, int e, ll cap) {                    // b^e, saturating at cap + 1
        ll r = 1;
        for (int i = 0; i < e; i++) { if (r > cap / b) return cap + 1; r *= b; }
        return r;
    }
    // count of x <= n with exactly k divisors, using only primes from index `first` on
    ll go(ll n, int k, int first = 0) {
        if (n <= 0 || k >= MK) return 0;
        if (k == 1) return 1;
        if (n == 1) return 0;
        if (k == 2) return max(0LL, PCF::lehmer(n) - first);
        if (dv[k].size() == 1) {                        // k is prime: one prime power p^(k-1)
            int e = dv[k][0] - 1;
            if (powCap(PCF::primes[first], e, n) > n) return 0;
            int lo = first, hi = (int)PCF::primes.size() - 1, ans = first - 1;
            while (lo <= hi) {                          // last prime with p^e <= n
                int mid = (lo + hi) / 2;
                if (powCap(PCF::primes[mid], e, n) > n) hi = mid - 1;
                else lo = mid + 1, ans = mid;
            }
            return ans - first + 1;
        }
        ll res = 0;
        for (int x : dv[k])                             // this prime carries exponent x - 1
            for (int i = first; i < (int)PCF::primes.size(); i++) {
                ll p = powCap(PCF::primes[i], x - 1, n);
                if (p > n) break;
                if (k / x == 1) { res++; continue; }
                if (powCap(PCF::primes[i + 1], minexp[k / x], n) > n / p) break;
                res += go(n / p, k / x, i + 1);
            }
        return res;
    }
}
// RANGE QUERY: answer(l, r) = go(r, k) - go(l - 1, k).
// SMALL n: just sieve tau with a linear sieve (07 - MultiplicativeSieve.cpp) and count.
// SMALLEST number with exactly k divisors is a different search - 15 - SmallestWithKDivisors.cpp.
// FACTS: tau(x) is odd iff x is a perfect square; the count of x <= n with tau(x) = 2 is pi(n),
// with tau(x) = 3 it is pi(sqrt n), and with tau(x) = 4 it is pi(cbrt n) plus the number of
// products p*q of two distinct primes below n.
