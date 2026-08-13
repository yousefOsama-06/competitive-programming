// RANKING / UNRANKING COMBINATORIAL OBJECTS, and CALENDAR ARITHMETIC. Small, and each one costs
// 20 minutes to re-derive under pressure.

// --- FACTORIAL NUMBER SYSTEM: rank and unrank a PERMUTATION. O(n^2) as written; use an
// order-statistic tree or a BIT for O(n log n) when n is large.
// order(p) = how many permutations of the same multiset are lexicographically smaller.
ll permRank(vector<int> p) {                                   // p is a permutation of 0..n-1
    int n = p.size();
    ll r = 0, f = 1;
    for (int i = n - 1; i >= 0; i--) {
        int c = 0;
        for (int j = i + 1; j < n; j++) if (p[j] < p[i]) c++;   // the Lehmer code digit
        r += c * f, f *= (n - i);
    }
    return r;
}
vector<int> permUnrank(int n, ll k) {                          // k-th permutation, 0-indexed
    vector<int> avail(n), p(n);
    iota(all(avail), 0);
    vector<ll> f(n + 1, 1);
    for (int i = 1; i <= n; i++) f[i] = f[i - 1] * i;
    for (int i = 0; i < n; i++) {
        ll d = k / f[n - 1 - i];
        k %= f[n - 1 - i];
        p[i] = avail[d], avail.erase(avail.begin() + d);
    }
    return p;
}
// NEXT PERMUTATION in place is std::next_permutation, O(n) - use it unless you need the rank.
// COMBINATIONS: rank of a k-subset {c_1<...<c_k} of [n] is sum C(c_i, i); unrank greedily by
// finding the largest c with C(c, i) <= remaining. Same shape, C instead of factorials.
// GRAY CODE is g = i ^ (i >> 1) (06 - grayCode.cpp).

// --- BALANCED BRACKET SEQUENCES: rank, unrank, and the next one in lexicographic order, with
// '(' < ')'. The counting table is the BALLOT NUMBERS: f[i][j] = number of ways to finish when i
// characters remain and j brackets are still open. f[n][0] is the Catalan number C_{n/2}.
// USES: "the k-th valid bracket sequence", "how many valid sequences are smaller than this one",
// iterating all sequences of length 2m without generating and filtering. Same skeleton works for
// any object counted by a DP: the rank is a walk that adds the subtree sizes you skip over.
// O(n^2) to build the table, O(n) per rank / unrank / next. Overflows for n > 60 - use __int128
// or a modulus if you only need the count.
vector<vector<ll>> ballot(int n) {                             // n = total length, must be even
    vector<vector<ll>> f(n + 1, vector<ll>(n + 2, 0));
    f[0][0] = 1;
    for (int i = 1; i <= n; i++)
        for (int j = 0; j <= i; j++)
            f[i][j] = f[i - 1][j + 1] + (j ? f[i - 1][j - 1] : 0);   // place '(' then ')'
    return f;
}
ll bracketRank(const string& s, const vector<vector<ll>>& f) {  // 0-indexed rank, s must be valid
    int n = s.size(), j = 0;
    ll r = 0;
    for (int i = 0; i < n; i++) {
        if (s[i] == ')') r += f[n - i - 1][j + 1];              // skip every sequence with '(' here
        j += s[i] == '(' ? 1 : -1;
    }
    return r;
}
string bracketUnrank(int n, ll k, const vector<vector<ll>>& f) {
    string s;
    int j = 0;
    for (int i = 0; i < n; i++) {
        ll c = f[n - i - 1][j + 1];                             // completions starting with '('
        if (k < c) s += '(', j++;
        else k -= c, s += ')', j--;
    }
    return s;
}
bool nextBracket(string& s) {                                  // in place; false if s is the last
    int n = s.size(), d = 0;
    for (int i = n - 1; i >= 0; i--) {
        d += s[i] == '(' ? -1 : 1;
        if (s[i] == '(' && d > 0) {                             // flip it to ')' and reset the rest
            d--;
            int open = (n - i - 1 - d) / 2;
            s = s.substr(0, i) + ')' + string(open, '(') + string(n - i - 1 - open, ')');
            return true;
        }
    }
    return false;
}

// --- JULIAN DAY NUMBER: date <-> integer, closed form, no loops, correct for the Gregorian
// calendar including every leap rule. Date differences, "what weekday", and "add 1000 days" all
// become integer arithmetic once you are here.
ll dateToInt(int y, int m, int d) {                            // y-m-d Gregorian -> day number
    return 1461 * (y + 4800 + (m - 14) / 12) / 4
         + 367 * (m - 2 - (m - 14) / 12 * 12) / 12
         - 3 * ((y + 4900 + (m - 14) / 12) / 100) / 4 + d - 32075;
}
array<int, 3> intToDate(ll jd) {                               // inverse: {y, m, d}
    ll x, n, i, j, dd;
    x = jd + 68569, n = 4 * x / 146097, x -= (146097 * n + 3) / 4;
    i = 4000 * (x + 1) / 1461001, x -= 1461 * i / 4 - 31;
    j = 80 * x / 2447, dd = x - 2447 * j / 80, x = j / 11;
    return {(int)(100 * (n - 49) + i + x), (int)(j + 2 - 12 * x), (int)dd};
}
int weekday(ll jd) { return (int)((jd + 1) % 7); }              // 0 = Sunday
// LEAP YEAR: y % 4 == 0 && (y % 100 != 0 || y % 400 == 0).
// ZELLER'S CONGRUENCE gives the weekday directly, but the day number is strictly more useful.

// --- STERN-BROCOT SEARCH: the simplest fraction satisfying a MONOTONE predicate. ---
// Descend the mediant tree, but take many steps in one direction at once (double the step size)
// so a 1e18 denominator costs O(log^2) instead of O(answer).
// USES: "smallest denominator q with property P", recovering p/q from a decimal prefix, the
// simplest fraction strictly inside an interval, and best rational approximation with a bound.
// ok() must be MONOTONE in the value p/q: false on small fractions, true from some point on.
// Returns the SMALLEST such fraction with denominator <= N. Each iteration takes as many steps
// as possible in one direction (found by doubling, then binary search), so it is O(log^2 N)
// rather than O(answer) - the difference between instant and hopeless at N = 1e18.
template <class F>
pair<ll, ll> fracSearch(ll N, F ok) {
    ll lp = 0, lq = 1, rp = 1, rq = 0;                          // 0/1 (not ok) .. 1/0 (ok)
    for (;;) {
        ll moved = 0;
        auto walk = [&](ll& ap, ll& aq, ll bp, ll bq, bool want) {   // steps from a toward b
            ll k = 0, s = 1;
            auto test = [&](ll t) {
                return aq + t * bq <= N && ok(ap + t * bp, aq + t * bq) == want;
            };
            while (test(k + s)) k += s, s *= 2;
            for (s /= 2; s; s /= 2) if (test(k + s)) k += s;
            ap += k * bp, aq += k * bq, moved += k;
        };
        walk(lp, lq, rp, rq, false);                            // push the FALSE end up
        if (lq + rq > N) return {rp, rq};
        walk(rp, rq, lp, lq, true);                             // pull the TRUE end down
        if (lq + rq > N || !moved) return {rp, rq};             // !moved: nothing left to refine
    }
}
// The Stern-Brocot tree IS the continued fraction expansion: going left/right k times is one
// partial quotient. See 04 - Number Theory/05 - Rational for the CF machinery and semiconvergents.
