/* ================= STRESS TESTING =================
 The single highest-value 20 lines in this notebook. Use it the moment a solution WAs on a
 test you cannot see. Write gen.cpp + brute.cpp, then loop until they disagree.

 ---- run.sh -------------------------------------------------------------
 g++ -O2 -o sol sol.cpp && g++ -O2 -o brute brute.cpp && g++ -O2 -o gen gen.cpp
 for i in $(seq 1 100000); do
     ./gen $i > in.txt
     ./sol   < in.txt > out1.txt
     ./brute < in.txt > out2.txt
     if ! diff -qb out1.txt out2.txt > /dev/null; then echo "WA on test $i"; cat in.txt; break; fi
 done
 -------------------------------------------------------------------------
 Windows cmd: replace the loop with `for /L %%i in (1,1,100000) do ( ... fc out1.txt out2.txt )`

 RULES THAT MAKE IT ACTUALLY WORK
  1. Generate SMALL cases (n <= 8, values <= 5). Bugs hide in tiny corner cases, not big ones.
  2. Seed the generator from argv[1] so every iteration differs and is reproducible.
  3. If the answer is not unique, do not diff - write a CHECKER that validates sol's output.
  4. If there is no brute force, stress against yourself: compare two different solutions, or
     compare an O(n^2) version of your own idea against the O(n log n) one.
  5. For "find any valid X" problems, verify the constraints instead of comparing.
 ================================================== */

// ---- gen.cpp ----
// #include <bits/stdc++.h>
// using namespace std;
// int main(int argc, char** argv) {
//     mt19937 rng(atoi(argv[1]));
//     auto rnd = [&](int l, int r) { return uniform_int_distribution<int>(l, r)(rng); };
//     int n = rnd(1, 8);
//     printf("%d\n", n);
//     for (int i = 0; i < n; i++) printf("%d ", rnd(1, 5));
//     puts("");
// }

// ---- in-process version: no files, no scripts. Put brute + fast in one binary. ----
// Fastest way to start; switch to the shell loop only when you need the failing input on disk.
template <class Gen, class Fast, class Brute>
void stress(Gen gen, Fast fast, Brute brute, int iters = 100000) {
    for (int it = 1; it <= iters; it++) {
        auto in = gen(it);
        auto a = fast(in), b = brute(in);
        if (!(a == b)) {
            printf("MISMATCH on iteration %d: fast=", it);
            cout << a << " brute=" << b << "\n";
            return;                                  // print `in` here too
        }
    }
    puts("all tests passed");
}
