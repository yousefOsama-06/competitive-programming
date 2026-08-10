

// Linear XOR Basis - Vector Space over GF(2)
// Time: O(LOG_BITS) per insert/query
struct Basis {
    vector<ll> basis;
    int log_bits, sz;

    Basis(int log_bits = 60) : log_bits(log_bits), sz(0), basis(log_bits, 0) {}

    bool insert(ll x) {
        for (int i = log_bits - 1; i >= 0; i--) {
            if (!(x & (1LL << i))) continue;
            if (!basis[i]) {
                basis[i] = x;
                sz++;
                return true;
            }
            x ^= basis[i];
        }
        return false;
    }

    ll getMax(ll res = 0) const {
        for (int i = log_bits - 1; i >= 0; i--) {
            res = max(res, res ^ basis[i]);
        }
        return res;
    }
};
