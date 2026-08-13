// HASHING UNDER POINT UPDATES, forward AND backward - the structure that answers "is s[l..r] a
// palindrome" while characters keep changing. Static prefix hashes cannot survive an update, and
// Manacher / eertree cannot be updated at all, so this is the only tool for that query shape.
// Two Fenwick trees over the modular values s[i]*p^i and s[i]*p^(n+1-i): point update O(log n),
// substring hash O(log n), palindrome test O(log n), longest palindrome at a centre O(log^2 n).
// MODULUS 2^61 - 1 with 128-bit multiplication: one modulus of that size is safer than two
// 32-bit ones (birthday bound ~ q^2 / 2^61) and shorter to write.
// INDEX CONVENTION: 1-INDEXED, s[1..n], and every l, r below is inclusive 1-based. get(l, r) is
// scaled so that equal substrings of EQUAL LENGTH compare equal - never compare across lengths.
struct DynHash {
    static const unsigned long long MD = (1ULL << 61) - 1;
    static unsigned long long mul(unsigned long long a, unsigned long long b) {
        __int128 c = (__int128)a * b;
        unsigned long long lo = (unsigned long long)(c & MD), hi = (unsigned long long)(c >> 61);
        lo += hi;
        return lo >= MD ? lo - MD : lo;
    }
    static unsigned long long ad(unsigned long long a, unsigned long long b) {
        return a + b >= MD ? a + b - MD : a + b;
    }
    static unsigned long long sb(unsigned long long a, unsigned long long b) {
        return a >= b ? a - b : a + MD - b;
    }
    static unsigned long long pw(unsigned long long b, unsigned long long e) {
        unsigned long long r = 1;
        for (; e; e >>= 1, b = mul(b, b)) if (e & 1) r = mul(r, b);
        return r;
    }
    int n;
    string s;                                                  // s[1..n]
    vector<unsigned long long> P, IP, ft[2];
    DynHash(const string& in) {
        n = in.size(), s = string(n + 1, 0);                    // s[0] unused; filled by upd below
        unsigned long long base = 131 + (unsigned long long)chrono::steady_clock::now()
                                            .time_since_epoch().count() % 1000000;
        P.resize(n + 2), IP.resize(n + 2), ft[0].assign(n + 1, 0), ft[1].assign(n + 1, 0);
        P[0] = IP[0] = 1;
        unsigned long long ib = pw(base, MD - 2);
        for (int i = 1; i <= n + 1; i++) P[i] = mul(P[i - 1], base), IP[i] = mul(IP[i - 1], ib);
        for (int i = 1; i <= n; i++) upd(i, in[i - 1]);
    }
    void add(int k, int i, unsigned long long v) {
        for (; i <= n; i += i & -i) ft[k][i] = ad(ft[k][i], v);
    }
    unsigned long long qr(int k, int i) {
        unsigned long long r = 0;
        for (; i > 0; i -= i & -i) r = ad(r, ft[k][i]);
        return r;
    }
    unsigned long long rng(int k, int l, int r) { return sb(qr(k, r), qr(k, l - 1)); }
    void upd(int i, char c) {                                  // set s[i] = c
        unsigned long long d = sb(c, s[i]);
        add(0, i, mul(d, P[i])), add(1, i, mul(d, P[n + 1 - i]));
        s[i] = c;
    }
    unsigned long long get(int l, int r) { return mul(rng(0, l, r), IP[l]); }
    unsigned long long rev(int l, int r) { return mul(rng(1, l, r), IP[n + 1 - r]); }
    bool isPal(int l, int r) { return get(l, r) == rev(l, r); }
    int oddRadius(int c) {                                     // longest s[c-k..c+k] palindrome
        int lo = 0, hi = min(c - 1, n - c), res = 0;
        while (lo <= hi) {
            int m = (lo + hi) / 2;
            isPal(c - m, c + m) ? (res = m, lo = m + 1) : hi = m - 1;
        }
        return res;                                            // length 2 * res + 1
    }
    int evenRadius(int c) {                                    // longest s[c-k+1..c+k] palindrome
        int lo = 1, hi = min(c, n - c), res = 0;
        while (lo <= hi) {
            int m = (lo + hi) / 2;
            isPal(c - m + 1, c + m) ? (res = m, lo = m + 1) : hi = m - 1;
        }
        return res;                                            // length 2 * res
    }
};
/* NOTES
 THE SCALING IS THE WHOLE GAME. Forward stores s[i] * p^i, so the raw range sum of [l, r] is
   p^l times the hash of the substring - multiply by p^-l. Backward stores s[i] * p^(n+1-i), whose
   range sum is p^(n+1-r) times the reversed substring's hash. Get one exponent wrong and every
   answer is silently false; test isPal on a known palindrome first.
 A SEGMENT TREE does the same with range ASSIGN ("set s[l..r] to c") lazily, which a Fenwick
   cannot: keep (hash, length) per node and lazily set hash = c * (p^len - 1) / (p - 1).
 SUBSTRING COMPARISON / SORTING under updates: binary search the first differing position with
   get(), O(log^2 n) per comparison - the standard "compare two suffixes with updates".
 LONGEST COMMON EXTENSION under updates is the same binary search, and is what makes this a
   drop-in replacement for a suffix array in dynamic problems.
 ANTI-HASH: the base is randomised at construction. Never hard-code a base in an open-hacking
   contest; with a fixed base and mod 2^61-1 the string is still forgeable by a prepared test. */
