int fac[N],inv[N];

int mul(const int &a, const int &b) {
    return 1ll * a * b % mod;
}

int add(int a, int b) {
    a += b;
    if (a >= mod)a -= mod;
    if (a < 0)a += mod;
    return a;
}

int pw(int b, int p) {
    if (p < 0)return 0;
    if (!p) return 1;
    int ret = pw(b, p >> 1);
    ret = mul(ret, ret);
    if (p & 1)
        ret = mul(ret, b);
    return ret;
}

void build(){
    fac[0] = 1;
    for (int i = 1; i < N; ++i) {
        fac[i] = mul(fac[i - 1],i);
    }
    inv[N - 1] = pw(fac[N - 1],mod - 2);
    for (int i = N - 2; ~i ; --i) {
        inv[i] = mul(inv[i + 1],i + 1);
    }
}

int nCr(int n,int r){
    if(r > n)return 0;
    return mul(mul(fac[n],inv[n - r]),inv[r]);
}