template <typename T>
vector<T> getPrimeFactors(T n) {
    //complexity: sqrt(n)
    vector<T> vec;
    for (ll i = 2; i * i <= n; i++) {
        while (n % i == 0) {
            vec.push_back(i);
            n /= i;
        }
    }
    if (n != 1) vec.push_back(n);
    return vec;
}