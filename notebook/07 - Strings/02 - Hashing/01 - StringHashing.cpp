// Double-mod polynomial hashing. RANDOM bases => not anti-hackable.
// No modular inverses: h(l,r) is compared already scaled, so get() is 2 mults.
ll HB1, HB2;
struct Hash {
    static const ll M1 = 1000000007, M2 = 998244353;
    int n;
    vector<ll> h1, h2, p1, p2;

    Hash(const string& s) : n(s.size()), h1(n + 1, 0), h2(n + 1, 0), p1(n + 1, 1), p2(n + 1, 1) {
        if (!HB1) {
            mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
            HB1 = 256 + rng() % (M1 - 300), HB2 = 256 + rng() % (M2 - 300);   // FULL range: a base drawn
        // from only 1e6 values gives collision probability n/1e6 per modulus (0.1 at n = 1e5)
        }
        for (int i = 0; i < n; i++) {
            h1[i + 1] = (h1[i] * HB1 + s[i]) % M1, p1[i + 1] = p1[i] * HB1 % M1;
            h2[i + 1] = (h2[i] * HB2 + s[i]) % M2, p2[i + 1] = p2[i] * HB2 % M2;
        }
    }
    pair<ll, ll> get(int l, int r) const {              // [l, r] inclusive, 0-based
        ll a = (h1[r + 1] - h1[l] * p1[r - l + 1]) % M1;
        ll b = (h2[r + 1] - h2[l] * p2[r - l + 1]) % M2;
        return {a < 0 ? a + M1 : a, b < 0 ? b + M2 : b};
    }
    bool eq(int l1, int r1, int l2, int r2) const {
        return r1 - l1 == r2 - l2 && get(l1, r1) == get(l2, r2);
    }
    // concat(A over lenA, B): h = hA * base^lenB + hB   (works per modulus)
};
// USES: substring equality O(1) * longest common prefix of two suffixes by binary search
//       * dedup of substrings * 2D grid patterns (hash rows, then hash the row-hashes)
//       * tree isomorphism (hash the sorted multiset of child hashes)
