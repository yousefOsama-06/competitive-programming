// SUFFIX ARRAY + LCP + sparse table for O(1) LCE. Build is O(n log n) (prefix doubling with a radix
// sort), Kasai gives the LCP array in O(n). Everything you can ask about substrings reduces to a
// range on this array - see 03 - SuffixArrayApplications.cpp.
struct SuffixArray {
    string S;
    // sa is the suffix array with the empty suffix being sa[0]
    // lcp[i] holds the lcp between sa[i], sa[i - 1]
    vector<int> logs, sa, lcp, rank;
    vector<vector<int>> table;

    SuffixArray() {};

    SuffixArray(string &s, int lim = 256) {
        S = s;
        int n = s.size() + 1, k = 0, a, b;
        vector<int> c(s.begin(), s.end() + 1), tmp(n), frq(max(n, lim));
        c.back() = 0; //0 is less than any character
        sa = lcp = rank = tmp, iota(sa.begin(), sa.end(), 0);
        for (int j = 0, p = 0; p < n; j = max(1, j * 2), lim = p) {
            p = j, iota(tmp.begin(), tmp.end(), n - j);
            for (int i = 0; i < n; i++) {
                if (sa[i] >= j)
                    tmp[p++] = sa[i] - j;
            }

            fill(frq.begin(), frq.end(), 0);

            for (int i = 0; i < n; i++) frq[c[i]]++;
            for (int i = 1; i < lim; i++) frq[i] += frq[i - 1];
            for (int i = n; i--;) sa[--frq[c[tmp[i]]]] = tmp[i];

            swap(c, tmp), p = 1, c[sa[0]] = 0;
            for (int i = 1; i < n; i++)
                a = sa[i - 1], b = sa[i], c[b] = (tmp[a] == tmp[b] && tmp[a + j] == tmp[b + j]) ? p - 1 : p++;
        }

        for (int i = 1; i < n; i++) rank[sa[i]] = i;
        for (int i = 0, j; i < n - 1; lcp[rank[i++]] = k)
            for (k &&k--, j = sa[rank[i] - 1];
                    s[i + k] == s[j + k];
        k++);
    }

    void preLcp() {
        int n = S.size() + 1;
        logs = vector<int>(n + 5);
        for (int i = 2; i < n + 5; ++i) {
            logs[i] = logs[i / 2] + 1;
        }
        // level-major: sized by logs[n], not a hard-coded 20 (which is OUT OF BOUNDS at
        // |s| >= 2^20 - and Codeforces routinely gives |s| up to 1e6), and no dead columns.
        table = vector<vector<int>>(logs[n] + 1, vector<int>(n));
        for (int i = 0; i < n; ++i) table[0][i] = lcp[i];
        for (int j = 1; j <= logs[n]; ++j)
            for (int i = 0; i + (1 << j) <= n; ++i)
                table[j][i] = min(table[j - 1][i], table[j - 1][i + (1 << (j - 1))]);
    }

    // ARGUMENTS ARE SUFFIX-ARRAY INDICES, not string positions. For two string positions
    // p and q use  lce(p, q)  below, which is what every application actually wants.
    int queryLcp(int i, int j) {
        if (i == j) return (int)S.size() - sa[i];
        if (i > j) swap(i, j);
        i++;
        int len = logs[j - i + 1];
        return min(table[len][i], table[len][j - (1 << len) + 1]);
    }
    int lce(int p, int q) { return queryLcp(rank[p], rank[q]); }   // longest common extension
};
