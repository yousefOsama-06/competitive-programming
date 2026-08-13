// NOTE: extGCD below is the same one as in 01 - Euclidean Algorithm. Paste only one copy.
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

// Solves x = a_i (mod m_i). Returns {x, lcm(m_1, ..., m_k)}.
pair<long long, long long> CRT(const vector<long long>& a, const vector<long long>& m) {
    long long res = a[0], lcm = m[0];
    for (int i = 1; i < a.size(); i++) {
        long long x, y;
        long long g = extGCD(lcm, m[i], x, y);
        if ((a[i] - res) % g != 0) return {-1, -1}; // No solution
        
        long long step = m[i] / g;
        long long k = ((a[i] - res) / g) % step * (x % step) % step;
        res = (long long)(((__int128)k * lcm + res) % ((__int128)lcm * step));
        lcm *= step;
        res = (res % lcm + lcm) % lcm;
    }
    return {res, lcm};
}
