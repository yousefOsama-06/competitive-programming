/* ============ SEARCH: TURN "FIND" INTO "CHECK" ============

 BINARY SEARCH ON THE ANSWER. If feasible(x) is monotone, stop optimising and find the boundary.
   TRIGGER: "minimise the maximum", "maximise the minimum", "minimum time such that", "possible
     within k". Also: the answer is an integer in a huge range and no formula presents itself.
   TRAPS: (1) the check itself overflows - break early once a running count passes the target;
     (2) `hi` must be PROVABLY feasible - seed it with the trivial answer, not 1e9;
     (3) if the predicate is not monotone you get a plausible wrong answer and no crash.

 BINARY SEARCH THE VALUE, COUNT WITH A SECOND SEARCH. For "k-th smallest X" over a set you cannot
   materialise (pairs, products, distances, subarray sums): binary search x, count how many are
   <= x. The counting routine is the real problem.
   TRAP: use firstTrue(count(x) >= k) so the returned x actually occurs.

 PARAMETRIC SEARCH / LAGRANGIAN (Aliens, wqs). Replace "exactly k" by a penalty per item and
   binary search the penalty. Requires the optimum to be CONVEX in k. See 08 - DP/01/07.
   TRAP: verify convexity by brute-forcing f(k) for small n; and the inner DP must break ties
     toward the largest count or the final subtraction is off by an arbitrary amount.

 TERNARY SEARCH. Unimodal f: cut a third each step. Reals: fixed ~200 iterations (never an eps
   loop). Integers: while (hi - lo > 2), then scan the survivors.
   TRAP: PLATEAUS KILL IT. f(m1) == f(m2) on a flat region can discard the true optimum. For any
     discretely convex f use binary search on the difference instead:
       argmin = firstTrue(lo, hi-1, [&](ll x){ return f(x+1) >= f(x); });

 TWO POINTERS - and why it is legal. Legal precisely when validity is MONOTONE IN l for fixed r:
   if [l, r] is valid then so is [l', r] for every l' >= l.
   TRAP: "subarray with sum <= S" is two-pointerable with positive values and WRONG with negative
     ones. Every time you reach for two pointers, say out loud "shrinking can only help". If it
     cannot, stop.

 SWEEP LINE. Sort events by one coordinate, maintain a structure over the other.
   TRAP: event ordering at TIES is the whole problem - for closed intervals process opens first;
     for touching rectangles process closes first. Getting it wrong is off-by-exactly-the-ties.

 COORDINATE COMPRESSION. Only the order matters; map <= 2n values to 0..m-1.
   TRAP: if the problem cares about GAPS (lengths, areas, "how many integers strictly between"),
     keep the original values to weight each compressed cell. Insert both l and r+1.

 DIVIDE AND CONQUER ON TIME. Put each object's lifetime on a segment tree over the timeline, DFS
   it with a ROLLBACK structure, answer at the leaves. This is how you delete from a DSU.
   TRAP: rollback DSU must use union by size only - path compression makes undo impossible, so
     find is O(log n), not alpha. Budget accordingly.

 MEET IN THE MIDDLE. Split in half, enumerate 2^(n/2) per side, recombine by sorting + binary
   search / two pointers / hash map.
   TRIGGER: n <= 40..44 with a subset-flavoured question.
   TRAP: memory (2^20 ll = 8 MB per array); and in "mod m" variants you must also consider the
     wrap-around pair. Put the extra element of an odd n in the CHEAPER half.

 SQRT DECOMPOSITION OF THE PROBLEM (not the data structure). Two complementary bad algorithms,
   O(x) and O(n/x): threshold at sqrt and use each where it wins. Heavy vs light vertices, small
   vs large divisors, big vs small queries, "sum a_i <= 1e5 so at most sqrt distinct values".
   TRAP: tune the threshold to the ACTUAL costs; if updates are 10x cheaper the optimum is far
     from sqrt(n). Measure.

 OFFLINE REORDERING. If nothing forces you online, sort the queries: by right endpoint (+ BIT),
   by threshold (+ DSU), by Mo's ordering, by time. The highest-yield contest move for "q queries
   on a static array".
   TRIGGER: no "xor with the previous answer" marker; all queries readable up front.
   TRAP: store answers by the ORIGINAL query id and print in input order.

 SMALL TO LARGE. Always insert the smaller container into the larger: each element moves
   O(log n) times. SWAP THE HANDLES, never copy - copying the big one in is O(n^2) and looks
   identical.

 AMORTIZATION / POTENTIAL. An occasionally-expensive operation is cheap on average if each
   expensive step DESTROYS something expensive to create. Justifies monotonic stacks, ODT, beats.
   TRAP: ODT's amortization needs assign-heavy input; it is O(n) per op in the worst case.
     Monotonic-stack amortization dies if you ever re-push an index.

 ADD ONE ELEMENT AT A TIME. Solve for prefixes and maintain the answer incrementally. Turns "for
   every prefix, output X" from O(n^2) into O(n log n) and usually reveals the structure.
   TRAP: LIS - lower_bound gives strictly increasing, upper_bound gives non-decreasing. The tails
     array is NOT the subsequence; reconstruct with parent pointers.

 PARALLEL BINARY SEARCH. q independent binary searches over one shared timeline: bucket queries
   by their current mid, sweep the timeline once, resolve one bit for every query.
   TRAP: reset or rebuild the shared structure between LEVELS. Forgetting produces monotone-
     looking garbage.

 ============ PROOF TOOLS: THE TEN MINUTES THAT SAVE THE SUBMIT ============

 EXCHANGE ARGUMENT (for greedy). Take any optimum; show that swapping two ADJACENT elements
   toward your greedy order never makes it worse. For a sorting greedy this DERIVES the
   comparator: compare cost(a then b) against cost(b then a).
   TRAP: the derived comparator must be a STRICT WEAK ORDERING. Ties returning true, or an
     overflowing cross-multiplication, makes std::sort run off the array - an RE that looks like
     a logic bug. Test with all-equal input, and cross-multiply in __int128.

 INVARIANTS. A quantity unchanged by every legal operation. If start and target differ on it the
   answer is "impossible" - and you have a proof instead of a guess.
   TRAP: an invariant proves IMPOSSIBILITY only. You still owe a construction for the other side.

 MONOVARIANTS. A quantity that strictly decreases and is bounded: proves termination and bounds
   the number of operations. (Euclid: a+b strictly decreases.)
   TRAP: on reals a monovariant proves nothing - you need integrality or a floor.

 PARITY AND COLOURING. Colour the board so every legal move changes the colour counts in a fixed
   way, then count. Try in order: checkerboard, columns mod k, (i+j) mod 3, a weight function.
   EXAMPLE: the mutilated chessboard has 32/30 of the two colours, so no domino tiling exists.
   TRAP: if no colouring produces an imbalance in five minutes, the obstruction is not parity.

 THE EXTREMAL PRINCIPLE. Argue about the largest / smallest / deepest object - extremes have
   neighbours on one side only, which collapses the cases.
   EXAMPLE: two-BFS tree diameter is correct because the farthest vertex from ANY start is an
     endpoint of SOME diameter.
   TRAP: that theorem is TREE-ONLY. It is false on general graphs and on negative weights.

 PIGEONHOLE. n+1 objects in n boxes. In practice: n+1 prefix sums but only n residues, so some
   subarray sum is divisible by n.
   TRAP: the constructive version needs the FIRST repeat - store the earliest index per residue.

 SYMMETRY AND WLOG. If the problem is invariant under swapping two roles, assume an order and
   divide by the symmetry factor at the end.
   TRAP: handle the diagonal i == j separately: (total - diagonal)/2 + diagonal. Under a modulus,
     "divide by 2" means multiply by inv2.

 STRATEGY STEALING. In a symmetric game where an extra move never hurts, the first player wins or
   draws: if the second player had a winning strategy the first could steal it.
   TRAP: pure EXISTENCE. It gives you no winning move, and it fails under misere play.

 ADVERSARY ARGUMENTS / QUERY LOWER BOUNDS. q binary queries distinguish at most 2^q inputs, so
   q >= log2(#inputs); comparison sorting needs log2(n!) ~ n log n.
   TRAP: assume the interactor is ADAPTIVE unless told otherwise - it fixes nothing in advance,
     only stays consistent. Strategies relying on "the answer was fixed before I started" lose.

 ============ CONSTRAINTS ARE A MESSAGE FROM THE SETTER ============
   n <= 10        permutations n!, or backtracking with pruning
   n <= 20        2^n bitmask DP, SOS, 3^n submask partition
   n <= 24..28    2^n with a small factor
   n <= 40..44    MEET IN THE MIDDLE, 2^(n/2)
   n <= 100       O(n^3)/O(n^4): Floyd, Gauss, interval DP, min-cost flow, Hungarian
   n <= 500       O(n^3) or O(n^2 log n); flows on n^2 edges
   n <= 5000      O(n^2): LCS, tree knapsack, or O(n^2/64) with a bitset
   n <= 1e5, log^2 slack   segtree of segtrees, merge-sort tree, parallel binary search, HLD
   n <= 2e5       O(n log n). This is the default and it means "find the n log n"
   n <= 1e6       O(n) or O(n log n) with a SMALL constant: no map, no set, fast IO mandatory
   n,q <= 1e5, offline     Mo's (n sqrt q) is in budget, ~3e7 pointer moves
   value <= 1e9, n small   coordinate compression, or binary search the value
   a_i <= 1e18    Miller-Rabin + Pollard rho, __int128, no sieve
   sum n <= 2e5 over tests per-test O(n log n); NEVER memset a MAXN global per test
   t <= 1e4, no sum bound  the intended solution is O(1) or O(log) per test
   mod 998244353  NTT / polynomial methods intended
   mod 1e9+7      counting DP / combinatorics, NOT NTT
   real output, 1e-6       geometry, ternary/binary search on reals, or expectation
   <= 20-30 oracle queries binary search (2^20 > 1e6) or bit-by-bit determination
   TL 3-5 s at n <= 1e5    the intended solution really is n sqrt n or n log^2 n
   ML 32-64 MB    offline/streaming, rolling DP arrays, no persistence

 THE /64 ARGUMENT. An "impossible" O(n^2) or O(n^3) over BOOLEANS becomes n^2/64 - fine up to
   ~1e10 raw ops. Reachability, subset sum, LCS, string matching, set intersection.
   TRAP: memory. n bitsets of n bits is n^2/8 bytes: at n = 5e4 that is 312 MB. Process in
     blocks of ~1000 and reuse. bitset needs a compile-time size; a vector<u64> hand-roll is
     ~30% slower and always available.

 THE "ANSWER IS SMALL" ARGUMENT. Prove the answer is <= a tiny constant, then brute force over
   it. Each operation halves something / clears a bit / merges two components; "OR of a range
   equals its SUM" means the values are bit-disjoint so at most 30 are nonzero; sum a_i <= 1e5
   means at most ~632 distinct values; a_i <= 1e6 means at most 7 distinct primes, 240 divisors;
   gcd over an expanding range takes only O(log V) distinct values.
   TRAP: prove the bound, do not observe it on samples, and handle "impossible" explicitly.
============================================================================== */
