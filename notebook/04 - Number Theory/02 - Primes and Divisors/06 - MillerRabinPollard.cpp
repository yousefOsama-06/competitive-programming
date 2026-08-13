// Primality + factorization for n up to 1e18.  isPrime O(log^3 n), factor O(n^(1/4)).
// THE gate for any problem with a_i up to 1e18 - divisors, phi, tau, sigma all follow from factor().
typedef unsigned long long u64;
u64 mulmod(u64 a, u64 b, u64 m) { return (__int128)a * b % m; }
u64 powmod(u64 a, u64 e, u64 m) {
    u64 r = 1; a %= m;
    for (; e; e >>= 1, a = mulmod(a, a, m)) if (e & 1) r = mulmod(r, a, m);
    return r;
}
// Deterministic for all n < 3.3e24 with these 7 bases (Miller-Rabin).
bool isPrime(u64 n) {
    if (n < 2 || n % 6 % 4 != 1) return (n | 1) == 3;
    u64 d = n - 1; int s = __builtin_ctzll(d); d >>= s;
    for (u64 a : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}) {
        u64 p = powmod(a % n, d, n); int i = s;
        while (p != 1 && p != n - 1 && a % n && i--) p = mulmod(p, p, n);
        if (p != n - 1 && i != s) return false;
    }
    return true;
}
u64 pollard(u64 n) {                                    // returns a non-trivial divisor (Brent)
    auto f = [n](u64 x) { return mulmod(x, x, n) + 1; };
    u64 x = 0, y = 0, t = 30, prd = 2, i = 1, q;
    while (t++ % 40 || gcd(prd, n) == 1) {
        if (x == y) x = ++i, y = f(x);
        if ((q = mulmod(prd, x > y ? x - y : y - x, n))) prd = q;
        x = f(x), y = f(f(y));
    }
    return gcd(prd, n);
}
vector<u64> factor(u64 n) {                             // unsorted multiset of primes
    if (n <= 1) return {};                              // guard: factor(0) would loop forever
    if (isPrime(n)) return {n};
    u64 x = pollard(n);
    auto l = factor(x), r = factor(n / x);
    l.insert(l.end(), all(r));
    return l;
}
// {prime, exponent} pairs, sorted
vector<pair<u64, int>> factorize(u64 n) {
    auto f = factor(n); sort(all(f));
    vector<pair<u64, int>> r;
    for (u64 p : f) (!r.empty() && r.back().first == p) ? r.back().second++ : (r.push_back({p, 1}), 0);
    return r;
}
vector<u64> divisors(u64 n) {                           // all divisors, unsorted
    vector<u64> d = {1};
    for (auto [p, e] : factorize(n)) {
        int sz = d.size();
        for (u64 pe = 1, k = 0; k < (u64)e; k++) {
            pe *= p;
            for (int i = 0; i < sz; i++) d.push_back(d[i] * pe);
        }
    }
    return d;
}
u64 phi(u64 n) { u64 r = n; for (auto [p, e] : factorize(n)) r -= r / p; return r; }      // Euler totient
u64 tau(u64 n) { u64 r = 1; for (auto [p, e] : factorize(n)) r *= e + 1; return r; }      // #divisors
u64 sigma(u64 n) {                                                                        // sum of divisors
    u64 r = 1;
    for (auto [p, e] : factorize(n)) { u64 t = 1, pe = 1; for (int i = 0; i < e; i++) t += (pe *= p); r *= t; }
    return r;
}
