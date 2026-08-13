// BIT-PARALLEL LCS - the length of the longest common subsequence of a and b in O(n m / 64),
// where n = |a|. The classic DP is O(n m) and dies at n = m = 5e4; this survives n = m = 1e5.
// IDEA: the LCS row is monotone and its DIFFERENCE vector is a bit string over the positions of
// a (bit i = "the LCS value increases at position i of a"). One row update is then a handful of
// word operations, of which the carry-propagating ADD is what does the actual work.
// PRECONDITION: none - this is exact, for arbitrary alphabets (map characters to 0..A-1).
// The answer is the POPCOUNT of the final vector; the bit positions themselves are NOT an LCS.
struct BitLCS {
    int n, W;                                                  // W = number of 64-bit words
    vector<vector<unsigned long long>> match;                  // match[c] = positions of c in a
    BitLCS(const string& a, int A = 26, char base = 'a') {
        n = a.size(), W = n / 64 + 1;
        match.assign(A, vector<unsigned long long>(W, 0));
        for (int i = 0; i < n; i++) match[a[i] - base][i >> 6] |= 1ULL << (i & 63);
    }
    int lcs(const string& b, char base = 'a') {
        vector<unsigned long long> row(W, 0), x(W);
        for (char ch : b) {
            const auto& mc = match[ch - base];
            for (int i = 0; i < W; i++) x[i] = row[i] | mc[i];  // x = row | match[c]
            unsigned long long carry = 1;                       // row = (row << 1) | 1
            for (int i = 0; i < W; i++) {
                unsigned long long nc = row[i] >> 63;
                row[i] = (row[i] << 1) | carry;
                carry = nc;
            }
            unsigned long long borrow = 0;                      // row = x - row
            for (int i = 0; i < W; i++) {
                unsigned long long d = x[i] - row[i] - borrow;
                borrow = (x[i] < row[i] + borrow) || (borrow && row[i] == ~0ULL);
                row[i] = d;
            }
            for (int i = 0; i < W; i++) row[i] = x[i] & ~row[i];// row = x & ~(x - row)
        }
        int res = 0;
        for (int i = 0; i < W; i++) res += __builtin_popcountll(row[i]);
        return res;
    }
};
// USAGE: build once on the SHORTER string (that is the one that costs memory and words),
//        then call lcs() for every other string - the build is reused.
//   BitLCS L(a); int k = L.lcs(b);
/* NOTES
 WHY POPCOUNT: after processing all of b, bit i of `row` is set iff LCS(a[0..i], b) is strictly
   greater than LCS(a[0..i-1], b) - the difference vector of a monotone row of the DP table.
 EDIT DISTANCE has the same treatment (Myers, 07 - Sequences/02) with two vectors instead of one,
   because its differences live in {-1, 0, +1} while the LCS row only ever steps by 0 or 1.
 LCS ITSELF (the subsequence, not its length) still needs the O(n m) table or Hirschberg
   (08 - DP/04 - Classics) - bit-parallelism gives you the LENGTH only.
 LCS OF PREFIXES: read the popcount after each character of b to get LCS(a, b[0..j]) for all j at
   no extra cost. That is what makes this the practical answer to "all-prefix" LCS queries.
 SPARSE ALTERNATIVE: when the number of matching pairs r is small, the Hunt-Szymanski algorithm
   is O(r log n) - better on near-distinct alphabets, worse on binary strings.
 ALPHABET: `match` is A * W words. For a large alphabet, map to the distinct characters of a
   first (everything else can never match and can be dropped from b). */
