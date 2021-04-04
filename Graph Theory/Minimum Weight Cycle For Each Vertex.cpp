#include<bits/stdc++.h>
using namespace std;

const int N = 305, inf = 1e9;

int g[N][N], d[N], vis[N], par[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cin >> g[i][j];
      if (g[i][j] == -1) {
        g[i][j] = inf;
      }
    }
  }
  for (int u = 1; u <= n; u++) {
    memset(vis, 0, sizeof vis);
    for (int i = 0; i <= n; i++) {
      d[i] = inf; par[i] = i;
    }
    d[u] = 0;
    par[u] = u;
    int ans = inf;
    while (1) {
      int cur = 0;
      for (int i = 1; i <= n; i++) {
        if (!vis[i] and d[i] < d[cur]) {
          cur = i;
        }
      }
      if (cur == 0) break;
      if (vis[cur]) break;
      vis[cur] = 1;
      if (par[cur] != cur) {
        ans = min(ans, d[cur] + g[cur][u]);
      }
      for (int i = 1; i <= n; i++) {
        if (i != u) {
          if (vis[i]) {
            if (par[i] != par[cur]) {
              ans = min(ans, d[cur] + d[i] + g[cur][i]);
            }
          }
          else if (d[cur] + g[cur][i] < d[i]) {
            d[i] = d[cur] + g[cur][i];
            if (cur == u) {
              par[i] = i;
            }
            else {
              par[i] = par[cur];
            }
          }
        }
      }
    }
    if (ans == inf) ans = -1;
    cout << ans << '\n';
  }
  return 0;
}
// https://codeforces.com/gym/100917/problem/F
