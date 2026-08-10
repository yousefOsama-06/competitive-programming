// Burnside's Lemma & Pólya Enumeration Theorem - Time: O(N)
// Counts distinct colorings under symmetry group G
// Formula: Number of orbits = (1 / |G|) * sum_{g in G} (k ^ number_of_cycles(g))
struct BurnsideLemma {
    // Calculates number of distinct necklets of length n with k colors under rotations
    static ll necklace(int n, int k) {
        ll total = 0;
        for (int i = 0; i < n; i++) {
            total += fast_pow(k, std::gcd(i, n));
        }
        return total / n;
    }

    // Calculates number of distinct bracelets of length n with k colors under rotation + reflection
    static ll bracelet(int n, int k) {
        ll total = 0;
        // Rotations
        for (int i = 0; i < n; i++) {
            total += fast_pow(k, std::gcd(i, n));
        }
        // Reflections
        if (n & 1) {
            total += (ll)n * fast_pow(k, (n + 1) / 2);
        } else {
            total += (ll)(n / 2) * fast_pow(k, n / 2) + (ll)(n / 2) * fast_pow(k, n / 2 + 1);
        }
        return total / (2 * n);
    }

private:
    static ll fast_pow(ll base, ll exp) {
        ll res = 1;
        while (exp > 0) {
            if (exp & 1) res *= base;
            base *= base;
            exp >>= 1;
        }
        return res;
    }
};
