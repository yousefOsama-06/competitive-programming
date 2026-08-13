// LEXICOGRAPHICALLY EXTREME SOLUTION of a GF(2) system, on top of 03 - GaussXorAndReal.cpp.
// gaussXor returns SOME solution (free variables = 0), which is the lexicographically SMALLEST one
// when variables are compared in index order x[0], x[1], ... . Problems that ask for the
// lexicographically LARGEST assignment, or that weight the variables, need this wrapper.
// THE TRICK: reverse the variable order before eliminating and reverse the answer back. Reduced
// row echelon form always puts a pivot as early as possible, so "free variables = 0" minimises the
// reversed vector, i.e. maximises the original. O(n * m / 64), same as one elimination.
// Careful: "lexicographically largest" here means as a SEQUENCE x[0] > x[1] > ...; if the problem
// means "largest as a binary NUMBER with x[0] the most significant bit", that is the same thing.
template <int M>
int gaussXorLexMax(vector<bitset<M + 1>> a, int m, bitset<M>& sol) {
    int n = a.size();
    for (int i = 0; i < n; i++) {                        // reverse the m variable columns
        bitset<M + 1> t;
        for (int j = 0; j < m; j++) t[j] = a[i][m - 1 - j];
        t[m] = a[i][m];
        a[i] = t;
    }
    bitset<M> s;
    int rank = gaussXor<M>(a, m, &s);
    if (rank < 0) return -1;                             // inconsistent
    for (int j = 0; j < m; j++) sol[j] = s[m - 1 - j];    // and reverse the answer back
    return rank;                                         // #solutions = 2^(m - rank)
}
// PRIORITY ORDER other than "by index": permute the columns by the priority you want (most
// important variable first), solve, permute back. Only the ORDER of the columns matters.
// MAXIMISE A LINEAR OBJECTIVE sum w_i x_i over the solution set: that is NOT lexicographic and is
// NP-hard in general (max-XOR-SAT). It is easy only when all w_i >= 0 and you may take each pivot
// greedily - i.e. when the objective is itself lexicographic.
// MAXIMISE THE XOR of the solution vector viewed as a number: build a basis of the NULL SPACE
// (one vector per free variable: set that free variable to 1, back-substitute the pivots), then
// run Basis::maxXor starting from the particular solution (04 - Linear Algebra/01).
// COUNTING: the solution set is a coset, so every count is 2^(m - rank) or 0; "number of solutions
// with x[i] = 1" is either 0, the full count, or half of it, depending on whether e_i is forced.
