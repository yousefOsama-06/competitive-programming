// AREA OF UNIONS. The three shapes that actually show up.

// --- UNION OF POLYGONS (any simple polygons, may overlap), O(n^2 log n). ---
// Works by, for every edge, measuring how much of it is "on the boundary of the union" and
// accumulating the shoelace contribution with the right multiplicity.
ld edgeRatio(Pd a, Pd b) { return sgn(b.x) ? a.x / b.x : a.y / b.y; }
ld polyUnion(vector<vector<Pd>>& ps) {                    // every polygon must be CCW
    ld ret = 0;
    for (size_t i = 0; i < ps.size(); i++)
        for (size_t v = 0; v < ps[i].size(); v++) {
            Pd A = ps[i][v], B = ps[i][(v + 1) % ps[i].size()];
            vector<pair<ld, int>> seg = {{0, 0}, {1, 0}};
            for (size_t j = 0; j < ps.size(); j++) {
                if (i == j) continue;
                for (size_t u = 0; u < ps[j].size(); u++) {
                    Pd C = ps[j][u], D = ps[j][(u + 1) % ps[j].size()];
                    int sc = sgn(A.cross(B, C)), sd = sgn(A.cross(B, D));
                    if (sc != sd) {
                        ld sa = C.cross(D, A), sb = C.cross(D, B);
                        if (min(sc, sd) < 0) seg.push_back({sa / (sa - sb), sc > sd ? 1 : -1});
                    } else if (!sc && !sd && j < i && sgn((B - A).dot(D - C)) > 0) {
                        seg.push_back({edgeRatio(C - A, B - A), 1});
                        seg.push_back({edgeRatio(D - A, B - A), -1});
                    }
                }
            }
            sort(all(seg));
            for (auto& s : seg) s.first = min(max(s.first, (ld)0), (ld)1);
            ld sum = 0;
            int cnt = seg[0].second;
            for (size_t j = 1; j < seg.size(); j++) {     // fraction of AB on the union boundary
                if (!cnt) sum += seg[j].first - seg[j - 1].first;
                cnt += seg[j].second;
            }
            ret += A.cross(B) * sum;
        }
    return ret / 2;
}

// --- UNION OF CIRCLES, O(n^2 log n) by integrating each arc that is on the outer boundary. ---
ld circleUnion(vector<Pd> c, vector<ld> r) {
    int n = c.size();
    ld total = 0;
    for (int i = 0; i < n; i++) {
        vector<pair<ld, int>> ev;
        int cover = 0;
        for (int j = 0; j < n; j++) {
            if (i == j) continue;
            ld d = (c[i] - c[j]).dist();
            if (d + r[i] <= r[j] + eps &&
                (r[i] < r[j] || (fabsl(r[i] - r[j]) < eps && j < i))) { cover = -1; break; }
            if (d >= r[i] + r[j] - eps || d + r[j] <= r[i] + eps) continue;
            ld a = (c[j] - c[i]).angle();
            ld w = acosl((d * d + r[i] * r[i] - r[j] * r[j]) / (2 * d * r[i]));
            ld L = a - w, R = a + w;
            if (L < -PI) L += 2 * PI;
            if (R > PI) R -= 2 * PI;
            if (L > R) ev.push_back({L, 1}), ev.push_back({PI, -1}),
                       ev.push_back({-PI, 1}), ev.push_back({R, -1});
            else ev.push_back({L, 1}), ev.push_back({R, -1});
        }
        if (cover < 0) continue;
        ev.push_back({-PI, 0}), ev.push_back({PI, 0});
        sort(all(ev));
        int cnt = 0; ld last = -PI;
        for (auto [a, t] : ev) {
            if (!cnt && a > last) {                          // arc [last, a] is on the boundary
                Pd p1 = c[i] + Pd(cosl(last), sinl(last)) * r[i];
                Pd p2 = c[i] + Pd(cosl(a), sinl(a)) * r[i];
                total += p1.cross(p2) / 2 + r[i] * r[i] * ((a - last) - sinl(a - last)) / 2;
            }
            cnt += t;
            last = max(last, a);
        }
    }
    return total;
}

// --- PERIMETER OF A UNION OF AXIS-ALIGNED RECTANGLES ---
// Run the rectUnionArea sweep TWICE (once per axis). At each event x, the vertical contribution
// is |coveredLen(after) - coveredLen(before)|; sum those, then repeat with x/y swapped.
