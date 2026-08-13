struct Line {
    mutable int64_t m, c, p;
    bool operator<(const Line& o) const { return m < o.m; }
    bool operator<(int64_t x) const { return p < x; }
};
struct HullDynamic : multiset<Line, less<>> {
    const int64_t inf = 2e18;
    int64_t div(int64_t a, int64_t b) {
        return a / b - ((a ^ b) < 0 && a % b); }
    bool isect(iterator x, iterator y) {
        if (y == end()) { x->p = inf; return false; }
        if (x->m == y->m) x->p = x->c > y->c ? inf : -inf;
        else x->p = div(y->c - x->c, x->m - y->m);
        return x->p >= y->p;
    }
    void add(int64_t k, int64_t m) {
        k *= -1, m *= -1;
        auto z = insert({k, m, 0}), y = z++, x = y;
        while(isect(y, z)) z = erase(z);
        if(x != begin() && isect(--x, y)) isect(x, y = erase(y));
        while((y = x) != begin() && (--x)->p >= y->p) isect(x, erase(y));
    }
    int64_t query(int64_t x) {
        if(empty()) return 1e18;
        auto l = *lower_bound(x);
        return -(l.m * x + l.c);
    }
};