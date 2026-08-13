// EXPRESSION PARSER (shunting-yard). Handles + - * / % ^, unary minus, parentheses.
// Left-associative except ^. Extend by editing prec() / applyOp().
struct Parser {
    static int prec(char op) {
        switch (op) {
            case '+': case '-': return 1;
            case '*': case '/': case '%': return 2;
            case '^': return 3;
            case '~': return 4;                       // unary minus (internal symbol)
            default: return -1;
        }
    }
    static bool rightAssoc(char op) { return op == '^' || op == '~'; }

    static ll apply(char op, ll a, ll b = 0) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/': return a / b;
            case '%': return a % b;
            case '~': return -a;
            case '^': { ll r = 1; while (b > 0) { if (b & 1) r *= a; a *= a, b >>= 1; } return r; }
        }
        return 0;
    }
    static ll eval(const string& s) {
        vector<ll> val;
        vector<char> ops;
        bool wantOperand = true;                      // distinguishes unary from binary minus
        auto pop = [&]() {
            char op = ops.back(); ops.pop_back();
            if (op == '~') { ll a = val.back(); val.pop_back(); val.push_back(apply(op, a)); }
            else { ll b = val.back(); val.pop_back(); ll a = val.back(); val.pop_back();
                   val.push_back(apply(op, a, b)); }
        };
        for (size_t i = 0; i < s.size(); i++) {
            char c = s[i];
            if (isspace((unsigned char)c)) continue;
            if (isdigit((unsigned char)c)) {
                ll x = 0;
                while (i < s.size() && isdigit((unsigned char)s[i])) x = x * 10 + (s[i++] - '0');
                i--;
                val.push_back(x), wantOperand = false;
            } else if (c == '(') ops.push_back(c), wantOperand = true;
            else if (c == ')') {
                while (!ops.empty() && ops.back() != '(') pop();
                if (!ops.empty()) ops.pop_back();
                wantOperand = false;
            } else {
                if (c == '-' && wantOperand) c = '~';  // unary
                while (!ops.empty() && ops.back() != '(' &&
                       (prec(ops.back()) > prec(c) || (prec(ops.back()) == prec(c) && !rightAssoc(c))))
                    pop();
                ops.push_back(c), wantOperand = (c != '~') || true;
            }
        }
        while (!ops.empty()) pop();
        return val.empty() ? 0 : val.back();
    }
};
// To build an AST instead of evaluating, push node indices instead of values.
// For boolean/comparison operators just add them to prec() with lower precedence.
