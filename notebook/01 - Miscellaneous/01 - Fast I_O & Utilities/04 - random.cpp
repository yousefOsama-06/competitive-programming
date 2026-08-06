mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
template <typename T>
T random(T l, T r) {
    return uniform_int_distribution(l, r)(rng);
}

vector<int> random(int n) {
    vector<int> v(n);
    // shuffle 1
    shuffle(v.begin(), v.end(), rng);
    // shuffle 2
    for (int i = 1; i < n; i++)
        swap(v[i], v[uniform_int_distribution<int>(0, i)(rng)]);
    return v;
}