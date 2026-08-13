// Needs: npow (02 - NTT.cpp).
// FWHT IN AN ARBITRARY BASE k: convolution where indices combine by DIGIT-WISE ADDITION MOD k in
// base k. For k = 2 this is exactly xor and the routine collapses to the usual FWHT; for k = 10 it
// is "add the decimal digits without carrying"; for k = 3 it is the ternary "no-carry" addition
// that Nim-like and ternary-state problems produce.
// Length must be n = k^d. Cost O(n * k * log_k n) - the transform along each digit is a length-k
// DFT, done with the k x k matrix M[p][q] = w^(pq).
// PRECONDITION: a primitive k-th root of unity must exist mod NMOD, i.e. k | NMOD - 1.
// 998244352 = 2^23 * 7 * 17, so k in {2,4,8,...,2^23, 7, 14, 17, 34, 119, ...} all work; k = 3
// does NOT - see the note at the bottom. Use a different NTT prime if you need another k.
void fwhtBase(vector<ll>& a, int k, bool inv) {
    int n = a.size();
    ll w = npow(NROOT, (NMOD - 1) / k);                  // primitive k-th root of unity
    if (inv) w = npow(w, NMOD - 2);
    vector<ll> wp(k);
    wp[0] = 1;
    for (int i = 1; i < k; i++) wp[i] = wp[i - 1] * w % NMOD;
    vector<ll> v(k);
    for (int len = 1; len < n; len *= k)
        for (int i = 0; i < n; i += len * k)
            for (int j = 0; j < len; j++) {
                for (int p = 0; p < k; p++) v[p] = a[i + j + p * len];
                for (int p = 0; p < k; p++) {
                    ll s = 0;
                    for (int q = 0; q < k; q++) s = (s + v[q] * wp[p * q % k]) % NMOD;
                    a[i + j + p * len] = s;
                }
            }
    if (inv) { ll ni = npow(n, NMOD - 2); for (ll& x : a) x = x * ni % NMOD; }
}
vector<ll> multiplyBase(vector<ll> a, vector<ll> b, int k) {
    int n = 1;
    while (n < (int)max(a.size(), b.size())) n *= k;     // n must be a power of k
    a.resize(n, 0), b.resize(n, 0);
    fwhtBase(a, k, false), fwhtBase(b, k, false);
    for (int i = 0; i < n; i++) a[i] = a[i] * b[i] % NMOD;
    fwhtBase(a, k, true);
    return a;
}
// k-TH POWER OF THE OPERATOR: to convolve one array with itself e times, transform once, raise
// every entry to the e-th power, transform back - O(n log n + n log e), not e convolutions.
// TERNARY when 3 does not divide MOD - 1: work in F_p[w] / (w^2 + w + 1), i.e. represent each
// value as a + b*w with w^2 = -1 - w. That is a genuine cube root of unity as long as 3 | p + 1.
// Multiplication is (a1+b1 w)(a2+b2 w) = (a1a2 - b1b2) + (a1b2 + a2b1 - b1b2) w. Everything else
// in the routine above is unchanged.
// PACKING TRICK: if your indices are BITMASKS but the operation is digit-wise addition mod 3
// (e.g. "each element used 0, 1 or 2 times"), rewrite each mask in base 3 first: the value with
// binary digits d_i becomes sum d_i 3^i.
// WHY NOT SUBSET-SUM/OR: OR-convolution is the zeta/Mobius transform (08 - DP/02 - SOS_DP.cpp);
// this file is only for the group Z_k^d, where the transform is invertible pointwise.
