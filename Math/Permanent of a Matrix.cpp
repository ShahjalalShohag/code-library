#include<bits/stdc++.h>
using namespace std;

const int N = 20;

long long dp[1 << N];
int n, a[N][N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    cin >> n;
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) cin >> a[i][j];
    }
    memset(dp, 0, sizeof dp);
    dp[(1 << n) - 1] = 1;
    for (int mask = (1 << n) - 2; mask >= 0; mask--) {
      int i = __builtin_popcount(mask);
      for (int j = 0; j < n; j++) if (!(mask >> j & 1)) dp[mask] += dp[mask | 1 << j] * a[i][j];
    }
    cout << dp[0] << '\n'; // permanent of the matrix
  }
  return 0;
}
