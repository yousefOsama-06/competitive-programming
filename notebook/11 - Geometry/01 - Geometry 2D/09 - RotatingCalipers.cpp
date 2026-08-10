// Rotating Calipers - O(N) Convex Polygon Diameter & Width
template<typename T>
ll hullDiameter(const vector<T>& S) {
    int n = S.size(), j = n < 2 ? 0 : 1;
    ll res = 0;
    for (int i = 0; i < j; i++) {
        for (;; j = (j + 1) % n) {
            res = max(res, (ll)(S[i] - S[j]).dist2());
            if ((S[(j + 1) % n] - S[j]).cross(S[i + 1] - S[i]) >= 0) break;
        }
    }
    return res;
}

template<typename T>
ld hullWidth(const vector<T>& S) {
    int n = S.size();
    if (n <= 2) return 0;
    int j = 1;
    ld res = 1e18;
    for (int i = 0; i < n; i++) {
        while ((S[(i + 1) % n] - S[i]).cross(S[(j + 1) % n] - S[j]) >= 0) j = (j + 1) % n;
        res = min(res, lineDist(S[j], S[i], S[(i + 1) % n]));
    }
    return res;
}
