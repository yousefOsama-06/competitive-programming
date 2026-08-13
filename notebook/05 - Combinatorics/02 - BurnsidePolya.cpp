// Needs: Mint (06 - Math/01); phi (02 - Primes and Divisors/06) for the necklace formulas.
// Burnside / Polya - count colourings up to symmetry.  #orbits = (1/|G|) sum_g |fix(g)|
// Needs Mint and phi() (single-value Euler totient).

// Necklaces: n beads in a circle, k colours, ROTATIONS only.
// A rotation by r has gcd(r,n) cycles -> k^gcd(r,n) fixed colourings; group by d = gcd.
Mint necklaces(ll n, ll k) {
    Mint s = 0;
    for (ll d = 1; d * d <= n; d++) if (n % d == 0) {
        s += Mint(phi(n / d)) * Mint(k).pow(d);
        if (d != n / d) s += Mint(phi(d)) * Mint(k).pow(n / d);
    }
    return s * Mint(n).inv();
}
// Bracelets: rotations AND reflections (dihedral group, |G| = 2n).
Mint bracelets(ll n, ll k) {
    Mint s = necklaces(n, k) * Mint(n);                 // = sum over the n rotations
    if (n & 1) s += Mint(n) * Mint(k).pow((n + 1) / 2);
    else s += Mint(n / 2) * (Mint(k).pow(n / 2) + Mint(k).pow(n / 2 + 1));
    return s * Mint(2 * n).inv();
}
// GENERAL RECIPE: for each group element g, count its CYCLES c(g) on the objects being
// coloured; it fixes k^c(g) colourings. Answer = (1/|G|) * sum_g k^c(g).
//   square grid n x n under the 4 rotations: cycles = n^2, ceil(n^2/4)... (work them out per problem)
//   cube faces under 24 rotations: cycle counts 6,3,3,4,2,2,... -> (k^6+3k^4+12k^3+8k^2)/24
// POLYA (colours have weights): replace k^c by prod over cycles of (sum of x_i^len).
Mint burnside(const vector<ll>& cyclesPerElement, ll k) {
    Mint s = 0;
    for (ll c : cyclesPerElement) s += Mint(k).pow(c);
    return s * Mint((ll)cyclesPerElement.size()).inv();
}
