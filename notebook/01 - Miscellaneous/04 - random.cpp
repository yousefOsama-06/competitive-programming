// RANDOMNESS. Seed from the clock so tests cannot be prepared against you.
mt19937_64 rng(chrono::steady_clock::now().time_since_epoch().count());
ll rnd(ll l, ll r) { return uniform_int_distribution<ll>(l, r)(rng); }   // inclusive

vector<int> randPerm(int n) {                           // random permutation of 0..n-1
    vector<int> v(n);
    iota(all(v), 0);
    shuffle(all(v), rng);
    return v;
}
// Shuffling the INPUT is a real algorithm, not just a utility:
//   - minimum enclosing circle is O(n) only after a shuffle
//   - quickselect / random pivots
//   - defeats anti-hash and anti-quicksort tests
//   - random rotation of points removes degenerate ties in a geometry sweep
