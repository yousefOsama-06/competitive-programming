// EUCLIDEAN-LIKE ALGORITHM (类欧几里得 / AtCoder Library `floor_sum`).
//   floorSum(n, m, a, b) = sum_{i=0}^{n-1} floor((a*i + b) / m)     in O(log m).
// This is THE tool for "sum over a lattice line": counting lattice points under a line, sums of
// floor/mod expressions, and anything of the shape sum floor((a i + b) / m) that a loop cannot do
// because n is up to 1e9 or more. Works like the Euclidean algorithm: reduce a mod m, then SWAP
// the roles of the axes (count by rows instead of by columns) and recurse.
// Requires n >= 0, m >= 1, a >= 0, b >= 0. Overflow: a*n + b must fit in ll (use __int128 if not).
ll floorSum(ll n, ll m, ll a, ll b) {
    ll ans = 0;
    if (a >= m) ans += (n - 1) * n / 2 * (a / m), a %= m;
    if (b >= m) ans += n * (b / m), b %= m;
    ll ymax = (a * n + b) / m;
    if (ymax == 0) return ans;
    ll xmax = ymax * m - b;
    ans += ymax * (n - (xmax + a - 1) / a);
    return ans + floorSum(ymax, a, m, (a - xmax % a) % a);
}
// NEGATIVE a or b: shift them into range first.
//   a < 0: let a2 = a mod m in [0,m); floorSum(n,m,a,b) = floorSum(n,m,a2,b) - (a2-a)/m * (n-1)n/2
//   b < 0: let b2 = b mod m in [0,m); subtract n * (b2-b)/m.  (Same trick, easier: use __int128.)
//
// WHAT IT SOLVES, directly:
//   * #lattice points (x,y) with 1 <= x <= n and 1 <= y <= (a x + b)/m       -> floorSum itself
//   * sum_{i=0}^{n-1} ((a i + b) mod m) = (a * n(n-1)/2 + b*n) - m * floorSum(n, m, a, b)
//   * sum_{i=1}^{n} floor(n / i) in O(sqrt n) instead - use DIVISOR BLOCKS (below), not this.
//   * "how many multiples of q lie in [l, r]" = floor(r/q) - floor((l-1)/q); the basic case.
//
// THE OTHER TWO SUMS of the same family (needed together often enough to be worth knowing they
// exist): S1 = sum i * floor((a i + b)/m) and S2 = sum floor((a i + b)/m)^2 obey the same
// recursion with a 3-tuple state - the "universal Euclidean algorithm" (万能欧几里得) generalises
// this to any monoid: you supply the two operators U ("move right") and R ("move up") and it
// composes the word for the line in O(log) monoid multiplications.
//
// DIVISOR BLOCKS (数论分块) - the other floor trick, O(sqrt n):
//   floor(n / i) is constant on maximal blocks; for a block starting at l the block ends at
//   r = n / (n / l). Use it for sum_{i=1}^{n} floor(n/i), sum of tau/sigma, Du sieve, Mobius sums.
//     for (ll l = 1, r; l <= n; l = r + 1) { r = n / (n / l); /* value n/l on [l, r] */ }
//   Two-argument version (blocks constant for BOTH n/i and m/i): r = min(n/(n/l), m/(m/l)).
