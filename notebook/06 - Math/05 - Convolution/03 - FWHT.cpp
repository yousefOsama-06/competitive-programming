// Fast Walsh-Hadamard Transform (FWHT) - Time: O(N log N)
// Computes Bitwise Convolutions: XOR, AND, OR for array of size N = 2^k
struct FWHT {
    enum Type { XOR, AND, OR };

    static void transform(vector<ll>& a, bool invert, Type type = XOR) {
        int n = a.size();
        for (int len = 1; 2 * len <= n; len <<= 1) {
            for (int i = 0; i < n; i += 2 * len) {
                for (int j = 0; j < len; j++) {
                    ll u = a[i + j], v = a[i + len + j];
                    if (type == XOR) {
                        a[i + j] = u + v;
                        a[i + len + j] = u - v;
                    } else if (type == AND) {
                        a[i + j] = invert ? u - v : u + v;
                    } else if (type == OR) {
                        a[i + len + j] = invert ? v - u : u + v;
                    }
                }
            }
        }
        if (type == XOR && invert) {
            for (ll &x : a) x /= n;
        }
    }

    // Computes C[k] = sum_{i op j = k} (A[i] * B[j]) where op in {XOR, AND, OR}
    static vector<ll> multiply(vector<ll> a, vector<ll> b, Type type = XOR) {
        int n = 1;
        while (n < max(a.size(), b.size())) n <<= 1;
        a.resize(n); b.resize(n);

        transform(a, false, type);
        transform(b, false, type);
        for (int i = 0; i < n; i++) a[i] *= b[i];
        transform(a, true, type);
        return a;
    }
};
