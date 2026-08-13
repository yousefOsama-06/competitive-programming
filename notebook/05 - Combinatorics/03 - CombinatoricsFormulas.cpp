/* ================= COMBINATORICS - FORMULA SHEET =================
 BASICS   C(n,k)=n!/(k!(n-k)!)   P(n,k)=n!/(n-k)!   multinomial n!/(k1!..km!)
   STARS AND BARS:  x1+..+xk = n, xi>=0 -> C(n+k-1, k-1);  xi>=1 -> C(n-1, k-1)
                    with upper bounds xi<=c: IE over which variables overflow
   Compositions of n into k parts C(n-1,k-1); into any parts 2^(n-1)

 BINOMIAL IDENTITIES  (memorise these - recognising one is worth 30 minutes)
   Pascal        C(n,k) = C(n-1,k-1) + C(n-1,k)
   Symmetry      C(n,k) = C(n,n-k)
   Absorption    k*C(n,k) = n*C(n-1,k-1)
   Hockey stick  sum_{i=r..n} C(i,r) = C(n+1, r+1)
   Vandermonde   sum_k C(m,k) C(n,p-k) = C(m+n, p)
   sum_k C(n,k) = 2^n           sum_k (-1)^k C(n,k) = [n==0]
   sum_k k*C(n,k) = n*2^(n-1)   sum_k C(n,k)^2 = C(2n,n)
   sum_k C(k,r) x^k ... upper negation: C(-n,k) = (-1)^k C(n+k-1, k)
   sum_{k} C(n,k) C(k,m) = C(n,m) 2^(n-m)
   Binomial inversion: f(n)=sum_k C(n,k) g(k)  <=>  g(n)=sum_k (-1)^(n-k) C(n,k) f(k)

 INCLUSION-EXCLUSION
   |A1 u..u An| = sum |Ai| - sum |Ai n Aj| + ...
   #surjections [n]->[k] = sum_{i=0..k} (-1)^i C(k,i) (k-i)^n = k! * S2(n,k)
   "exactly m" from "at least m":  E(m) = sum_{j>=m} (-1)^(j-m) C(j,m) A(j)
   derangements D(n) = n! sum_{k=0..n} (-1)^k/k! = (n-1)(D(n-1)+D(n-2)), D0=1,D1=0
   permutations with exactly k fixed points = C(n,k) * D(n-k)

 CATALAN  Cat(n) = C(2n,n)/(n+1) = C(2n,n) - C(2n,n+1),  Cat(n+1) = sum Cat(i)Cat(n-i)
   1,1,2,5,14,42,132,429,1430,4862  - counts: balanced bracket strings of length 2n;
   binary trees with n nodes; triangulations of an (n+2)-gon; monotone lattice paths under
   the diagonal; stack-sortable permutations; non-crossing partitions/chords.
   BALLOT / CYCLE LEMMA: paths from 0 to k in n steps staying > 0 = (k/n) C(n, (n+k)/2)
   REFLECTION: #paths (0,0)->(a,b) touching the line y=x+c  =  #paths to the reflected point

 STIRLING
   2nd kind S2(n,k) = #partitions of n LABELLED items into k non-empty UNLABELLED blocks
      S2(n,k) = k*S2(n-1,k) + S2(n-1,k-1);  S2(n,k) = (1/k!) sum_i (-1)^i C(k,i)(k-i)^n
      x^n = sum_k S2(n,k) * x_falling(k)          surjections = k! S2(n,k)
   1st kind (unsigned) c(n,k) = #permutations of n with exactly k cycles
      c(n,k) = c(n-1,k-1) + (n-1) c(n-1,k);   sum_k c(n,k) x^k = x(x+1)..(x+n-1)
   BELL B(n) = sum_k S2(n,k) = #set partitions.  B(n+1) = sum_k C(n,k) B(k).  Bell triangle.

 PARTITIONS (unordered, into positive parts)
   p(n) via pentagonal number theorem, O(n sqrt n):
     p(n) = sum_{k>=1} (-1)^(k+1) [ p(n - k(3k-1)/2) + p(n - k(3k+1)/2) ]
   partitions into <= k parts = partitions into parts <= k (conjugate)

 TWELVEFOLD WAY  (n balls into k boxes)
   balls  boxes  any                 injective        surjective
   dist.  dist.  k^n                 k!/(k-n)!        k! S2(n,k)
   ident. dist.  C(n+k-1, n)         C(k,n)           C(n-1, n-k)
   dist.  ident. sum_{i<=k} S2(n,i)  [n<=k]           S2(n,k)
   ident. ident. p_k(n)              [n<=k]           p(n into exactly k parts)

 BURNSIDE / POLYA   #orbits = (1/|G|) sum_{g in G} |fix(g)|
   necklaces (rotations), n beads, k colours:  (1/n) sum_{d|n} phi(n/d) k^d
   bracelets (+ reflections): (necklace sum + reflection sum) / (2n),
     reflection sum = n*k^((n+1)/2) if n odd, (n/2)(k^(n/2) + k^(n/2+1)) if n even

 TREES / GRAPHS
   Cayley: n^(n-2) labelled trees on n vertices; with given degrees d_i: (n-2)! / prod (d_i-1)!
   Prufer code <-> labelled tree, bijective
   Matrix-Tree: #spanning trees = any cofactor of the Laplacian (D - A)
   LGV: #non-intersecting path systems = det( M ) with M_ij = #paths a_i -> b_j

 GENERATING FUNCTIONS
   OGF   1/(1-x) = sum x^n            1/(1-x)^k = sum C(n+k-1, k-1) x^n
         x/(1-x-x^2) = Fibonacci      (1-sqrt(1-4x))/(2x) = Catalan
         prod 1/(1-x^i) = partitions  prod (1+x^i) = distinct-part partitions
   EGF   e^x = sum x^n/n!             EGF of set partitions = e^(e^x - 1)
   EXPONENTIAL FORMULA: if C(x) is the EGF of connected structures, e^C(x) is the EGF of all
   Coefficient extraction: [x^n] A(x)B(x) = sum_k a_k b_(n-k)  (a convolution -> NTT)
   Lagrange inversion, for F(0)=0, F'(0)!=0:  [x^n] G(F^-1)(x) = (1/n)[x^(n-1)] G'(x)(x/F(x))^n

 MISC
   hook length: #standard Young tableaux of shape lambda = n! / prod(hook lengths)
   Erdos-Ko-Rado, Dilworth (min chain cover = max antichain), Sperner (max antichain C(n,n/2))
================================================================== */
