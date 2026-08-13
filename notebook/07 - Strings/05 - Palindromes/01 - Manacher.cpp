// MANACHER - the radius of the longest palindrome centred at every position, in O(n).
// Answers "is s[l..r] a palindrome" in O(1) and counts all palindromic substrings in one pass.
struct Manacher {
    vector<int> p;
    
    // Returns a vector where p[i] is the radius of the longest palindrome around center i
    // Centers are interleaved: 0 is before string, 1 is first char, 2 is between 1st & 2nd, etc.
    Manacher(string s) {
        string t = "#";
        for (char c : s) {
            t += c;
            t += "#";
        }
        int n = t.size();
        p.assign(n, 0);
        int c = 0, r = 0;
        for (int i = 0; i < n; i++) {
            int mirror = 2 * c - i;
            if (i < r) p[i] = min(r - i, p[mirror]);
            while (i - 1 - p[i] >= 0 && i + 1 + p[i] < n && t[i - 1 - p[i]] == t[i + 1 + p[i]]) {
                p[i]++;
            }
            if (i + p[i] > r) {
                c = i;
                r = i + p[i];
            }
        }
    }
    
    // Check if substring [l, r] (0-based) is a palindrome in O(1)
    bool is_palindrome(int l, int r) {
        int len = r - l + 1;
        int center = l + r + 1;
        return p[center] >= len;
    }
};
