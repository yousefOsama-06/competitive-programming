// GRAY CODE - orders 0..2^n-1 so consecutive values differ in exactly ONE bit. Use it to walk all
// subsets while changing one element at a time (incremental DP, XOR-basis sweeps, Tower of Hanoi).
// Gray Code Generator - O(2^N)
// g(n) = n ^ (n >> 1)
vector<int> grayCode(int n) {
    vector<int> res(1 << n);
    for (int i = 0; i < (1 << n); i++) {
        res[i] = i ^ (i >> 1);
    }
    return res;
}

int rev_g (int g) {
  int n = 0;
  for (; g; g >>= 1)
    n ^= g;
  return n;
}
