#define sz(aa) (int)aa.size()
template<class T>
struct sparseTable {
    vector<vector<T>> jmp;
    void build(const vector<T>& V){
        jmp.resize(1,V);
        for (int pw = 1, k = 1; pw * 2 <= sz(V); pw *= 2, ++k) {
            jmp.emplace_back(sz(V) - pw * 2 + 1);
            for (int j = 0; j < sz(jmp[k]); ++j) {
                jmp[k][j] = max(jmp[k - 1][j], jmp[k - 1][j + pw]);
            }
        }
    }
    T query(int l, int r) {
        assert(l <= r);
        int dep = 31 - __builtin_clz(r - l + 1);
        return max(jmp[dep][l], jmp[dep][r - (1 << dep) + 1]);
    }
};
