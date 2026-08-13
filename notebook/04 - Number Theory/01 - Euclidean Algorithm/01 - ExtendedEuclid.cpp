// EXTENDED EUCLID - returns g = gcd(a,b) and x, y with a*x + b*y = g. This is the one routine that
// gives you modular inverses for a NON-prime modulus, solves linear Diophantine equations, and
// underpins CRT. |x| <= b/(2g) and |y| <= a/(2g), so no overflow for 64-bit inputs.
long long extGCD(long long a, long long b, long long &x, long long &y) {
    if (b == 0) {
        x = 1; y = 0;
        return a;
    }
    long long x1, y1;
    long long d = extGCD(b, a % b, x1, y1);
    x = y1;
    y = x1 - y1 * (a / b);
    return d;
}

long long modInverse(long long a, long long m) {
    long long x, y;
    long long g = extGCD(a, m, x, y);
    if (g != 1) return -1; // Inverse doesn't exist
    return (x % m + m) % m;
}
