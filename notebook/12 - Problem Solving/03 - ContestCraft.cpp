/* ============ CONSTRUCTIVE, INTERACTIVE, RANDOMISED, EMPIRICAL ============

 BUILD GREEDILY AND PROVE AS YOU GO. Emit the answer one decision at a time, always taking the
   best local choice that KEEPS THE REMAINDER FEASIBLE - that check is the whole problem.
   TRAP: for LEXICOGRAPHICALLY SMALLEST, greedy alone is wrong. You need "pick the smallest
     candidate such that the rest is still completable", and the completability test must be
     exact. If you cannot write that test, you cannot do lexicographic greedy.

 BUILD RECURSIVELY. Solve n from n/2 (or n-1, or four quadrants) and glue with a fixed pattern.
   TRIGGER: n is a power of two, or the object is self-similar (Gray code, Hanoi, de Bruijn).
   TRAP: base cases are usually n = 1 AND n = 2. Recursive output of 2^n lines needs buffered
     output - endl will TLE.

 EXTEND FROM A KNOWN SMALL CASE. Brute force n = 1..8, find the pattern of CONSTRUCTION (not just
   the count), then prove the inductive step.
   TRAP: the small cases are exactly where the pattern fails. Find the smallest n where it first
     works and special-case everything below. Nearly every constructive WA is a small-n exception.

 CERTIFICATE CONSTRUCTION (prove the NO). Output the witness: the negative cycle, the odd cycle,
   the Hall violator, the parity mismatch. Producing it also validates your YES logic.
   TRAP: after n Bellman-Ford rounds you have a vertex AFFECTED BY a negative cycle, not one ON
     it. Walk n parent pointers back first, then follow parents until a vertex repeats.

 INTERACTIVE: BUDGET FIRST, ALGORITHM SECOND. Compare 2^Q against the number of possibilities.
   Q ~ log n -> binary search;  Q ~ n -> one query per element;  Q ~ n log n -> sorting/merging.
   TRAPS: (1) FLUSH after every query (endl, or fflush) or you get "idleness limit exceeded",
     which looks nothing like the real bug; (2) count your queries locally and assert the limit;
     (3) assume the interactor is ADAPTIVE - it only has to stay consistent with past answers.

 RANDOM SHUFFLE to destroy adversarial order: Kuhn's matching, quickselect, incremental convex
   hull, minimum enclosing circle (which is O(n) only after a shuffle), KD-tree splits.
   TRAP: seed from the clock (mt19937_64 with steady_clock), never srand(time(0)) + rand().
     unordered_map needs the splitmix64 custom hash or it is O(n^2) on a crafted test.

 RANDOM SAMPLING. If a target occupies more than a 1/c fraction, k samples miss it with
   probability (1 - 1/c)^k.
   TRIGGER: "majority element of a range", "a value shared by at least half".
   TRAP: sampling gives a CANDIDATE - you must verify it exactly. And the failure probability
     compounds over q queries: with q = 1e5 you need per-query failure below 1e-12, so ~40
     samples, not 20.

 RANDOM WEIGHTS FOR UNIQUENESS (Zobrist / XOR hashing). Give each value a random 64-bit label;
   then multiset equality, "every value appears a multiple of k times", and "this subtree shape
   occurred before" become single-number comparisons.
   TRAP: one random assignment has a real false-positive rate - repeat ~30 times with FRESH
     randoms. A fixed small set of randoms is breakable by Gaussian elimination.

 HASHING AS O(1) EQUALITY. Polynomial hashing for substrings, 2D for submatrices.
   TRAP: RANDOM base, and either double-mod or one 61-bit Mersenne mod. Fixed base 31 with mod
     1e9+7 is anti-hashed by a Thue-Morse construction. Birthday bound: comparing m hashes
     pairwise collides with probability ~m^2/(2*MOD), so m = 1e6 against a single 1e9+7 mod is a
     coin flip.

 THE REPETITION MATH. k independent trials fail with p^k. Budget PER QUERY, not per test, then
   multiply by 100 for safety. For primality use the DETERMINISTIC witness set
   {2,3,5,7,11,13,17,19,23,29,31,37} (valid below 3.3e24) - zero failure probability, same cost.

 STRESS TESTING AS A THINKING TOOL. gen + brute + a diff loop; then SHRINK the counterexample
   until it fits on one line and stare at it.
   TRAPS: (1) the generator must produce DEGENERATE cases - all equal, n = 1, all zero, maximum
     values, duplicate coordinates. A uniform generator over [1, 1e9] never produces a tie and
     will never find your tie bug. (2) If the problem says "print any", diffing is useless -
     write a CHECKER. (3) Stress the brute force against the samples first.

 BRUTE FORCE SMALL CASES, THEN LOOK IT UP. Compute n = 1..12 exactly, then search OEIS, or feed
   the sequence to Berlekamp-Massey, or fit a polynomial by Lagrange interpolation.
   TRAP: an OEIS match on 4 terms is noise - get 8, then confirm the 9th. BM needs terms computed
     under the SAME prime you will use, and at least 2x the order.

 PRINT THE DP TABLE AND LOOK FOR STRUCTURE. Dump the n x n table (and the argmin table) for
   n = 20 and look for monotone rows, monotone opt[i][j], convex differences, sparsity.
   That is how you decide between Knuth, divide & conquer, and Aliens.
   TRAP: monotonicity that holds at n = 20 can fail at n = 200. Also verify the quadrangle
     inequality numerically on random quadruples.

 INSTRUMENT, DO NOT GUESS (for TLE). Print a counter of inner-loop iterations on your largest
   local test and compare against 1e8-1e9. That distinguishes "wrong complexity" from "wrong
   constant", which need completely different fixes. Compile locally with -O2.

 ============ CONTEST CRAFT ============

 READING THE SET
   Minute 0-15: all three read DIFFERENT problems, front to back. Nobody codes. On the board, per
     problem: a one-line restatement, n, and a guess at the family.
   Order by (confidence x 1/effort), not by letter. Setters mis-order ~30% of the time.
   Use the scoreboard from minute 30 - it is the best difficulty signal you have.
   ONE coder at the keyboard, always. A problem is not "started" until it has a written plan with
     its complexity and its main data structure.

 WHEN TO ABANDON - when any TWO hold:
   * 25+ minutes with no NEW idea (the idea count stopped growing, not the code).
   * The complexity is 10x over budget with no plan to close it.
   * Two teammates failed to find the bug and 5 minutes of stress testing found nothing
     => you are testing the wrong thing or misread the statement. Re-read it OUT LOUD.
   * Another unattempted problem has more solves.
   Write the state on paper before switching, and set a hard return time.

 DEBUGGING UNDER TIME PRESSURE - in this order:
   1. RE-READ THE STATEMENT: output format, the modulus, 1-based vs 0-based, multi-test.
      About a quarter of "impossible" bugs are misreads.
   2. Re-read your input parsing (n and m swapped; looping to n over an array sized m).
   3. Run ALL the samples, including the one you skipped.
   4. Run n = 1, n = 2, and the all-equal case.
   5. Stress test. This beats reading code whenever you have a brute force.
   6. Only then read the code - out loud, to a teammate, for INTENT. Silent re-reading just
      re-hallucinates the same thing.
   7. Nothing after 15 minutes: REWRITE the suspicious function from scratch.

 PRE-SUBMIT CHECKLIST (40 seconds, every submit)
   [ ] Overflow: every product of two inputs; inside comparators; inside prefix sums.
   [ ] Modulus: right constant, +MOD after subtraction, inverse instead of division, non-negative.
   [ ] n = 1, m = 1, k = 0, single-element array.
   [ ] Degenerate input: empty string, zero edges, one-node tree, all equal, all zero, negatives.
   [ ] STATE RESET between test cases: every global, vector, counter, visited array, the answer
       variable, and any function-local static. Clear only what you touched if sum n is bounded.
   [ ] Right bound read: MAXN vs the actual limit; the sum over queries can exceed the per-test n.
   [ ] Uninitialised: dp arrays that need -INF not 0; DSU parent[i] = i.
   [ ] Recursion depth: DFS on a path with n = 2e5 is 2e5 frames - move big locals out.
   [ ] Output format: newline vs space, YES vs Yes, -1 vs IMPOSSIBLE, setprecision.
   [ ] endl removed from any loop running more than ~1e4 times; debug output removed.
   [ ] The `cin >> tc` line matches whether the problem is multi-test.

 VERDICT TRIAGE
   WA  -> which test? Test 1 means a misread or a format error; a late test means an edge case or
          overflow. Then: modulus, overflow, ties/duplicates (strict vs non-strict), stress test,
          unreset state, floating-point comparison.
   TLE -> is the complexity really what you think (count the loops on paper)? endl in a loop /
          missing sync_with_stdio. map/set in a hot loop. Reallocation and pass-by-value. memset
          of a huge global per test. Infinite loop: a binary search with l = mid.
          Only then change algorithm. A /64 win never fixes a wrong complexity class.
   RE  -> array bounds first: rebuild with -fsanitize=address,undefined -D_GLIBCXX_DEBUG and run
          the samples; it finds it in seconds. Then stack overflow, division by zero, a comparator
          that is not a strict weak ordering (crashes INSIDE std::sort), .back()/.top() on an
          empty container, 0- vs 1-indexed access.
   MLE -> n x n arrays (5000^2 ints = 100 MB); vector<vector<int>> for a graph (5-8x the raw
          data - use CSR); persistent structures (count the nodes before allocating); recursion
          frames; go offline and stream instead of storing versions.
   Idleness on an interactive -> a missing flush, or debug output on stdout.

 TEAM PROTOCOL
   Never submit without the checklist: a rejection costs 20 penalty minutes plus morale.
   One person owns the printed notebook and fetches pages; the coder does not browse.
   Paste, adapt, then read once - the #1 template bug is an index-convention mismatch.
   Two solutions disagree? Trust the brute force. Always.
   Last 30 minutes: stop starting new problems; finish the nearest one and re-check the submits.

 THE TEN MOST EXPENSIVE RECURRING BUGS (by contest-time cost)
   1. Overflow in an intermediate product, invisible because the final answer is small.
   2. State not reset between test cases - passes locally, fails on test 2.
   3. Wrong strictness (< vs <=) in a comparator, a binary-search predicate, or a tie-break.
   4. Two pointers on a non-monotone predicate: plausible output, silently wrong.
   5. Binary search bounds: hi not provably feasible, or l = mid causing an infinite loop.
   6. 0- vs 1-based off-by-one when pasting a template.
   7. endl in an output loop, or a missing sync_with_stdio, at n = 1e6.
   8. Negative modulo in C++ (-3 % 5 == -3).
   9. A greedy that was never proved and passes the samples.
  10. Recursion depth on a path-shaped tree with n = 2e5.
========================================================================== */
