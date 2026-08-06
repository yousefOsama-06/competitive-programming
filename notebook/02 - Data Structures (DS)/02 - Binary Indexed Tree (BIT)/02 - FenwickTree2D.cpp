template<typename T>
class FenwickTree2D {
public:
    vector<vector<T>> tree;
    int n, m;

    // Default constructor
    FenwickTree2D() : n(0), m(0) {}

    // Constructor with dimensions
    FenwickTree2D(int n, int m) {
        init(n, m);
    }

    void init(int n, int m) {
        this->n = n;
        this->m = m;
        // 1-based indexing needs sizes n + 1 and m + 1. Padding by 2 is safe.
        tree.assign(n + 2, vector<T>(m + 2, 0)); 
    }

    // Point update: Adds 'val' to the cell (x, y)
    void update(int x, int y, T val) {
        x++; y++; // Convert 0-based to 1-based
        for (int i = x; i <= n; i += i & -i) {
            for (int j = y; j <= m; j += j & -j) {
                tree[i][j] += val;
            }
        }
    }

    // Point assignment: Sets the cell (x, y) to 'val'
    void assign(int x, int y, T val) {
        update(x, y, val - getRange(x, y, x, y));
    }

    // 2D Prefix sum: Returns the sum of the subgrid from (0, 0) to (x, y)
    T getPrefix(int x, int y) {
        x++; y++; // Convert 0-based to 1-based
        if (x <= 0 || y <= 0) return 0;
        
        // Clamp to maximum dimensions if queries exceed bounds
        x = min(x, n);
        y = min(y, m);
        
        T ret = 0;
        for (int i = x; i > 0; i -= i & -i) {
            for (int j = y; j > 0; j -= j & -j) {
                ret += tree[i][j];
            }
        }
        return ret;
    }

    // 2D Range sum: Returns the sum of the subgrid from top-left (x1, y1) to bottom-right (x2, y2)
    T getRange(int x1, int y1, int x2, int y2) {
        if (x1 > x2 || y1 > y2) return 0;
        
        // Inclusion-Exclusion Principle
        return getPrefix(x2, y2) 
             - getPrefix(x1 - 1, y2) 
             - getPrefix(x2, y1 - 1) 
             + getPrefix(x1 - 1, y1 - 1);
    }
};
