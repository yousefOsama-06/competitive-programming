// Needs: multiply (06 - Math/05 - Convolution/02 - NTT.cpp), modulus NMOD = 998244353.
// SUM OF BINOMIALS OVER A CONGRUENCE CLASS:  r[j] = sum over k = j (mod m) of C(n, k), for every
// j at once, with n up to 1e18 and m up to ~1e5. O(m log m log n).
// THE IDEA: (1 + x)^n has C(n, k) as its coefficients, so the answer is that polynomial reduced
// MODULO x^m - 1 (i.e. exponents folded by k mod m). Binary-exponentiate (1 + x) in the ring
// Z_p[x] / (x^m - 1): one NTT product plus a fold per bit.
// PRECONDITIONS: works modulo NMOD (998244353); no condition on m at all - unlike the classical
// roots-of-unity filter, this needs no primitive m-th root to exist.
vector<ll> binomByResidue(ll n, int m) {                // r[j] = sum_{k = j mod m} C(n, k)
    auto fold = [&](vector<ll> v) {                     // reduce modulo x^m - 1
        for (int i = (int)v.size() - 1; i >= m; i--) v[i - m] = (v[i - m] + v[i]) % NMOD;
        v.resize(min<int>(v.size(), m));
        return v;
    };
    vector<ll> res = {1}, base = fold({1, 1});
    for (; n; n >>= 1) {
        if (n & 1) res = fold(multiply(res, base));
        base = fold(multiply(base, base));
    }
    res.resize(m);
    return res;
}
// THE CLASSICAL ROOTS-OF-UNITY FILTER, which is what you use when m is TINY (2, 3, 4, ...) and
// you want a closed form instead of a convolution. With w a primitive m-th root of unity:
//   sum_{k = r (mod m)} a_k  =  (1/m) * sum_{j=0}^{m-1} w^(-j r) * A(w^j),     A(x) = sum a_k x^k
// so for a_k = C(n, k), A(x) = (1+x)^n and
//   sum_{k = r (mod m)} C(n, k)  =  (1/m) * sum_{j=0}^{m-1} w^(-j r) * (1 + w^j)^n.
// Examples: m = 2 gives 2^(n-1) for both classes (n >= 1); m = 4 gives the classic
//   (2^n + 2^((n+2)/2) * cos(n pi / 4)) / 4  style answers.
// WHERE TO GET w: if m | p - 1 use g^((p-1)/m) for a primitive root g mod p; otherwise work in
// the extension F_p[x]/(cyclotomic), or just use this polynomial version, which sidesteps it.
// SAME TECHNIQUE, other sequences: replace (1 + x) by ANY generating function and you filter its
// coefficients by residue - e.g. "number of subsets with sum divisible by m", "number of paths of
// length n whose displacement is 0 mod m". Reduce modulo x^m - 1 and exponentiate.
