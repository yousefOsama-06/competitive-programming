// GRID DIRECTION VECTORS. First 4 entries = 4-neighbourhood (L,R,U,D), all 8 = king moves.
// Knight moves: dx {1,1,-1,-1,2,2,-2,-2}, dy {2,-2,2,-2,1,-1,1,-1}. Guard with 0<=x<n && 0<=y<m.
int dx[] = {0, 0, -1, 1, -1, -1, 1, 1};
int dy[] = {-1, 1, 0, 0, -1, 1, -1, 1};
char di[] = {'L', 'R', 'U', 'D'};

// KNIGHT DISTANCE ON AN INFINITE BOARD, O(1). BFS is fine up to a 1000x1000 board; past that, or
// when the coordinates are 1e9, you need this closed form. Reduce by symmetry to x, y >= 0, then
// the answer is driven by three lower bounds - you cover at most 2 in either axis per move and at
// most 3 of (x + y) per move - and parity, since every move changes x + y by an odd amount. Three
// positions beat the formula and must be special-cased; they are the classic reason a hand-rolled
// version WAs. Exact integer arithmetic, any magnitude that fits in ll.
ll knightDist(ll x, ll y) {
    x = abs(x), y = abs(y);
    if (x < y) swap(x, y);                              // now x >= y >= 0
    if (x == 1 && y == 0) return 3;                     // the three exceptions, all near the
    if (x == 2 && y == 2) return 4;                     // origin, where the board "runs out"
    ll d = max({(x + 1) / 2, (y + 1) / 2, (x + y + 2) / 3});
    while ((d & 1) != ((x + y) & 1)) d++;               // parity: x + y flips every move
    return d;
}
// ON A BOUNDED BOARD the formula is wrong near the edges and corners - BFS, or use the formula
// and repair the O(1) corner cases by hand. On a HALF-PLANE (x, y >= 0) the same three exceptions
// are exactly the corrections needed, which is what the code above assumes.
// KING distance is max(|dx|, |dy|); ROOK is 0/1/2; BISHOP is 0/1/2 when the colours match and
// unreachable otherwise. Those need no code, only the reminder that they are not Manhattan.