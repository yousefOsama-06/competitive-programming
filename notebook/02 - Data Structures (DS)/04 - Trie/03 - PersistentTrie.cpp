// Persistent Binary Trie - Space: O(N * BITS), Time: O(BITS) per insert/query
// Supports versioning and range max XOR queries between versions [l_ver, r_ver]
struct PersistentTrie {
    struct Node {
        int cnt = 0;
        array<int, 2> nxt = {-1, -1};
    };

    vector<Node> tree;
    vector<int> roots;
    int max_bit;

    PersistentTrie(int max_bit = 30) : max_bit(max_bit) {
        roots.pb(add_node());
    }

    int add_node(int old_node = -1) {
        if (old_node != -1) {
            tree.pb(tree[old_node]);
        } else {
            tree.pb(Node());
        }
        return tree.size() - 1;
    }

    // Inserts x starting from prev_root and returns the new version root
    int insert(int prev_root, ll x) {
        int new_root = add_node(prev_root);
        int u = new_root, prev_u = prev_root;
        tree[u].cnt++;

        for (int i = max_bit; i >= 0; i--) {
            int b = (x >> i) & 1;
            int prev_child = (prev_u != -1) ? tree[prev_u].nxt[b] : -1;
            tree[u].nxt[b] = add_node(prev_child);
            u = tree[u].nxt[b];
            if (prev_u != -1) prev_u = tree[prev_u].nxt[b];
            tree[u].cnt++;
        }
        roots.pb(new_root);
        return new_root;
    }

    // Returns max (x ^ val) over elements inserted in version range [l_ver, r_ver]
    ll max_xor(int l_ver, int r_ver, ll x) const {
        int u_r = roots[r_ver], u_l = (l_ver > 0) ? roots[l_ver - 1] : -1;
        ll res = 0;

        for (int i = max_bit; i >= 0; i--) {
            int b = (x >> i) & 1;
            int want = b ^ 1;

            int count_r = (u_r != -1 && tree[u_r].nxt[want] != -1) ? tree[tree[u_r].nxt[want]].cnt : 0;
            int count_l = (u_l != -1 && tree[u_l].nxt[want] != -1) ? tree[tree[u_l].nxt[want]].cnt : 0;

            if (count_r - count_l > 0) {
                res |= (1LL << i);
                u_r = tree[u_r].nxt[want];
                u_l = (u_l != -1 && tree[u_l].nxt[want] != -1) ? tree[u_l].nxt[want] : -1;
            } else {
                u_r = (u_r != -1 && tree[u_r].nxt[b] != -1) ? tree[u_r].nxt[b] : -1;
                u_l = (u_l != -1 && tree[u_l].nxt[b] != -1) ? tree[u_l].nxt[b] : -1;
            }
        }
        return res;
    }
};
