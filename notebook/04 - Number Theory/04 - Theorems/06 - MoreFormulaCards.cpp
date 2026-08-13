/* ============ NUMBER THEORY, SECOND FORMULA SHEET ============
 Everything here was machine-verified against brute force / sympy.

 QUADRATIC RECIPROCITY.  p, q distinct ODD primes.
   (p/q)(q/p) = (-1)^( (p-1)/2 * (q-1)/2 )
     => (p/q) = (q/p), UNLESS p = q = 3 (mod 4), where (p/q) = -(q/p)
   SUPPLEMENTS: (-1/p) = 1 iff p = 1 (mod 4);  (2/p) = 1 iff p = +-1 (mod 8);
                (3/p) = 1 iff p = +-1 (mod 12)
   EULER'S CRITERION (p odd prime AND p !| a):  a^((p-1)/2) = (a/p) (mod p).
     If p | a the value is 0, not +-1 - state the hypothesis.
   #k-th ROOTS of a != 0 mod p:  d = gcd(k, p-1); the count is d if a^((p-1)/d) = 1, else 0.
 JACOBI SYMBOL (a/n) for ODD n > 0, n NOT necessarily prime - O(log n), no factoring: */
int jacobi(ll a, ll n) {                                       // n odd, n > 0
    int r = 1;
    a = ((a % n) + n) % n;                                     // (a/n) is periodic in a mod n
    while (a) {
        while (a % 2 == 0) { a /= 2; if (n % 8 == 3 || n % 8 == 5) r = -r; }
        swap(a, n);
        if (a % 4 == 3 && n % 4 == 3) r = -r;
        a %= n;
    }
    return n == 1 ? r : 0;
}
/* CAUTION: (a/n) = 1 does NOT prove a is a QR mod a composite n. (a/n) = -1 DOES prove it is
   not. Use it as a cheap filter, and to find a non-residue fast for Tonelli-Shanks.

 FIBONACCI  (F_0 = 0, F_1 = 1)
   FAST DOUBLING, O(log n), no matrices:
     F(2k) = F(k) * (2 F(k+1) - F(k)),   F(2k+1) = F(k)^2 + F(k+1)^2
   gcd(F_m, F_n) = F_gcd(m,n)   =>   F_m | F_n iff m | n  (m >= 3)
   Cassini:  F_{n-1} F_{n+1} - F_n^2 = (-1)^n
   d'Ocagne: F_m F_{n+1} - F_{m+1} F_n = (-1)^n F_{m-n}
   sum_{i<=n} F_i = F_{n+2} - 1        sum_{i<=n} F_i^2 = F_n F_{n+1}
   F_92 is the last Fibonacci number fitting in a signed 64-bit integer.
 PISANO PERIOD pi(m) = the period of F(n) mod m
   pi(1)=1, pi(2)=3, and pi(m) is EVEN for every m > 2
   pi(lcm(a,b)) = lcm(pi(a), pi(b))          pi(p^k) = p^(k-1) * pi(p)
   pi(m) <= 6m, with EQUALITY exactly at m = 2 * 5^k   (verified: m = 10, 50, 250)
   p = +-1 (mod 5) => pi(p) | p-1;   p = +-2 (mod 5) => pi(p) | 2(p+1)
 ZECKENDORF: every n >= 1 is a UNIQUE sum of NON-CONSECUTIVE Fibonacci numbers, found
   greedily from the top. The basis of "Fibonacci base" digit DP and of Wythoff's game.

 PELL:  x^2 - D y^2 = 1, D > 0 non-square. Always infinitely many solutions.
   sqrt(D) = [a0; a1, ..., a_L] is periodic with a_L = 2 a0 and a palindromic middle.
   FUNDAMENTAL SOLUTION = the convergent h_k / k_k with
       k = L - 1   if the period L is EVEN,      k = 2L - 1  if L is ODD.
   ALL solutions: x_m + y_m sqrt(D) = (x1 + y1 sqrt(D))^m, i.e.
       x_{m+1} = x1 x_m + D y1 y_m,   y_{m+1} = x1 y_m + y1 x_m
   NEGATIVE PELL x^2 - D y^2 = -1 is solvable IFF the period L is ODD; then the fundamental
   solution is the convergent at k = L - 1.
   SIZE WARNING: solutions explode. D = 61 gives x = 1766319049, but D = 661 gives a
   39-digit x. `ll` is useless past D ~ 150 - use bignum, or work modulo the answer.
   CF MACHINERY (a0 = floor(sqrt D); m = 0, d = 1, a = a0), all exact integers:
       m' = d*a - m,    d' = (D - m'^2) / d,    a' = floor((a0 + m') / d')
   BEST RATIONAL APPROXIMATION: the closest p/q to x with q <= N is a convergent OR a
   semiconvergent (t h1 + h2)/(t k1 + k2) with 1 <= t <= a_next. Check both.
   FAREY MEDIANT: for neighbours a/b < c/d (bc - ad = 1), (a+c)/(b+d) is the unique fraction
   with the SMALLEST denominator strictly between them.

 SUMS OF SQUARES
   TWO:   n = x^2 + y^2 possible IFF every prime p = 3 (mod 4) has an EVEN exponent.
          r_2(n) = 4 (d_1(n) - d_3(n)), d_i = #divisors congruent to i mod 4.
   THREE: n = x^2 + y^2 + z^2 possible IFF n is NOT of the form 4^a (8b + 7).
   FOUR:  ALWAYS possible (Lagrange).  r_4(n) = 8 * sum over d | n with 4 !| d of d.
   PRIMITIVE PYTHAGOREAN TRIPLES: (m^2 - k^2, 2mk, m^2 + k^2), m > k > 0, gcd(m,k) = 1,
     m - k odd.
   GAUSSIAN INTEGERS Z[i]: the norm N(a+bi) = a^2 + b^2 is multiplicative. Primes of Z[i]:
     1+i (over 2); p = 1 (mod 4) SPLITS as pi * conj(pi); p = 3 (mod 4) stays prime.
     gcd by rounded division: q = round(a conj(b) / N(b)), r = a - qb, then Euclid on norms.

 FROBENIUS / CHICKEN MCNUGGET.  a, b >= 2, gcd(a,b) = 1:
   largest non-representable  g(a,b) = ab - a - b;   #non-representable = (a-1)(b-1)/2.
   For k >= 3 coins there is NO closed form: run Dijkstra on residues mod a_1 (d[r] = the
   smallest representable value congruent to r), then g = max_r d[r] - a_1. O(a_1 k log a_1).
 BEATTY / RAYLEIGH.  r, s > 1 IRRATIONAL with 1/r + 1/s = 1  =>  floor(n r) and floor(n s),
   n >= 1, PARTITION the positive integers exactly. (Wythoff's losing positions are exactly
   (floor(n phi), floor(n phi^2)).)
 THREE-DISTANCE (Steinhaus). The points {a}, {2a}, ..., {na} cut [0,1) into intervals of AT
   MOST 3 distinct lengths, and when there are 3 the largest is the sum of the other two.
   Use: min over k <= n of (k*a mod m) in O(log) via the continued fraction of a/m.

 DETERMINISTIC MILLER-RABIN BASE SETS (all machine-verified)
   n < 2^32              : {2, 7, 61}                          (valid to 4759123141)
   n < 3.2e18            : the first 12 primes {2..37}
   n < 3.317e24 > 2^64   : {2, 325, 9375, 28178, 450775, 9780504, 1795265022}
   {2,3,5,7} is NOT enough even for 32 bits: 3215031751 = 151*751*28351 passes all four.
   Skip a base a when n divides a, or small primes are reported composite.

 NTT PRIMES (form, primality and primitive root all machine-verified)
   p                    = c * 2^k + 1      g     max transform length
   998244353            = 119 * 2^23 + 1    3      2^23     <- the default
   167772161            =   5 * 2^25 + 1    3      2^25
   469762049            =   7 * 2^26 + 1    3      2^26
   754974721            =  45 * 2^24 + 1   11      2^24     <- NOT 9*2^23+1 (common misprint)
   1004535809           = 479 * 2^21 + 1    3      2^21
   2013265921           =  15 * 2^27 + 1   31      2^27
   1224736769           =  73 * 2^24 + 1    3      2^24
   3221225473           =   3 * 2^30 + 1    5      2^30
   9223372036737335297  = 549755813881 * 2^24 + 1, g = 3   (64-bit, needs __int128 mulmod)
   THE 3-PRIME TRIPLE for an arbitrary modulus: 167772161 * 469762049 * 754974721 = 5.95e25.
   THE BINDING CONSTRAINT IS LENGTH, NOT MAGNITUDE: 754974721 caps the transform at 2^24, so
   the input length is capped at 2^23. At that length the magnitude headroom is still 6x.

 64-BIT CEILINGS - memorise these
   a*b fits in signed ll        iff a, b < 3037000499  ( = floor(sqrt(2^63 - 1)) )
   n(n+1)/2 fits in ll          iff n <= 4294967295
   sum of primes <= n fits      iff n <~ 3e10          (the sum is ~ n^2 / (2 ln n))
   sum of phi(i), i <= n, fits  iff n <= 5508509576    (the sum is ~ 0.3040 n^2)
   max tau(n): 1344 for n <= 1e9,  6720 for 1e12,  103680 for 1e18
   NEED __int128 FOR: any modmul with modulus > 3.04e9 (Pollard, BSGS, Tonelli, CRT, Garner);
     floorSum with n > 1e9; Cornacchia's b*b > p test; prime/totient SUMS at n >= 1e10.
   FFT PRECISION, the real criterion (not "|result| < 1e15"):
     safe iff (sum a_i^2 + sum b_i^2) * log2(N) < 9e14.
     Splitting at sqrt(M) additionally needs M^1.5 < 9.2e18, i.e. M <~ 4.4e12, with the
     inputs already reduced into [0, M).
   FWHT: the forward transform inflates magnitudes by up to N, so the pointwise product is
     bounded by (N * max|a|)^2. Under a modulus the XOR inverse must MULTIPLY BY inv(N) mod p -
     an integer `x /= n` is exact over Z and WRONG mod p.
============================================================== */
