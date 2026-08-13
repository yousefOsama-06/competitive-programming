// MONTGOMERY MULTIPLICATION - modular multiplication for a FIXED ODD 64-bit modulus without any
// hardware division: 2-3x faster than (__int128)a*b % m. Use it in the hot loop of Miller-Rabin,
// Pollard rho, BSGS, matrix power or any modpow-heavy routine with a 64-bit modulus. For a
// COMPILE-TIME modulus the compiler already turns % into a multiply-shift, so this buys nothing;
// the win is for a modulus known only at run time.
// TRICK: work with the representative a*R mod m where R = 2^64. reduce(x) computes x*R^-1 mod m
// with two multiplications and no division, so (aR)(bR)*R^-1 = (ab)R stays in the same form.
// PRECONDITIONS: m must be ODD (the Newton iteration inverts m modulo 2^64) and m < 2^63.
// Values returned by get() are in [0, m); mul/add work on the Montgomery form.
typedef unsigned long long u64;                          // same typedef as 02/06 - keep one copy
struct Mont {
    u64 mod, minv, r2;                                   // minv = m^-1 mod 2^64
    void set(u64 m) {
        mod = minv = m;
        for (int i = 0; i < 5; i++) minv *= 2 - minv * m;      // Newton: doubles the bits each time
        r2 = (u64)(-(__uint128_t)m % m);                       // R^2 mod m
    }
    u64 reduce(__uint128_t x) const {
        u64 y = (u64)(x >> 64) - (u64)(((__uint128_t)((u64)x * minv) * mod) >> 64);
        return (long long)y < 0 ? y + mod : y;
    }
    u64 in(u64 a) const { return reduce((__uint128_t)(a % mod) * r2); }   // to Montgomery form
    u64 get(u64 a) const { u64 y = reduce(a); return y >= mod ? y - mod : y; }   // back to normal
    u64 mul(u64 a, u64 b) const { return reduce((__uint128_t)a * b); }
    u64 add(u64 a, u64 b) const { a += b - mod; return (long long)a < 0 ? a + mod : a; }
    u64 sub(u64 a, u64 b) const { a -= b; return (long long)a < 0 ? a + mod : a; }
    u64 pow(u64 a, u64 e) const {                        // a already in Montgomery form
        u64 r = in(1);
        for (; e; e >>= 1, a = mul(a, a)) if (e & 1) r = mul(r, a);
        return r;
    }
};
// USAGE:  Mont M; M.set(n);  u64 x = M.in(3);  x = M.pow(x, d);  cout << M.get(x);
// Do NOT mix forms: everything inside the loop stays in Montgomery form, converting only at the
// boundaries. Comparisons against 1 and m-1 (Miller-Rabin) must compare against M.in(1) and
// M.in(m-1), which you compute once.
// EVEN MODULUS: split m = 2^k * odd, run Montgomery on the odd part and the trivial bit-mask on
// the power of two, then CRT. Usually not worth it - just use __int128.
// ALTERNATIVE: BARRETT reduction works for even moduli too (precompute floor(2^64/m) and replace
// the division by a multiply-shift); it is a little slower but has no parity restriction.
