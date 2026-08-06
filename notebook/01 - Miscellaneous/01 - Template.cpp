#include <bits/stdc++.h>

using namespace std;

//#define int long long
#define ll long long
#define ld long double
#define fi first
#define se second
#define pb push_back
#define eb emplace_back
#define all(x) x.begin(), x.end()
#define rall(x) x.rbegin(), x.rend()
#define ones(n) __builtin_popcountll(n)
#define msb(n) (63 - __builtin_clzll(n))
#define lsb(n) __builtin_ctzll(n)

const int N = 2e5 + 5, M = 1e3 + 5, LOG = 31;
const int inf = 0x3f3f3f3f;
const ll llinf = 0x3f3f3f3f3f3f3f3f;
const int MOD = 1e9 + 7;
const double eps = 1e-9, PI = acos(-1);

void testCase() {
}


void preCompute() {
}

int32_t main() {
#ifdef C_Lion
    freopen("input.txt", "r", stdin);
    freopen("output.txt", "w", stdout);
    freopen("errors.txt", "w", stderr);
#else
    //    freopen("input.in", "r", stdin);
    //    freopen("output.out", "w", stdout);
#endif

    ios_base::sync_with_stdio(false), cin.tie(nullptr), cout.tie(nullptr);
    cout << fixed << setprecision(static_cast<int32_t>(-log10(eps)));
    preCompute();

    int tc = 1;
    cin >> tc;
    for (int TC = 1; TC <= tc; TC++) {
        // cout << "Case #" << TC << ": ";
        testCase();
    }
}

/*




*/
