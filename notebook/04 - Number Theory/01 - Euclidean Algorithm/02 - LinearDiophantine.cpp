// Linear Diophantine Equations (2 Variables & N Variables)
// 1. Solves a * x + b * y = c (Particular solution & GCD check)
// 2. Solves a_1*x_1 + a_2*x_2 + ... + a_n*x_n = c for N variables
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

    // Solves a * x + b * y = c. Returns false if no solution exists
    static bool solve_2var(ll a, ll b, ll c, ll &x, ll &y, ll &g) {
        if (a == 0 && b == 0) {
            if (c == 0) { x = y = g = 0; return true; }
            return false;
        }
        g = ext_gcd(abs(a), abs(b), x, y);
        if (c % g != 0) return false;
        x *= c / g;
        y *= c / g;
        if (a < 0) x = -x;
        if (b < 0) y = -y;
        return true;
    }

    // Solves a_1*x_1 + a_2*x_2 + ... + a_n*x_n = c for N variables.
    // Returns true if a solution exists and populates x vector
    static bool solve_nvar(const vector<ll>& a, ll c, vector<ll>& x) {
        int n = a.size();
        if (n == 0) return c == 0;
        x.assign(n, 0);

        vector<ll> prefix_g(n);
        prefix_g[0] = abs(a[0]);
        for (int i = 1; i < n; i++) prefix_g[i] = std::gcd(prefix_g[i - 1], a[i]);

        if (prefix_g.back() == 0) return c == 0;
        if (c % prefix_g.back() != 0) return false;

        ll cur_c = c;
        for (int i = n - 1; i > 0; i--) {
            ll x_prev, y_curr, g;
            solve_2var(prefix_g[i - 1], a[i], cur_c, x_prev, y_curr, g);
            x[i] = y_curr;
            cur_c = prefix_g[i - 1] * x_prev;
        }
        x[0] = cur_c / a[0];
        return true;
    }
};
