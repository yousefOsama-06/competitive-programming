int mul(const ll &a,const ll&b){
    return (a % MOD + MOD) * (b % MOD + MOD) % MOD;
}

int add(const ll &a,const ll&b){
    return (a + b + 2 * MOD)%MOD;
}

typedef vector<vector<int>> matrix;

matrix operator*(const matrix &lhs, const matrix &rhs) {
    int n = lhs.size();
    int m = rhs[0].size();
    int s1 = lhs[0].size(),s2 = rhs.size();
    assert(s1 == s2);
    matrix ret(n,vector<int>(m));
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < s1; ++j)
            for (int k = 0; k < m; ++k)
                ret[i][k] = add(ret[i][k], mul(lhs[i][j],rhs[j][k]));
    return ret;
}

matrix Identity(int n) {
    matrix ret(n,vector<int>(n));
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