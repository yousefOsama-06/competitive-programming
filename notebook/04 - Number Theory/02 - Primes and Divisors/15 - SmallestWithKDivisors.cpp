// Needs: powmod (06 - MillerRabinPollard.cpp), divisors of k (03 - Divisors.cpp or factorize).
// THE SMALLEST NUMBER WITH EXACTLY k DIVISORS. The answer explodes (k = 1e9 already needs ~100
// digits), so it is compared by the SUM OF LOGARITHMS and reported modulo MOD.
// Structure: x = p_0^(e_0) * p_1^(e_1) * ... over the FIRST primes in order, with
// (e_0+1)(e_1+1)... = k and e_0 >= e_1 >= ... (swapping a larger exponent onto a smaller prime
// never hurts). So: walk the primes in order, and at prime i choose a divisor x | k for the
// factor (e_i + 1) = x. Memoise on (prime index, remaining k).
// PRECONDITIONS: pr must hold the first ~64 primes (2, 3, 5, ...); dvs = all divisors of k that
// are > 1, ASCENDING (that order is what makes the break-early pruning valid). k >= 1.
// COMPLEXITY: O(#states * tau(k)) with tiny constants - milliseconds for k up to 1e12.
struct SmallestKDiv {
    vector<ll> pr, dvs;
    vector<ld> lg;
    vector<map<ll, pair<ld, ll>>> memo;                 // memo[i][rem] = {log of x, x mod MOD}
    SmallestKDiv(vector<ll> pr, vector<ll> dvs) : pr(pr), dvs(dvs) {
        for (ll p : pr) lg.push_back(logl((ld)p));
        memo.assign(pr.size() + 1, {});
    }
    pair<ld, ll> go(int i, ll rem) {                    // use primes pr[i], pr[i+1], ... only
        if (rem == 1) return {0.0L, 1};
        auto it = memo[i].find(rem);
        if (it != memo[i].end()) return it->second;
        pair<ld, ll> best = {1e30L, 0};
        for (ll x : dvs) {
            if (rem % x) continue;
            ld z = lg[i] * (ld)(x - 1);                 // cost of putting exponent x-1 on pr[i]
            if (z > best.first) break;                  // dvs ascending => all later are worse
            auto cur = go(i + 1, rem / x);
            cur.first += z;
            cur.second = cur.second * powmod(pr[i], x - 1, MOD) % MOD;
            best = min(best, cur);
        }
        return memo[i][rem] = best;
    }
    ll solve(ll k) { return k == 1 ? 1 : go(0, k).second; }
};
// EXACT SMALL ANSWERS (for a sanity check): k = 1,2,3,4,5,6,7,8,9,10 -> 1, 2, 4, 6, 16, 12, 64,
// 24, 36, 48. k = 100 -> 45360. The sequence is OEIS A005179.
// LONG DOUBLE is enough here: two candidates tie only when the numbers are equal, and the log
// sums differ by more than 1e-9 otherwise for any k that fits in 64 bits. If paranoid, compare
// the exponent vectors exactly with big integers, or compare logs and break ties by the vectors.
// RELATED: the MAXIMUM number of divisors below n (highly composite numbers) is found by the
// same DFS with the objective flipped - keep the primes in order and try e_i = 0..60 with
// e_0 >= e_1 >= ...; tau maxes at 1344 for n <= 1e9, 6720 for 1e12, 103680 for 1e18.
