// SUBSEQUENCE AUTOMATON (a.k.a. the "next occurrence" table). O(n * A) build, O(1) transition.
// INDEX CONVENTION: nxt[i][c] = the SMALLEST j >= i with s[j] == c, or n if there is none.
// There are n + 1 rows, i = 0..n; row i is the state "we have consumed s[0..i-1]".
// After matching a character at j you move to row j + 1 - never to row j.
// WHEN: everything of the form "is t a subsequence of s", counting or ranking DISTINCT
// subsequences, and shortest strings that are NOT subsequences. Build it from the RIGHT.
vector<array<int, 26>> subseqAutomaton(const string& s) {
    int n = s.size();
    vector<array<int, 26>> nxt(n + 1);
    for (int c = 0; c < 26; c++) nxt[n][c] = n;
    for (int i = n - 1; i >= 0; i--) nxt[i] = nxt[i + 1], nxt[i][s[i] - 'a'] = i;
    return nxt;
}
// --- IS t A SUBSEQUENCE OF s? O(|t|) per query after the build (the reason to build at all:
// the two-pointer scan is O(|s| + |t|) per query, which loses the moment there are many queries).
bool isSubsequence(const string& t, const vector<array<int, 26>>& nxt) {
    int n = nxt.size() - 1, i = 0;
    for (char ch : t) {
        int j = nxt[i][ch - 'a'];
        if (j == n) return false;
        i = j + 1;
    }
    return true;
}
// --- NUMBER OF DISTINCT NON-EMPTY SUBSEQUENCES, O(n * A). Each distinct subsequence is read by
// exactly ONE path in the automaton (always take the earliest occurrence), so just count paths.
// f[i] = number of distinct non-empty subsequences of s[i..], counted as "first letter is c".
ll distinctSubsequences(const string& s, const vector<array<int, 26>>& nxt, ll mod = MOD) {
    int n = s.size();
    vector<ll> f(n + 2, 0);
    for (int i = n; i >= 0; i--) {
        f[i] = 0;
        for (int c = 0; c < 26; c++) if (nxt[i][c] < n) f[i] = (f[i] + f[nxt[i][c] + 1] + 1) % mod;
    }
    return f[0];
}
// --- K-TH DISTINCT SUBSEQUENCE in lexicographic order (1-indexed, non-empty). Use the UNCAPPED
// counts f[] above (saturating at k, not modular - a modular count cannot be compared).
string kthSubsequence(const string& s, const vector<array<int, 26>>& nxt, ll k) {
    int n = s.size();
    vector<ll> f(n + 2, 0);                                    // f[i] saturated at 2e18
    for (int i = n; i >= 0; i--)
        for (int c = 0; c < 26; c++) if (nxt[i][c] < n)
            f[i] = min((ll)2e18, f[i] + f[nxt[i][c] + 1] + 1);
    if (k > f[0]) return "";                                   // fewer than k subsequences
    string res;
    int i = 0;
    while (k > 0) {
        for (int c = 0; c < 26; c++) {
            int j = nxt[i][c];
            if (j == n) continue;
            ll cur = f[j + 1] + 1;                             // subsequences starting with c
            if (k <= cur) { res += char('a' + c), i = j + 1, k--; break; }
            k -= cur;
        }
    }
    return res;
}
// --- SHORTEST STRING OVER THE ALPHABET THAT IS NOT A SUBSEQUENCE of s, O(n * A).
// g[i] = that length for s[i..]; a missing letter gives 1, otherwise pay one letter and recurse.
int shortestNonSubsequence(const string& s, const vector<array<int, 26>>& nxt, int A = 26) {
    int n = s.size();
    vector<int> g(n + 2, 0);
    for (int i = n; i >= 0; i--) {
        g[i] = inf;
        for (int c = 0; c < A; c++)
            g[i] = min(g[i], nxt[i][c] == n ? 1 : 1 + g[nxt[i][c] + 1]);
    }
    return g[0];                                               // reconstruct by replaying the min
}
/* VARIANTS
 DISTINCT COMMON SUBSEQUENCES of s and t: run both automata in lockstep,
   f[i][j] = sum over c of (f[nxt1[i][c] + 1][nxt2[j][c] + 1] + 1), O(n m A).
 COUNTING SUBSEQUENCES EQUAL TO t (occurrences, not distinct) is the other classic DP,
   dp[i][j] = dp[i-1][j] + (s[i] == t[j]) * dp[i-1][j-1], O(n m) - the automaton does not help.
 DISTINCT SUBSEQUENCES OF LENGTH EXACTLY k: add the length as a dimension, O(n k A).
 LARGE ALPHABET: store nxt as a vector of sorted position lists per character and binary search;
   that is O(n + A) memory and O(log n) per transition, which is the right trade when A is 1e5.
 DO NOT CONFUSE with the SUFFIX automaton (04 - Suffix Structures/02), which recognises
   SUBSTRINGS. This one recognises SUBSEQUENCES and is a DAG with no suffix links. */
