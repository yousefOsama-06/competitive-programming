// NOTE: 02 - CHT.cpp also declares `struct Line` / `struct HullDynamic`, but that one is the
//       UPPER hull (maximum) and this one is the LOWER hull (minimum) - they are NOT
//       interchangeable. If you need both, rename one pair to UpperHull / LowerHull.
/* ==============================================================================
 * DYNAMIC CONVEX HULL TRICK (CHT) - AVERAGE SUBARRAY TEMPLATE
 * ==============================================================================
 *
 * HOW TO USE THE 4 VARIATIONS:
 *
 * 1. MAX Average, LONGEST length  (Default)
 *    - Code: Use `lhs < rhs` in Query::operator<
 *    - Call: hull.insert_line(i, S[i]); hull.query_tangent_index(i, S[i]);
 *
 * 2. MAX Average, SHORTEST length
 *    - Code: Use `lhs <= rhs` in Query::operator<
 *    - Call: hull.insert_line(i, S[i]); hull.query_tangent_index(i, S[i]);
 *
 * 3. MIN Average, LONGEST length
 *    - Code: Use `lhs < rhs` in Query::operator<
 *    - Call: hull.insert_line(i, -S[i]); hull.query_tangent_index(i, -S[i]); // Negated
 *
 * 4. MIN Average, SHORTEST length
 *    - Code: Use `lhs <= rhs` in Query::operator<
 *    - Call: hull.insert_line(i, -S[i]); hull.query_tangent_index(i, -S[i]); // Negated
 * ============================================================================== */

struct Query {
    ll i, Si;
};

struct Line {
    ll m, b;
    mutable function<const Line *()> succ;

    bool operator<(const Line &other) const {
        return m < other.m;
    }

    bool operator<(const Query &q) const {
        const Line *s = succ();
        if (!s) return false;

        // CORRECTED ALGEBRA: Direct cross-multiplication of slopes
        __int128_t lhs = (__int128_t)(q.Si - b) * (q.i - s->m);
        __int128_t rhs = (__int128_t)(q.Si - s->b) * (q.i - m);

        // --- TIE-BREAKING CONTROL ---
        // Change this return statement based on length requirements:
        // return lhs < rhs;  // <-- Use for LONGEST subarray
        // return lhs <= rhs; // <-- Use for SHORTEST subarray
        return lhs < rhs;
    }
};

struct HullDynamic : public multiset<Line, less<>> {
    bool bad(iterator y) {
        auto z = next(y);
        if (y == begin()) {
            if (z == end()) return false;
            // CORRECTED: Lower hull keeps smallest y
            return y->m == z->m && y->b >= z->b;
        }
        auto x = prev(y);
        if (z == end()) return y->m == x->m && y->b >= x->b;

        return (__int128_t)(x->b - y->b) * (z->m - y->m) <= (__int128_t)(y->b - z->b) * (y->m - x->m);
    }

    void insert_line(ll m, ll b) {
        auto y = insert({m, b});
        y->succ = [=] { return next(y) == end() ? 0 : &*next(y); };
        if (bad(y)) {
            erase(y);
            return;
        }
        while (next(y) != end() && bad(next(y))) erase(next(y));
        while (y != begin() && bad(prev(y))) erase(prev(y));
    }

    int query_tangent_index(ll i, ll Si) {
        auto l = *lower_bound(Query{i, Si});
        return l.m;
    }
};

// USAGE (longest subarray with average >= 0, after subtracting the target from every element):
//   S[0..n] = prefix sums; the answer for each i is i - (tangent index from the hull of (j, S[j])).
//   HullDynamic hull; hull.insert_line(0, 0);
//   for (int i = 1; i <= n; i++) {
//       int k = hull.query_tangent_index(i, S[i]);          // best j < i
//       ans[i] = i - k;
//       hull.insert_line(i, S[i]);
//   }
// WHY IT WORKS: avg(j+1..i) >= 0  <=>  S[i] - S[j] >= 0, and among all valid j you want the
// SMALLEST, which is the point of tangency from (i, S[i]) to the lower hull of the points (j, S[j]).
// Binary search on the answer + prefix sums is the shorter O(n log n) alternative; this is O(n).
// THE FOUR VARIATIONS, all from the same hull - only the tie-break and the sign change:
//   MAX average, LONGEST  segment: insert(i, S[i]),  tangent(i, S[i]),  strict `<`  in the compare
//   MAX average, SHORTEST segment: insert(i, S[i]),  tangent(i, S[i]),  non-strict `<=`
//   MIN average, LONGEST  segment: insert(i, -S[i]), tangent(i, -S[i]), strict `<`
//   MIN average, SHORTEST segment: insert(i, -S[i]), tangent(i, -S[i]), non-strict `<=`
// (Minimising is just maximising the negated prefix sums; the tie-break decides which of two
// equally-good segments you keep, which is what "longest" versus "shortest" means here.)
