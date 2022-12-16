#include<bits/stdc++.h>
using namespace std;

// O(2^(m / 2) * m)
const int N = 42;
int g[N][N], c[N], cost[1 << 20], dp[1 << 20], need[1 << 20];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  int last;
  for (int i = 1; i <= n; i++) {
    int x; cin >> x;
    --x;
    if (i == 1) g[x][x] = 1; //self loop is okay, it means we must take x in our vertex cover
    if (i == n) g[x][x] = 1;
    if (i > 1) {
      g[last][x] = 1;
      g[x][last] = 1;
    }
    last = x;
  }
  // graph containing m nodes
  for (int i = 0; i < m; i++) {
    cin >> c[i];
  }
  // in this problem, answer for m = 1 is c[0], change this when necessary
  if (m == 1) {
    cout << c[0] << '\n';
    return 0;
  }
  int one = m / 2, two = m - m / 2;
  for (int mask = 0; mask < (1 << one); mask++) {
    bool good = true;
    for (int i = 0; i < one; i++) {
      for (int j = 0; j < m; j++) {
        if (g[i][j]) {
          if (j < one) {
            if (~mask >> i & 1 and ~mask >> j & 1) {
              good = false;
            }
          }
          else {
            if (~mask >> i & 1) {
              need[mask] |= 1 << (j - one);
            }
          }
        }
      }
      if (mask >> i & 1) {
        cost[mask] += c[i];
      }
    }
    if (!good) {
      cost[mask] = 1e9;
    }
  }
  for (int mask = 0; mask < (1 << two); mask++) {
    bool good = true;
    for (int i = 0; i < two; i++) {
      for (int j = 0; j < two; j++) {
        if (g[i + one][j + one]) {
          if (~mask >> i & 1 and ~mask >> j & 1) {
            good = false;
          }
        }
      }
      if (mask >> i & 1) {
        dp[mask] += c[i + one];
      }
    }
    if (!good) {
      dp[mask] = 1e9;
    }
  }
  for (int i = 0; i < two; i++) {
    for (int mask = (1 << two) - 1 ; mask >= 0 ; mask--) {
      if ((mask & (1 << i)) == 0) dp[mask] = min(dp[mask], dp[mask ^ (1 << i)]);
    }
  }
  int ans = 1e9;
  for (int mask = 0; mask < (1 << one); mask++) {
    ans = min(ans, cost[mask] + dp[need[mask]]);
  }
  cout << ans << '\n';
  return 0;
}
// https://codeforces.com/contest/1767/problem/E