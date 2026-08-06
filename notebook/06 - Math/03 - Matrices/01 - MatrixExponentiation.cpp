int modSum(ll a, ll b) {
    if (a < 0)
        a += MOD;
    if (b < 0)
        b += MOD;
    a += b;
    if (a >= MOD)
        a -= MOD;
    return a;
}

int modProd(ll a, ll b) {
    if (a < 0)
        a += MOD;
    if (b < 0)
        b += MOD;
    a *= b;
    if (a >= MOD)
        a %= MOD;
    return a;
}

typedef vector<vector<int>> matrix;

matrix operator*(const matrix& lhs, const matrix& rhs) {
    int n = lhs.size();
    int m = rhs[0].size();
    int s1 = lhs[0].size(), s2 = rhs.size();
    assert(s1 == s2);
    matrix ret(n, vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < s1; ++j)
            for (int k = 0; k < m; ++k)
                ret[i][k] = modSum(ret[i][k], modProd(lhs[i][j], rhs[j][k]));
    return ret;
}

matrix Identity(int n) {
    matrix ret(n, vector<int>(n));
    for (int i = 0; i < n; ++i) {
        ret[i][i] = 1;
    }
    return ret;
}

matrix mat_power(matrix x, ll p) {
    matrix res = Identity(x.size());
    while (p) {
        if (p & 1) res = (res * x);
        x = (x * x);
        p >>= 1;
    }
    return res;
}