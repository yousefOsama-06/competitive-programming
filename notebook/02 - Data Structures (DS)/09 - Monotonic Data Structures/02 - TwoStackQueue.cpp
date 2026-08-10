// Monotonic Queue via Two Stacks (Sliding Window Min/Max)
// Space: O(N), Time: O(1) amortized per push/pop/get
struct TwoStackQ {
    struct Node {
        ll mx = -llinf, mn = llinf, val;

        Node() : val(0) {}
        Node(ll x) : mx(x), mn(x), val(x) {}
    };

    stack<Node> a, b;

    int size() {
        return a.size() + b.size();
    }

    void mrg(Node &a, Node &b) {
        a.mn = min(a.mn, b.mn);
        a.mx = max(a.mx, b.mx);
    }

    // Pushes value into the queue
    void push(ll val) {
        auto nd = Node(val);
        if (!a.empty()) mrg(nd, a.top());
        a.push(nd);
    }

    // Transfers elements from stack a to stack b when b is empty
    void move() {
        while (!a.empty()) {
            auto nd = Node(a.top().val);
            if (!b.empty()) mrg(nd, b.top());
            b.push(nd);
            a.pop();
        }
    }

    // Returns min/max of all current elements in the queue in O(1)
    Node get() {
        Node res;
        if (!b.empty()) mrg(res, b.top());
        if (!a.empty()) mrg(res, a.top());
        return res;
    }

    // Removes the oldest element from the queue
    void pop() {
        if (b.empty()) move();
        if (!b.empty()) b.pop();
    }
};