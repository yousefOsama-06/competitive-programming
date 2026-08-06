int fact[N], inv[N], invFact[N]; ///used in nCr(), nPr(), calcFact()

int modSum(ll a, ll b) {
    if (a < 0)
        a += MOD;
    if (b < 0)
        b += MOD;
    a += b;
    if (a >= MOD)
        a -= MOD;
    return a;

    a = (a % MOD + MOD) % MOD;
    b = (b % MOD + MOD) % MOD;
    return (a + b) % MOD;
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


    a = (a % MOD + MOD) % MOD;
    b = (b % MOD + MOD) % MOD;
    return (a * b) % MOD;
}

int modPow(int a, ll b) {
    int result = 1;
    while (b) {
        if (b & 1)
            result = modProd(result, a);
        a = modProd(a, a);
        b >>= 1;
    }
    return result;
}


int modInverse(int a) {
    return modPow(a, MOD - 2);
}


int modDiv(int a, int b) {
    return modProd(a, modInverse(b));
}

int nCr(int n, int r) {
    if (r < 0 || r > n)
        return 0;
    //    return modDiv(fact[n], modProd(fact[r], fact[n - r]));
    return modProd(fact[n], modProd(invFact[r], invFact[n - r]));
}

int nPr(int n, int r) {
    if (r < 0 || r > n)
        return 0;
    //    return modDiv(fact[n], fact[n - r]);
    return modProd(fact[n], invFact[n - r]);
}

void build() {
    fact[0] = invFact[0] = 1;
    for (int i = 1; i < N; i++) {
        fact[i] = modProd(fact[i - 1], i);
        invFact[i] = modDiv(invFact[i - 1], i);
        inv[i] = modInverse(i);
    }
}