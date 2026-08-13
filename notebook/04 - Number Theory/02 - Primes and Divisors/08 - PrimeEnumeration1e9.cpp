// ENUMERATE EVERY PRIME UP TO 1e9 IN ~0.5s, and in a 30-smooth wheel that costs 1 BIT per 30
// integers (so ~33 MB at n = 1e9 versus ~60 MB for the odds-only bitset in 04 - Sieve1e9.cpp).
// Credit: min_25. Use THIS when you need the primes themselves at n >= 1e8; use 04 when you only
// need the primality bitmap and want six lines instead of sixty.
// HOW IT IS FAST: (1) a mod-30 wheel - of the 30 residues only {1,7,11,13,17,19,23,29} can be
// prime, so one byte covers 30 integers; (2) the pattern of the small primes (<= Q) is computed
// ONCE into `pre` and memcpy'd over each block; (3) the sieve runs block by block so everything
// stays in L1/L2 cache, with each prime remembering where it left off in each of the 8 residues.
vector<int> sievePrimes(const int N, const int Q = 17, const int L = 1 << 15) {
    static const int rs[] = {1, 7, 11, 13, 17, 19, 23, 29};
    struct P { P(int p) : p(p) {} int p; int pos[8]; };
    auto approxPrimeCount = [](const int N) -> int {
        return N > 60184 ? N / (log(N) - 1.1) : max(1., N / (log(N) - 1.11)) + 1;
    };
    const int v = sqrt(N), vv = sqrt(v);
    vector<bool> isp(v + 1, true);
    for (int i = 2; i <= vv; ++i) if (isp[i])
        for (int j = i * i; j <= v; j += i) isp[j] = false;
    const int rsize = approxPrimeCount(N + 30);
    vector<int> primes = {2, 3, 5};
    int psize = 3;
    primes.resize(rsize);
    vector<P> sp;
    size_t pbeg = 0;
    int prod = 1;
    for (int p = 7; p <= v; ++p) {
        if (!isp[p]) continue;
        if (p <= Q) prod *= p, ++pbeg, primes[psize++] = p;
        auto pp = P(p);
        for (int t = 0; t < 8; ++t) {
            int j = (p <= Q) ? p : p * p;
            while (j % 30 != rs[t]) j += p << 1;
            pp.pos[t] = j / 30;
        }
        sp.push_back(pp);
    }
    vector<unsigned char> pre(prod, 0xFF);                     // the small primes' pattern, once
    for (size_t pi = 0; pi < pbeg; ++pi) {
        auto pp = sp[pi];
        const int p = pp.p;
        for (int t = 0; t < 8; ++t) {
            const unsigned char m = ~(1 << t);
            for (int i = pp.pos[t]; i < prod; i += p) pre[i] &= m;
        }
    }
    const int bs = (L + prod - 1) / prod * prod;
    vector<unsigned char> block(bs);
    unsigned char* pb = block.data();
    const int M = (N + 29) / 30;
    for (int beg = 0; beg < M; beg += bs, pb -= bs) {
        int end = min(M, beg + bs);
        for (int i = beg; i < end; i += prod) copy(pre.begin(), pre.end(), pb + i);
        if (beg == 0) pb[0] &= 0xFE;                           // 1 is not prime
        for (size_t pi = pbeg; pi < sp.size(); ++pi) {
            auto& pp = sp[pi];
            const int p = pp.p;
            for (int t = 0; t < 8; ++t) {
                int i = pp.pos[t];
                const unsigned char m = ~(1 << t);
                for (; i < end; i += p) pb[i] &= m;
                pp.pos[t] = i;                                 // resume here in the next block
            }
        }
        for (int i = beg; i < end; ++i)
            for (int m = pb[i]; m > 0; m &= m - 1)
                primes[psize++] = i * 30 + rs[__builtin_ctz(m)];
    }
    while (psize > 0 && primes[psize - 1] > N) --psize;
    primes.resize(psize);
    return primes;
}
// pi(1e9) = 50847534, so the output vector alone is ~200 MB of int - if you only need to ITERATE
// the primes, sieve block by block and consume them inside the loop instead of storing them.
// Q = 17 means the wheel pre-pattern covers 7*11*13*17 = 17017 bytes; raising Q makes `pre`
// bigger but the inner loop shorter. L = 1<<15 keeps a block inside L1.
// FOR PRIMALITY OF ONE NUMBER up to 1e18 use Miller-Rabin (06); for the FACTORISATION of one
// number use Pollard rho (06); for pi(n) without the primes use Lucy_Hedgehog (04 - Theorems/03).
