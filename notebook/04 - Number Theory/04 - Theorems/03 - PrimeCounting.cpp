// LUCY_HEDGEHOG: pi(n) = #primes <= n, and the SUM of primes <= n, in O(n^(3/4)) time and
// O(sqrt n) memory. Handles n up to ~1e12 in well under a second; 1e13 in a few seconds.
// Idea: S(v, p) = (count or sum) of the integers in [2, v] left after sieving by all primes < p.
// Sieving by p only touches v >= p^2, so the whole table lives on the O(sqrt n) distinct
// values of floor(n / i).
ll primePi(ll n) {
    if (n < 2) return 0;
    ll r = (ll)sqrtl((ld)n);
    while (r * r > n) r--;
    while ((r + 1) * (r + 1) <= n) r++;
    vector<ll> S(r + 1), L(r + 1);                       // S[v] for v <= r, L[i] for v = n/i
    for (ll i = 1; i <= r; i++) S[i] = i - 1, L[i] = n / i - 1;
    for (ll p = 2; p <= r; p++) {
        if (S[p] == S[p - 1]) continue;                  // p is not prime
        ll sp = S[p - 1], p2 = p * p;
        for (ll i = 1; i <= min(r, n / p2); i++)
            L[i] -= (i * p <= r ? L[i * p] : S[n / (i * p)]) - sp;
        for (ll v = r; v >= p2; v--) S[v] -= S[v / p] - sp;
    }
    return L[1];
}
// Sum of all primes <= n. Same recurrence with f(x) = x(x+1)/2 - 1 and a factor of p.
ll primeSum(ll n) {
    if (n < 2) return 0;
    ll r = (ll)sqrtl((ld)n);
    while (r * r > n) r--;
    while ((r + 1) * (r + 1) <= n) r++;
    // __int128: n(n+1)/2 wraps for n > 4.29e9, and the ANSWER (~n^2/(2 ln n)) wraps at n ~ 3e10.
    auto f = [](ll x) { return x % 2 ? (lll)(x + 1) / 2 * x - 1 : (lll)x / 2 * (x + 1) - 1; };
    vector<ll> S(r + 1), L(r + 1);
    for (ll i = 1; i <= r; i++) S[i] = f(i), L[i] = f(n / i);
    for (ll p = 2; p <= r; p++) {
        if (S[p] == S[p - 1]) continue;
        ll sp = S[p - 1], p2 = p * p;
        for (ll i = 1; i <= min(r, n / p2); i++)
            L[i] -= p * ((i * p <= r ? L[i * p] : S[n / (i * p)]) - sp);
        for (ll v = r; v >= p2; v--) S[v] -= p * (S[v / p] - sp);
    }
    return L[1];
}
// SAME SKELETON computes the prefix sum of ANY completely-multiplicative f (change the two
// initialisations and the multiplier). For general multiplicative f use the Min_25 sieve, which
// runs this as its first phase and then re-adds the prime-power contributions.
// APPROXIMATIONS for sanity checks: pi(n) ~ n/ln n, the k-th prime ~ k ln k.
