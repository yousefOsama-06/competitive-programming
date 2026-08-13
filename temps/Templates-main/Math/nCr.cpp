ll fac[N],inv[N];

ll mul(const ll &a,const ll&b){
    return (a % mod + mod) * (b % mod + mod) % mod;
}

ll add(const ll &a,const ll&b){
    return (a + b + 2 * mod)%mod;
}

ll pw(ll b, ll p) {
    if(p < 0)return 0;
    if (!p) return 1LL;
    ll ret = pw(b, p >> 1LL);
    ret = mul(ret,ret);
    if (p & 1LL)
        ret = mul(ret,b);
    return ret;
}


void build(){
    fac[0] = 1;
    for (int i = 1; i < N; ++i) {
        fac[i] = mul(fac[i - 1],i);
    }
    inv[N - 1] = pw(fac[N - 1],MOD - 2);
    for (int i = N - 2; ~i ; --i) {
        inv[i] = mul(inv[i + 1],i + 1);
    }
}

ll nCr(int n,int r){
    if(r > n)return 0;
    return mul(mul(fac[n],inv[n - r]),inv[r]);
}