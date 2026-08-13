// Needs: powmod (02 - Primes and Divisors/06 - MillerRabinPollard.cpp).
// ARITHMETIC IN F_p[sqrt(D)] = { a + b*sqrt(D) : a, b in F_p } - the quadratic extension field
// when D is a NON-RESIDUE mod p (then it is F_{p^2}), or a ring of "split" numbers when it is.
// WHAT IT BUYS YOU:
//   * CIPOLLA's square root mod p: always O(log p) multiplications, and unlike Tonelli-Shanks it
//     does not degrade when p - 1 has a huge power of two. (Tonelli is in 02 - DiscreteLog...)
//   * Closed forms that need an irrational: Fibonacci via ((1+sqrt5)/2)^n mod p, and ANY order-2
//     linear recurrence x_{n+1} = a x_n + b x_{n-1}, even when the discriminant is a non-residue
//     mod p (no matrix needed - one field power).
//   * Lucas sequences / Frobenius primality tests, and n-th terms of (a + b sqrt(D))^n identities.
// PRECONDITIONS: QP an odd prime, all inputs reduced into [0, QP). QP up to ~4.6e18 (the __int128
// products need QP^2 < 2^127).
ll QP, QD;                                              // the modulus and the radicand
ll qmul(ll x, ll y) { return (ll)((__int128)x * y % QP); }
struct Quad {
    ll a, b;                                            // a + b*sqrt(QD)
    Quad(ll a = 0, ll b = 0) : a(a), b(b) {}
    Quad operator*(const Quad& o) const {
        return Quad((qmul(a, o.a) + qmul(QD, qmul(b, o.b))) % QP,
                    (qmul(a, o.b) + qmul(b, o.a)) % QP);
    }
    Quad operator+(const Quad& o) const { return Quad((a + o.a) % QP, (b + o.b) % QP); }
    Quad operator-(const Quad& o) const { return Quad((a - o.a + QP) % QP, (b - o.b + QP) % QP); }
    Quad conj() const { return Quad(a, (QP - b) % QP); }        // the Frobenius x -> x^p
    ll norm() const { return (qmul(a, a) - qmul(QD, qmul(b, b)) % QP + QP) % QP; }
};
Quad qpow(Quad x, ll e) {                               // O(log e) with 3 multiplications a step
    Quad r(1, 0);
    for (; e; e >>= 1, x = x * x) if (e & 1) r = r * x;
    return r;
}
// CIPOLLA: x with x^2 = a (mod p), p an odd prime. Returns -1 if a is not a quadratic residue.
ll cipolla(ll a, ll p) {
    a %= p; if (a < 0) a += p;
    if (a <= 1) return a;
    if (powmod(a, (p - 1) / 2, p) != 1) return -1;      // Euler's criterion
    QP = p;
    ll t = 1;
    while (true) {                                      // find t with t^2 - a a NON-residue
        QD = ((__int128)t * t - a) % p; QD = (QD + p) % p;
        if (QD && powmod(QD, (p - 1) / 2, p) == p - 1) break;
        t++;
    }
    return qpow(Quad(t, 1), (p + 1) / 2).a;             // the sqrt(QD) part is provably 0
}
// The other root is p - x. Expected O(log p) tries for t (half the residues work).
// FIBONACCI mod p: set QD = 5, phi = (1 + sqrt 5)/2 = Quad((p+1)/2, (p+1)/2); then
//   F(n) = (phi^n - (1 - phi)^n) / sqrt(5), i.e. qpow(phi, n).b * 2 (mod p) since the two
//   conjugate parts cancel. Works for every odd p, whether or not 5 is a residue.
// GENERAL ORDER-2 RECURRENCE x_{n+1} = A x_n + B x_{n-1}: the roots of z^2 = A z + B live in
// F_p[sqrt(A^2 + 4B)], so x_n is a linear combination of their n-th powers - one qpow call.
