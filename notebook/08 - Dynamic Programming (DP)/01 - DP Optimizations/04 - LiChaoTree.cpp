// To get an instance : node *root = EMPTY;
typedef pair<long long, long long> line;
int start_x, end_x;
line neutral{0,-1e18};

ll sub(const line &l, ll x) {
    return l.first * x + l.second;
}

double inter(const line &l1, const line &l2) {
    return (l2.second - l1.second) / (l1.first - l2.first - .0);
}

extern struct node *const EMPTY;
struct node {
    line l;
    node *lf, *rt;
    node() : l(neutral), lf(this), rt(this) {}
    node(line l) : l(l), lf(EMPTY), rt(EMPTY) {}
};
node *const EMPTY = new node;

void insert(line l, int lx, int rx, node *&cur, ll ns = start_x, ll ne = end_x) {
    if(rx < ns || lx > ne) return;
    if(ns >= lx && ne <= rx) {
        if (cur == EMPTY) {
            cur = new node(l);
            return;
        }
        if (l.first == cur->l.first) {
            cur->l = max(cur->l, l);
            return;
        }

        auto x = inter(l, cur->l);
        if (x < ns || x > ne) {
            if (sub(l, ns) > sub(cur->l, ns)) cur->l = l;
            return;
        }
        ll mid = ns + (ne - ns) / 2;
        if (x < mid) {
            if (sub(l, ne) > sub(cur->l, ne)) swap(l, cur->l);
            insert(l, lx, rx, cur->lf, ns, mid);
        } else {
            if (sub(l, ns) > sub(cur->l, ns)) swap(l, cur->l);
            insert(l,lx, rx, cur->rt, mid + 1, ne);
        }
        return;
    }

    if (cur == EMPTY) {
        cur = new node(neutral);
    }
    ll mid = ns + (ne - ns) / 2;
    insert(l, lx, rx, cur->lf, ns, mid);
    insert(l, lx, rx, cur->rt, mid + 1, ne);
}

ll query(ll x, node *cur, ll ns = start_x, ll ne = end_x) {
    auto ret = sub(cur->l, x);
    if (x < ns || x > ne)
        return sub(neutral, x);
    if (ns == ne)
        return ret;
    ll mid = ns + (ne - ns) / 2;
    if (x <= mid)
        return max(ret, query(x, cur->lf, ns, mid));
    return max(ret, query(x, cur->rt, mid + 1, ne));
}