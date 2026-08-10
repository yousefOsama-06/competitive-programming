// Formal Power Series (FPS) / Generating Functions - Time: O(N log N)
// Operations: Derivative, Integral, Polynomial Inverse, Logarithm ln(F(x))
template<int MOD = 998244353>
struct PolynomialFPS {
    typedef vector<ll> Poly;

    static Poly deriv(const Poly& f) {
        int n = f.size();
        if (n <= 1) return {0};
        Poly g(n - 1);
        for (int i = 1; i < n; i++) g[i - 1] = f[i] * i % MOD;
        return g;
    }

    static Poly integ(const Poly& f) {
        int n = f.size();
        Poly g(n + 1);
        for (int i = 0; i < n; i++) g[i + 1] = f[i] * modInverse(i + 1, MOD) % MOD;
        return g;
    }

    // Polynomial Inverse mod x^deg - O(N log N) via Newton's Method
    static Poly inv(const Poly& f, int deg) {
        if (deg == 1) return {modInverse(f[0], MOD)};
        Poly g = inv(f, (deg + 1) / 2);
        Poly f_cut(f.begin(), f.begin() + min((int)f.size(), deg));
        
        // g_new = g * (2 - f * g) mod x^deg
        Poly fg = multiply(f_cut, g);
        fg.resize(deg);
        for (int i = 0; i < deg; i++) {
            fg[i] = (i == 0 ? 2 : 0) - fg[i];
            if (fg[i] < 0) fg[i] += MOD;
        }
        Poly res = multiply(g, fg);
        res.resize(deg);
        return res;
    }

    // Polynomial Logarithm ln(F(x)) mod x^deg - O(N log N) (requires F[0] == 1)
    static Poly log(const Poly& f, int deg) {
        Poly g = integ(multiply(deriv(f), inv(f, deg)));
        g.resize(deg);
        return g;
    }

private:
    static Poly multiply(Poly a, Poly b) {
        // Basic NTT polynomial multiplication helper
        int n = 1;
        while (n < a.size() + b.size()) n <<= 1;
        Poly res(a.size() + b.size() - 1, 0);
        for (int i = 0; i < a.size(); i++) {
            for (int j = 0; j < b.size(); j++) {
                res[i + j] = (res[i + j] + a[i] * b[j]) % MOD;
            }
        }
        return res;
    }

    static ll modInverse(ll a, ll m) {
        ll x, y;
        ext_gcd(a, m, x, y);
        return (x % m + m) % m;
    }

    static ll ext_gcd(ll a, ll b, ll &x, ll &y) {
        if (b == 0) { x = 1; y = 0; return a; }
        ll x1, y1, d = ext_gcd(b, a % b, x1, y1);
        x = y1; y = x1 - y1 * (a / b);
        return d;
    }
};
