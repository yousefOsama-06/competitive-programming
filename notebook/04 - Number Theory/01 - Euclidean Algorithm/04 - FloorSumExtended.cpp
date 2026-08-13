// EUCLIDEAN-LIKE ALGORITHM, THE FULL TRIPLE. With q_i = floor((a*i + b) / c) it returns
//     f = sum_{i=0..n} q_i        g = sum_{i=0..n} i * q_i        h = sum_{i=0..n} q_i^2
// all three at once in O(log c). NOTE i runs to n INCLUSIVE, unlike floorSum in 03, which stops
// at n-1: floorSum(n, m, a, b) = fgh(n - 1, a, b, m).f.
// Reach for it when a lattice-line sum has an i or a square in it: sum of i*floor, sum of
// (a i + b mod c)^2, the second moment of a modular AP, "sum over lattice points under a line of
// (x + y)" and friends. The recursion is the same as floorSum: strip a/c and b/c, then swap axes.
// Requires n >= -1, a >= 0, b >= 0, c >= 1. Exact (no modulus) in __int128, which is good for
// n and m = (a n + b)/c up to ~1e11 (h is of order n*m^2). Take everything mod p if they are
// bigger - the recursion only ever adds and multiplies, so it is modulus-friendly.
struct FGH { __int128 f, g, h; };
FGH fgh(ll n, ll a, ll b, ll c) {
    if (n < 0) return {0, 0, 0};
    __int128 M = n, s1 = M * (M + 1) / 2, s2 = M * (M + 1) * (2 * M + 1) / 6;
    if (a >= c || b >= c) {                              // q_i = A*i + B + (reduced q_i)
        __int128 A = a / c, B = b / c;
        FGH t = fgh(n, a % c, b % c, c);
        return {t.f + A * s1 + B * (M + 1), t.g + A * s2 + B * s1,
                t.h + 2 * B * t.f + 2 * A * t.g + A * A * s2 + B * B * (M + 1) + 2 * A * B * s1};
    }
    __int128 m = ((__int128)a * n + b) / c;              // the largest value any q_i takes
    if (m == 0) return {0, 0, 0};
    FGH t = fgh((ll)m - 1, c, c - b - 1, a);             // count by rows instead of by columns
    __int128 f = M * m - t.f;
    return {f, (m * M * (M + 1) - t.f - t.h) / 2, M * m * (m + 1) - 2 * t.g - 2 * t.f - f};
}
// DERIVED SUMS (r_i = (a*i + b) mod c = a*i + b - c*q_i):
//   sum r_i   = a*s1 + b*(n+1) - c*f
//   sum r_i^2 = sum (a i + b)^2 - 2c * (a*g + b*f) + c^2 * h
//   sum i*r_i = a*s2 + b*s1 - c*g
// The same recursion with a bigger state gives sum i^k1 * q_i^k2 for any k1 + k2 <= K in
// O(K^3 log c) - rarely needed. The clean way to state the whole family is the UNIVERSAL
// EUCLIDEAN ALGORITHM: walk the line as a word in {U = "step up", R = "step right"} and
// multiply the letters in ANY monoid; supply U and R and you get whatever statistic you like.
