template<typename T>
class FenwickTree {
public:
    vector<T> tree;
    int n;

    // Default constructor
    FenwickTree() : n(0) {}

    // Constructor with size
    FenwickTree(int n) {
        init(n);
    }

    void init(int n) {
        this->n = n;
        // 1-based indexing needs size n + 1. Padding by 2 is safe.
        tree.assign(n + 2, 0); 
    }

    void update(int x, T val) {
        x++; // Convert 0-based to 1-based
        for (; x <= n; x += x & -x) {
            tree[x] += val;
        }
    }

    void assign(int x, T val) {
        update(x, val - getRange(x, x));
    }

    T getPrefix(int x) {
        x++; // Convert 0-based to 1-based
        if (x <= 0) return 0;
        T ret = 0;
        for (; x; x -= x & -x) {
            ret += tree[x];
        }
        return ret;
    }

    T getRange(int l, int r) {
        if (l > r) return 0;
        return getPrefix(r) - getPrefix(l - 1);
    }

    // Returns the 0-based index of the first prefix sum >= x
    int lowerBound(T x) {
        int pos = 0;
        // sz > 0 is enough; don't break early if x == 0 to handle 0-value elements properly
        for (int sz = (1 << __lg(n)); sz > 0; sz >>= 1) {
            if (pos + sz <= n && tree[pos + sz] < x) {
                x -= tree[pos + sz];
                pos += sz;
            }
        }
        return pos; // pos is the 0-based index (since 1-based would be pos + 1)
    }
};