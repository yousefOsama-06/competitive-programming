// Needs: Mint (06 - Math/01 - Modular Arithmetic/01 - Mint.cpp).
// MIN_25 SIEVE - prefix sum of ANY multiplicative f in O(n^(3/4) / log n), n up to ~1e10-1e11.
// You must supply two things:
//   c[]  : f(p) written as a POLYNOMIAL in p, f(p) = c[0] + c[1]*p + c[2]*p^2 (degree <= 2 here,
//          extend pref() with more power sums for higher degree),
//   fpe  : f(p^e) for a prime power, given (p, e, p^e).
// PHASE 1 sieves sum of p^k over primes <= v for every v = floor(n/i) (this is exactly Lucy's
// method, 04 - Theorems/03 - PrimeCounting.cpp). PHASE 2 (S) walks every "smallest prime factor"
// class and re-adds the prime-power contributions. Memory O(sqrt n * deg).
// EXAMPLES: phi -> c = {-1, 1}, fpe = p^e - p^(e-1);   mu -> c = {-1}, fpe = (e == 1 ? -1 : 0);
//           tau -> c = {2}, fpe = e + 1;               Id -> c = {0, 1}, fpe = p^e;
//           f(p^e) = p^e * (p^e - 1) -> c = {0, -1, 1} (that is f(p) = p^2 - p).
// If f is COMPLETELY multiplicative, phase 1 alone is the answer (use Lucy directly).
struct Min25 {
    ll n, rt;
    int np;
    vector<ll> pr, w;                                   // primes <= sqrt(n); values floor(n/i)
    vector<int> id1, id2;                               // position of v in w, for v <= rt / v > rt
    vector<vector<Mint>> g, sp;                         // g[k][j] over w[j]; sp[k][i] = prefix p^k
    vector<Mint> c;
    function<Mint(ll, int, ll)> fpe;

    int id(ll x) { return x <= rt ? id1[x] : id2[n / x]; }
    Mint pref(ll x, int k) {                            // sum_{i=2..x} i^k  (mod MOD)
        Mint z = x % MOD;
        if (k == 0) return z - 1;
        if (k == 1) return z * (z + 1) / 2 - 1;
        return z * (z + 1) * (2 * z + 1) / 6 - 1;
    }
    Min25(ll n, vector<Mint> c, function<Mint(ll, int, ll)> fpe) : n(n), c(c), fpe(fpe) {
        rt = (ll)sqrtl((ld)n);
        while (rt * rt > n) rt--;
        while ((rt + 1) * (rt + 1) <= n) rt++;
        int d = c.size();
        vector<char> comp(rt + 2, 0);
        for (ll i = 2; i <= rt; i++)
            if (!comp[i]) { pr.push_back(i); for (ll j = i * i; j <= rt; j += i) comp[j] = 1; }
        np = pr.size();
        sp.assign(d, vector<Mint>(np + 1, 0));
        for (int i = 0; i < np; i++) {
            Mint t = 1;
            for (int k = 0; k < d; k++) sp[k][i + 1] = sp[k][i] + t, t *= pr[i] % MOD;
        }
        id1.assign(rt + 2, 0), id2.assign(rt + 2, 0);
        for (ll i = 1, j; i <= n; i = j + 1) {          // w is DECREASING: w[0] = n, back = 1
            ll x = n / i; j = n / x;
            w.push_back(x);
            (x <= rt ? id1[x] : id2[n / x]) = w.size() - 1;
        }
        g.assign(d, vector<Mint>(w.size()));
        for (int k = 0; k < d; k++)
            for (int j = 0; j < (int)w.size(); j++) g[k][j] = pref(w[j], k);
        for (int i = 0; i < np; i++)                    // sieve out multiples of pr[i]
            for (int j = 0; j < (int)w.size() && pr[i] <= w[j] / pr[i]; j++) {
                int u = id(w[j] / pr[i]);
                Mint pk = 1;
                for (int k = 0; k < d; k++) g[k][j] -= pk * (g[k][u] - sp[k][i]), pk *= pr[i] % MOD;
            }
    }
    Mint S(ll x, int j) {                               // sum of f(i), i <= x, spf(i) > pr[j-1]
        if (j >= 1 && pr[j - 1] >= x) return 0;
        Mint res = 0;
        for (int k = 0; k < (int)c.size(); k++) res += c[k] * (g[k][id(x)] - sp[k][j]);
        for (int i = j; i < np && pr[i] <= x / pr[i]; i++) {
            ll pw = pr[i];
            for (int e = 1; pw <= x; e++) {
                res += fpe(pr[i], e, pw) * (S(x / pw, i + 1) + Mint(e != 1));
                if (pw > x / pr[i]) break;
                pw *= pr[i];
            }
        }
        return res;
    }
    Mint solve() { return n < 1 ? Mint(0) : S(n, 0) + 1; }   // the +1 is f(1)
};
// The "+ Mint(e != 1)" term is f(p^e) counted for the number p^e itself (only for e >= 2, since
// e = 1 is already inside the prime sum). No memoisation is needed anywhere - that is the point.
// SIBLINGS: 10 - PowerfulNumberSieve.cpp is usually FASTER (O(sqrt n)) when you can find a g with
// g(p) = f(p) and an easy prefix sum; 04 - Theorems/05 - DuSieve.cpp is the O(n^(2/3)) tool when
// a Dirichlet partner makes the convolution telescope.
