// LYNDON FACTORISATION (Duval) and MINIMAL CYCLIC ROTATION (Duval), both O(n), O(1) memory.
// A Lyndon word is strictly smaller than all of its proper suffixes. Every string factors
// UNIQUELY as w1 >= w2 >= ... >= wk with each wi Lyndon.
vector<string> duval(const string& s) {
    int n = s.size(), i = 0;
    vector<string> f;
    while (i < n) {
        int j = i + 1, k = i;
        while (j < n && s[k] <= s[j]) s[k] < s[j] ? k = i : k++, j++;
        while (i <= k) f.push_back(s.substr(i, j - k)), i += j - k;
    }
    return f;
}
// Smallest cyclic rotation: run Duval on s+s and take the factor starting before n.
int minRotation(string s) {
    int n = s.size();
    s += s;
    int i = 0, ans = 0;
    while (i < n) {
        ans = i;
        int j = i + 1, k = i;
        while (j < (int)s.size() && s[k] <= s[j]) s[k] < s[j] ? k = i : k++, j++;
        while (i <= k) i += j - k;
    }
    return ans;                                  // index in the ORIGINAL string
}
// USES: canonical form of a cyclic string (necklace dedup -> pairs with Burnside);
//       "is A a rotation of B" (compare minRotation forms, or search A in B+B);
//       lexicographically smallest result of rotating; runs / repetitions via Lyndon roots.
