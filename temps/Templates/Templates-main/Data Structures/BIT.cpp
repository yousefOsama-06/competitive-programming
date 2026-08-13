template<typename T>
class FenwickTree {
public:
    vector<T> tree;
    int n;

    void init(int n) {
        tree.assign(n + 2, 0);
        this->n = n;
    }

    T merge(T &x, T &y) { return x + y; }

    void update(int x, T val) {
        for (; x <= n; x += x & -x) {
            tree[x] = merge(tree[x], val);
        }
    }

    T getPrefix(int x) {
        if (x <= 0)return 0;
        T ret = 0;
        for (; x; x -= x & -x) {
            ret = merge(ret, tree[x]);
        }
        return ret;
    }

    T getRange(int l, int r) {
        return getPrefix(r) - getPrefix(l - 1);
    }

    int lowerBound(ll x) {
        int pos = 0;
        for (int sz = (1 << __lg(n)); sz > 0 && x; sz >>= 1) {
            if (pos + sz <= n && tree[pos + sz] < x) {
                x -= tree[pos + sz];
                pos += sz;
            }
        }
        return pos + 1;
    }
};
