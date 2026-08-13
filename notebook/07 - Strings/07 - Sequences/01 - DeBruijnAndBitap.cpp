// DE BRUIJN SEQUENCES and BIT-PARALLEL MATCHING.

// --- DE BRUIJN SEQUENCE B(k, n): a CYCLIC string over an alphabet of size k, of length k^n, in
// which every one of the k^n words of length n appears exactly once as a (cyclic) substring.
// USES: "open the safe with the fewest keypresses" (the classic), minimum-length probe sequences,
// and hashing tricks (a de Bruijn constant is how you find the index of the lowest set bit).
// CONSTRUCTION (FKM / Lyndon): concatenate, in lexicographic order, every Lyndon word whose
// length DIVIDES n. That is exactly what this O(k^n) recursion emits.
string deBruijn(int k, int n) {
    string s;
    vector<int> a(k * n, 0);
    function<void(int, int)> db = [&](int t, int p) {
        if (t > n) {
            if (n % p == 0) for (int i = 1; i <= p; i++) s += char('0' + a[i]);
        } else {
            a[t] = a[t - p];
            db(t + 1, p);
            for (int j = a[t - p] + 1; j < k; j++) a[t] = j, db(t + 1, t);
        }
    };
    db(1, 1);
    return s;                                                  // length k^n, read cyclically
}
// The same object is an EULERIAN CIRCUIT in the de Bruijn graph (vertices = words of length n-1,
// edges = words of length n), so Hierholzer on that graph is the alternative construction - and
// the BEST theorem then counts how many distinct de Bruijn sequences exist: (k!)^(k^(n-1)) / k^n.

// --- BITAP / SHIFT-AND: exact matching in O(n*m/64) with bitsets, and the natural home for
// approximate matching. Build one bitmask per character marking where it occurs in the pattern;
// then one pass over the text ANDs shifted copies together.
// Beats KMP only when you need the extra flexibility (k mismatches, wildcards, "any of these
// patterns"), or when you want ALL occurrences as a bitset to intersect with a range.
vector<int> bitap(const string& t, const string& p) {
    int n = t.size(), m = p.size();
    if (!m || m > n) return {};
    vector<unsigned long long> mask(256, 0);
    // Works for m <= 64 with a single word; for longer patterns use bitset<M> and shift it.
    if (m > 64) return {};
    for (int i = 0; i < m; i++) mask[(unsigned char)p[i]] |= 1ULL << i;
    unsigned long long st = 0, hit = 1ULL << (m - 1);
    vector<int> res;
    for (int i = 0; i < n; i++) {
        st = ((st << 1) | 1) & mask[(unsigned char)t[i]];
        if (st & hit) res.push_back(i - m + 1);
    }
    return res;
}
/* WHAT BITAP GENERALISES TO, which is the reason to carry it
 k MISMATCHES: keep k+1 state words R0..Rk, where Rj = "matched a prefix with at most j errors".
   Rj_new = ((Rj << 1) & mask[c]) | Rj-1 | (Rj-1 << 1) | (Rj-1_old << 1) | 1
   covering substitution, insertion and deletion respectively - that is Wu-Manber, O(k n m / 64).
 WILDCARDS in the PATTERN: set mask[c] bits for every character the class accepts. A '?' just
   means "every mask has that bit". This is free - no FFT needed - and is the right tool when the
   alphabet is small and the pattern is short. Use the FFT method (01 - String Matching/04) when
   the pattern is long or wildcards appear in the TEXT too.
 MULTIPLE PATTERNS of the same length: OR their masks and check which one actually matched.
 OCCURRENCES INSIDE [l, r]: keep the whole occurrence set as a bitset and popcount a range - that
   is what makes "how many times does p occur in t[l..r]" O(n/64) per query with no suffix
   structure at all.
 MYERS' BIT-VECTOR ALGORITHM computes full edit distance in O(n m / 64) with the same idea
   (carry-save arithmetic on the DP's difference vectors); it is the fastest practical exact edit
   distance and the escape hatch when the O(nm) DP is 1e10.  */
