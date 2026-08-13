// Needs: Mint (06 - Math/01 - Modular Arithmetic/01 - Mint.cpp).
/* ============ SYMBOLIC METHOD: spec -> generating function ============
 Decide labelled (EGF) or unlabelled (OGF), then translate the construction:
   UNLABELLED (OGF), A built from atoms with GF B(z):
     SEQ  (ordered list)     A = 1 / (1 - B)
     MSET (unordered, rep)   A = exp( sum_{k>=1} B(z^k) / k )          <- Euler transform
     PSET (unordered, no rep)A = exp( sum_{k>=1} (-1)^(k-1) B(z^k)/k )
     CYC  (up to rotation)   A = sum_{k>=1} (phi(k)/k) * log(1/(1 - B(z^k)))
   LABELLED (EGF):
     SEQ  A = 1/(1-B)        SET  A = exp(B)        CYC  A = log(1/(1-B))
 "SET of connected pieces = exp(connected)" is the exponential formula.
====================================================================== */

// EULER TRANSFORM: a[n] = #atoms of size n  ->  b[n] = #MULTISETS of atoms of total size n.
// prod_{n>=1} (1 - x^n)^(-a_n) = 1 + sum b_n x^n.     O(n log n) via the harmonic sum.
vector<Mint> eulerTransform(const vector<Mint>& a) {   // a[0] unused; returns b with b[0] = 1
    int n = a.size() - 1;
    vector<Mint> c(n + 1, 0), b(n + 1, 0);
    for (int d = 1; d <= n; d++)                       // c_n = sum_{d | n} d * a_d
        for (int m = d; m <= n; m += d) c[m] += Mint(d) * a[d];
    b[0] = 1;
    for (int i = 1; i <= n; i++) {                     // b_n = (c_n + sum_{k<n} c_k b_{n-k}) / n
        Mint s = c[i];
        for (int k = 1; k < i; k++) s += c[k] * b[i - k];
        b[i] = s * inv_[i];
    }
    return b;
}
int muSmall(int n) {                                   // Mobius of one small n, by trial division
    int r = 1;
    for (int p = 2; (ll)p * p <= n; p++) if (n % p == 0) {
        n /= p;
        if (n % p == 0) return 0;
        r = -r;
    }
    return n > 1 ? -r : r;
}
// INVERSE Euler transform: b -> a  (given multiset counts, recover the atom counts).
vector<Mint> eulerInverse(const vector<Mint>& b) {
    int n = b.size() - 1;
    vector<Mint> c(n + 1, 0), a(n + 1, 0);
    for (int i = 1; i <= n; i++) {                     // c_n = n*b_n - sum_{d<n} c_d b_{n-d}
        Mint s = Mint(i) * b[i];
        for (int d = 1; d < i; d++) s -= c[d] * b[i - d];
        c[i] = s;
    }
    for (int i = 1; i <= n; i++) {                     // a_n = (1/n) sum_{d|n} mu(n/d) c_d
        Mint s = 0;
        for (int d = 1; d <= i; d++) if (i % d == 0) s += Mint(muSmall(i / d)) * c[d];
        a[i] = s * inv_[i];
    }
    return a;
}
// UNLABELLED ROOTED TREES: r_1 = 1, and r_{n+1} = (Euler transform of r_1..r_n)[n]
// (a rooted tree = a root plus a MULTISET of rooted subtrees).
vector<Mint> rootedTrees(int n) {
    vector<Mint> r(n + 1, 0);
    if (n >= 1) r[1] = 1;
    for (int i = 1; i < n; i++) {
        vector<Mint> a(i + 1, 0);
        for (int j = 1; j <= i; j++) a[j] = r[j];
        auto b = eulerTransform(a);
        r[i + 1] = b[i];
    }
    return r;                                          // 0,1,1,2,4,9,20,48,115,286,719,...
}
// PARTITIONS INTO PARTS FROM A SET S = eulerTransform with a[s] = 1 for s in S.
// COMPOSITIONS (ordered) use SEQ instead: 1/(1 - sum x^s).
