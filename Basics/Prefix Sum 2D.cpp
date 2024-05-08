#include<bits/stdc++.h>
using namespace std;

const int N = 1005;
int a[N][N], pref[N][N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      cin >> a[i][j];
    }
  }
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= m; j++) {
      pref[i][j] = pref[i - 1][j] + pref[i][j - 1] - pref[i - 1][j - 1] + a[i][j];
    }
  }
  int q; cin >> q;
  while (q--) {
    int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
    int ans = 0;
    // for (int i = x1; i <= x2; i++) {
    //   for (int j = y1; j <= y2; j++) {
    //     ans += a[i][j];
    //   }
    // }
    ans = pref[x2][y2] - pref[x1 - 1][y2] - pref[x2][y1 - 1] + pref[x1 - 1][y1 - 1];
    cout << ans << '\n';
  }
  return 0;
}
