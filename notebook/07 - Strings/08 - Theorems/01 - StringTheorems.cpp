/* STRING THEOREMS - the facts that turn an O(n^2) idea into an O(n) one. No code: these are the
   statements you need to REMEMBER, because none of them is derivable at the table.

PERIODICITY
 BORDER-PERIOD DUALITY: p is a period of s (s[i] = s[i+p] whenever both exist) IFF n - p is the
   length of a border. So the periods and the borders are the same set read backwards, and the
   prefix function gives all of them (07 - Strings/01/05).
 FINE AND WILF: if p and q are both periods of s and p + q - gcd(p, q) <= n, then gcd(p, q) is
   also a period. The bound is TIGHT (Fibonacci words). Consequences:
   - the borders of any string form O(log n) ARITHMETIC PROGRESSIONS (each progression is one
     "period class"); this is what makes palindromic-factorisation and border-sum DPs O(n log n)
     instead of O(n^2) - see 05 - Palindromes/04.
   - two occurrences of a pattern P closer than |P| / 2 force P to be periodic; hence in any text
     window of length 2|P| a non-periodic pattern occurs at most twice (the basis of constant-space
     matching and of "the occurrences form O(log) arithmetic progressions").
 CRITICAL FACTORISATION: every string s with period p has a position i < p such that the local
   period at i equals the global period. Splitting there makes the two halves comparable
   independently - the basis of the two-way (Crochemore-Perrin) O(n) constant-space matcher.
 PRIMITIVITY: s is a power t^k with k >= 2 IFF n % (n - pi[n-1]) == 0 and n - pi[n-1] < n. A
   primitive word has exactly n distinct rotations; a non-primitive one has n / k.
 THE CYCLE LEMMA / rotations: s and t are rotations of each other IFF t is a substring of s + s
   IFF minRotation(s) == minRotation(t) (06 - Rotations/01).

REPETITIONS
 RUNS THEOREM (Bannai et al. 2015): a string of length n has FEWER THAN n runs (maximal
   repetitions), and the sum of their exponents is < 3n. Both bounds are what make "enumerate all
   repetitions" linear rather than quadratic. Construction: 06 - Rotations/02.
 THREE SQUARES LEMMA: if three squares start at the same position and u is a prefix of v is a
   prefix of w, then |u| + |v| <= |w|. Hence at most O(log n) squares start at any position, and a
   string of length n contains fewer than n DISTINCT squares.
 CHEN-FOX-LYNDON: every string factors uniquely as w1 >= w2 >= ... >= wk with each wi Lyndon, and
   Duval computes it in O(n) with O(1) memory. The last factor is the smallest suffix; the first
   is the longest Lyndon prefix. Lyndon roots are the pivot of the runs algorithm.
 A LYNDON WORD is strictly smaller than all of its proper suffixes; equivalently it is the unique
   minimum among its own rotations, and it is primitive. There are (1/n) * sum over d | n of
   mu(d) * A^(n/d) Lyndon words of length exactly n over an alphabet of size A (Witt's formula).

SUBSTRINGS AND SUFFIX STRUCTURES
 ENDPOS EQUIVALENCE (suffix automaton): two substrings are equivalent when they occur at exactly
   the same set of END positions. The classes form a TREE under "is a suffix of" (the suffix-link
   tree), there are at most 2n - 1 states and 3n - 4 transitions, and one class contains exactly
   the strings of lengths (len[link[v]], len[v]] - so a class contributes len[v] - len[link[v]]
   distinct substrings. Summing that is the count of distinct substrings.
 THE SUFFIX-LINK TREE of a SAM built on the REVERSED string IS the suffix tree of the string.
 A STRING OF LENGTH n HAS AT MOST n DISTINCT PALINDROMIC SUBSTRINGS (eertree node count), and the
   palindromic suffixes of any prefix again form O(log n) arithmetic progressions.
 DISTINCT SUBSTRINGS = n(n+1)/2 - sum of the LCP array; adding one character to the end changes
   that by (new length) - (longest suffix that already occurred), which is the SAM's len[last].

MATCHING BOUNDS
 KNUTH-MORRIS-PRATT does at most 2n character comparisons; Boyer-Moore is sublinear on average
   but O(nm) in the worst case without Galil's rule; Z and prefix functions are exactly linear.
 A PATTERN WITH WILDCARDS is not solvable by automata in linear time; the FFT convolution
   sum (p_i - t_j)^2 p_i t_j is the standard O(n log n) route (01 - String Matching/04).
 EDIT DISTANCE cannot be computed in O(n^(2-eps)) unless SETH fails - so the O(n m / 64)
   bit-parallel version (07 - Sequences/02) is essentially the last word.
 LCS OF k STRINGS is NP-hard in k; for k = 2 it is O(n m / 64) and no truly subquadratic
   algorithm is known (same SETH barrier).

COUNTING
 NUMBER OF DISTINCT NECKLACES (rotation classes) of length n over A letters:
   (1/n) * sum over d | n of phi(d) * A^(n/d)                                (Burnside)
 NUMBER OF BRACELETS adds the reflections; see 05 - Combinatorics/02 - BurnsidePolya.cpp.
 DE BRUIJN SEQUENCES B(A, n): there are (A!)^(A^(n-1)) / A^n distinct ones (07 - Sequences/01). */
