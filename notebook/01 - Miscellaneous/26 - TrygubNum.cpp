// TRYGUB NUMBER - a big number that eats 1e5+ updates of the form "add x * b^e" in O(1)
// amortized carries, and answers "what is digit k" at any time.
// WHAT: maintains V = sum over e of d[e] * base^e exactly, where base = b^K packs K base-b digits
// into one machine digit. add(x, e) does V += x * b^e with x AND e of either sign. kthDigit(k)
// returns floor(V / b^k) mod b, i.e. the k-th digit of V in base b counting from the units digit
// at k = 0. sign() returns +1 / 0 / -1.
// THE TRICK: digits are NOT normalised. d[e] may be anywhere in (-base, base), the carry is
// pushed lazily one position at a time and stops as soon as it dies, and only NONZERO digits are
// stored, in a map - so a negative e (a fractional position) costs nothing and the number needs
// no length. Amortized it is O(1) carry steps per add (binary-counter argument), each step one
// map operation, so O(log D) with D = number of stored digits. Memory O(D).
// WHEN: 1e5 updates "add 5 * 10^k" to a huge number with digit queries interleaved. A plain
// vector is O(len) per update; a segment tree over digits drowns in carry logic. Also the clean
// way to compare two such numbers: subtract, look at sign().
// NEGATIVE e: split e = q*K + j with FLOOR division, not C++ truncation. For e = -1, K = 30 the
// language gives e/K == 0 and e%K == -1, which would multiply x by b^(-1). Take
// j = ((e % K) + K) % K first, then q = (e - j) / K, which is exact in both signs.
// NEGATIVE x: nothing special in add - integer division truncates toward zero, so after
// d[q] -= t * base the residue KEEPS the sign of d[q] and |d[q]| < base. That is exactly how
// negative digits appear, and it is why kthDigit needs the borrow below.
// THE BORROW RULE in kthDigit (the whole difficulty): the digits below q may sum to a negative
// number. Their total is bounded by base^q in absolute value (sum of (base-1)*base^e for e < q
// is base^q - 1), so the top stored digit below q decides the SIGN of the whole low part, and a
// negative low part eats exactly 1 from position q: `if (prev(it)->se < 0) a--`. This is why
// zeros MUST be erased from the map - a stored 0 sitting just under q would answer "not
// negative" and lose the borrow.
// OVERFLOW BUDGET: a digit lives in (-base, base) and add pushes x * b^(K-1) into it, so you
// need base + |x| * b^(K-1) < 9.2e18. b = 2, K = 30 (base ~ 1.07e9) and b = 10, K = 9
// (base = 1e9) both leave room for |x| <= 1e9. Bigger K = shorter carry chains, so take the
// largest one the budget allows.
struct TrygubNum {
    ll b, K, base; vector<ll> pw;                      // pw[i] = b^i, base = b^K = one machine
    map<ll, ll> d;                                     // digit. e -> digit, ZEROS ARE ERASED
    TrygubNum(ll b, ll K) : b(b), K(K), pw(K + 1, 1) {
        for (ll i = 1; i <= K; i++) pw[i] = pw[i - 1] * b;
        base = pw[K];
    }
    void add(ll x, ll e) {                             // V += x * b^e, both signs allowed
        ll j = ((e % K) + K) % K, q = (e - j) / K;     // FLOOR split e = q*K + j, 0 <= j < K
        d[q] += x * pw[j];
        ll t;
        do {
            t = d[q] / base;                           // truncates toward 0: residue keeps sign
            d[q] -= t * base, d[q + 1] += t;
            if (!d[q]) d.erase(q);                     // erasing zeros is load-bearing, see above
            q++;
        } while (t);                                   // carry died -> stop, O(1) amortized
        if (!d[q]) d.erase(q);                         // the digit above may have been left at 0
    }
    int kthDigit(ll k) {                               // floor(V / b^k) mod b, k >= 0
        ll q = k / K, a = 0;
        auto it = d.lower_bound(q);
        if (it != d.end() && it->fi == q) a = it->se;
        if (it != d.begin() && prev(it)->se < 0) a--;  // low part is negative: it borrows a 1
        a = ((a % base) + base) % base;
        return (int)(a / pw[k % K] % b);               // the (k % K)-th base-b digit of a
    }
    int sign() {                                       // top stored digit dominates all the rest
        return d.empty() ? 0 : (prev(d.end())->se > 0 ? 1 : -1);
    }
};
// DEGENERATE: empty number -> sign() 0 and kthDigit() 0 everywhere. add(0, e) stores nothing.
// Adding and then subtracting the same term returns the map to empty. k above the top digit
// gives 0. kthDigit is meant for V >= 0; for V < 0 it still returns floor(V / b^k) mod b, which
// is the b-complement expansion (...bbb1 for -1), not the digits of |V| - negate first.
// VARIANTS / RELATED
// - Reading the whole number: from prev(d.end())->fi * K + K - 1 downward through kthDigit.
// - Only nonnegative e and no queries until the end: just use a vector and normalise once.
// - Full arithmetic (multiply, divide, print): 21 - BigInt.cpp. This struct only does += x*b^e.
// - Same lazy-carry idea works for "add x at position e" in any positional structure; the map is
//   the only reason it stays sparse.
