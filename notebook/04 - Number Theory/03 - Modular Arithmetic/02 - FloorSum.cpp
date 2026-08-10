// Floor Sum - Time: O(log m), Space: O(log m)
// Computes sum_{i=0}^{n-1} floor((a * i + b) / m)
// Valid for 0 <= n, 0 < m, 0 <= a, 0 <= b
ll floor_sum(ll n, ll m, ll a, ll b) {
    ll ans = 0;
    if (a >= m) {
        ans += (n - 1) * n / 2 * (a / m);
        a %= m;
    }
    if (b >= m) {
        ans += n * (b / m);
        b %= m;
    }

    ll y_max = (a * n + b) / m;
    ll x_max = y_max * m - b;
    if (y_max == 0) return ans;
    ans += (n - (x_max + a - 1) / a) * y_max;
    ans += floor_sum(y_max, a, m, (a - x_max % a) % a);
    return ans;
}
