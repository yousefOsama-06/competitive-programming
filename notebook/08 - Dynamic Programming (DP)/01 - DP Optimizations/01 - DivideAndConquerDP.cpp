// Divide & Conquer DP Optimization - Time: O(K * N log N), Space: O(N)
// Applies when DP transition is dp[k][i] = min_{j < i} (dp[k-1][j] + cost(j+1, i))
// Condition: opt(i, j) <= opt(i, j + 1) (Monotonicity of optimal split points)
int a[N];
ll curCost;
int curL = 0, curR = -1, freq[N];
pair<int, ll> pre[N], cur[N];
// Modify at will :
int lo(int ind) { return 1; }
int hi(int ind) { return ind; }

void add(int val) {
    curCost += freq[val]++;
}

void remove(int val) {
    curCost -= --freq[val];
}

ll Cost(int l, int r) {
    while (curR < r)
        add(a[++curR]);
    while (curL > l)
        add(a[--curL]);
    while (curR > r)
        remove(a[curR--]);
    while (curL < l)
        remove(a[curL++]);
    return curCost;
}

ll f(int ind, int k) { return pre[k - 1].nd + Cost(k, ind); }
void store(int ind, int k, ll v) { cur[ind] = pair(k, v); }

void rec(int L, int R, int LO, int HI) {
    if (L > R) return;
    int mid = (L + R) >> 1;
    pair best(llinf, LO);
    for (int k = max(LO, lo(mid)); k <= min(HI, hi(mid)); ++k)
        best = min(best, pair(f(mid, k), k));
    store(mid, best.second, best.first);
    rec(L, mid - 1, LO, best.second);
    rec(mid + 1, R, best.second, HI);
}

void solve(int L, int R) { rec(L, R, -inf, inf); }