// 2D BIT WITH RANGE UPDATE AND RANGE QUERY - add v to a RECTANGLE, ask the sum of a RECTANGLE, in
// O(log n log m) with four BITs and no lazy propagation anywhere.
// WHEN: grid problems with rectangle add / rectangle sum. 02 - FenwickTree2D only does point
// update; 04 - BITRangeUpdateRangeQuery is the 1D version of exactly this trick; a 2D segment tree
// with lazy propagation does NOT exist in any usable form (you cannot push a tag through two
// dimensions), which is why this identity is the standard answer.
// KEY INVARIANT: work on the 2D difference array d, where a[i][j] = sum of d over the rectangle
// [0..i] x [0..j]. A rectangle add touches exactly FOUR cells of d. Expanding the double prefix
// sum gives
//   pre(x, y) = (x+1)(y+1) * S(d) - (y+1) * S(d*i) - (x+1) * S(d*j) + S(d*i*j)
// over i <= x, j <= y - four independent BIT-able quantities, hence four BITs.
template <typename T = ll>
struct BIT2DRange {
    int n, m;
    vector<vector<T>> b[4];
    BIT2DRange(int n, int m) : n(n), m(m) {
        for (int k = 0; k < 4; k++) b[k].assign(n + 2, vector<T>(m + 2, 0));
    }
    void add(int x, int y, T v) {                              // d[x][y] += v (internal)
        if (x > n || y > m) return;
        T c[4] = {v, v * x, v * y, v * x * y};
        for (int i = x + 1; i <= n + 1; i += i & -i)
            for (int j = y + 1; j <= m + 1; j += j & -j)
                for (int k = 0; k < 4; k++) b[k][i][j] += c[k];
    }
    void upd(int x1, int y1, int x2, int y2, T v) {            // add v to the rectangle
        add(x1, y1, v), add(x1, y2 + 1, -v), add(x2 + 1, y1, -v), add(x2 + 1, y2 + 1, v);
    }
    T pre(int x, int y) {                                      // sum over [0..x] x [0..y]
        if (x < 0 || y < 0) return 0;
        T s[4] = {0, 0, 0, 0};
        for (int i = x + 1; i > 0; i -= i & -i)
            for (int j = y + 1; j > 0; j -= j & -j)
                for (int k = 0; k < 4; k++) s[k] += b[k][i][j];
        return (T)(x + 1) * (y + 1) * s[0] - (T)(y + 1) * s[1] - (T)(x + 1) * s[2] + s[3];
    }
    T qry(int x1, int y1, int x2, int y2) {
        return pre(x2, y2) - pre(x1 - 1, y2) - pre(x2, y1 - 1) + pre(x1 - 1, y1 - 1);
    }
};
// MEMORY is 4 * n * m of your value type - 12.8 GB at 2000x2000 with ll. If that hurts, use int
// where safe, or compress one axis and go offline (03 - Offline2DFenwickTree).
// THE SAME EXPANSION generalises: range-add of a polynomial of degree k in each axis needs
// (k+1)^2 BITs. In 1D the arithmetic-progression case is 01 - Segment Tree/13.
// XOR instead of sum: the (x+1)(y+1) coefficients become parities, so keep four BITs and multiply
// by the parity of (x+1)*(y+1) etc. Usually simpler to just use a 2D segment tree of counts.
// SPARSE GRIDS: replace each inner vector by a hash map, or compress coordinates first - the outer
// structure is unchanged.
