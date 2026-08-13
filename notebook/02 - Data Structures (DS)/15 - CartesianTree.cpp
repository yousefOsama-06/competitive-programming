// CARTESIAN TREE in O(n): heap-ordered by VALUE, BST-ordered by INDEX.
// The root of any range is its minimum, so "range minimum" structure becomes a TREE -
// which turns range problems into subtree problems (and enables divide & conquer on the min).
// Built with a monotonic stack; ties go left so the tree is unique.
struct CartTree {
    int n, root = -1;
    vector<int> l, r, par;

    CartTree(const vector<ll>& a) : n(a.size()), l(n, -1), r(n, -1), par(n, -1) {
        vector<int> st;
        for (int i = 0; i < n; i++) {
            int last = -1;
            while (!st.empty() && a[st.back()] > a[i]) last = st.back(), st.pop_back();
            l[i] = last;
            if (last != -1) par[last] = i;
            if (!st.empty()) r[st.back()] = i, par[i] = st.back();
            st.push_back(i);
        }
        root = st[0];
    }
};
// USES
//  - "for every element, the maximal range where it is the minimum" = its subtree range,
//    which is the contribution technique (sum over subarrays of the min) in tree form
//  - largest rectangle in a histogram = best over nodes of value * subtreeSize
//  - divide & conquer "solve(l, r) splits at the minimum" without recomputing RMQ
//  - counting subarrays whose minimum is a given element
//  - a Treap built on (index, random priority) is exactly a Cartesian tree - same code shape
