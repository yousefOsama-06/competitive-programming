struct Mint {
    int v;

    Mint(long long val = 0) {
        v = int(val % MOD);
        if (v < 0) v += MOD;
    }

    Mint operator+(const Mint& o) const { return Mint(v + o.v); }
    Mint operator-(const Mint& o) const { return Mint(v - o.v); }
    Mint operator*(const Mint& o) const { return Mint(1LL * v * o.v); }
    Mint operator/(const Mint& o) const { return *this * o.inv(); }

    Mint& operator+=(const Mint& o) {
        v += o.v;
        if (v >= MOD) v -= MOD;
        return *this;
    }

    Mint& operator-=(const Mint& o) {
        v -= o.v;
        if (v < 0) v += MOD;
        return *this;
    }

    Mint& operator*=(const Mint& o) {
        v = int(1LL * v * o.v % MOD);
        return *this;
    }

    Mint& operator/=(const Mint& o) {
        v = int(1LL * v * o.inv().v % MOD);
        return *this;
    }

    Mint pow(long long p) const {
        Mint a = *this, res = 1;
        while (p > 0) {
            if (p & 1) res *= a;
            a *= a;
            p >>= 1;
        }
        return res;
    }

    Mint inv() const { return pow(MOD - 2); }

    friend ostream& operator<<(ostream& os, const Mint& m) {
        os << m.v;
        return os;
    }
};

Mint fact[N], inv[N], invFact[N];

Mint nCr(int n, int r) {
    if (r < 0 || r > n)
        return 0;
    // return fact[n] / (fact[r] * fact[n - r]);
    return fact[n] * invFact[r] * invFact[n - r];
}

Mint nPr(int n, int r) {
    if (r < 0 || r > n)
        return 0;
    // return fact[n] / fact[n - r];
    return fact[n] * invFact[n - r];
}

void build() {
    fact[0] = invFact[0] = 1;
    for (int i = 1; i < N; i++) {
        fact[i] = fact[i - 1] * i;
        invFact[i] = invFact[i - 1] / i;
        inv[i] = Mint(i).inv();
    }
}