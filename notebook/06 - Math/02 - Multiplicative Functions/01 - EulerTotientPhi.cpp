// EULER TOTIENT SIEVE - phiArr[i] = #{1<=k<=i : gcd(k,i)=1} for all i < N in O(N log log N).
// Needed for: a^b mod m with gcd(a,m)=1 (Euler's theorem), counting coprime pairs, Mobius identities
// (sum_{d|n} phi(d) = n), and the order of the multiplicative group mod n.
// NAME: phiArr, not phi - a global `phi` collides with the phi() FUNCTION in
// 04 - Number Theory/02 - Primes and Divisors/06 - MillerRabinPollard.cpp.
int phiArr[N];

void calculatePhi() {
    for (int i = 0; i < N; ++i) phiArr[i] = i & 1 ? i : i / 2;
    for (int i = 3; i < N; i += 2)
        if (phiArr[i] == i)
            for (int j = i; j < N; j += i) phiArr[j] -= phiArr[j] / i;
}