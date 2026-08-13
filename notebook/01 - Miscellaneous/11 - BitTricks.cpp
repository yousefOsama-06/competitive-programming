/* ===================== BIT MANIPULATION CHEAT SHEET =====================
 BUILTINS (add ll suffix for 64-bit: __builtin_popcountll, ...)
   __builtin_popcount(x)  #set bits        __builtin_ctz(x)  #trailing zeros (x != 0)
   __builtin_clz(x)       #leading zeros   __builtin_parity(x) popcount & 1
   __lg(x) = 63 - clzll(x) = floor(log2 x)     (GCC only; write it yourself elsewhere)
   lowest set bit: x & -x        clear it: x &= x - 1        set/flip: x |= b, x ^= b
   is power of two: x && !(x & (x - 1))
   round UP to a power of two (x >= 1): x == 1 ? 1ULL : 2ULL << (63 - __builtin_clzll(x - 1))
     - clzll(0) is UNDEFINED, so x = 1 must be special-cased, and the shift must be 64-bit

 SUBMASK ENUMERATION - iterate every subset of m:
   for (int s = m; ; s = (s - 1) & m) { ...; if (!s) break; }
   Over ALL masks this is O(3^n) total (each element is in/out/neither).

 SUPERMASK enumeration (every s with m subset of s, within n bits):
   for (int s = m; s < (1 << n); s = (s + 1) | m) { ... }

 K-SUBSETS in increasing order (Gosper's hack):
   for (int s = (1 << k) - 1; s < (1 << n); ) { ...;
       int c = s & -s, r = s + c; s = (((r ^ s) >> 2) / c) | r; }

 GRAY CODE  g(i) = i ^ (i >> 1)     inverse: x ^= x >> 1 repeatedly (see 06 - grayCode.cpp)
   consecutive gray codes differ in exactly one bit - use for "change one thing at a time".

 XOR FACTS
   x ^ x = 0, x ^ 0 = x, xor is its own inverse -> prefix xor answers range xor
   xor of 0..n = [n, 1, n+1, 0][n % 4]
   a + b = (a ^ b) + 2*(a & b)     a | b = (a ^ b) + (a & b)     a + b = (a | b) + (a & b)
   "OR of a range equals its SUM"  <=>  the values are pairwise BIT-DISJOINT
     (so at most 30 non-zero values fit in any such window -> two pointers)

 COUNTING PER BIT - the standard decomposition: handle each of the 30/60 bits independently.
   sum over pairs of (a_i ^ a_j) = sum over bits b of (#with bit) * (#without bit) * 2^b

 BITSET
   std::bitset<N> b; b._Find_first(), b._Find_next(i)  (GCC) - iterate set bits fast
   b <<= k, b |= other  are O(N/64) - this is what makes bitset DP and reachability work.
========================================================================= */

// Iterate all submasks of m (including m and 0).
template <class F> void forEachSubmask(int m, F f) {
    for (int s = m;; s = (s - 1) & m) { f(s); if (!s) break; }
}
// All n-bit masks with exactly k bits set, in increasing order (Gosper's hack).
vector<int> kSubsets(int n, int k) {
    vector<int> r;
    if (k == 0) return {0};
    for (int s = (1 << k) - 1; s < (1 << n);) {
        r.push_back(s);
        int c = s & -s, x = s + c;
        s = (((x ^ s) >> 2) / c) | x;
    }
    return r;
}
// Sum over all pairs (i<j) of (a_i XOR a_j), by handling each bit independently.
ll pairXorSum(const vector<ll>& a, int B = 62) {
    ll n = a.size(), r = 0;
    for (int b = 0; b < B; b++) {
        ll c = 0;
        for (ll x : a) c += (x >> b) & 1;
        r += c * (n - c) % MOD * ((1LL << b) % MOD) % MOD;
    }
    return r % MOD;
}
