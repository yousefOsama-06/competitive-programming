// Linear Sieve & Smallest Prime Factor (SPF) - Time: O(N), Space: O(N)
// Computes primes list, SPF array, primality test, and O(log N) prime factorization
struct Sieve {
    int n;
    vector<int> primes;
    vector<int> spf;

    Sieve(int n = 1e7) : n(n), spf(n + 1, 0) {
        for (int i = 2; i <= n; i++) {
            if (spf[i] == 0) {
                spf[i] = i;
                primes.pb(i);
            }
            for (int p : primes) {
                if (p > spf[i] || (ll)i * p > n) break;
                spf[i * p] = p;
            }
        }
    }

    bool is_prime(int x) const {
        return x >= 2 && x <= n && spf[x] == x;
    }

    // O(log N) prime factorization using precomputed SPF array
    vector<int> get_prime_factors(int x) const {
        vector<int> factors;
        while (x > 1) {
            factors.pb(spf[x]);
            x /= spf[x];
        }
        return factors;
    }
};