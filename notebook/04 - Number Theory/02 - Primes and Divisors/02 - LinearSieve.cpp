// Linear Sieve - Time: O(N), Space: O(N)
// Finds all prime numbers up to N and computes SPF in exact linear O(N) time
const int MAXN = 1e7;
vector<int> primes;
int spf[MAXN + 1];

void linear_sieve(int n = MAXN) {
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
