// 2D Monotonic Queue (Static & Dynamic Subgrid Min/Max)
// 1. Sliding Window 2D Min/Max: O(N * M) total for fixed K x L subgrid
// 2. DynamicMonotonicQueue2D: Queue of 1D TwoStackQ's supporting dynamic row push/pop with O(1) 2D min/max query

// Dynamic 2D Queue using TwoStackQ composition for dynamic height / width subgrids
struct DynamicQueue2D {
    // 1D Queue supporting dynamic push/pop/min/max
    struct Queue1D {
        struct Node {
            ll mn = llinf, mx = -llinf, val;
            Node() : val(0) {}
            Node(ll x) : mn(x), mx(x), val(x) {}
        };
        stack<Node> a, b;
        void mrg(Node &x, Node &y) {
            x.mn = min(x.mn, y.mn);
            x.mx = max(x.mx, y.mx);
        }
        void push(ll val) {
            Node nd(val);
            if (!a.empty()) mrg(nd, a.top());
            a.push(nd);
        }
        void move() {
            while (!a.empty()) {
                Node nd(a.top().val);
                if (!b.empty()) mrg(nd, b.top());
                b.push(nd); a.pop();
            }
        }
        void pop() {
            if (b.empty()) move();
            if (!b.empty()) b.pop();
        }
        pair<ll, ll> get_min_max() {
            Node res;
            if (!b.empty()) mrg(res, b.top());
            if (!a.empty()) mrg(res, a.top());
            return {res.mn, res.mx};
        }
    };

    // Composition of 1D queues into a 2D queue
    struct Node2D {
        Queue1D q;
        ll mn = llinf, mx = -llinf;
    };
    stack<Node2D> a, b;

    void push_row(const vector<ll>& row) {
        Node2D nd;
        for (ll val : row) nd.q.push(val);
        auto [r_mn, r_mx] = nd.q.get_min_max();
        nd.mn = r_mn; nd.mx = r_mx;
        if (!a.empty()) {
            nd.mn = min(nd.mn, a.top().mn);
            nd.mx = max(nd.mx, a.top().mx);
        }
        a.push(nd);
    }

    void move() {
        while (!a.empty()) {
            Node2D nd = a.top();
            a.pop();
            auto [r_mn, r_mx] = nd.q.get_min_max();
            nd.mn = r_mn; nd.mx = r_mx;
            if (!b.empty()) {
                nd.mn = min(nd.mn, b.top().mn);
                nd.mx = max(nd.mx, b.top().mx);
            }
            b.push(nd);
        }
    }

    void pop_row() {
        if (b.empty()) move();
        if (!b.empty()) b.pop();
    }

    // Returns {2D_min, 2D_max} of the current dynamic subgrid in O(1)
    pair<ll, ll> get_min_max() {
        ll mn = llinf, mx = -llinf;
        if (!b.empty()) { mn = min(mn, b.top().mn); mx = max(mx, b.top().mx); }
        if (!a.empty()) { mn = min(mn, a.top().mn); mx = max(mx, a.top().mx); }
        return {mn, mx};
    }
};

// Static 2D Sliding Window Min for fixed K x L subgrid - O(N * M)
template<typename T = ll>
struct MonotonicQueue2D {
    int n, m;

    vector<vector<T>> sliding_window_2d(const vector<vector<T>>& grid, int K, int L) {
        n = grid.size(); m = grid[0].size();
        vector<vector<T>> row_min(n, vector<T>(m - L + 1));
        for (int i = 0; i < n; i++) {
            deque<int> dq;
            for (int j = 0; j < m; j++) {
                while (!dq.empty() && grid[i][dq.back()] >= grid[i][j]) dq.pop_back();
                dq.pb(j);
                if (dq.front() <= j - L) dq.pop_front();
                if (j >= L - 1) row_min[i][j - L + 1] = grid[i][dq.front()];
            }
        }
        vector<vector<T>> res(n - K + 1, vector<T>(m - L + 1));
        for (int j = 0; j < m - L + 1; j++) {
            deque<int> dq;
            for (int i = 0; i < n; i++) {
                while (!dq.empty() && row_min[dq.back()][j] >= row_min[i][j]) dq.pop_back();
                dq.pb(i);
                if (dq.front() <= i - K) dq.pop_front();
                if (i >= K - 1) res[i - K + 1][j] = row_min[dq.front()][j];
            }
        }
        return res;
    }
};
