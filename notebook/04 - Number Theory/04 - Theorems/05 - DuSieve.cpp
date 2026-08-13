// DU SIEVE (Dirichlet prefix sums) - prefix sums of a multiplicative f in SUBLINEAR time.
// Pick g with an easy prefix sum G and an easy (f*g) prefix sum; then
//     g(1)*S(n) = sum_{i=1..n} (f*g)(i)  -  sum_{i=2..n} g(i) * S(floor(n/i))
// Sieve S(x) directly for x <= LIM (take LIM ~ n^(2/3)); recurse with memoisation above that.
// Total O(n^(2/3)) with the sieve, O(n^(3/4)) without.
//   mu * 1  = eps  ->  M(n)   = 1 - sum_{i=2..n} M(n/i)
//   phi * 1 = Id   ->  Phi(n) = n(n+1)/2 - sum_{i=2..n} Phi(n/i)
struct DuSieve {
    ll LIM;
    vector<ll> mu, phi;                               // prefix sums for x <= LIM
    unordered_map<ll, ll> memoM, memoP;

    DuSieve(ll n) {
        LIM = max((ll)1000, (ll)powl((ld)n, 2.0L / 3));
        vector<int> spf(LIM + 1, 0), primes, m(LIM + 1, 0), p(LIM + 1, 0);
        m[1] = p[1] = 1;
        for (ll i = 2; i <= LIM; i++) {
            if (!spf[i]) spf[i] = i, primes.push_back(i), m[i] = -1, p[i] = i - 1;
            for (int q : primes) {
                if (q > spf[i] || i * q > LIM) break;
                spf[i * q] = q;
                if (q == spf[i]) m[i * q] = 0, p[i * q] = p[i] * q;
                else m[i * q] = -m[i], p[i * q] = p[i] * (q - 1);
            }
        }
        mu.assign(LIM + 1, 0), phi.assign(LIM + 1, 0);
        for (ll i = 1; i <= LIM; i++) mu[i] = mu[i - 1] + m[i], phi[i] = phi[i - 1] + p[i];
    }
    ll M(ll n) {                                      // sum_{i=1..n} mu(i)   (Mertens)
        if (n <= LIM) return mu[n];
        auto it = memoM.find(n);
        if (it != memoM.end()) return it->second;
        ll r = 1;
        for (ll l = 2, rr; l <= n; l = rr + 1) { rr = n / (n / l); r -= (rr - l + 1) * M(n / l); }
        return memoM[n] = r;
    }
    ll P(ll n) {                                      // sum_{i=1..n} phi(i)
        if (n <= LIM) return phi[n];
        auto it = memoP.find(n);
        if (it != memoP.end()) return it->second;
        ll r = n % 2 ? (n + 1) / 2 * n : n / 2 * (n + 1);
        for (ll l = 2, rr; l <= n; l = rr + 1) { rr = n / (n / l); r -= (rr - l + 1) * P(n / l); }
        return memoP[n] = r;
    }
};
// USES: squarefree count up to n = sum_{d<=sqrt n} mu(d) * floor(n/d^2);
//       #coprime pairs up to n = 2*Phi(n) - 1;  sum of gcd/lcm over pairs at n ~ 1e10;
//       any "sum over 1..n of a multiplicative function" that a linear sieve cannot reach.
