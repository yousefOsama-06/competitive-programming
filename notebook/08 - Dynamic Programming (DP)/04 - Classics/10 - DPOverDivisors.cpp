// DP OVER THE DIVISOR LATTICE OF ONE NUMBER - the exact analogue of SOS DP, with "submask"
// replaced by "divisor". The divisors of n form a lattice isomorphic to a product of chains (one
// per prime, of length e_p), so the same "one dimension at a time" trick works:
//     for each prime p:  for divisors in increasing order:  g[d] += g[d / p]
// gives g[d] = sum over e | d of f[e] in O(D * omega(n)) instead of O(D^2) pair testing, where
// D = number of divisors (<= 1344 below 1e9) and omega = number of distinct primes (<= 9).
// WHEN: "for every divisor d of n, count the array elements whose gcd with n is exactly d" (the
// mobius direction), "sum over all divisors of a divisor", multiplicative-function DPs restricted
// to one n, and the CF-style "count subsets whose gcd is exactly d".
// INDEX CONVENTION: divs is SORTED ASCENDING and f is indexed by POSITION in that array; pos maps
// a divisor value to its index. Sorted order guarantees d / p comes before d, which is what makes
// the in-place update legal - do not shuffle divs.
struct DivisorLattice {
    vector<ll> divs;
    vector<ll> primes;
    map<ll, int> pos;
    DivisorLattice(ll n, const vector<ll>& primeFactors) : primes(primeFactors) {
        for (ll i = 1; i * i <= n; i++)
            if (n % i == 0) { divs.push_back(i); if (i != n / i) divs.push_back(n / i); }
        sort(divs.begin(), divs.end());
        for (int i = 0; i < (int)divs.size(); i++) pos[divs[i]] = i;
    }
    void zeta(vector<ll>& f) {                                 // g[d] = sum over e | d of f[e]
        for (ll p : primes)
            for (int i = 0; i < (int)divs.size(); i++)
                if (divs[i] % p == 0) f[i] += f[pos[divs[i] / p]];
    }
    void mobius(vector<ll>& f) {                               // exact inverse of zeta
        for (ll p : primes)
            for (int i = (int)divs.size() - 1; i >= 0; i--)
                if (divs[i] % p == 0) f[i] -= f[pos[divs[i] / p]];
    }
    void superZeta(vector<ll>& f) {                            // g[d] = sum over d | e of f[e]
        for (ll p : primes)
            for (int i = (int)divs.size() - 1; i >= 0; i--)
                if (divs[i] % p == 0) f[pos[divs[i] / p]] += f[i];
    }
    void superMobius(vector<ll>& f) {                          // exact inverse of superZeta
        for (ll p : primes)
            for (int i = 0; i < (int)divs.size(); i++)
                if (divs[i] % p == 0) f[pos[divs[i] / p]] -= f[i];
    }
};
/* THE CANONICAL USE
 cnt[d] = number of array elements divisible by d (easy: for each element, add 1 at gcd(a, n));
 then superZeta turns "exactly d" counts into "divisible by d" counts and superMobius turns them
 back. Counting subsets with gcd exactly d is then: ways[d] = 2^(divisible by d) - 1, followed by
 superMobius over the divisor lattice.
 OVER 1..N INSTEAD OF THE DIVISORS OF ONE n: the same transforms are the harmonic loops
   for (d = 1..N) for (m = 2d, 3d, ...) f[m] += f[d]   (divisor zeta,   O(N log N))
   for (d = N..1) for (m = 2d, 3d, ...) f[d] += f[m]   (multiple zeta,  O(N log N))
 and their mobius inverses reverse the inner direction. See 04 - Number Theory/04/02.
 GCD CONVOLUTION c[k] = sum over gcd(i,j) = k of a[i] b[j]: multipleZeta both, multiply pointwise,
 multipleMobius the result. LCM convolution is the same with the divisor transforms.
 WHY NOT JUST TEST ALL PAIRS: D^2 is 1.8e6 for D = 1344, fine once - but inside a loop over
 queries or over n it is not, and the lattice version is 12000 operations.
 THE PRIME LIST must be the DISTINCT primes of n (factorise with 04 - NT/02/06 Pollard). Passing a
 prime twice double-counts and silently corrupts every value. */
