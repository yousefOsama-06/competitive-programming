// COPRIME (PRIME) BASIS - factor a set of numbers RELATIVE TO EACH OTHER, without ever factoring
// them. Given n values up to 1e18 (far past Pollard-rho comfort when n is large), it builds a set
// of PAIRWISE COPRIME basis elements such that every input is a product of basis powers. Then
// each input becomes an exponent vector, and gcd / lcm / divisibility / "product is a square" all
// become coordinatewise min / max / <= on those vectors.
// WHEN: "is a_i | prod a_j", "make all numbers equal by moving prime factors", "count subsets
// whose product is a perfect k-th power", any problem where only the RELATIVE factorisations of
// the given numbers matter and the numbers are too big to factor.
// SIZE: at most n * log log(max a) basis elements; build is O(n^2 log(max a)) worst case, and
// far less in practice. factorize(x) is O(|basis| * log x).
// NOTE the basis elements are not necessarily prime - they are only pairwise coprime, which is
// all that matters. They ARE prime whenever the inputs force it.
template <typename T>
struct PrimeBasis {
    vector<T> basis;
    void reducePair(T& x, T& y) {                       // strip the common part greedily
        bool sw = 0;
        if (x > y) sw ^= 1, swap(x, y);
        while (x > 1 && y % x == 0) { y /= x; if (x > y) sw ^= 1, swap(x, y); }
        if (sw) swap(x, y);
    }
    void split(int pos, T& val) {                       // basis[pos] and val share a factor
        while (basis[pos] % val == 0) basis[pos] /= val;
        vector<T> cur = {basis[pos], val};
        for (int ptr = 1; ptr < (int)cur.size(); ptr++)
            for (int i = 0; i < ptr; i++) {
                reducePair(cur[i], cur[ptr]);
                if (cur[ptr] == 1 || cur[i] == 1) break;
                T g = gcd(cur[ptr], cur[i]);
                if (g > 1) cur[ptr] /= g, cur[i] /= g, cur.push_back(g);
            }
        basis[pos] = cur[0], val = cur[1];
        for (int i = 2; i < (int)cur.size(); i++) if (cur[i] > 1) basis.push_back(cur[i]);
        if (basis[pos] == 1) swap(basis[pos], basis.back()), basis.pop_back();
    }
    void add(T val) {
        for (int i = 0; i < (int)basis.size(); i++) {
            reducePair(val, basis[i]);
            if (basis[i] == 1) { swap(basis[i], basis.back()); basis.pop_back(); break; }
            if (val == 1) return;
            if (gcd(basis[i], val) > 1) split(i, val);
        }
        if (val > 1) basis.push_back(val);
    }
    vector<int> factorize(T x) {                        // exponent vector of x over the basis
        vector<int> f(basis.size());
        for (int i = 0; i < (int)basis.size(); i++) while (x % basis[i] == 0) f[i]++, x /= basis[i];
        return f;
    }
};
// AFTER BUILDING: add() EVERY input first, then factorize() them - the basis must be final, or
// old vectors have the wrong length. Verify with: every factorize(a_i) must multiply back to a_i
// (i.e. dividing a_i by all basis powers leaves 1), and all pairs of basis elements are coprime.
// The vectors support: gcd = coordinatewise min, lcm = max, a | b iff vec(a) <= vec(b)
// coordinatewise, a*b = sum, a/b = difference, x is a perfect square iff all coordinates are even.
