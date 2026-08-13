// Linear sieve computing ANY multiplicative function for all i <= n in O(N).
// Shipped: spf, phi, mu, tau (#divisors), sigma (sum of divisors).
// To add your own f: you need f(p), f(p^k) and f(a*b)=f(a)f(b) for coprime a,b.
struct MultSieve {
    int n;
    vector<int> spf, primes, phi, mu, tau, cnt;         // cnt[i] = exponent of spf[i] in i
    vector<ll> sigma, pw;                               // pw[i] = spf[i]^cnt[i]

    MultSieve(int n) : n(n), spf(n + 1), phi(n + 1), mu(n + 1), tau(n + 1), cnt(n + 1),
                       sigma(n + 1), pw(n + 1) {
        phi[1] = mu[1] = tau[1] = sigma[1] = pw[1] = 1;
        for (int i = 2; i <= n; i++) {
            if (!spf[i]) {
                spf[i] = i, primes.push_back(i);
                phi[i] = i - 1, mu[i] = -1, tau[i] = 2, cnt[i] = 1, pw[i] = i, sigma[i] = i + 1LL;
            }
            for (int p : primes) {
                if (p > spf[i] || (ll)i * p > n) break;
                int j = i * p;
                spf[j] = p;
                if (p == spf[i]) {                      // p divides i: same prime, exponent grows
                    cnt[j] = cnt[i] + 1, pw[j] = pw[i] * p;
                    phi[j] = phi[i] * p;
                    mu[j] = 0;
                    tau[j] = tau[i] / (cnt[i] + 1) * (cnt[j] + 1);
                    sigma[j] = sigma[i] / ((pw[i] * p - 1) / (p - 1)) * ((pw[j] * p - 1) / (p - 1));
                } else {                                // coprime: multiply
                    cnt[j] = 1, pw[j] = p;
                    phi[j] = phi[i] * (p - 1);
                    mu[j] = -mu[i];
                    tau[j] = tau[i] * 2;
                    sigma[j] = sigma[i] * (p + 1);
                }
            }
        }
    }
    vector<int> factors(int x) const {                  // O(log x) via spf
        vector<int> f;
        while (x > 1) f.push_back(spf[x]), x /= spf[x];
        return f;
    }
};
