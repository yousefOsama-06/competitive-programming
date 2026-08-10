// Polygon Area (Shoelace Formula) - O(N)
template<typename T>
ld polygonArea(const vector<T>& v) {
    ld area = 0;
    int n = v.size();
    for (int i = 0; i < n; i++) {
        area += v[i].cross(v[(i + 1) % n]);
    }
    return 0.5 * abs(area);
}
