const int MAXN = 200005;                         // array bound (Template.cpp calls this N)
int a[MAXN];
vector<int> pos[MAXN];
int K_LIMIT = 2;

struct Node {
    vector<pair<int, int>> cands;
};

Node tree_seg[4 * MAXN];

Node merge_nodes(const Node& left, const Node& right) {
    vector<pair<int, int>> combined;
    combined.reserve(left.cands.size() + right.cands.size());

    int i = 0, j = 0;
    while (i < left.cands.size() && j < right.cands.size()) {
        if (left.cands[i].first == right.cands[j].first) {
            combined.push_back({left.cands[i].first, left.cands[i].second + right.cands[j].second});
            i++; j++;
        } else if (left.cands[i].first < right.cands[j].first) {
            combined.push_back(left.cands[i++]);
        } else {
            combined.push_back(right.cands[j++]);
        }
    }
    while (i < left.cands.size()) combined.push_back(left.cands[i++]);
    while (j < right.cands.size()) combined.push_back(right.cands[j++]);

    if (combined.size() <= K_LIMIT - 1) {
        return {combined};
    }

    vector<int> counts;
    counts.reserve(combined.size());
    for (const auto& p : combined) {
        counts.push_back(p.second);
    }

    nth_element(counts.begin(), counts.begin() + K_LIMIT - 1, counts.end(), greater<int>());
    int penalty = counts[K_LIMIT - 1];

    vector<pair<int, int>> survived;
    survived.reserve(K_LIMIT - 1);
    for (const auto& p : combined) {
        if (p.second > penalty) {
            survived.push_back({p.first, p.second - penalty});
        }
    }

    return {survived};
}

void build(int node, int start, int end) {
    if (start == end) {
        tree_seg[node].cands.push_back({a[start], 1});
        return;
    }
    int mid = start + (end - start) / 2;
    build(2 * node, start, mid);
    build(2 * node + 1, mid + 1, end);
    tree_seg[node] = merge_nodes(tree_seg[2 * node], tree_seg[2 * node + 1]);
}

Node query_candidates(int node, int start, int end, int l, int r) {
    if (r < start || end < l) return {{}};
    if (l <= start && end <= r) return tree_seg[node];

    int mid = start + (end - start) / 2;
    Node left_res = query_candidates(2 * node, start, mid, l, r);
    Node right_res = query_candidates(2 * node + 1, mid + 1, end, l, r);

    if (left_res.cands.empty()) return right_res;
    if (right_res.cands.empty()) return left_res;

    return merge_nodes(left_res, right_res);
}

// O(log N) fast static frequency count
int get_frequency(int val, int l, int r) {
    auto& v = pos[val];
    auto right_it = upper_bound(v.begin(), v.end(), r);
    auto left_it = lower_bound(v.begin(), v.end(), l);
    return distance(left_it, right_it);
}

void solve() {
    int n, q;
    cin >> n >> q;

    for (int i = 1; i <= n; i++) {
        cin >> a[i];
        pos[a[i]].push_back(i);
    }

    build(1, 1, n);

    while (q--) {
        int l, r;
        cin >> l >> r;

        int len = r - l + 1;
        Node res = query_candidates(1, 1, n, l, r);

        int max_freq = 0;
        for (const auto& p : res.cands) {
            int cand = p.first;
            int freq = get_frequency(cand, l, r);
            max_freq = max(max_freq, freq);
        }

        if (max_freq > (len + 1) / 2) {
            cout << 2 * max_freq - len << "\n";
        } else {
            cout << 1 << "\n";
        }
    }
}
