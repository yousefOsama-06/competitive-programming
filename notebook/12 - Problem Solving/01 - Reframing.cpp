/* ================= REFRAMING: CHANGE THE PROBLEM, NOT THE EFFORT =================
 This chapter has no algorithms - it has the moves you make BEFORE you know which template to
 paste. Stuck for 10 minutes? Do not re-read the statement a fourth time. Walk the six questions,
 then walk the TRIGGERS below and try each against your problem.

 THE SIX QUESTIONS
   1. What object am I counting / optimising over? Say it in one sentence with no input format.
   2. What is INVARIANT under the allowed operation?
   3. What does the ANSWER look like (a prefix? one of the inputs? a threshold? always <= 2?)
   4. Can I CHECK a candidate faster than I can find one?          -> binary search the answer
   5. What do the constraints forbid, and what does the leftover budget exactly equal?
   6. What would the brute force print for n = 1..8?

 --- REVERSE THE PROCESS ---
   Run time backwards: deletions become insertions, splits become merges. Every structure is
   add-friendly and delete-hostile (DSU above all), so reversing turns impossible into trivial.
   TRIGGER: things are removed one at a time and you must answer after each removal.
   EXAMPLE: CF 722C Destroying Array - process destructions in reverse, union with restored
     neighbours.  Also CSES 1163 Traffic Lights.
   TRAP: emit answers in reverse too, and check the off-by-one at both ends with n = 1.

 --- WORK BACKWARDS FROM THE GOAL ---
   Define the answer at terminal states and propagate backwards. Standard for games and for
   "what must have been true one step earlier".
   TRIGGER: few terminal states, huge forward branching, small backward branching.
   TRAP: on a CYCLIC game graph this is NOT a plain DP - you need retrograde BFS with out-degree
     counters (a position is losing only once ALL successors are known winning). See 10 - Game
     Theory. Writing win[v] = OR over children on a cyclic graph silently mislabels draws.

 --- COMPLEMENTARY COUNTING ---
   Count the bad ones and subtract; count P(X <= x) instead of P(X = x).
   TRIGGER: "at least one", "there exists", "not all equal", "some pair".
   EXAMPLE: E[max] = sum over x of P(max >= x), with P(max <= x) = (x/n)^k.
   TRAP: the complement is easy for AT MOST / AT LEAST, not for EXACTLY. Getting from one to the
     other is binomial inversion: exactly_k = sum_{j>=k} (-1)^(j-k) C(j,k) atleast_j.

 --- THE CONTRIBUTION TECHNIQUE ---
   Instead of "for each subarray, compute f", ask "for each element, in how many objects does it
   contribute, and how much". Turns O(n^2) enumeration into O(n).
   TRIGGER: "sum over all subarrays / pairs / subsets of [min | max | gcd | #distinct | bottleneck]".
   EXAMPLE: sum over subarrays of the minimum -> monotonic stack gives each element's span.
     Sum over pairs of the bottleneck -> Kruskal, each merge contributes w * |A| * |B| pairs.
   TRAP: ties. With equal values the span must be strict on ONE side and non-strict on the other,
     or every duplicated minimum is counted twice. Decide which side BEFORE coding.

 --- SWAP THE ORDER OF SUMMATION ---
   sum_i sum_{d | i} f(d) = sum_d f(d) * floor(n/d). Move the outer loop to the variable whose
   inner count has a closed form.
   TRIGGER: a double sum whose inner range depends on the outer index; divisors, multiples,
     ancestors, intervals containing a point.
   EXAMPLE: sum of sigma(i) for i <= n = sum_d d * floor(n/d), then divisor blocks -> O(sqrt n).

 --- LINEARITY OF EXPECTATION ---
   E[sum X_i] = sum E[X_i], with NO independence needed. Decompose into indicators.
   TRIGGER: "expected number of ...", or an expectation over a process you cannot untangle.
   TRAP: linearity does not survive max, min, or division. E[max] != max E, E[1/X] != 1/E[X].
     For an expected maximum use the complementary-counting tail sum above.

 --- FIX THE EXTREMUM ---
   Add an artificial outer loop "for each choice of the maximum / the leftmost / the last move".
   Everything else becomes bounded relative to the fixed thing.
   TRIGGER: the statement mentions max or min over a range; a Cartesian-tree recursion is available.
   EXAMPLE: CF 1156E - fix the position of the maximum, then enumerate the SMALLER side.
   TRAP: always iterate the smaller side (small-to-large, O(n log n) total). Iterating the left
     side unconditionally is O(n^2) on a sorted permutation - and it passes the samples.

 --- DUALIZE ---
   max-flow = min-cut; max bipartite matching = min vertex cover (Konig); min path cover of a DAG
   = n - matching; longest antichain = min chain cover (Dilworth); "a perfect matching exists" =
   "no Hall violator".
   TRIGGER: the statement asks for a MINIMUM removal / blocking set / number of chains - that is
     almost always the dual of a maximum you can compute.
   TRAP: Konig gives the SIZE for free, but recovering the cover needs the alternating-BFS
     construction (L \ Z) union (R and Z). And Konig is BIPARTITE ONLY.

 --- MODEL IT AS A GRAPH ---
   Objects -> nodes, relations -> edges. States can be tuples: (cell, fuel), (vertex, parity),
   (position, mask).
   TRIGGER: "can I get from A to B", "minimum number of operations", "are these constraints
     consistent", "each x points at exactly one y" (functional graph).
   TRAP: compute |V| * |E| explicitly before coding. In a layered graph the answer is usually
     "any layer", not layer 0.

 --- MODEL IT AS INTERVALS / A TIMELINE ---
   Recast objects as [start, end] and use a sweep, greedy-by-endpoint, or a difference array.
   Tree subtrees become intervals via the Euler tour.
   TRIGGER: "from time a to time b", "covers l..r", "is available during", subtree aggregates.
   TRAP: decide ONCE whether [1,3] and [3,5] overlap, and encode it in exactly one place.

 --- MODEL IT AS A POLYNOMIAL ---
   "For every possible sum, count the ways" is a convolution. Multiplying GFs = combining
   independent choices; x^k = "used k of the budget".
   TRIGGER: modulus 998244353 (NTT is intended); independent parts whose sizes add.
   TRAP: convolving an array with itself double-counts i != j and mis-counts i = j.

 --- MODEL IT AS LINEAR ALGEBRA OVER GF(2) ---
   Each element is a bit-vector; "can I make X" = "is X in the span"; the number of subsets giving
   a representable Y is 2^(n - rank).
   TRIGGER: XOR anywhere; "product is a perfect square" (exponent parity vector over primes);
     switch-toggling puzzles; "each constraint satisfied an even number of times".
   TRAP: 2^(n-rank) counts solutions only if the system is CONSISTENT - check first. And subtract
     the empty subset when the problem wants a non-empty one.

 --- MODEL IT AS THE SUBSET LATTICE ---
   Bitmask DP plus zeta/Mobius (SOS) to aggregate over submasks in O(2^n n) instead of O(3^n).
   TRIGGER: n <= 20 with a "set of chosen items" state, or a <= 22-bit value domain with queries
     "over all y contained in x".
   TRAP: the bit loop goes OUTSIDE the mask loop. Swapping them gives a wrong-but-plausible array.
     Submask enumeration must be  for (s = m; ; s = (s-1) & m) { ...; if (!s) break; }.

 --- THINK ABOUT WHAT THE ANSWER LOOKS LIKE ---
   Characterise the optimum's SHAPE first: it is a prefix; it is one of the inputs; it is at most
   2; it has a threshold structure. Then search only that shape.
   TRIGGER: the output is one small number regardless of n; the problem screams "constructive".
   TRAP: "the answer is at most k" needs a matching lower-bound example, or you print k-1.

 --- CHANGE OF VARIABLES ---
   Difference array (range add -> two point updates; "make all equal" -> zero the differences),
   prefix sums (subarray -> pair of points), Chebyshev <-> Manhattan via (x+y, x-y), logs
   (products -> sums).
   TRIGGER: operations act on ranges; the objective is max(|dx|,|dy|) or |dx|+|dy|.
   TRAP: C++ % is negative for negative operands - use ((x % n) + n) % n. Seed the empty prefix.

 --- PER-BIT / PER-PRIME INDEPENDENCE ---
   If the objective decomposes across bits (XOR/AND/OR sums, popcount weights) or across primes
   (gcd/lcm exponent-wise), solve 30 tiny problems and recombine.
   EXAMPLE: sum over pairs of (a_i XOR a_j) = sum_b 2^b * cnt0_b * cnt1_b.
   TRAP: bit-independence holds for SUMS, not for COMPARISONS. "Count pairs with a_i^a_j < k" is
     not separable - that needs a binary trie walked high bit first.

 --- REDUCE TO SOMETHING YOU ALREADY HAVE ---
   "minimum adjacent swaps to sort" = inversions; "maximum pairwise-compatible set" = matching;
   "minimise the maximum edge on a path" = MST bottleneck / Kruskal reconstruction tree; "longest
   chain of pairs" = LIS; "pairwise gap constraints" = difference constraints + Bellman-Ford;
   "each person points at one other" = functional graph.
   TRIGGER: you can state the problem without the story. Do that, then match it.
   TRAP: the disguise is usually ALMOST the classic - one extra constraint breaks the reduction.
     Check the reduction on the samples by hand before trusting it.
================================================================================= */
