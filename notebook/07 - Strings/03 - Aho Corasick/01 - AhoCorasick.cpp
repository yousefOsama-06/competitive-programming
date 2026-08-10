struct Mapper {
    int operator()(char c) const { return c - 'a'; }
};

template <int ALPHABET, typename Mapper>
struct Aho {
    struct Node {
        array<int, ALPHABET> nxt;
        int link = 0;
        int terminal_idx = -1; // -1 if not a terminal, else idx of string insertion
        int exit_link = 0; // Points to the nearest terminal suffix
        int depth = 0; // Size of the prefix the node represents
        Node() { nxt.fill(0); }
    };

    vector<Node> t;
    vector<int> bfs_order;
    Mapper get_idx; // Instance of the mapping function
    int word_count = 0; // Tracks the order of inserted strings

    // Constructor optionally accepts a custom mapper instance
    Aho(Mapper mapper = Mapper()) : get_idx(mapper) {
        t.emplace_back();
    }

    int insert(const string& p) {
        int u = 0;
        for (char c : p) {
            int v = get_idx(c);
            if (!t[u].nxt[v]) {
                t[u].nxt[v] = t.size();
                t.emplace_back();
                t.back().depth = t[u].depth + 1; // New node's depth is parent's depth + 1
            }
            u = t[u].nxt[v];
        }

        // If this exact string hasn't been inserted before, assign it the current index
        if (t[u].terminal_idx == -1) {
            t[u].terminal_idx = word_count;
        }
        word_count++; // Increment for the next insertion

        return u;
    }

    void build() {
        queue<int> q;
        for (int c = 0; c < ALPHABET; ++c) {
            if (t[0].nxt[c]) {
                q.push(t[0].nxt[c]);
            }
        }

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            bfs_order.push_back(u);

            for (int c = 0; c < ALPHABET; ++c) {
                int v = t[u].nxt[c];
                if (v) {
                    t[v].link = t[t[u].link].nxt[c];

                    // Compute the exit link:
                    // If the immediate suffix link is a terminal, point to it.
                    // Otherwise, copy its exit link.
                    t[v].exit_link = (t[t[v].link].terminal_idx != -1) ? t[v].link : t[t[v].link].exit_link;

                    q.push(v);
                }
                else {
                    t[u].nxt[c] = t[t[u].link].nxt[c];
                }
            }
        }
    }

    int advance(int u, char c) {
        return t[u].nxt[get_idx(c)];
    }
};