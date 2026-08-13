// BIG INTEGERS, non-negative, base 1e9. Enough for the usual "the answer does not fit in 64 bits"
// problems: exact factorials / Catalan numbers / determinants, huge Fibonacci, exact comparisons.
// Add/sub O(n), schoolbook multiply O(n*m) (fine to ~1e4 digits), divide by a small int O(n).
// If you need big*big beyond that, convolve the limbs with FFT and carry.
struct Big {
    static const ll B = 1000000000;
    vector<ll> d;                                              // little-endian limbs, base 1e9
    Big(ll v = 0) { for (; v > 0; v /= B) d.push_back(v % B); }
    Big(const string& s) {
        for (int i = s.size(); i > 0; i -= 9)
            d.push_back(stoll(s.substr(max(0, i - 9), min(9, i))));
        trim();
    }
    void trim() { while (!d.empty() && d.back() == 0) d.pop_back(); }
    bool operator<(const Big& o) const {
        if (d.size() != o.d.size()) return d.size() < o.d.size();
        for (int i = d.size() - 1; i >= 0; i--) if (d[i] != o.d[i]) return d[i] < o.d[i];
        return false;
    }
    Big operator+(const Big& o) const {
        Big r; ll c = 0;
        for (size_t i = 0; i < max(d.size(), o.d.size()) || c; i++) {
            if (i < d.size()) c += d[i];
            if (i < o.d.size()) c += o.d[i];
            r.d.push_back(c % B), c /= B;
        }
        return r;
    }
    Big operator-(const Big& o) const {                        // requires *this >= o
        Big r = *this; ll c = 0;
        for (size_t i = 0; i < r.d.size(); i++) {
            r.d[i] -= c + (i < o.d.size() ? o.d[i] : 0);
            if ((c = r.d[i] < 0)) r.d[i] += B;
        }
        r.trim();
        return r;
    }
    Big operator*(const Big& o) const {
        if (d.empty() || o.d.empty()) return Big();
        vector<lll> t(d.size() + o.d.size(), 0);
        for (size_t i = 0; i < d.size(); i++)
            for (size_t j = 0; j < o.d.size(); j++) t[i + j] += (lll)d[i] * o.d[j];
        Big r; lll c = 0;
        for (size_t i = 0; i < t.size(); i++) c += t[i], r.d.push_back((ll)(c % B)), c /= B;
        for (; c; c /= B) r.d.push_back((ll)(c % B));
        r.trim();
        return r;
    }
    Big operator*(ll v) const { return *this * Big(v); }
    Big operator/(ll v) const {                                // small divisor only
        Big r = *this; lll c = 0;
        for (int i = r.d.size() - 1; i >= 0; i--) c = c * B + r.d[i], r.d[i] = (ll)(c / v), c %= v;
        r.trim();
        return r;
    }
    ll operator%(ll v) const {
        lll c = 0;
        for (int i = d.size() - 1; i >= 0; i--) c = (c * B + d[i]) % v;
        return (ll)c;
    }
    string str() const {
        if (d.empty()) return "0";
        string s = to_string(d.back());
        for (int i = d.size() - 2; i >= 0; i--) {
            string t = to_string(d[i]);
            s += string(9 - t.size(), '0') + t;
        }
        return s;
    }
};
// SIGNED: keep a separate sign flag and dispatch +/- to add/sub on magnitudes; comparison first.
// BASE 1e9 packs 9 decimal digits per limb, so printing is trivial - that is the whole point.
// FAST DIVISION of two Bigs is long and almost never needed; if a problem really wants it, binary
// search the quotient limb by limb, or move to Python-style thinking and avoid it.
// SQRT: binary search with the multiply above, or Newton on doubles then fix up by a few steps.
