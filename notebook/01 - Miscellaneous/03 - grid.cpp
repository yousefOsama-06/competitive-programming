// GRID DIRECTION VECTORS. First 4 entries = 4-neighbourhood (L,R,U,D), all 8 = king moves.
// Knight moves: dx {1,1,-1,-1,2,2,-2,-2}, dy {2,-2,2,-2,1,-1,1,-1}. Guard with 0<=x<n && 0<=y<m.
int dx[] = {0, 0, -1, 1, -1, -1, 1, 1};
int dy[] = {-1, 1, 0, 0, -1, 1, -1, 1};
char di[] = {'L', 'R', 'U', 'D'};