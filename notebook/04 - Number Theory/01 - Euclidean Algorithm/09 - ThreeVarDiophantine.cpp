// NUMBER OF NONNEGATIVE SOLUTIONS OF a*x + b*y + c*z = n, in O(log max(a,b,c)).
// (Popoviciu's formula for two coins, extended to three by Binner - the denumerant of a
// three-coin system.) Reach for it when a, b, c and n are all up to 1e18 and the DP over n or
// the O(n / max) loop over one variable are both hopeless. For two coins use the much simpler
// count of lattice points under a line, 05 - LatticeUnderLine.cpp.
// PRECONDITIONS: a, b, c >= 1, n >= 0. Handles arbitrary gcds (the wrapper strips them).
// All arithmetic is __int128 because the numerator N1 is of order n^2 and the products a*b*c and
// c*b*b1p reach max(a,b,c)^3; keep a, b, c below ~1e12 and n below ~1e18.
__int128 gcd128(__int128 a, __int128 b) {
    while (b) { __int128 t = a % b; a = b, b = t; }
    return a;
}
__int128 xgcd128(__int128 a, __int128 b, __int128 &x, __int128 &y) {
    if (!b) { x = 1, y = 0; return a; }
    __int128 x1, y1, d = xgcd128(b, a % b, x1, y1);
    x = y1, y = x1 - y1 * (a / b);
    return d;
}
__int128 inv128(__int128 a, __int128 m) {               // m >= 1, gcd(a, m) = 1
    __int128 x, y; xgcd128(((a % m) + m) % m, m, x, y);
    return ((x % m) + m) % m;
}
__int128 fsum128(__int128 n, __int128 m, __int128 a, __int128 b) {   // = floorSum, in __int128
    __int128 ans = 0;
    if (a >= m) ans += (n - 1) * n / 2 * (a / m), a %= m;
    if (b >= m) ans += n * (b / m), b %= m;
    __int128 ymax = (a * n + b) / m;
    if (ymax == 0) return ans;
    __int128 xmax = ymax * m - b;
    ans += ymax * (n - (xmax + a - 1) / a);
    return ans + fsum128(ymax, a, m, (a - xmax % a) % a);
}
__int128 denum3coprime(__int128 a, __int128 b, __int128 c, __int128 n) {  // a,b,c PAIRWISE coprime
    auto fix = [](__int128 v, __int128 m) { return v == 0 ? m : v; };
    __int128 b1 = fix((-n % a * inv128(b, a) % a + a) % a, a);
    __int128 c1 = fix(b % a * inv128(c, a) % a, a);
    __int128 c2 = fix((-n % b * inv128(c, b) % b + b) % b, b);
    __int128 a2 = fix(c % b * inv128(a, b) % b, b);
    __int128 a3 = fix((-n % c * inv128(a, c) % c + c) % c, c);
    __int128 b3 = fix(a % c * inv128(b, c) % c, c);
    __int128 N1 = n * (n + a + b + c);
    N1 += c * b * b1 * (a + 1 - c1 * (b1 - 1));
    N1 += a * c * c2 * (b + 1 - a2 * (c2 - 1));
    N1 += b * a * a3 * (c + 1 - b3 * (a3 - 1));
    return N1 / (2 * a * b * c) + fsum128(b1, a, c1, 0) + fsum128(c2, b, a2, 0)
                                + fsum128(a3, c, b3, 0) - 2;
}
__int128 denum3(__int128 a, __int128 b, __int128 c, __int128 n) {
    __int128 g = gcd128(gcd128(a, b), c);
    if (n % g) return 0;
    a /= g, b /= g, c /= g, n /= g;
    __int128 g1 = gcd128(b, c), g2 = gcd128(c, a), g3 = gcd128(a, b);
    __int128 n1 = n * inv128(a, g1) % g1, n2 = n * inv128(b, g2) % g2, n3 = n * inv128(c, g3) % g3;
    return denum3coprime(a / g2 / g3, b / g3 / g1, c / g1 / g2,
                         (n - a * n1 - b * n2 - c * n3) / g1 / g2 / g3);
}
// TWO COINS, for reference: #solutions of a*x + b*y = n with gcd(a,b) = 1 and x, y >= 0 is
//   n/(a*b) - {n * inv(a) mod b / b} - {n * inv(b) mod a / a} + 1   (Popoviciu),
// i.e. floor(n/(ab)) or that plus one; the largest n with NO solution is a*b - a - b.
// FOUR OR MORE coins: no such formula. Use 08 - CoinReachability.cpp (needs min a_i small), or
// a generating-function / polynomial approach when n is small.
