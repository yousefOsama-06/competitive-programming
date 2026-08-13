// Needs: fib (01 - FibonacciAndPisano.cpp), powmod (02 - Primes and Divisors/06).
// LCM OF FIBONACCI NUMBERS  lcm(F(a1), F(a2), ..., F(an)) mod p - the numbers themselves have
// billions of digits, so everything is done modulo p and division is a modular inverse.
// THE KEY IDENTITY is gcd(F(i), F(j)) = F(gcd(i, j)), which turns the usual
//   lcm(S) = lcm(S') * x / gcd(lcm(S'), x)      (x = the last element)
// into                                                          -- all indices, no big numbers --
//   lcm(F(A)) = lcm(F(A')) * F(x) / lcm( F(gcd(a_i, x)) for a_i in A' )
// i.e. the denominator is again an lcm of Fibonacci numbers, on the SMALLER index set of gcds.
// So recurse on sets of indices; memoise on the sorted, deduplicated index vector.
// COMPLEXITY: O(#distinct gcd-closed subsets * n * log) - bounded by the number of divisors of
// the a_i in practice, fast for n up to a few hundred with a_i up to 1e9.
// CAUTION: this DIVIDES modulo p, so p must be prime AND no F(k) involved may be 0 mod p. With
// p = 1e9+7 that is safe for the usual constraints; if you hit a zero, use a different prime.
map<vector<int>, ll> fibLcmMemo;
ll fibLcm(vector<int> a) {
    sort(a.rbegin(), a.rend());
    while (!a.empty() && a.back() <= 2) a.pop_back();   // F(1) = F(2) = 1 contribute nothing
    a.erase(unique(all(a)), a.end());
    if (a.empty()) return 1;
    if (a.size() == 1) return fib(a[0], MOD);
    auto it = fibLcmMemo.find(a);
    if (it != fibLcmMemo.end()) return it->second;
    vector<int> b(a.begin(), a.end() - 1);
    ll res = fibLcm(b);
    for (int& x : b) x = gcd(x, a.back());
    res = res * fib(a.back(), MOD) % MOD * powmod(fibLcm(b), MOD - 2, MOD) % MOD;
    return fibLcmMemo[a] = res;
}
// SAME PATTERN, other sequences: any sequence with gcd(u_i, u_j) = u_gcd(i,j) (a "strong
// divisibility sequence") works verbatim - F(n), 2^n - 1 (lcm of Mersenne numbers), and the
// Lucas sequences U_n(P, Q) with gcd(P, Q) = 1 in general.
// USEFUL COMPANIONS: F(m) | F(n) iff m | n (for m >= 3); gcd(F(m), F(n)) = F(gcd(m,n));
// lcm over ALL i <= n of F(i) has a product formula via the primitive parts (Zsygmondy), but the
// recursion above is what you want in a contest.
