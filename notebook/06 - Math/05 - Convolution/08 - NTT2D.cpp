// Needs: ntt, npow (02 - NTT.cpp).
// 2-D CONVOLUTION of two bivariate polynomials mod NMOD = 998244353:
//   c[i][j] = sum_{p+r=i, q+s=j} a[p][q] * b[r][s],   in O(R*C*log(R*C)) with R, C the padded
// dimensions. The transform SEPARATES: NTT every row, then every column, multiply pointwise,
// invert columns then rows. Same idea works for any number of dimensions.
// REACH FOR IT when the state has TWO additive coordinates: "count paths with a given (x, y)
// displacement", "polynomials in two variables" (a variable per statistic you must track jointly),
// 2-D pattern matching by sums of products, and grid DPs whose transition is a translation.
typedef vector<vector<ll>> Poly2;
void ntt2d(Poly2& a, bool inv) {
    int R = a.size(), C = a[0].size();
    for (int i = 0; i < R; i++) ntt(a[i], inv);
    vector<ll> col(R);
    for (int j = 0; j < C; j++) {
        for (int i = 0; i < R; i++) col[i] = a[i][j];
        ntt(col, inv);
        for (int i = 0; i < R; i++) a[i][j] = col[i];
    }
}
Poly2 multiply2D(const Poly2& a, const Poly2& b) {
    int rr = a.size() + b.size() - 1, cc = a[0].size() + b[0].size() - 1, R = 1, C = 1;
    while (R < rr) R <<= 1;
    while (C < cc) C <<= 1;
    Poly2 x(R, vector<ll>(C, 0)), y(R, vector<ll>(C, 0));
    for (size_t i = 0; i < a.size(); i++)
        for (size_t j = 0; j < a[0].size(); j++) x[i][j] = a[i][j];
    for (size_t i = 0; i < b.size(); i++)
        for (size_t j = 0; j < b[0].size(); j++) y[i][j] = b[i][j];
    ntt2d(x, false), ntt2d(y, false);
    for (int i = 0; i < R; i++)
        for (int j = 0; j < C; j++) x[i][j] = x[i][j] * y[i][j] % NMOD;
    ntt2d(x, true);
    x.resize(rr);
    for (auto& row : x) row.resize(cc);
    return x;
}
// CHEAPER ALTERNATIVE - FLATTENING. If one dimension is small (C0 columns in a and C1 in b), pack
// the 2-D array into a 1-D one with stride W >= C0 + C1 - 1 (index i*W + j) and do a SINGLE 1-D
// convolution: the strides are wide enough that the coordinates never interfere. One NTT of length
// R*W instead of R + C transforms - usually faster, and it is three lines. Use the 2-D routine
// when both dimensions are large and comparable.
// The same separability argument gives 2-D FWHT (xor in both coordinates) and 2-D subset-sum
// transforms; nothing about it is special to the NTT.
// MEMORY: R*C longs. 4096 x 4096 is already 128 MB - flatten instead when the product is large.
