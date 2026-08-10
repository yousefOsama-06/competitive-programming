// Convex Hull (Andrew's Monotone Chain) - O(N log N)
// Returns vertices in counter-clockwise order
template<typename P>
vector<P> convexHull(vector<P> pts) {
    int n = pts.size(), k = 0;
    if (n <= 2) return pts;
    vector<P> h(2 * n);
    sort(all(pts));
    for (int i = 0; i < n; i++) {
        while (k >= 2 && h[k - 2].cross(h[k - 1], pts[i]) <= 0) k--;
        h[k++] = pts[i];
    }
    for (int i = n - 2, t = k + 1; i >= 0; i--) {
        while (k >= t && h[k - 2].cross(h[k - 1], pts[i]) <= 0) k--;
        h[k++] = pts[i];
    }
    h.resize(k - 1);
    return h;
}
