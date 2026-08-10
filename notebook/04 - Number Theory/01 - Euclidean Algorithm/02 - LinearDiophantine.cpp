// Linear Diophantine Equation - Solves a * x + b * y = c
// Returns false if no integer solution exists (i.e. c is not divisible by gcd(a, b))
struct LinearDiophantine {
    static ll ext_gcd(ll a, ll b, ll &x, ll &y) {
        if (b == 0) {
            x = 1; y = 0;
            return a;
        }
        ll x1, y1;
        ll d = ext_gcd(b, a % b, x1, y1);
        x = y1;
        y = x1 - y1 * (a / b);
        return d;
    }

    // Finds one particular solution (x, y) for a * x + b * y = c
    static bool find_any_solution(ll a, ll b, ll c, ll &x, ll &y, ll &g) {
        g = ext_gcd(abs(a), abs(b), x, y);
        if (c % g != 0) return false;
        x *= c / g;
        y *= c / g;
        if (a < 0) x = -x;
        if (b < 0) y = -y;
        return true;
    }
};
