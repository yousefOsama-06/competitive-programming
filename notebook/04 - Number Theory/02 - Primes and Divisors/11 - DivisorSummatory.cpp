// DIVISOR SUMMATORY FUNCTION  D(n) = sum_{i=1..n} tau(i) = sum_{i=1..n} floor(n/i)
// in ~O(n^(1/3)) instead of the usual O(sqrt n) divisor blocks. About 100 ms at n = 1e18.
// Reach for it when n is 1e14 or more (blocks are fine up to ~1e12), or when many queries make
// the sqrt too slow. The result exceeds 64 bits around n ~ 4e17, hence __int128.
// HOW: the hyperbola x*y = n is walked with a STERN-BROCOT search over slopes. Starting from the
// point nearest the diagonal, we repeatedly take the longest lattice segment of the current slope
// that stays under the hyperbola (counting the trapezoid under it in O(1)), and refine the slope
// by mediants when the curvature no longer allows it. Below y ~ n^(1/3) the slopes get shorter
// than one step, so the tail is finished with a plain loop.
__int128 divisorSummatory(unsigned long long n) {
    if (n == 0) return 0;
    auto out = [n](unsigned long long x, unsigned y) { return x * y > n; };   // above the curve?
    auto cut = [n](unsigned long long x, unsigned dx, unsigned dy) {
        return (__uint128_t)x * x * dy >= (__uint128_t)n * dx;                // slope too steep
    };
    unsigned long long sn = (unsigned long long)sqrtl((ld)n);
    while (sn * sn > n) sn--;
    while ((sn + 1) * (sn + 1) <= n) sn++;
    unsigned long long cn = (unsigned long long)powl((ld)n, 0.34L);           // ~ n^(1/3)
    unsigned long long x = n / sn;
    unsigned y = (unsigned)(n / x + 1);
    __int128 ret = 0;
    stack<pair<unsigned, unsigned>> st;
    st.emplace(1, 0), st.emplace(1, 1);                 // the Stern-Brocot stack of slopes
    while (true) {
        unsigned lx, ly;
        tie(lx, ly) = st.top(); st.pop();
        while (out(x + lx, y - ly)) {                   // walk as long as the segment fits
            ret += (__int128)x * ly + (__int128)(ly + 1) * (lx - 1) / 2;
            x += lx, y -= ly;
        }
        if (y <= cn) break;
        unsigned rx = lx, ry = ly;
        while (true) {                                  // pop until a usable slope is on top
            tie(lx, ly) = st.top();
            if (out(x + lx, y - ly)) break;
            rx = lx, ry = ly, st.pop();
        }
        while (true) {                                  // mediant refinement
            unsigned mx = lx + rx, my = ly + ry;
            if (out(x + mx, y - my)) st.emplace(lx = mx, ly = my);
            else { if (cut(x + mx, lx, ly)) break; rx = mx, ry = my; }
        }
    }
    for (--y; y > 0; --y) ret += n / y;                 // the small-y tail, plain loop
    return ret * 2 - (__int128)sn * sn;                 // Dirichlet hyperbola symmetry
}
// EXACT VALUES for testing: D(10) = 27, D(100) = 482, D(1e6) = 13970034, D(1e9) = 20951330018,
// D(1e18) = 31264402477813376929. Asymptotically D(n) = n ln n + (2*gamma - 1) n + O(n^(1/3)).
// RELATED SUMS with the same blocks: sum sigma(i) = sum i*floor(n/i), and any sum_{i} f(n/i);
// for those, plain O(sqrt n) blocks (03 - FloorSum.cpp) are the right tool.
