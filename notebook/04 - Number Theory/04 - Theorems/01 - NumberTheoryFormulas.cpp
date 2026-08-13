/* ================= NUMBER THEORY - FORMULA SHEET =================
 MULTIPLICATIVE FUNCTIONS  (f(ab)=f(a)f(b) for gcd(a,b)=1)
   tau(n)=#divisors=prod(e_i+1)   sigma(n)=sum of divisors=prod (p^(e+1)-1)/(p-1)
   phi(n)=n*prod(1-1/p)           mu(n)= 0 if NOT SQUAREFREE (some p^2 | n), else (-1)^#primes
   sum_{d|n} phi(d) = n           sum_{d|n} mu(d) = [n==1]        (this is the key identity)
   sum_{d|n} mu(d)/d = phi(n)/n   sum_{d|n} mu(d)*tau(n/d) = 1

 DIRICHLET CONVOLUTION  (f*g)(n) = sum_{d|n} f(d) g(n/d)
   1*1 = tau      Id*1 = sigma      phi*1 = Id      mu*1 = eps(=[n==1])
   mu is the inverse of 1, so f = g*1  <=>  g = f*mu.

 MOBIUS INVERSION
   g(n) = sum_{d|n} f(d)        <=>  f(n) = sum_{d|n} mu(d) g(n/d)
   g(n) = sum_{n|d, d<=N} f(d)  <=>  f(n) = sum_{n|d, d<=N} mu(d/n) g(d)      (the "upward" form)
   Standard use: [gcd(i,j)==1] = sum_{d | gcd(i,j)} mu(d).

 GCD / LCM SUMS  (all O(N log N) or better with a mu/phi sieve)
   #coprime pairs (i,j)<=n        = sum_{d=1..n} mu(d) * floor(n/d)^2
   sum_{i,j<=n} gcd(i,j)          = sum_{d=1..n} phi(d) * floor(n/d)^2
   sum_{d|n} d*phi(n/d)           = sum_{i=1..n} gcd(i,n)
   sum_{i=1..n} lcm(i,n)          = n/2 * (1 + sum_{d|n} d*phi(d))
   sum_{i,j<=n} lcm(i,j)          = sum_g g * sum_{a,b<=n/g, gcd(a,b)=1} a*b
   general: sum_{i,j} f(gcd(i,j)) = sum_d (f*mu)(d) * floor(n/d)^2

 DIVISOR BLOCKING  - evaluates sum_{i=1..n} F(floor(n/i)) in O(sqrt n):
   for (ll l = 1, r; l <= n; l = r + 1) { r = n / (n / l); }    // floor(n/i) == n/l on [l, r]
   Gives sum tau(i), sum sigma(i), sum floor(n/i), and Min_25 / Lucy-style prefix sums.

 MODULAR
   Fermat:   a^(p-1) = 1 (mod p), p prime, p !| a     => a^-1 = a^(p-2)
   Euler:    a^phi(m) = 1 (mod m) if gcd(a,m)=1
   GENERALISED EULER (a and m NOT coprime), for n >= log2(m):
             a^n = a^(n mod phi(m) + phi(m))  (mod m)          <- power towers
   Wilson:   (p-1)! = -1 (mod p)  iff p prime
   CRT:      x = a_i (mod m_i) has a unique solution mod lcm(m_i) iff a_i = a_j (mod gcd(m_i,m_j))
   Legendre: exponent of p in n!  = sum_{i>=1} floor(n / p^i)  = (n - s_p(n)) / (p - 1)
   Kummer:   exponent of p in C(n,r) = #carries when adding r and (n-r) in base p
   LTE (p odd, p | a-b, p !| a,b):  v_p(a^n - b^n) = v_p(a-b) + v_p(n)
       p=2 (a, b both ODD):
         n odd  : v_2(a^n - b^n) = v_2(a-b)          <- the case everyone forgets
         n even : v_2(a^n - b^n) = v_2(a-b) + v_2(a+b) + v_2(n) - 1
         n odd  : v_2(a^n + b^n) = v_2(a+b)
         n even : v_2(a^n + b^n) = 1
   LTE for SUMS (p odd, p | a+b, p !| a,b):
             n odd  -> v_p(a^n + b^n) = v_p(a+b) + v_p(n)
             n even -> p does not divide a^n + b^n at all

 QUADRATIC RESIDUES (p odd prime)
   Euler's criterion: a^((p-1)/2) = +1 if a is a QR, -1 if not
   #QRs = (p-1)/2.  x^2=a has 0 or 2 roots.  Use Tonelli-Shanks.
   -1 is a QR iff p = 1 (mod 4);  2 is a QR iff p = +-1 (mod 8)

 PRIMITIVE ROOTS exist exactly for m = 1, 2, 4, p^k, 2p^k (p odd prime). Count = phi(phi(m)).

 REPRESENTATIONS
   n is a sum of two squares  iff every prime p = 3 (mod 4) has an EVEN exponent
   n is a sum of three squares iff n != 4^a (8b+7)      every n is a sum of four squares
   Primitive Pythagorean triples: (m^2-k^2, 2mk, m^2+k^2), m>k>0, gcd(m,k)=1, m-k odd

 SIZE FACTS (for choosing an algorithm)
   max tau(n): 1344 for n<=1e9, 6720 for n<=1e12, 103680 for n<=1e18
   sum_{i<=n} tau(i) ~ n ln n ;  sum_{i<=n} phi(i) ~ 3n^2/pi^2
   pi(n) ~ n/ln n ;  n-th prime ~ n ln n ;  Bertrand: a prime always lies in (n, 2n)
   gaps: for n<=1e18 the max prime gap is < 1500
   Pisano period pi(m) | ... : F(n) mod m repeats with period <= 6m
================================================================== */
