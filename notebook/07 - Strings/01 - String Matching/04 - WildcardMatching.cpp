// Needs: conv (06 - Math/05 - Convolution/01 - FFT.cpp).
// STRING MATCHING WITH WILDCARDS in O(n log n). A wildcard matches any character, on EITHER side.
// Map every real character to a positive value and every wildcard to 0. Then position i matches iff
//        sum_j  ( p[j] - t[i+j] )^2 * p[j] * t[i+j]  ==  0,
// because each term is >= 0 and vanishes exactly when the characters agree or one of them is 0.
// Expanding gives three correlations: sum p^3 t - 2 sum p^2 t^2 + sum p t^3, and a correlation is a
// convolution with the pattern reversed.
vector<int> wildcardMatch(const string& t, const string& p, char wild = '?') {
    int n = t.size(), m = p.size();
    if (m > n) return {};
    // The identity needs values >= 0 and wildcard == 0. Compress the characters that actually
    // occur to 1..k - with a raw `c - 'a' + 1` any character below 'a' is NEGATIVE and the terms
    // can cancel, which reports matches that do not exist (e.g. text "_b" against pattern "Z]").
    map<char, ll> id;
    for (char c : t) if (c != wild) id.emplace(c, 0);
    for (char c : p) if (c != wild) id.emplace(c, 0);
    ll k = 0;
    for (auto& [c, v] : id) v = ++k;
    auto val = [&](char c) { return c == wild ? 0LL : id[c]; };
    vector<ll> a(m), b(n);
    for (int i = 0; i < m; i++) a[i] = val(p[m - 1 - i]);       // reversed pattern
    for (int i = 0; i < n; i++) b[i] = val(t[i]);
    auto pw = [](vector<ll> v, int k) { for (ll& x : v) { ll y = 1; for (int i = 0; i < k; i++) y *= x; x = y; } return v; };
    auto c1 = conv(pw(a, 3), b), c2 = conv(pw(a, 2), pw(b, 2)), c3 = conv(a, pw(b, 3));
    vector<int> res;
    for (int i = 0; i + m <= n; i++)
        if (c1[i + m - 1] - 2 * c2[i + m - 1] + c3[i + m - 1] == 0) res.push_back(i);
    return res;                                                // 0-based start positions
}
// PRECISION: the largest intermediate is about k^4 * m, where k is the number of DISTINCT real
// characters (which is why compressing is free). With k = 26 and m = 1e5 that is
// ~4.6e10, comfortable for double FFT. For a big alphabet, compress the characters that actually
// occur first, or run the three convolutions under two NTT primes and CRT.
// COUNT MISMATCHES at every shift (no wildcards, small alphabet): for each letter c convolve the
// 0/1 indicator of c in p (reversed) with the indicator of c in t; summing gives the number of
// MATCHES per shift, and mismatches = m - matches. O(|sigma| n log n).
// k-MISMATCH: same counts, keep shifts with matches >= m - k.
// SUBSET / "at most one character class" matching: encode each position as a bitmask over the
// alphabet and convolve the complement indicators; a shift is valid iff the total is 0.
