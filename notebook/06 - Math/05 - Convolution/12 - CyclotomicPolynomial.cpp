// CYCLOTOMIC POLYNOMIALS Phi_n(x) = prod over primitive n-th roots of unity of (x - w).
// They are the IRREDUCIBLE factors of x^n - 1 over the integers:  x^n - 1 = prod_{d | n} Phi_d(x),
// deg Phi_n = phi(n), and every Phi_n has integer coefficients (tiny ones - the first n with a
// coefficient outside {-1, 0, 1} is n = 105).
// By Mobius inversion  Phi_n(x) = prod_{d | n} (x^d - 1)^mu(n/d), and both multiplying and
// dividing by (x^d - 1) are O(deg) in place, so one Phi_n costs O(sigma(n)) - linear, no FFT.
// REACH FOR IT when a problem factors x^n - 1 (roots-of-unity filters, "count sequences whose sum
// is 0 mod n", cyclic codes), when you need the minimal polynomial of a root of unity, or for
// determinants of circulants factored per divisor.
void mulXd(vector<ll>& a, int d) {               // a *= (x^d - 1)
    int n = a.size();
    a.insert(a.begin(), d, 0);
    for (int i = 0; i < n; i++) a[i] -= a[i + d];
}
void divXd(vector<ll>& a, int d) {               // a /= (x^d - 1), exact division assumed
    int n = a.size();
    for (int i = n - 1 - d; i >= 0; i--) a[i] += a[i + d];
    a.erase(a.begin(), a.begin() + d);
}
int muOne(int n) {                               // Mobius of a single n, by trial division
    int r = 1;
    for (int p = 2; (ll)p * p <= n; p++) if (n % p == 0) {
        n /= p;
        if (n % p == 0) return 0;
        r = -r;
    }
    return n > 1 ? -r : r;
}
vector<ll> cyclotomic(int n) {                   // coefficients, low degree first; size phi(n)+1
    vector<ll> a{1};
    for (int d = 1; d <= n; d++) if (n % d == 0 && muOne(n / d) == 1) mulXd(a, d);
    for (int d = 1; d <= n; d++) if (n % d == 0 && muOne(n / d) == -1) divXd(a, d);
    return a;                                    // do the multiplications FIRST, then the divisions
}
// FACTS
//  Phi_p(x) = 1 + x + ... + x^(p-1) for prime p;  Phi_2n(x) = Phi_n(-x) for odd n > 1;
//  Phi_{n p}(x) = Phi_n(x^p) if p | n, and Phi_n(x^p) / Phi_n(x) otherwise.
//  Phi_n(1) = p if n is a prime power p^k, and 1 for any other n > 1.
//  ORDER OF 2 MOD n and friends: the irreducible factors of x^n - 1 over F_q are the factors of
//  the Phi_d, each splitting into phi(d) / ord_d(q) factors of degree ord_d(q).
//  x^n - 1 over F_p (p not dividing n) is squarefree; if p | n, x^n - 1 = (x^(n/p) - 1)^p.
// ROOTS-OF-UNITY FILTER (the usual reason you are here):
//   sum_{k = r mod m} [x^k] F(x) = (1/m) * sum_{j=0}^{m-1} w^(-jr) * F(w^j),  w a primitive m-th
//   root of unity. Mod p you need m | p - 1 for w to exist; otherwise work in F_p[x] / Phi_m(x),
//   where x IS a primitive m-th root by construction - which is exactly what this file gives you.
// OVERFLOW: coefficients stay small (bounded by a few hundred for n < 1e5) so ll is safe; if you
// want them mod p, reduce afterwards.
