// JOSEPHUS PROBLEM. n people in a circle 0..n-1, every k-th is removed; who survives?
// Recurrence: J(1) = 0, J(n) = (J(n-1) + k) mod n. O(n).
ll josephus(ll n, ll k) { ll r = 0; for (ll i = 2; i <= n; i++) r = (r + k) % i; return r; }
// k = 2 closed form: write n = 2^m + l with 0 <= l < 2^m, the survivor is 2*l  (0-indexed).
ll josephus2(ll n) { return 2 * (n - (1LL << (63 - __builtin_clzll(n)))); }
// LARGE n, SMALL k: one full lap kills floor(n/k) people, so recurse on n - floor(n/k) and map the
// index back. O(k log n).
ll josephusFast(ll n, ll k) {
    if (n == 1) return 0;
    if (k == 1) return n - 1;
    if (k > n) return (josephusFast(n - 1, k) + k) % n;
    ll r = josephusFast(n - n / k, k) - n % k;
    return r < 0 ? r + n : r + r / (k - 1);
}
// ORDER OF ELIMINATION / the j-th person to die: keep a BIT over alive flags and binary-search the
// ((pos + k - 1) mod alive + 1)-th alive person each step. O(n log n) and handles "k changes".
// LAST TWO SURVIVORS: run the recurrence starting from J(2) = {0, 1} instead of J(1) = 0.

// OTHER ONE-LINE CLASSICS worth having on paper:
// GRAY CODE:      g(i) = i ^ (i >> 1);   inverse: x ^= x >> 1, x >> 2, ... until 0.
// n-th PERMUTATION (factorial number system): digit i = k / (n-1-i)!, then k %= (n-1-i)!.
// 15-PUZZLE SOLVABLE iff (#inversions of the tiles, blank excluded) + (row of the blank counted
//   from the BOTTOM, 1-based) is EVEN, for a 4x4 board. For an odd width w, solvable iff the
//   inversion count is even. General w x h: inversions + (blank row from bottom) parity rule above
//   when w is even, plain inversion parity when w is odd.
// TOWER OF HANOI: 2^n - 1 moves; on move t (1-based) move disk (t & -t) counted by trailing zeros,
//   and for odd n the smallest disk cycles A->C->B->A, for even n A->B->C->A.
// BULLS AND COWS / stable-marriage style pairings: Gale-Shapley is O(n^2), always terminates, and
//   is optimal for the proposing side.
// CATALAN-STYLE BALLOT COUNTING: paths from (0,0) to (n,m) staying strictly above y = x are
//   (n-m)/(n+m) * C(n+m, m)  (Bertrand's ballot theorem).
