// NEGATIVE-BASE ARITHMETIC. toNegBase(n, b) writes ANY integer n, either sign, in base -b with
// digits in [0, b): n == sum d[i] * (-b)^i, digits LEAST SIGNIFICANT FIRST, no leading zero, and
// zero is the single digit {0}. Every integer has exactly one such representation and there is no
// sign anywhere - that is the entire point of the base. fromNegBase inverts it.
// WHEN: "print n in base -2", "add two negabinary strings", puzzle problems where a value must be
// encoded without a sign, and the folklore fact that negabinary addition needs a NEGATIVE carry.
// THE DIVISION RULE: d = n mod b forced into [0, b), then n = (n - d) / (-b), an EXACT division
// since b divides n - d. Do not write n /= -b and patch d afterwards: C++ truncates toward zero,
// so after d += b the quotient also needs n += 1, and that is the step everyone forgets.
// WHY IT TERMINATES: |n - d| <= |n| + b - 1, so the new |n| is at most |n|/b + (b-1)/b, which is
// strictly smaller than |n| whenever |n| >= 2; from |n| == 1 it reaches 0 in at most two steps.
// The digit count is O(log_b |n|) + 1, and the sign of n alternates as you descend.
// ADDITION digit by digit: s = x[i] + y[i] + carry lands in [-b, 2b - 1]; force the digit into
// [0, b) by moving b, and since the place value of the next digit is -b, moving +b DOWN here means
// carrying -1 UP. A leftover carry of +1 appends the digit 1; a leftover of -1 appends TWO digits,
// b-1 then 1, because -1 == (b-1) + 1 * (-b). The same loop does subtraction (pass sgn = -1): the
// range of s only widens to [-b, b], still inside what the one carry line handles.
// COMPLEXITY: O(log_b |n|) per conversion, O(len) per add. DEGENERATE: n == 0 gives {0}; adding to
// or from {0} works; results are stripped of leading zeros so equality is plain vector ==.
// EXACTNESS: exact integers. Budget: |n| <= 9.2e18 minus b for toNegBase (n - d must not overflow,
// so LLONG_MIN is out), and fromNegBase must be told digits whose value fits in ll.
vi toNegBase(ll n, int b) {                        // b >= 2; the base used is -b
    vi d;
    while (n) {
        int r = (int)(n % b);
        if (r < 0) r += b;                         // C++ % keeps the sign of n; digits must be >= 0
        d.pb(r);
        n = (n - r) / -b;                          // exact division, never a truncation
    }
    return d.empty() ? vi{0} : d;
}
ll fromNegBase(const vi& d, int b) {
    ll v = 0;
    for (int i = sz(d) - 1; i >= 0; i--) v = v * -b + d[i];       // Horner, top digit down
    return v;
}
vi negAdd(vi x, const vi& y, int b, int sgn = 1) {               // x + sgn * y, sgn is 1 or -1
    x.resize(max(sz(x), sz(y)), 0);
    int c = 0;
    for (int i = 0; i < sz(x); i++) {
        int s = x[i] + (i < sz(y) ? sgn * y[i] : 0) + c;
        c = s < 0 ? 1 : (s >= b ? -1 : 0);         // next place is worth -b, so shifting the digit
        x[i] = s + b * c;                          // by -b here is a carry of +1, and vice versa
    }
    if (c == 1) x.pb(1);
    else if (c == -1) x.pb(b - 1), x.pb(1);        // -1 == (b-1) * 1 + 1 * (-b)
    while (sz(x) > 1 && !x.back()) x.pop_back();   // normalise, so == is equality of values
    return x;
}
// VARIANTS
//  - NEGATE: negAdd({0}, x, b, -1). Do not hunt for a bit pattern, subtraction already does it.
//  - SIGN: a normalised nonzero number is NEGATIVE iff its digit count is even, i.e. the leading
//    digit sits on an odd power of -b. Compare x and y by the sign of negAdd(x, y, b, -1).
//  - MULTIPLY BY -b: prepend a zero digit (shift). Divide by -b: drop d[0].
//  - MULTIPLY: schoolbook into a wide accumulator, then normalise from the bottom with the same
//    rule - while (d[i] < 0) d[i] += b, d[i+1] += 1; while (d[i] >= b) d[i] -= b, d[i+1] -= 1.
//  - NEGABINARY (b == 2) IN ONE LINE, no loop: the digits of n as a bit pattern are
//    (n + 0xAAAAAAAAAAAAAAAAull) ^ 0xAAAAAAAAAAAAAAAAull, bit i being d[i] - the mask is the whole
//    carry cascade done in parallel. All 64 digits must fit, which covers every negative ll but
//    only n <= (4^32 - 1) / 3 = 6148914691236517205 upward; past that it silently truncates.
//  - BASE -1+i represents every Gaussian integer with digits {0, 1}: take d = (re + im) & 1, then
//    (re, im) -> ((im - re + d) / 2, -(re + im - d) / 2). Same loop, same termination argument.
