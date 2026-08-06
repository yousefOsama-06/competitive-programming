mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());

template <typename T>
struct ImplicitTreap {
    struct Node {
        T val, sum;
        T lazy = 0;       // Lazy tag for range additions
        bool rev = false; // Lazy tag for range reversals
        int sz = 1;
        uint64_t p = rng();
        Node *l = nullptr, *r = nullptr;

        Node(T v = T()) : val(v), sum(v) {}
    }* root = nullptr;

    // Default Constructor
    ImplicitTreap() = default;

    // Vector Constructor - Builds treap in O(N) time
    ImplicitTreap(const vector<T>& a) {
        build(a);
    }

    ~ImplicitTreap() { destroy(root); }

    void destroy(Node* t) {
        if (!t) return;
        destroy(t->l);
        destroy(t->r);
        delete t;
    }

    int sz(Node* t) { return t ? t->sz : 0; }
    T sub_val(Node* t) { return t ? t->sum : 0; }

    // Propagates pending lazy updates to children
    void push(Node* t) {
        if (!t) return;
        
        // 1. Process pending reversal
        if (t->rev) {
            swap(t->l, t->r);
            if (t->l) t->l->rev ^= true;
            if (t->r) t->r->rev ^= true;
            t->rev = false;
        }

        // 2. Process pending value addition
        if (t->lazy != 0) {
            if (t->l) {
                t->l->val += t->lazy;
                t->l->sum += t->lazy * sz(t->l);
                t->l->lazy += t->lazy;
            }
            if (t->r) {
                t->r->val += t->lazy;
                t->r->sum += t->lazy * sz(t->r);
                t->r->lazy += t->lazy;
            }
            t->lazy = 0; // Clear the tag
        }
    }

    Node* update(Node* t) {
        if (t) {
            t->sz = 1 + sz(t->l) + sz(t->r);
            t->sum = t->val + sub_val(t->l) + sub_val(t->r);
        }
        return t;
    }

    // --- O(N) Linear Time Build ---
    void build(const vector<T>& a) {
        destroy(root);
        root = nullptr;
        if (a.empty()) return;

        vector<Node*> st;
        for (const T& val : a) {
            Node* curr = new Node(val);
            Node* last = nullptr;

            // Maintain max-heap property on random priority 'p'
            while (!st.empty() && st.back()->p < curr->p) {
                last = st.back();
                st.pop_back();
            }

            curr->l = last;
            if (!st.empty()) {
                st.back()->r = curr;
            }
            st.push_back(curr);
        }

        // Post-order pass to compute subtree sizes and sums in O(N)
        auto recompute = [&](auto& self, Node* t) -> void {
            if (!t) return;
            self(self, t->l);
            self(self, t->r);
            update(t);
        };

        root = st.front(); // Bottom of stack is the root
        recompute(recompute, root);
    }

    // --- Dynamic Array Operations ---

    Node* merge(Node* l, Node* r) {
        push(l);
        push(r); 
        if (!l || !r) return l ? l : r;
        if (l->p > r->p) {
            l->r = merge(l->r, r);
            return update(l);
        }
        r->l = merge(l, r->l);
        return update(r);
    }

    pair<Node*, Node*> split(Node* t, int k) {
        if (!t) return {nullptr, nullptr};
        push(t);
        
        int left_sz = sz(t->l);
        if (left_sz < k) {
            auto [l, r] = split(t->r, k - left_sz - 1);
            t->r = l;
            return {update(t), r};
        } else {
            auto [l, r] = split(t->l, k);
            t->l = r;
            return {l, update(t)};
        }
    }

    int size() { return sz(root); }

    void insert(int idx, T val) {
        auto [l, r] = split(root, idx);
        Node* node = new Node(val);
        root = merge(merge(l, node), r);
    }

    void erase(int idx) {
        if (idx < 0 || idx >= sz(root)) return;
        auto [left_mid, right] = split(root, idx + 1);
        auto [left, mid] = split(left_mid, idx);
        destroy(mid);
        root = merge(left, right);
    }

    void reverse_range(int l, int r) {
        if (l >= r || l < 0 || r >= sz(root)) return;

        auto [left_mid, right] = split(root, r + 1);
        auto [left, mid] = split(left_mid, l);

        if (mid) mid->rev ^= true;

        root = merge(merge(left, mid), right);
    }

    void add_range(int l, int r, T add_val) {
        if (l > r || l < 0 || r >= sz(root)) return;
        
        auto [left_mid, right] = split(root, r + 1);
        auto [left, mid] = split(left_mid, l);
        
        if (mid) {
            mid->val += add_val;
            mid->sum += add_val * sz(mid);
            mid->lazy += add_val;
        }
        
        root = merge(merge(left, mid), right);
    }

    T query_range(int l, int r) {
        if (l > r || l < 0 || r >= sz(root)) return T();

        auto [left_mid, right] = split(root, r + 1);
        auto [left, mid] = split(left_mid, l);

        T ans = sub_val(mid);

        root = merge(merge(left, mid), right);
        return ans;
    }

    T get(int idx) {
        return query_range(idx, idx);
    }

    void print(Node* t) {
        if (!t) return;
        push(t);
        print(t->l);
        cout << t->val << " ";
        print(t->r);
    }

    void print() {
        print(root);
        cout << '\n';
    }
};