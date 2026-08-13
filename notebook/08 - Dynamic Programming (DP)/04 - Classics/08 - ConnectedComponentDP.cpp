// CONNECTED-COMPONENT DP (a.k.a. the "broken interval" or profile-free connectivity DP).
// SHAPE OF THE PROBLEM: you must build one object out of n items processed in SORTED order, the
// cost depends on how far apart consecutive items end up, and the final object must be CONNECTED
// (one block, one path, one permutation with a cyclic condition). The state is NOT "which items
// are used" (2^n) but "how many disjoint fragments exist so far", because the fragments are
// interchangeable - only their COUNT matters, and every fragment can be glued later.
// STATE: dp[i][c][cost][ends] = first i items placed, c open fragments, accumulated cost, and
// `ends` = how many of the two outer boundaries have been fixed (0, 1 or 2) when the object is a
// PATH rather than a cycle. Drop `ends` for objects with no distinguished endpoints.
// THE COST TRICK, which is the whole point: when moving from item i to item i+1, EVERY fragment
// boundary that is still open must eventually cross the gap (a[i+1] - a[i]). There are
// (2c - ends) such open boundaries, so add (a[i+1] - a[i]) * (2c - ends) to the cost NOW, once,
// instead of trying to know later which items are adjacent. That is what removes the 2^n.
// COMPLEXITY: O(n^2 * K) states with O(1) transitions, K = cost budget. The classic instance is
// JOI Skyscraper: count permutations of a[] with sum |a[i+1] - a[i]| <= K.
// PRECONDITION: sort a[] first - the cost accounting above is only valid for sorted items.
ll skyscraper(vector<ll> a, int K) {
    int n = a.size();
    sort(a.begin(), a.end());
    if (n == 1) return 1;
    // dp[c][cost][ends]
    vector<vector<array<ll, 3>>> dp(n + 2, vector<array<ll, 3>>(K + 1, {0, 0, 0}));
    dp[0][0][0] = 1;
    for (int i = 0; i < n; i++) {
        vector<vector<array<ll, 3>>> nd(n + 2, vector<array<ll, 3>>(K + 1, {0, 0, 0}));
        ll gap = i ? a[i] - a[i - 1] : 0;
        for (int c = 0; c <= i; c++)
            for (int cost = 0; cost <= K; cost++)
                for (int e = 0; e < 3; e++) {
                    ll v = dp[c][cost][e];
                    if (!v) continue;
                    ll nc = cost + gap * (2 * c - e);           // pay for every open boundary
                    if (nc > K) continue;
                    auto add = [&](int c2, int e2, ll ways) {
                        // after placing an item at least one fragment MUST exist: without this
                        // guard the state (c = 0, ends = 1) survives and inflates the count
                        if (c2 < 1 || c2 > n || ways <= 0) return;
                        nd[c2][nc][e2] = (nd[c2][nc][e2] + v % MOD * (ways % MOD)) % MOD;
                    };
                    add(c + 1, e, c + 1 - e);                   // new fragment, interior
                    add(c + 1, e + 1, e < 2 ? 2 - e : 0);       // new fragment, at an end
                    add(c, e, 2 * c - e);                       // append to one fragment's side
                    add(c, e + 1, e < 2 ? 2 - e : 0);           // append and make it an end
                    if (c >= 2) add(c - 1, e, c - 1);           // merge two fragments
                }
        dp = nd;
    }
    ll res = 0;
    for (int cost = 0; cost <= K; cost++) res = (res + dp[1][cost][2]) % MOD;
    return res;                                                // one fragment, both ends fixed
}
/* READING THE TRANSITIONS (they are always these five, in every problem of this family)
 NEW FRAGMENT: the item starts a block of its own. Interior blocks can be created in
   (c + 1 - ends) ways (the gaps between existing blocks); if it is to become an outer end of the
   final object, that is one of the (2 - ends) remaining end slots.
 EXTEND: glue the item to one of the (2c - ends) open sides of an existing block, or spend it as
   an outer end of that block (again 2 - ends ways).
 MERGE: join two adjacent blocks with this item, (c - 1) ordered choices of which pair.
 ACCEPT at the end only c == 1 (connected!) and ends == 2 (both boundaries closed).
 OTHER PROBLEMS IN THIS FAMILY: "arrange n numbers in a row minimising the sum of adjacent
   differences with constraints", counting linear extensions with gap costs, "cut a rod into
   pieces so that ..." and any permutation problem whose cost telescopes over sorted values.
 IF THE OBJECT IS A CYCLE, delete the `ends` dimension entirely and accept c == 1.
 WATCH THE OVERCOUNTING: `add(c, e + 1, 2 - e)` and `add(c + 1, e + 1, 2 - e)` both consume an end
   slot - forgetting one of them, or letting e exceed 2, silently changes the answer. */
