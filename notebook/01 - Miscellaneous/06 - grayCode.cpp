vector<int> grayCode(int n) {
    vector<int> vec = {0};
    vector<bool> used(1 << n, false);
    used[0] = true;
    while (vec.size() < (1 << n)) {
        int bit = 0;
        while (used[vec.back() ^ (1 << bit)])
            bit++;
        vec.push_back(vec.back() ^ (1 << bit));
        used[vec.back() ^ (1 << bit)] = true;
    }
    return vec;
}