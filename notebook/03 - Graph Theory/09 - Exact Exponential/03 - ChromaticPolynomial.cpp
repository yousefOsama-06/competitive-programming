// Needs: Mint (06 - Math/01 - Mint.cpp).
// CHROMATIC POLYNOMIAL P(x) = the number of proper colourings with at most x colours, for n <= 20.
// O(2^n * n^2) to get all of P(0..n), then O(n^2) per evaluation elsewhere. 0-indexed; adj[] is an
// adjacency BITMASK (bit j of adj[i] set iff i-j is an edge), no self loops.
// 01 - CliqueColouringMIS finds the chromatic NUMBER with the same inclusion-exclusion; this gets
// the whole polynomial, which is what you need for "count the proper k-colourings" (k possibly
// huge), for "count acyclic orientations", and for deletion-contraction arguments.
// THE FORMULA. With I(S) = the number of independent subsets of S (including the empty one),
//     P(k) = sum over S of (-1)^(n-|S|) * I(S)^k.
// P has degree n, so evaluating it at 0..n pins it down; Lagrange interpolation gives any other
// point. All arithmetic is mod MOD, so a zero result is "almost surely" a genuine zero.
struct ChromaticPolynomial {
    int n;
    vector<Mint> p;                                            // p[k] = P(k) for k = 0..n
    ChromaticPolynomial(const vector<int>& adj) : n(adj.size()), p(n + 1, 0) {
        int F = 1 << n;
        vector<Mint> I(F), pw(F, 1);
        I[0] = 1;
        for (int S = 1; S < F; S++) {                          // v is in the subset, or it is not
            int v = __builtin_ctz(S), rest = S ^ 1 << v;
            I[S] = I[rest] + I[rest & ~adj[v]];
        }
        for (int k = 1; k <= n; k++) {
            Mint tot = 0;
            for (int S = 0; S < F; S++) {
                pw[S] *= I[S];                                 // pw[S] = I(S)^k
                tot += ((n - __builtin_popcount(S)) & 1) ? Mint(0) - pw[S] : pw[S];
            }
            p[k] = tot;
        }
    }
    Mint at(ll x) const {                                      // P(x) for any x, by interpolation
        if (x >= 0 && x <= n) return p[x];
        Mint ans = 0;
        for (int i = 0; i <= n; i++) {
            Mint num = p[i];
            for (int j = 0; j <= n; j++) if (j != i) num *= (Mint(x) - j) / (Mint(i) - j);
            ans += num;
        }
        return ans;
    }
    int chromaticNumber() const {
        for (int k = 1; k <= n; k++) if (!(p[k] == Mint(0))) return k;
        return n;
    }
};
// KNOWN VALUES, useful as checks: a tree on n vertices has P(x) = x (x-1)^(n-1); the cycle C_n has
// (x-1)^n + (-1)^n (x-1); K_n has x(x-1)...(x-n+1); a forest of k components on n vertices in total
// has P(x) = x^k (x-1)^(n-k) only when every component is a star... just use the tree formula per
// component and multiply, since P is MULTIPLICATIVE over connected components.
// ACYCLIC ORIENTATIONS of G = |P(-1)| = (-1)^n P(-1)   (Stanley's theorem). That also counts the
// DAGs you can make by directing the edges of G, which is how "count the DAGs whose underlying
// graph is G" problems are solved.
// DELETION-CONTRACTION: P_G(x) = P_{G-e}(x) - P_{G/e}(x). Exponential in general, but it is the
// right tool when the graph is nearly a tree or you can memoise on small components.
// LABELLED DAGs ON n VERTICES (a different question - no fixed graph): with a_n the count,
// a_n = sum_{k=1..n} (-1)^(k+1) C(n,k) 2^(k(n-k)) a_{n-k}, by inclusion-exclusion on the sources.
