ll pw(ll b, ll p, ll MOD) {
    if (!p)
        return 1;
    ll ans = pw(b, p / 2, MOD);
    ans = (ans * ans) % MOD;
    if (p % 2) ans = (ans * b) % MOD;
    return ans;
}

ll inv(ll x, ll MOD) { return pw(x, MOD - 2, MOD); }

vector<ll> gauss(vector<vector<ll> > &a, ll MOD) {
    int n = a.size(), m = a[0].size() - 1;

    for (int i = 0 ; i < n ; i++)
        for (int j = 0 ; j <= m ; j++)
            a[i][j] = (a[i][j] % MOD + MOD) % MOD;

    vector<int> where(m, -1);
    for (int col = 0, row = 0 ; col < m && row < n ; col++) {
        int sel = row;
        for (int i = row ; i < n ; i++)
            if (a[i][col] > a[sel][col])
                sel = i;

        if (a[sel][col] == 0) {
            where[col] = -1;
            continue;
        }

        for (int i = col ; i <= m ; i++)
            swap(a[sel][i], a[row][i]);
        where[col] = row;

        ll c_inv = inv(a[row][col], MOD);
        for (int i = 0 ; i < n ; i++)
            if (i != row) {
                if (a[i][col] == 0) continue;
                ll c = (a[i][col] * c_inv) % MOD;
                for (int j = 0 ; j <= m ; j++)
                    a[i][j] = (a[i][j] - c * a[row][j] % MOD + MOD) % MOD;
            }

        row++;
    }
    vector<ll> ans(m, 0);
    ll ways = 1;

    for (int i = 0 ; i < m ; i++)
        if (where[i] != -1) ans[i] = (a[where[i]][m] * inv(a[where[i]][i], MOD)) % MOD;
        else ways = (ways * MOD) % MOD;

    for (int i = 0 ; i < n ; i++) {
        ll sum = a[i][m] % MOD;
        for (int j = 0 ; j < m ; j++)
            sum = (sum + MOD - (ans[j] * a[i][j]) % MOD) % MOD;

        if (sum != 0) return {}; //Has No Sol
    }

    return ans;
}

const double EPS = 1e-6;
const int INF = 2; // it doesn't actually have to be infinity or a big number

int gauss(vector<vector<double> > a, vector<double> &ans) {
    int n = (int) a.size();
    int m = (int) a[0].size() - 1;

    vector<int> where(m, -1);
    for (int col = 0, row = 0 ; col < m && row < n ; ++col) {
        int sel = row;
        for (int i = row ; i < n ; ++i)
            if (abs(a[i][col]) > abs(a[sel][col]))
                sel = i;
        if (abs(a[sel][col]) < EPS)
            continue;
        for (int i = col ; i <= m ; ++i)
            swap(a[sel][i], a[row][i]);
        where[col] = row;

        for (int i = 0 ; i < n ; ++i)
            if (i != row) {
                double c = a[i][col] / a[row][col];
                for (int j = col ; j <= m ; ++j)
                    a[i][j] -= a[row][j] * c;
            }
        ++row;
    }

    ans.assign(m, 0);
    for (int i = 0 ; i < m ; ++i)
        if (where[i] != -1)
            ans[i] = a[where[i]][m] / a[where[i]][i];
    for (int i = 0 ; i < n ; ++i) {
        double sum = 0;
        for (int j = 0 ; j < m ; ++j)
            sum += ans[j] * a[i][j];
        if (abs(sum - a[i][m]) > EPS)
            return 0;
    }

    for (int i = 0 ; i < m ; ++i)
        if (where[i] == -1)
            return INF;
    return 1;
}
