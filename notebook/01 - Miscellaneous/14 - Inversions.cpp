// INVERSIONS: #pairs i < j with a[i] > a[j]. O(n log n) by merge sort (also sorts a).
ll inversions(vector<int>& a) {
    int n = a.size();
    if (n < 2) return 0;
    vector<int> buf(n);
    function<ll(int, int)> rec = [&](int l, int r) -> ll {          // [l, r)
        if (r - l < 2) return 0;
        int m = (l + r) / 2;
        ll res = rec(l, m) + rec(m, r);
        int i = l, j = m, k = l;
        while (i < m && j < r) {
            if (a[i] <= a[j]) buf[k++] = a[i++];
            else buf[k++] = a[j++], res += m - i;                   // a[i..m) all beat a[j]
        }
        while (i < m) buf[k++] = a[i++];
        while (j < r) buf[k++] = a[j++];
        copy(buf.begin() + l, buf.begin() + r, a.begin() + l);
        return res;
    };
    return rec(0, n);
}
// WHY IT KEEPS APPEARING
//  - minimum number of ADJACENT SWAPS to sort = inversion count
//  - minimum adjacent swaps to turn a into b: relabel b's positions, then count inversions
//  - "how far is this permutation from sorted" / bubble-sort distance
//  - inversions modulo 2 = permutation PARITY (= sign, used in determinants and 15-puzzle)
// BIT VERSION (needed when you must also support updates, or count online):
//   compress values, sweep left to right, ans += (#inserted so far) - query(a[i]), then add a[i].
