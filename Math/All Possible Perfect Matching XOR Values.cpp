#include<bits/stdc++.h>
using namespace std;

const int N = 1030;

int a[N][N], p[N], n, vis[N];
void yo(int cur) {
  if (vis[cur]) return;
  vis[cur] = 1;
  for (int i = 1; i <= n; i++) {
    for (int j = i + 1; j <= n; j++) {
      int nxt = cur ^ a[i][p[i]] ^ a[j][p[j]] ^ a[i][p[j]] ^ a[j][p[i]];
      swap(p[i], p[j]);
      yo(nxt);
      swap(p[i], p[j]);
    }
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    cin >> n;
    int cur = 0;
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        cin >> a[i][j];
        if (i == j) p[i] = i, cur ^= a[i][i];
      }
    }
    memset(vis, 0, sizeof vis);
    yo(cur);
    for (int i = 0; i < N; i++) if (vis[i]) cout << i << ' ';
    cout << '\n';
  }
  return 0;
}
//https://www.codechef.com/problems/CHEFLST