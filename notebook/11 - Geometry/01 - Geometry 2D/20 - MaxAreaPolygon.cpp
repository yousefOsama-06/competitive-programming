// MAXIMUM-AREA POLYGON WITH GIVEN SIDE LENGTHS. Given n side lengths in any order, the polygon
// of largest area that has exactly those sides is CYCLIC - all its vertices lie on one circle
// (and the order of the sides does not matter, which is the surprising half of the theorem). So
// the whole problem is "find the circumradius R", and each side a_i subtends a central angle
// 2 * asin(a_i / (2R)). Two cases: if the centre is INSIDE, the angles sum to 2 * pi; if the
// longest side is long enough to push the centre OUTSIDE, its angle is subtracted instead and the
// signed sum is 0. Both are monotone in R, so binary search.
// WHEN: "arrange these n sticks into the polygon of largest area"; the n = 3 case is Heron and
// the n = 4 case is Brahmagupta, and this is the general answer for both. Also the standing proof
// that a regular polygon maximises area among equilateral ones.
// COMPLEXITY: O(n * 100) - one binary search with 100 iterations, each O(n).
// DEGENERATE: no polygon exists at all unless the longest side is strictly shorter than the sum
// of the others (the polygon inequality); the routine returns -1 for that, and for n < 3. Sides
// of length 0 are harmless. All sides equal gives the regular n-gon, area n r^2 sin(2 pi / n) / 2
// with r = a / (2 sin(pi / n)) - use that to check the code.
// FLOATING POINT, necessarily: R is irrational. asin loses precision when a_i / (2R) is near 1,
// which is exactly the degenerate "longest side is a diameter" case, so clamp the argument.
ld maxAreaPolygon(vector<ld> a) {
    int n = sz(a);
    if (n < 3) return -1;
    int m = max_element(all(a)) - a.begin();
    ld s = 0;
    for (int i = 0; i < n; i++) if (i != m) s += a[i];
    if (a[m] >= s) return -1;                           // the polygon inequality fails
    auto ang = [&](ld R, int sg) {                      // sum of central angles, a[m] signed
        ld t = 0;
        for (int i = 0; i < n; i++) {
            ld u = 2 * asinl(min((ld)1, a[i] / (2 * R)));
            t += (i == m ? sg * u : u);
        }
        return t;
    };
    ld lo = a[m] / 2, hi = 1e18;                        // R >= a[m] / 2 always
    int sg = ang(lo, 1) > 2 * PI ? 1 : -1;              // centre inside (+1) or outside (-1)
    ld tgt = sg > 0 ? 2 * PI : 0;                       // ang(.) is DECREASING in R for sg = +1,
    for (int it = 0; it < 200; it++) {                  // INCREASING for sg = -1
        ld mid = (lo + hi) / 2;
        ((ang(mid, sg) > tgt) == (sg > 0) ? lo : hi) = mid;
    }
    ld R = (lo + hi) / 2, area = 0;
    for (int i = 0; i < n; i++) {
        ld u = 2 * asinl(min((ld)1, a[i] / (2 * R)));
        area += (i == m ? sg : 1) * R * R * sinl(u) / 2;
    }
    return area;
}
// THE VERTICES, if the problem wants them and not just the area: walk the angles. Start at
// (R, 0), and after each side rotate by its central angle (negated for the long side when the
// centre is outside). p_0 = R * (cos 0, sin 0), theta_{k+1} = theta_k + u_k, p_k = R * (cos, sin).
// TRIANGLE (n = 3), closed form, no search: HERON with the stable sorted form,
//   sort a >= b >= c, A = sqrt((a+(b+c))(c-(a-b))(c+(a-b))(a+(b-c))) / 4.
// CYCLIC QUADRILATERAL (n = 4): BRAHMAGUPTA, A = sqrt((s-a)(s-b)(s-c)(s-d)) with s the
// semiperimeter - and by this theorem that IS the maximum over all quadrilaterals with those
// sides. General quadrilateral with diagonals p, q (BRETSCHNEIDER):
//   A = sqrt(4 p^2 q^2 - (b^2 + d^2 - a^2 - c^2)^2) / 4.
// MAXIMUM AREA WITH GIVEN PERIMETER, no side constraints: the circle. Among n-gons: the regular
// one. This file is the constrained version of the same isoperimetric fact.
// A DIFFERENT PROBLEM often confused with this one: pick the triangle of maximum area from a set
// of given points - that is rotating calipers on the hull, O(n^2), and is not here.
