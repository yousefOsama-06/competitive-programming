// Needs: Mint + buildFact (01 - Mint.cpp), lagrangeConsecutive (06 - Interpolation/01).
// SUM OF A GEOMETRIC TIMES A POLYNOMIAL:   S(n) = sum_{i=0}^{n} a^i * f(i)   for n up to 1e18,
// where f is a polynomial of degree <= k given by its values f(0), f(1), ..., f(k). O(k log MOD).
// REACH FOR IT for "sum i^k r^i", weighted prefix sums in a linear recurrence, expected values
// where each step is discounted by a factor a, and anything of the shape sum a^i * (polynomial).
// WHY IT WORKS: for a != 1 the answer has the closed form S(n) = a^n * g(n) + c with g a
// polynomial of degree exactly k and c a constant. Apply the operator (D h)(n) = h(n+1) - a*h(n):
// it kills a^n * g one degree at a time, so D^(k+1) annihilates the a^n g(n) part completely and
// turns the constant into c * (1 - a)^(k+1). That single equation gives c; then g is sampled and
// Lagrange-interpolated. For a == 1 the sum is just a polynomial of degree k+1 in n.
Mint expPolySum(vector<Mint> y, Mint a, ll n) {          // y[i] = f(i) for i = 0..k
    int k = y.size() - 1;
    if (n < 0) return 0;
    y.push_back(lagrangeConsecutive(y, k + 1));          // need f(k+1) too, so k+2 samples of S
    vector<Mint> S(k + 2);
    Mint p = 1;
    for (int i = 0; i <= k + 1; i++) {
        S[i] = (i ? S[i - 1] : Mint(0)) + p * y[i];
        p *= a;
    }
    if (a == Mint(1)) return lagrangeConsecutive(S, n);  // S is a polynomial of degree k+1
    Mint c = 0;                                          // c * (1-a)^(k+1) = D^(k+1) S at 0
    for (int i = 0; i <= k + 1; i++) {
        Mint t = C(k + 1, i) * (Mint(0) - a).pow(k + 1 - i) * S[i];
        c += t;
    }
    c /= (Mint(1) - a).pow(k + 1);
    vector<Mint> g(k + 1);                               // g(i) = (S(i) - c) / a^i
    Mint ia = Mint(1) / a, q = 1;
    for (int i = 0; i <= k; i++) g[i] = (S[i] - c) * q, q *= ia;
    return lagrangeConsecutive(g, n) * a.pow(n) + c;
}
// SPECIAL CASES WORTH KNOWING
//   f == 1:      sum_{i<=n} a^i = (a^(n+1) - 1) / (a - 1), and n+1 when a == 1. Do NOT route a
//                trivial geometric series through this - and watch a == 1 mod MOD, which happens
//                for a = MOD + 1 style inputs and makes the closed form divide by zero.
//   a == 1:      pure Faulhaber, powerSum in 06 - Interpolation/01.
//   INFINITE sum with |a| < 1 formally: sum_{i>=0} a^i f(i) = (applied k+1 times) - or read it off
//                the rational generating function, see 03 - BostanMori.cpp.
// PRECONDITION: MOD prime and larger than k + 2 (we invert factorials and (1 - a)^(k+1)); a != 1
// for the second branch, which the code checks. n may be as large as you like: a.pow(n) and
// lagrangeConsecutive both take a ll argument.
// SAMPLING f: if you are handed the COEFFICIENTS of f instead of its values, evaluate it at
// 0..k+1 by Horner in O(k^2) or by one multipoint evaluation.
