bool isPrime[N];

void seive(int n = N) {
//complexity: n*log(log(n))
    memset(isPrime, true, sizeof isPrime);
    isPrime[0] = false;
    isPrime[1] = false;
    for (int i = 2; i * i < n; i++) {
        if (isPrime[i]) {
            for (int j = i * i; j < n; j += i) {
                isPrime[j] = false;
            }
        }
    }
}