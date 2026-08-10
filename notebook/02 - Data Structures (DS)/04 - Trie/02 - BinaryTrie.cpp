// Binary Trie for 64-bit integers and XOR operations
// Time: O(BITS) per operation
struct BinaryTrie {
    vector<array<int, 2>> nxt;
    vector<int> cnt;
    int max_bit;

    BinaryTrie(int max_bit = 60) : max_bit(max_bit) {
        add_node();
    }

    int add_node() {
        nxt.push_back({-1, -1});
        cnt.push_back(0);
        return nxt.size() - 1;
    }

    void insert(ll x, int val = 1) {
        int u = 0;
        cnt[u] += val;
        for (int i = max_bit; i >= 0; i--) {
            int b = (x >> i) & 1;
            if (nxt[u][b] == -1) nxt[u][b] = add_node();
            u = nxt[u][b];
            cnt[u] += val;
        }
    }

    // Returns element in trie maximizing (x ^ element)
    ll max_xor(ll x) const {
        if (!cnt[0]) return -1e18;
        int u = 0;
        ll res = 0;
        for (int i = max_bit; i >= 0; i--) {
            int b = (x >> i) & 1;
            int want = b ^ 1;
            if (nxt[u][want] != -1 && cnt[nxt[u][want]] > 0) {
                res |= (1LL << i);
                u = nxt[u][want];
            } else {
                u = nxt[u][b];
            }
        }
        return res;
    }
};