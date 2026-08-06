struct Compress {
    map<int, int> m;
    vector<int> undo;

    Compress(vector<int>& v, int base = 0) {
        m.clear();
        set<int> s;
        for (int x : v) {
            s.insert(x);
        }
        undo.resize(s.size() + base);
        int i = base;
        for (int x : s) {
            undo[i] = x;
            m[x] = i++;
        }
        for (int& x : v)
            x = m[x];
    }
};