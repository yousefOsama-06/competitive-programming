vector<int> spf;

void seive(int n = N) {
    //complexity: n*log(log(n))
    spf.resize(n);
    iota(all(spf), 0);
    for (int i = 2; i * i < n; i++) {
        if (spf[i] == i) {
            for (int j = i * i; j < n; j += i) {
                spf[j] = min(spf[j], i);
            }
        }
    }
}

vector<int> getPrimeFactors(int n) {
    //complexity: log(n)
    vector<int> ans;
    while (n != 1) {
        ans.pb(spf[n]);
        n /= spf[n];
    }
    return ans;
}