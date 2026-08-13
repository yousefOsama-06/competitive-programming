// COMPILER PRAGMAS. O3 + unrolling, and AVX2 for auto-vectorised loops (bitset, simple array math).
// Put them at the very top, before any include. Risk: `target` crashes judges without those CPU
// features - never use avx2 on an unknown judge, and never on Codeforces problems with old
// machines.
// GCC Optimizations
#pragma GCC optimize("O3,unroll-loops")
#pragma GCC target("avx2,bmi,bmi2,lzcnt,popcnt")
