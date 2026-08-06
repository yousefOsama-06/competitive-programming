int modSum(ll a, ll b) {
    ///complexity: 1
    if (a < 0)
        a += MOD;
    if (b < 0)
        b += MOD;
    a += b;
    if (a >= MOD)
        a %= MOD;
    return a;
}

int modProd(ll a, ll b) {
    ///complexity: 1
    if (a < 0)
        a += MOD;
    if (b < 0)
        b += MOD;
    a *= b;
    if (a >= MOD)
        a %= MOD;
    return a;
}

int fastPow(int a, ll b) {
    ///complexity: log(b)
    if (b == 0) return 1;
    int temp = fastPow(a, b / 2);
    if (b % 2 == 0)
        return modProd(temp, temp);
    return modProd(modProd(a, temp), temp);
}


int modInverse(int a) {
    ///complexity: log(mod)
    return fastPow(a, MOD - 2);
}


struct Hash {
    vector<pair<int, int>> hash_value = {{0, 0}};
    const pair<int, int> p = {67, 97};
    vector<pair<int, int>> p_pow = {{1, 1}};
    vector<pair<int, int>> inv = {{1, 1}};

    void compute(string const &s) {
        for (char c: s) {
            if (c >= 'a' && c <= 'z') {
                c = c - 'a' + 1;
            } else if (c >= 'A' && c <= 'Z') {
                c = c - 'A' + 30;
            } else {
                c = c - '0' + 60;
            }
            int x = modSum(hash_value.back().st, modProd(c, p_pow.back().st));
            int a = modProd(p_pow.back().st, p.st);

            int y = modSum(hash_value.back().nd, modProd(c, p_pow.back().nd));
            int b = modProd(p_pow.back().nd, p.nd);

            hash_value.emplace_back(x, y);
            p_pow.emplace_back(a, b);
            if (inv.size() == 1) {
                inv.emplace_back(modInverse(a), modInverse(b));
            } else {
                inv.emplace_back(modProd(inv[1].st, inv.back().st), modProd(inv[1].nd, inv.back().nd));
            }
        }
    }

    pair<int, int> get(int l, int r) {
        l++, r++;
        pair<int, int> ans;
        ans.st = modSum(hash_value[r].st, -hash_value[l - 1].st);
        ans.st = modProd(ans.st, inv[l].st);


        ans.nd = modSum(hash_value[r].nd, -hash_value[l - 1].nd);
        ans.nd = modProd(ans.nd, inv[l].nd);

        return ans;
    }
};