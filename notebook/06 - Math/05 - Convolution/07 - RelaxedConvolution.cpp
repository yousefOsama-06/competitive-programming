// Needs: multiply (02 - NTT.cpp).
// ONLINE (RELAXED) CONVOLUTION. You feed a[i] and b[i] one index at a time and immediately get
// c[i] = sum_{j+k=i} a[j] b[k], BEFORE you have to supply a[i+1] - which is exactly what a
// self-referential DP needs. Total O(n log^2 n) for all n steps; ~1s at n = 2e5.
// REACH FOR IT whenever f[i] depends on a convolution of f with itself (or with something f
// determines) up to index i - 1: f[n] = sum f[k] f[n-1-k] (Catalan-shaped DPs), f[n] = sum
// g[k] f[n-k] where you only learn g[n] after computing f[n-1], probability DPs with a feedback
// loop, ABC315-H style. Divide and conquer over the recursion is the alternative and is usually
// harder to write; a plain O(n^2) double loop is the thing you are replacing.
// The blocks are scheduled so that the product of two length-w blocks is done exactly when both
// halves are known, and every coefficient is covered exactly once.
struct Relaxed {
    vector<ll> a, b, c;
    int k = 0;
    Relaxed(int n) : a(n), b(n), c(2 * n, 0) {}
    // supply a[i] = x and b[i] = y (i must equal the current index); returns c[i].
    ll extend(ll x, ll y) {
        a[k] = x % NMOD, b[k] = y % NMOD;
        int s = k + 2;
        for (int w = 1; s % w == 0 && w < s; w <<= 1) {
            for (int t = 0; t < 2; t++) {
                if (t == 0 || w * 2 != s) {              // the symmetric block is done once only
                    vector<ll> f(a.begin() + w - 1, a.begin() + 2 * w - 1);
                    vector<ll> g(b.begin() + k - w + 1, b.begin() + k + 1);
                    f = multiply(f, g);
                    for (size_t i = 0; i < f.size() && k + i < c.size(); i++)
                        c[k + i] = (c[k + i] + f[i]) % NMOD;
                }
                swap(a, b);                              // and again, so a/b are restored
            }
        }
        return c[k++];
    }
};
// SELF-CONVOLUTION shorthand: pass the same value twice, extend(f[i], f[i]).
// TYPICAL SHAPE
//   Relaxed R(n); f[0] = 1;
//   for (i = 1; i <= n; i++) { ll t = R.extend(f[i-1], f[i-1]); f[i] = /* uses t */; }
//   Note the OFF-BY-ONE: after the i-th extend you know coefficient i-1 of the product, so the
//   value you feed in must be the one that is already final.
// WHEN NOT TO: if the recurrence is f[n] = sum_{k<n} g[k] f[n-k] with g KNOWN in advance, the
// cheaper standard answer is divide and conquer over [l, r) (contribute [l, mid) to [mid, r) with
// one multiply), same O(n log^2 n) but a smaller constant and no bookkeeping.
// If instead the whole system is polynomial (f = F(f) as power series), Newton iteration on the
// series (04 - Polynomial.cpp: inv / log / exp / sqrt) is O(n log n) and beats both.
