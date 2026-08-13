// Needs: 07 - Strings/04 - Suffix Structures/05 - SuffixArraySAIS.cpp   (saIs, kasai)
// ALL RUNS (maximal repetitions) of s in O(n log n) - O(n) if the LCE structure is linear.
// A RUN is a triple (l, r, p): s[l..r] has period p, r - l + 1 >= 2p, and it cannot be extended
// left or right with the same period. Every tandem repeat, every square, every "x repeated k
// times" substring lives inside exactly one run, so runs are THE compact encoding of all
// repetitions - and by the RUNS THEOREM there are FEWER THAN n of them, with total exponent
// sum(r - l + 1)/p < 3n. Enumerating squares directly is quadratic; enumerating runs is not.
// WHEN: count/locate squares or k-th powers, "longest substring that is a repetition", periodic
// structure of a string, and as the input to Lempel-Ziv style factorisations.
// INDEX CONVENTION: 0-based, l and r INCLUSIVE, p is the SMALLEST period of s[l..r].
// HOW (Bannai et al.): a run's period is a LYNDON ROOT - for one of the two alphabet orders, the
// run contains a Lyndon word of length p starting at some i. So take every i, let l[i] be the
// longest Lyndon prefix of the suffix i (that is "next smaller suffix" minus i), and try to
// extend the pair (i, i + l[i]) both ways with LCE. Do it for BOTH orders (< and its reverse).
struct RunsFinder {
    struct LceSA {                                             // suffix array + O(1) LCE
        int n, LG;
        vector<int> sa, rnk, lcp, lg;
        vector<vector<int>> sp;
        void build(const string& s) {
            n = s.size(), sa = suffixArray(s), lcp = kasai(s, sa), rnk.assign(n, 0);
            for (int i = 0; i < n; i++) rnk[sa[i]] = i;
            lg.assign(n + 1, 0);
            for (int i = 2; i <= n; i++) lg[i] = lg[i / 2] + 1;
            LG = lg[n] + 1, sp.assign(LG, vector<int>(n, 0)), sp[0] = lcp;
            for (int k = 1; k < LG; k++)
                for (int i = 0; i + (1 << k) <= n; i++)
                    sp[k][i] = min(sp[k - 1][i], sp[k - 1][i + (1 << (k - 1))]);
        }
        int lce(int i, int j) {                                // lcp of the suffixes i and j
            if (i >= n || j >= n) return 0;
            if (i == j) return n - i;
            int l = rnk[i], r = rnk[j];
            if (l > r) swap(l, r);
            int k = lg[r - l];
            return min(sp[k][l + 1], sp[k][r - (1 << k) + 1]);
        }
    };
    int n;
    LceSA F, B;                                                // forward, and on the reversal
    vector<int> crnk;                                          // suffix ranks under the FLIPPED
    RunsFinder(const string& s) {                              // alphabet order
        n = s.size();
        string r(s.rbegin(), s.rend()), c = s;
        for (char& ch : c) ch = (char)(255 - (unsigned char)ch);
        F.build(s), B.build(r);
        vector<int> csa = suffixArray(c);
        crnk.assign(n, 0);
        for (int i = 0; i < n; i++) crnk[csa[i]] = i;
        // NOT the reverse of F.rnk: flipping the alphabet flips every comparison EXCEPT the
        // prefix rule ("abc" < "abcd" in both orders), so the second order needs its own array.
    }
    int lceF(int i, int j) { return F.lce(i, j); }              // common prefix of s[i..], s[j..]
    int lceB(int i, int j) {                                   // common suffix of s[..i], s[..j]
        if (i < 0 || j < 0) return 0;
        return B.lce(n - 1 - i, n - 1 - j);
    }
    vector<array<int, 3>> runs() {
        vector<array<int, 3>> res;
        for (int ord = 0; ord < 2; ord++) {                    // both alphabet orders
            const vector<int>& rk = ord ? crnk : F.rnk;
            vector<int> nxt(n, n), st;                         // next suffix smaller in this order
            for (int i = n - 1; i >= 0; i--) {
                while (!st.empty() && rk[st.back()] > rk[i]) st.pop_back();
                nxt[i] = st.empty() ? n : st.back();
                st.push_back(i);
            }
            for (int i = 0; i < n; i++) {
                int p = nxt[i] - i, j = i + p;                 // p = Lyndon root length
                if (j > n) continue;
                int b = j < n ? lceF(i, j) : 0, a = lceB(i - 1, j - 1);
                if (a + b < p || p == 0) continue;
                int l = i - a, r = j + b - 1;
                if (r - l + 1 >= 2 * p) res.push_back({l, r, p});
            }
        }
        sort(res.begin(), res.end());                          // (l, r, p) with p increasing
        vector<array<int, 3>> out;                             // keep the SMALLEST p per (l, r)
        for (auto& x : res)
            if (out.empty() || out.back()[0] != x[0] || out.back()[1] != x[1]) out.push_back(x);
        return out;                                            // < n runs
    }
};
/* WHAT TO DO WITH THE RUNS
 NUMBER OF SQUARES (as occurrences, x x with |x| = q): a run (l, r, p) with length L = r - l + 1
   contains, for every multiple q of ... no: for every q with p <= q and 2q <= L AND q a multiple
   of p, exactly L - 2q + 1 occurrences of a square of half-length q. Summing over runs is O(n)
   amortised by the runs theorem (total exponent < 3n).
 NUMBER OF DISTINCT SQUARES is at most n (the "three squares lemma"); get them by taking, per
   run, the primitively rooted squares and deduplicating with the run's root.
 K-TH POWERS: a run (l, r, p) contains a k-th power iff L >= k p, and then L - k p + 1 of them.
 LONGEST REPETITION = the run maximising L / p, read straight off the list.
 MAIN-LORENTZ is the alternative: divide & conquer with z-functions, O(n log n), ~40 lines, and
   it reports tandem repeats as O(n log n) grouped triples instead of < n runs. Prefer runs.
 PRIMITIVITY of s[l..r] (is it a proper power?) is a prefix-function question, not a runs one:
   see 01 - String Matching/05, "n % (n - pi) == 0".
 TESTING: on a^n there is exactly ONE run (0, n-1, 1); on "abababa" exactly one, (0, 6, 2). If
   you get duplicates with different p, the Lyndon-root loop ran with only one order. */
