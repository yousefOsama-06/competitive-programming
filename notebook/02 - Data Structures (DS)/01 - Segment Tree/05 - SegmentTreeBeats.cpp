struct Node {
    int mn;
    int mnCnt;
    int secondMn;

    int mx;
    int mxCnt;
    int secondMx;

    int sum;

    int lazyAdd;
    int lazyEqual;
    bool isLazyEqual;

    Node() {
        mx = -inf;
        secondMx = -inf;
        mxCnt = 0;

        mn = inf;
        secondMn = inf;
        mnCnt = 0;

        sum = 0;

        lazyAdd = 0;
        lazyEqual = 0;
        isLazyEqual = 0;
    };

    Node(int x) {
        mx = x;
        secondMx = -inf;
        mxCnt = 1;

        mn = x;
        secondMn = inf;
        mnCnt = 1;

        sum = x;

        lazyAdd = 0;
        lazyEqual = 0;
        isLazyEqual = 0;
    }
};

struct SegTree {
    int tree_size;
    vector<Node> seg_data;

    SegTree(int n) {
        tree_size = 1;
        while (tree_size < n) tree_size *= 2;
        seg_data.resize(2 * tree_size, Node());
    }

    Node merge(Node& lf, Node& ri) {
        Node ans = Node();

        ans.sum = lf.sum + ri.sum;

        ans.mx = max(lf.mx, ri.mx);
        ans.secondMx = max(lf.secondMx, ri.secondMx);
        ans.mxCnt = 0;
        if (lf.mx == ans.mx) ans.mxCnt += lf.mxCnt;
        else ans.secondMx = max(ans.secondMx, lf.mx);
        if (ri.mx == ans.mx) ans.mxCnt += ri.mxCnt;
        else ans.secondMx = max(ans.secondMx, ri.mx);

        ans.mn = min(lf.mn, ri.mn);
        ans.secondMn = min(lf.secondMn, ri.secondMn);
        ans.mnCnt = 0;
        if (lf.mn == ans.mn) ans.mnCnt += lf.mnCnt;
        else ans.secondMn = min(ans.secondMn, lf.mn);
        if (ri.mn == ans.mn) ans.mnCnt += ri.mnCnt;
        else ans.secondMn = min(ans.secondMn, ri.mn);

        return ans;
    }

    void init(vector<int>& nums, int ni, int lx, int rx) {
        if (rx - lx == 1) {
            if (lx < nums.size())
                seg_data[ni] = Node(nums[lx]);
            return;
        }

        int mid = lx + (rx - lx) / 2;
        init(nums, 2 * ni + 1, lx, mid);
        init(nums, 2 * ni + 2, mid, rx);

        seg_data[ni] = merge(seg_data[2 * ni + 1], seg_data[2 * ni + 2]);
    }

    void init(vector<int>& nums) {
        init(nums, 0, 0, tree_size);
    }

    void doPushEq(int ni, int lx, int rx, int x) {
        seg_data[ni].mn = seg_data[ni].mx = seg_data[ni].lazyEqual = x;
        seg_data[ni].isLazyEqual = 1;
        seg_data[ni].mnCnt = seg_data[ni].mxCnt = rx - lx;
        seg_data[ni].secondMx = -inf;
        seg_data[ni].secondMn = inf;

        seg_data[ni].sum = x * (rx - lx);
        seg_data[ni].lazyAdd = 0;
    }

    void doPushMinEq(int ni, int lx, int rx, int x) {
        if (seg_data[ni].mn >= x) doPushEq(ni, lx, rx, x);
        else if (seg_data[ni].mx > x) {
            if (seg_data[ni].secondMn == seg_data[ni].mx)
                seg_data[ni].secondMn = x;
            seg_data[ni].sum -= (seg_data[ni].mx - x) * seg_data[ni].mxCnt;
            seg_data[ni].mx = x;
        }
    }

    void doPushMaxEq(int ni, int lx, int rx, int x) {
        if (seg_data[ni].mx <= x) doPushEq(ni, lx, rx, x);
        else if (seg_data[ni].mn < x) {
            if (seg_data[ni].secondMx == seg_data[ni].mn)
                seg_data[ni].secondMx = x;

            seg_data[ni].sum += (x - seg_data[ni].mn) * seg_data[ni].mnCnt;
            seg_data[ni].mn = x;
        }
    }

    void doPushSum(int ni, int lx, int rx, int x) {
        if (seg_data[ni].mn == seg_data[ni].mx)
            doPushEq(ni, lx, rx, seg_data[ni].mn + x);
        else {
            seg_data[ni].mx += x;
            if (seg_data[ni].secondMx != -inf)
                seg_data[ni].secondMx += x;

            seg_data[ni].mn += x;
            if (seg_data[ni].secondMn != inf)
                seg_data[ni].secondMn += x;

            seg_data[ni].sum += (rx - lx) * x;
            seg_data[ni].lazyAdd += x;
        }
    }

    void propagete(int ni, int lx, int rx) {
        if (rx - lx == 1) return;

        int mid = lx + (rx - lx) / 2;
        if (seg_data[ni].isLazyEqual) {
            doPushEq(2 * ni + 1, lx, mid, seg_data[ni].lazyEqual);
            doPushEq(2 * ni + 2, mid, rx, seg_data[ni].lazyEqual);
            seg_data[ni].lazyEqual = seg_data[ni].isLazyEqual = 0;
        }
        else {
            doPushSum(2 * ni + 1, lx, mid, seg_data[ni].lazyAdd);
            doPushSum(2 * ni + 2, mid, rx, seg_data[ni].lazyAdd);
            seg_data[ni].lazyAdd = 0;

            doPushMinEq(2 * ni + 1, lx, mid, seg_data[ni].mx);
            doPushMinEq(2 * ni + 2, mid, rx, seg_data[ni].mx);

            doPushMaxEq(2 * ni + 1, lx, mid, seg_data[ni].mn);
            doPushMaxEq(2 * ni + 2, mid, rx, seg_data[ni].mn);
        }
    }

    void minimize(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || lx >= r || seg_data[ni].mx <= x)
            return;
        if (lx >= l && rx <= r && seg_data[ni].secondMx < x) {
            doPushMinEq(ni, lx, rx, x);
            return;
        }

        propagete(ni, lx, rx);

        int mid = lx + (rx - lx) / 2;
        minimize(l, r, x, 2 * ni + 1, lx, mid);
        minimize(l, r, x, 2 * ni + 2, mid, rx);

        seg_data[ni] = merge(seg_data[2 * ni + 1], seg_data[2 * ni + 2]);
    }

    void minimize(int l, int r, int x) {
        minimize(l, r, x, 0, 0, tree_size);
    }

    void maximize(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || lx >= r || seg_data[ni].mn >= x)
            return;
        if (lx >= l && rx <= r && seg_data[ni].secondMn > x) {
            doPushMaxEq(ni, lx, rx, x);
            return;
        }

        propagete(ni, lx, rx);

        int mid = lx + (rx - lx) / 2;
        maximize(l, r, x, 2 * ni + 1, lx, mid);
        maximize(l, r, x, 2 * ni + 2, mid, rx);

        seg_data[ni] = merge(seg_data[2 * ni + 1], seg_data[2 * ni + 2]);
    }

    void maximize(int l, int r, int x) {
        maximize(l, r, x, 0, 0, tree_size);
    }

    void add(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || lx >= r)
            return;
        if (lx >= l && rx <= r) {
            doPushSum(ni, lx, rx, x);
            return;
        }

        propagete(ni, lx, rx);

        int mid = lx + (rx - lx) / 2;
        add(l, r, x, 2 * ni + 1, lx, mid);
        add(l, r, x, 2 * ni + 2, mid, rx);

        seg_data[ni] = merge(seg_data[2 * ni + 1], seg_data[2 * ni + 2]);
    }

    void add(int l, int r, int x) {
        add(l, r, x, 0, 0, tree_size);
    }

    void set(int l, int r, int x, int ni, int lx, int rx) {
        if (rx <= l || lx >= r)
            return;
        if (lx >= l && rx <= r) {
            doPushEq(ni, lx, rx, x);
            return;
        }

        propagete(ni, lx, rx);

        int mid = lx + (rx - lx) / 2;
        set(l, r, x, 2 * ni + 1, lx, mid);
        set(l, r, x, 2 * ni + 2, mid, rx);

        seg_data[ni] = merge(seg_data[2 * ni + 1], seg_data[2 * ni + 2]);
    }

    void set(int l, int r, int x) {
        set(l, r, x, 0, 0, tree_size);
    }

    Node get_range(int l, int r, int ni, int lx, int rx) {
        propagete(ni, lx, rx);

        if (lx >= l && rx <= r)
            return seg_data[ni];
        if (rx <= l || lx >= r)
            return Node();

        int mid = (lx + rx) / 2;

        Node lf = get_range(l, r, 2 * ni + 1, lx, mid);
        Node ri = get_range(l, r, 2 * ni + 2, mid, rx);
        return merge(lf, ri);
    }

    int get_sum(int l, int r) {
        return get_range(l, r, 0, 0, tree_size).sum;
    }

    int get_mx(int l, int r) {
        return get_range(l, r, 0, 0, tree_size).mx;
    }

    int get_mn(int l, int r) {
        return get_range(l, r, 0, 0, tree_size).mn;
    }
};