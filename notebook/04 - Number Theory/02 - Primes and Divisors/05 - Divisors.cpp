template <typename T>
vector<T> getDivisors(T n) {
    //complexity: sqrt(n)
    vector<T> vec;
    for (ll i = 1; i * i <= n; i++) {
        if (n % i == 0) {
            vec.push_back(i);
            if (n / i != i)
                vec.push_back(n / i);
        }
    }
    return vec;
}