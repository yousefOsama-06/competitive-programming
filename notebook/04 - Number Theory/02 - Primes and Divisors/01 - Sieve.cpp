// Linear Sieve & Smallest Prime Factor (SPF) - Time: O(N), Space: O(N)
// Computes primes list, SPF array, primality test, and O(log N) prime factorization
const int MAXN = 1e7;
vector<int> primes;
int spf[MAXN + 1];

void sieve(int n = MAXN) {
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

bool is_prime(int x) {
    return x >= 2 && spf[x] == x;
}

// O(log N) prime factorization using precomputed SPF array
vector<int> get_prime_factors(int n) {
    vector<int> factors;
    while (n > 1) {
        factors.pb(spf[n]);
        n /= spf[n];
    }
    return factors;
}