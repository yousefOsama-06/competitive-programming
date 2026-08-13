// Fast Bitset Sieve up to N = 10^9 - Time: ~0.3s for 10^9, Space: N / 16 bytes (~60MB)
// Stores odd numbers only (index i represents number 2*i + 1)
// WARNING: the bitset is ~60 MB - the instance MUST be global/static, never a local.
template <int MAXPR = (int)1e9>
struct Sieve1e9 {
    bitset<MAXPR / 2> is_prime_odd;

    Sieve1e9(int n = MAXPR) {
        is_prime_odd.set();
        is_prime_odd[0] = 0; // 1 is not prime
        for (int i = 1; (2 * i + 1) * (2 * i + 1) <= n; i++) {
            if (is_prime_odd[i]) {
                int p = 2 * i + 1;
                // bound is (n-1)/2, NOT n/2: n/2 writes one index past the end of the bitset
                for (int j = 2 * i * (i + 1); j <= (n - 1) / 2; j += p) {
                    is_prime_odd[j] = 0;
                }
            }
        }
    }

    bool is_prime(int x) const {
        if (x == 2) return true;
        if (x < 2 || x % 2 == 0) return false;
        return is_prime_odd[x / 2];
    }
};
