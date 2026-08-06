const int SZ = 2, mod = 1e9 + 7;

int modSum(ll a, ll b) {
    if (a < 0)
        a += mod;
    if (b < 0)
        b += mod;
    a += b;
    if (a >= mod)
        a -= mod;
    return a;
}

int modProd(ll a, ll b) {
    if (a < 0)
        a += mod;
    if (b < 0)
        b += mod;
    a *= b;
    if (a >= mod)
        a %= mod;
    return a;
}

typedef array<array<int, SZ>, SZ> matrix;

matrix operator*(const matrix &lhs, const matrix &rhs) {
    matrix ret{};
    for (int i = 0; i < SZ; ++i)
        for (int j = 0; j < SZ; ++j)
            for (int k = 0; k < SZ; ++k)
                ret[i][k] = modSum(ret[i][k], modProd(lhs[i][j], rhs[j][k]));
    return ret;
}

matrix Identity(int n) {
    matrix ret = {};
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