// Binary trie over [0, 2^(B+1)). insert / ERASE / query all O(B).
// erase() is what lets you keep the trie equal to the current root->node path during a DFS
// ("min XOR with any ancestor"): add on entry, add(-1) on exit.
struct BTrie {
    static const int B = 30;
    vector<array<int, 2>> ch = {{-1, -1}};
    vector<int> cnt = {0};

    int node() { ch.push_back({-1, -1}); cnt.push_back(0); return ch.size() - 1; }
    void add(ll x, int d = 1) {                         // d = +1 insert, -1 erase
        int u = 0; cnt[0] += d;
        for (int i = B; i >= 0; i--) {
            int b = x >> i & 1;
            if (ch[u][b] < 0) ch[u][b] = node();
            u = ch[u][b], cnt[u] += d;
        }
    }
    int cn(int u, int b) const { return u < 0 || ch[u][b] < 0 ? 0 : cnt[ch[u][b]]; }
    int size() const { return cnt[0]; }

    ll bestXor(ll x, bool mx) const {                   // mx=1 -> max x^y, mx=0 -> min x^y
        if (!cnt[0]) return -1;
        int u = 0; ll r = 0;
        for (int i = B; i >= 0; i--) {
            int b = (x >> i & 1) ^ mx;                  // bit we'd like y to have
            if (!cn(u, b)) b ^= 1;
            r |= (ll)((x >> i & 1) ^ b) << i;
            u = ch[u][b];
        }
        return r;
    }
    ll maxXor(ll x) const { return bestXor(x, 1); }
    ll minXor(ll x) const { return bestXor(x, 0); }

    int cntLess(ll x, ll k) const {                     // #{y in trie : (x ^ y) < k}
        int u = 0, r = 0;
        for (int i = B; i >= 0 && u >= 0; i--) {
            int xb = x >> i & 1;
            if (k >> i & 1) r += cn(u, xb), u = ch[u][xb ^ 1];   // xor-bit 0 branch is all < k
            else u = ch[u][xb];
        }
        return r;
    }
};
