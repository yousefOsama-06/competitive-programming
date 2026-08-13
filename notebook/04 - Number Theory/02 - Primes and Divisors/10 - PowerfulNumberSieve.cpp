// Needs: Mint (06 - Math/01 - Modular Arithmetic/01 - Mint.cpp).
// POWERFUL NUMBER SIEVE - prefix sum of a multiplicative f in O(sqrt n) calls to an easy prefix
// sum. Faster and shorter than Min_25 whenever you can find a helper g with
//   (1) g(p) = f(p) for every prime p, and (2) the prefix sum G(x) = sum_{i<=x} g(i) is cheap.
// Then f = g * h (Dirichlet), and h(p) = f(p) - g(p) = 0, so h is supported ONLY on POWERFUL
// numbers (every exponent >= 2) - and there are only O(sqrt n) of those below n. Hence
//   F(n) = sum_{d powerful, d <= n} h(d) * G(n / d),
// which this DFS evaluates. h(p^e) is derived on the fly from f and g by the convolution
// recurrence h(p^e) = f(p^e) - sum_{i<0..e-1} h(p^i) * g(p^(e-i)), so you never derive it by hand.
// PRECONDITION: g(p) = f(p) for ALL primes - check it, the algorithm is silently wrong otherwise.
// COST: O(sqrt n) nodes, each one call to G. With G in O(1) that is O(sqrt n) total; with G in
// O(sqrt x) it is still about O(n^(2/3)). Needs primes up to sqrt(n) (a plain sieve).
// EXAMPLE: f(p^e) = p^e + [e >= 2] pairs with g = Id (G(x) = x(x+1)/2). f = "phi times something"
// style problems usually pair with g = Id or g = 1 or g = sigma (G(x) = sum d*floor(x/d)).
struct PowerfulSieve {
    ll n;
    vector<ll> pr;                                      // primes up to sqrt(n)
    function<Mint(ll, int)> f, g;                       // f(p^e), g(p^e), e >= 1
    function<Mint(ll)> G;                               // prefix sum of g
    PowerfulSieve(ll n, vector<ll> pr, function<Mint(ll, int)> f, function<Mint(ll, int)> g,
                  function<Mint(ll)> G) : n(n), pr(pr), f(f), g(g), G(G) {}
    Mint dfs(int i, ll m, Mint h) {                     // m = a powerful number, h = h(m)
        Mint res = h * G(n / m);
        for (ll lim = n / m; i < (int)pr.size() && pr[i] <= lim / pr[i]; i++) {
            ll p = pr[i], pw = p * p;
            vector<Mint> hp = {1, 0};                   // h(p^0) = 1, h(p^1) = 0
            for (int e = 2; pw <= lim; e++) {
                Mint v = f(p, e);
                for (int j = 0; j < e; j++) v -= hp[j] * g(p, e - j);
                hp.push_back(v);
                res += dfs(i + 1, m * pw, h * v);
                if (pw > lim / p) break;
                pw *= p;
            }
        }
        return res;
    }
    Mint solve() { return dfs(0, 1, 1); }
};
// COUNTING POWERFUL NUMBERS is itself a useful fact: every powerful number is uniquely a^2*b^3
// with b squarefree, so there are about 2.173*sqrt(n) of them below n.
// WHEN g IS HARD TO GUESS: try g = Id^k for the k with f(p) ~ p^k, then fix the difference; or
// fall back to 09 - Min25Sieve.cpp, which needs no partner function at all.
