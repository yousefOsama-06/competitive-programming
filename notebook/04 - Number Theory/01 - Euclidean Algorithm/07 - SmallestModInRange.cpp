// SMALLEST x >= 0 WITH l <= (a*x mod p) <= r, or -1 if no such x exists.  O(log p).
// The "when does the ticking counter first land in the window" problem: jumps of size a on a
// circle of circumference p, first time inside the arc [l, r]. Also solves "smallest x with
// a*x mod p <= r" (l = 0) and, by symmetry, the first time OUTSIDE a window.
// PRECONDITIONS: 0 <= a < p, 0 <= l <= r < p, p >= 1. p need NOT be prime.
// Idea: if ceil(l/a)*a <= r we are done in one step. Otherwise write p = k*a + b; asking for
// l <= a*x - p*y <= r turns into the same question one Euclidean step down, with the roles of
// the modulus and the multiplier swapped - so the recursion depth is O(log p), like gcd.
// OVERFLOW: the internal products are up to p*p, hence the __int128.
ll smallestModIn(ll p, ll a, ll l, ll r) {
    if (a == 0) return l == 0 ? 0 : -1;
    auto cdiv = [](__int128 x, __int128 y) { return (ll)((x + y - 1) / y); };
    ll c = cdiv(l, a);                                  // first x with a*x >= l, ignoring wraps
    if ((__int128)a * c <= r) return c;
    ll b = p % a;                                       // p = (p/a)*a + b
    ll y = smallestModIn(a, b, a - r % a, a - l % a);   // -r <= b*y (mod a) <= -l
    return y == -1 ? -1 : cdiv((__int128)l + (__int128)b * y, a) + p / a * y;
}
// COMPANIONS
//   count of such x in a range          -> 06 - ModCountInRange.cpp
//   the whole solution set              -> it is a union of O(sqrt p) APs, see
//                                          03 - Modular Arithmetic/09 - ModularAPBlocks.cpp
//   smallest x with a*x mod p == target -> solve the linear congruence instead
//                                          (03 - Modular Arithmetic/04 - CongruencesAndTowers.cpp)
// TYPICAL USE: interactive/recovery problems where you know x mod M for a huge M and want the
// smallest representative that also satisfies 1 <= q <= N; and "minimum of (k*a mod p) over
// k <= n", which the three-distance theorem says is achieved at a continued-fraction denominator.
