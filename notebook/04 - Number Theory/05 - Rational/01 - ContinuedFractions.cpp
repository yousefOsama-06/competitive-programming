// Continued fractions, convergents, best rational approximation, Stern-Brocot search.

// x = [a0; a1, a2, ...]. For a rational p/q this is just the Euclidean algorithm's quotients.
vector<ll> cfrac(ll p, ll q) {
    vector<ll> a;
    // FLOOR division: C++ truncates toward zero, so cfrac(-7,2) would give [-3,-2] instead of
    // the correct [-4,2] and every convergent would be wrong.
    while (q) { ll d = p / q - ((p % q) && ((p < 0) ^ (q < 0))); a.push_back(d); ll r = p - d * q; p = q, q = r; }
    return a;
}
// Convergents h[i]/k[i] of a continued fraction: h = a*h1 + h2, k = a*k1 + k2.
// Every convergent is a BEST approximation: no fraction with a smaller denominator is closer.
vector<pair<ll, ll>> convergents(const vector<ll>& a) {
    vector<pair<ll, ll>> c;
    ll h1 = 1, h2 = 0, k1 = 0, k2 = 1;
    for (ll x : a) {
        ll h = x * h1 + h2, k = x * k1 + k2;
        c.push_back({h, k});
        h2 = h1, h1 = h, k2 = k1, k1 = k;
    }
    return c;
}
// Best rational approximation to p/q with denominator <= N.
// The answer is a convergent OR a SEMICONVERGENT (t*h1+h2)/(t*k1+k2) - you must compare both.
pair<ll, ll> bestApprox(ll p, ll q, ll N) {
    ll h1 = 1, h2 = 0, k1 = 0, k2 = 1, P = p, Q = q;
    pair<ll, ll> best = {p >= 0 ? 0 : -((-p + q - 1) / q), 1};      // 0/1 (or floor) is legal
    auto better = [&](pair<ll, ll> a, pair<ll, ll> b) {             // is a closer to p/q than b?
        __int128 da = (__int128)llabs(p * a.second - a.first * q) * b.second;
        __int128 db = (__int128)llabs(p * b.second - b.first * q) * a.second;
        return da != db ? da < db : a.second < b.second;
    };
    while (Q) {
        ll a = P / Q, r = P % Q;
        if (k1 && a * k1 + k2 > N) {                                // cannot take the full step
            ll t = (N - k2) / k1;
            if (t > 0) { pair<ll, ll> c = {t * h1 + h2, t * k1 + k2}; if (better(c, best)) best = c; }
            break;
        }
        ll h = a * h1 + h2, k = a * k1 + k2;
        if (k > N) break;
        h2 = h1, h1 = h, k2 = k1, k1 = k;
        if (better({h1, k1}, best)) best = {h1, k1};
        P = Q, Q = r;
    }
    return best;
}
// STERN-BROCOT SEARCH: find the simplest fraction p/q with f(p/q) true, when f is monotone.
// Walk the mediant tree; take exponential jumps in each direction so it is O(log^2).
// Classic uses: "smallest denominator between two reals", recovering a fraction from a decimal,
// and answering "is the answer rational with small denominator" style problems.
// FAREY / mediant fact: if a/b < c/d are neighbours (bc - ad = 1) then their mediant
// (a+c)/(b+d) is the unique simplest fraction strictly between them.
