#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;

int x[N], y[N], c[N], dp[1 << 20], msk[N], ans[N];

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, r; cin >> n >> m >> r;
  for (int i = 1; i <= n; i++) {
    cin >> x[i] >> y[i];
  }
  // total m bitsets and size of each is n
  for (int i = 1; i <= m; i++) {
    int X, Y; cin >> X >> Y >> c[i];
    for (int j = 1; j <= n; j++) {
      if (1LL * (X - x[j]) * (X - x[j]) + 1LL * (Y - y[j]) * (Y - y[j]) <= 1LL * r * r) {
        msk[j] |= 1 << (i - 1);
      }
    }
  }
  //msk[i] = mask of the i-th index
  for (int i = 1; i <= n; i++) {
    dp[msk[i] ^ ((1 << m) - 1)]++;
  }
  for (int i = 0; i < m; i++) {
    for (int mask = (1 << m) - 1; mask >= 0; mask--) {
      if (~mask >> i & 1) {
        dp[mask] += dp[mask ^ (1 << i)];
      }
    }
  }
  // dp[mask] = n - (popcount of the union of the bitsets which are on in mask)
  for (int i = 1; i <= n; i++) {
    ans[i] = 2e9 + 9;
  }
  for (int mask = 0; mask < (1 << m); mask++) {
    int cost = 0;
    for (int i = 0; i < m; i++) {
      if (mask >> i & 1) {
        cost += c[i + 1];
      }
    }
    ans[n - dp[mask]] = min(ans[n - dp[mask]], cost);
  }
  for (int i = n - 1; i >= 1; i--) {
    ans[i] = min(ans[i], ans[i + 1]);
  }
  for (int i = 1; i <= n; i++) {
    if (ans[i] == 2e9 + 9) ans[i] = -1;
    cout << ans[i] << '\n';
  }
  return 0;
}
// https://tlx.toki.id/contests/troc-16/problems/F
