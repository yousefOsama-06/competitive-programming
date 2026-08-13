// Needs: modInverse / extGCD (04 - Number Theory/01), CRT (04 - Number Theory/03/01 - CRT.cpp).
// REEDS-SLOANE: the shortest linear recurrence fitting a sequence MODULO A COMPOSITE m.
// Berlekamp-Massey (06 - Interpolation/02) divides by the discrepancy, so it needs a FIELD and
// silently produces garbage whenever a leading coefficient is not invertible mod m. This is the
// replacement: it factors m into prime powers, runs a shift-register synthesis over each Z/p^e
// (tracking, for every 2-adic-style valuation g, the best approximation whose discrepancy has that
// valuation), and CRTs the coefficient vectors back together.
// REACH FOR IT when the problem's modulus is 1e9, 2^32, 10^k or any other composite and you still
// want the "brute force a few terms, guess the recurrence, extrapolate" attack. O(n^2 log m).
// OUTPUT: A[0..L] with A[0] = 1 and sum_j A[j] * s[i-j] = 0 mod m, i.e.
//   s[i] = -(A[1] s[i-1] + ... + A[L] s[i-L])  mod m.   Feed 2L or more terms.
namespace RS {
int deg(const vector<ll>& a) {                   // -inf encoded as a large negative number
    return (a.size() > 1 || (a.size() == 1 && a[0])) ? (int)a.size() - 1 : -40000;
}
int L(const vector<ll>& a, const vector<ll>& b) { return max(deg(a), deg(b) + 1); }
void ext(vector<ll>& a, size_t d) { if (d > a.size()) a.resize(d, 0); }

// shift-register synthesis modulo p^e (m = p^e); returns the connection polynomial
vector<ll> primePower(const vector<ll>& s, ll m, ll p, int e) {
    vector<vector<ll>> a(e), b(e), an(e), bn(e), ao(e), bo(e);
    vector<ll> t(e), u(e), r(e), to(e, 1), uo(e), pw(e + 1, 1);
    for (int i = 1; i <= e; i++) pw[i] = pw[i - 1] * p;
    for (int i = 0; i < e; i++) {
        a[i] = an[i] = {pw[i]};
        b[i] = {0}, bn[i] = {s[0] * pw[i] % m};
        t[i] = s[0] * pw[i] % m;
        if (!t[i]) t[i] = 1, u[i] = e;
        else for (u[i] = 0; t[i] % p == 0; t[i] /= p) u[i]++;
    }
    for (size_t k = 1; k < s.size(); k++) {
        for (int g = 0; g < e; g++) if (L(an[g], bn[g]) > L(a[g], b[g])) {
            int h = e - 1 - u[g];
            ao[g] = a[h], bo[g] = b[h], to[g] = t[h], uo[g] = u[h], r[g] = k - 1;
        }
        a = an, b = bn;
        for (int o = 0; o < e; o++) {
            ll d = 0;
            for (size_t i = 0; i < a[o].size() && i <= k; i++) d = (d + a[o][i] * s[k - i]) % m;
            if (!d) { t[o] = 1, u[o] = e; continue; }
            for (u[o] = 0, t[o] = d; t[o] % p == 0; t[o] /= p) u[o]++;
            int g = e - 1 - u[o];
            if (L(a[g], b[g]) == 0) { ext(bn[o], k + 1); bn[o][k] = (bn[o][k] + d) % m; continue; }
            ll c = t[o] % m * modInverse(to[g] % m, m) % m * pw[u[o] - uo[g]] % m;
            int sh = k - r[g];
            ext(an[o], ao[g].size() + sh), ext(bn[o], bo[g].size() + sh);
            for (size_t i = 0; i < ao[g].size(); i++)
                an[o][i + sh] = (an[o][i + sh] - c * ao[g][i]) % m;
            for (size_t i = 0; i < bo[g].size(); i++)
                bn[o][i + sh] = (bn[o][i + sh] - c * bo[g][i]) % m;
            for (ll& x : an[o]) x = (x % m + m) % m;
            for (ll& x : bn[o]) x = (x % m + m) % m;
            while (an[o].size() > 1 && !an[o].back()) an[o].pop_back();
            while (bn[o].size() > 1 && !bn[o].back()) bn[o].pop_back();
        }
    }
    return an[0];
}
}
vector<ll> reedsSloane(vector<ll> s, ll m) {
    vector<array<ll, 3>> fac;                    // {p^e, p, e}
    ll t = m;
    for (ll i = 2; i * i <= t; i++) if (t % i == 0) {
        ll c = 0, q = 1;
        while (t % i == 0) t /= i, c++, q *= i;
        fac.push_back({q, i, c});
    }
    if (t > 1) fac.push_back({t, t, 1});
    vector<vector<ll>> per;
    size_t n = 0;
    for (auto& f : fac) {
        vector<ll> ss = s;
        for (ll& x : ss) x = (x % f[0] + f[0]) % f[0];
        per.push_back(RS::primePower(ss, f[0], f[1], (int)f[2]));
        n = max(n, per.back().size());
    }
    vector<ll> res(n);
    for (size_t i = 0; i < n; i++) {
        vector<ll> c(fac.size()), md(fac.size());
        for (size_t j = 0; j < fac.size(); j++)
            md[j] = fac[j][0], c[j] = i < per[j].size() ? per[j][i] : 0;
        res[i] = CRT(c, md).first;
    }
    return res;                                  // res[0] == 1; s[i] = -sum_{j>=1} res[j]*s[i-j]
}
// AFTER YOU HAVE THE RECURRENCE: extending it is trivial (the leading coefficient is 1, so no
// inversion is ever needed) - just iterate s[i] = -(sum_{j>=1} A[j] s[i-j]) mod m.
// For the k-th term with k = 1e18 mod a COMPOSITE, do Kitamasa-style polynomial exponentiation
// mod the connection polynomial: it only multiplies, never divides, so a composite modulus is fine.
// CHEAPER FIRST TRIES, in order:
//  - if m is prime, use Berlekamp-Massey; this file is strictly heavier.
//  - if m is squarefree, run BM under each prime factor separately and CRT. That is 10 lines and
//    covers m = 1e9+6-style inputs; the prime-POWER case is the only one that really needs this.
//  - if the sequence is not constant-coefficient at all (factorials, binomials), you want
//    06 - PRecursive.cpp instead, and it also works over composite moduli when the leading
//    polynomial happens to be invertible.
// PRECONDITION: m >= 1 and the factorisation loop is trial division, so m up to ~1e18 needs
// Pollard rho (04 - Number Theory/02/06) instead of the loop above.
