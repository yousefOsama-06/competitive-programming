// Needs: multiply / npow (02 - NTT.cpp), Poly / exp_ / log_ / cut (05 - Convolution/04).
// NUMBER OF SUBSETS WITH EACH SUM, for every sum 0..m, in O(m log m) - not O(n * m).
// Given a multiset of n positive values, the generating function is prod_i (1 + x^(a_i)), which
// naively costs n multiplications. Take logarithms: with cnt[v] = how many times v occurs,
//   log prod_v (1 + x^v)^cnt[v] = sum_v cnt[v] * sum_{j>=1} (-1)^(j+1) x^(v j) / j,
// and the double sum is a HARMONIC sum, O(m log m) terms in total. Then one exp brings it back.
// REACH FOR IT for Yosupo "#P subset sum", "for every k, how many subsets sum to k", partition
// counting with a bounded multiset of part sizes, and knapsack COUNTING when m is 1e5 and n is
// large. A plain O(n*m) knapsack (or the bitset one) is better when n is small or you only need
// feasibility rather than counts.
// PRECONDITION: all values >= 1 (handle a_i = 0 separately - each zero doubles every count).
Poly subsetSumCounts(const vector<int>& a, int m) {      // result[k] = #subsets summing to k
    vector<int> cnt(m + 1, 0);
    for (int v : a) if (v >= 1 && v <= m) cnt[v]++;
    Poly p(m + 1, 0);
    for (int v = 1; v <= m; v++) if (cnt[v])
        for (int j = 1; (ll)v * j <= m; j++) {
            ll t = (ll)cnt[v] % NMOD * npow(j, NMOD - 2) % NMOD;
            if (j & 1) p[v * j] = (p[v * j] + t) % NMOD;
            else p[v * j] = (p[v * j] - t + NMOD) % NMOD;
        }
    return exp_(p, m + 1);
}
// THE SAME MACHINE, DIFFERENT SIGN - this is the symbolic method (09 - EulerTransformAndSymbolic):
//   MULTISET (each value usable any number of times)  = exp( sum_j  B(x^j) / j )
//   POWERSET (each value usable at most once)         = exp( sum_j (-1)^(j+1) B(x^j) / j )
// with B(x) = sum_v cnt[v] x^v. Swapping the sign is the only difference between "partitions into
// parts from a set" and "subsets of a multiset".
// GOING BACKWARDS: given the counts, log_ recovers B and hence the multiset - that is the inverse
// Euler transform, the standard "recover the item sizes from the subset-sum counts" problem.
// ADD OR REMOVE ONE ITEM in O(m): multiply or divide the answer by (1 + x^v) directly (division by
// 1 + x^v is the recurrence r[k] -= r[k - v]) - much cheaper than rebuilding.
// K ITEMS ONLY: to also track the SIZE of the subset you need a second variable; do the same
// construction with x^(v) y and truncate in both, or run a 2-D convolution (05 - Convolution/08).
