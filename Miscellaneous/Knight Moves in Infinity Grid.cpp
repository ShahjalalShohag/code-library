#define ll long long int
// Minimum number of knight moves from (x,y) to
// (0,0) in non-negative infinite chessboard
ll knight_move(ll x, ll y) {
  ll cnt = max({(x + 1) / 2, (y + 1) / 2, (x + y + 2) / 3});
  while((cnt % 2) != (x + y) % 2) cnt++;
  if(x == 1 && !y) return 3;
  if(y == 1 && !x) return 3;
  if(x == y && x == 2) return 4;
  return cnt;
}
