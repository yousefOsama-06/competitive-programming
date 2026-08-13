#include <bits/stdc++.h>
using namespace std;

//#define int long long
#define ll long long
#define lll __int128                            // used unnamed by CHT, BigInt, PrimeCounting
#define ld long double
#define fi first
#define se second
#define pb push_back
#define eb emplace_back
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define sz(x) (int)(x).size()
#define ones(n) __builtin_popcountll(n)
#define msb(n) (63 - __builtin_clzll(n))
#define lsb(n) __builtin_ctzll(n)
typedef vector<int> vi;
typedef pair<int, int> pii;

const int N = 2e5 + 5, M = 1e3 + 5, LOG = 31;
const int inf = 0x3f3f3f3f;
const ll llinf = 0x3f3f3f3f3f3f3f3f;
const int MOD = 1e9 + 7;
const ld eps = 1e-9, PI = acosl(-1.0L);        // ld, not double: geometry needs the digits

#ifdef LOCAL                                    // compile with -DLOCAL
template <class T> void _p(const T& x);
void _p(const string& s) { cerr << '"' << s << '"'; }
void _p(char c) { cerr << '\'' << c << '\''; }
template <class A, class B> void _p(const pair<A, B>& p) {
    cerr << '('; _p(p.fi); cerr << ", "; _p(p.se); cerr << ')';
}
template <class T> void _p(const T& x) {
    if constexpr (is_arithmetic_v<T>) cerr << x;
    else { cerr << '{'; int f = 1;
           for (auto& e : x) { if (!f) cerr << ", "; f = 0; _p(e); } cerr << '}'; }
}
void _pr() { cerr << "\n"; }
template <class H, class... T> void _pr(const H& h, const T&... t) {
    _p(h); if (sizeof...(t)) cerr << " | "; _pr(t...);
}
#define dbg(...) (cerr << "[" << #__VA_ARGS__ << "] = ", _pr(__VA_ARGS__))
#else
#define dbg(...)
#endif

void testCase() {
}

void preCompute() {
}

int32_t main() {
    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    cout << fixed << setprecision(10);
    preCompute();

    int tc = 1;
    cin >> tc;                                  // DELETE THIS LINE for single-test problems
    for (int TC = 1; TC <= tc; TC++) {
        // cout << "Case #" << TC << ": ";
        testCase();
    }
}

/*



*/
