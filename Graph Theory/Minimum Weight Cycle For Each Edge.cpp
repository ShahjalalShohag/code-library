#include<bits/stdc++.h>
using namespace std;

const int N = 205, inf = 1e9;

int d[N], vis[N], par[N];
int g[N][N], ans[N * N], last[N], dp[N * N], id[N][N], W[N * N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t, cs = 0; cin >> t;
  while (t--) {
    int n, m; cin >> n >> m;
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        g[i][j] = inf;
      }
    }
    for (int i = 1; i <= m; i++) {
      int u, v, w; cin >> u >> v >> w;
      ++u; ++v;
      g[u][v] = g[v][u] = w;
      id[u][v] = id[v][u] = i;
      ans[i] = inf;
      W[i] = w;
    }
    for (int u = 1; u <= n; u++) {
      memset(vis, 0, sizeof vis);
      for (int i = 0; i <= n; i++) {
        d[i] = inf; par[i] = i; last[i] = -1;
      }
      for (int i = 1; i <= m; i++) {
        dp[i] = inf;
      }
      d[u] = 0;
      par[u] = u;
      last[u] = 0;
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
          dp[id[last[cur]][cur]] = min(dp[id[last[cur]][cur]], d[cur] + g[cur][u]);
          ans[id[cur][u]] = min(ans[id[cur][u]], d[cur] + g[cur][u]);
          ans[id[u][cur]] = min(ans[id[u][cur]], d[cur] + g[cur][u]);
        }
        for (int i = 1; i <= n; i++) {
          if (i != u) {
            if (vis[i]) {
              if (par[i] != par[cur]) {
                dp[id[last[cur]][cur]] = min(dp[id[last[cur]][cur]], d[cur] + d[i] + g[cur][i]);
                dp[id[last[i]][i]] = min(dp[id[last[i]][i]], d[cur] + d[i] + g[cur][i]);
                ans[id[cur][i]] = min(ans[id[cur][i]], d[cur] + d[i] + g[cur][i]);
                ans[id[i][cur]] = min(ans[id[i][cur]], d[cur] + d[i] + g[cur][i]);
              }
            }
            else if (d[cur] + g[cur][i] < d[i]) {
              d[i] = d[cur] + g[cur][i];
              last[i] = cur;
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
      for (int i = 1; i <= n; i++) {
        if (last[i] == -1) continue;
        int x = dp[id[last[i]][i]];
        for (int cur = i; cur; cur = last[cur]) {
          if (last[cur]) {
            dp[id[last[cur]][cur]] = min(dp[id[last[cur]][cur]], x);
          }
        }
      }
      for (int i = 1; i <= m; i++) {
        ans[i] = min(ans[i], dp[i]);
      }
    }
    cout << "Case " << ++cs << ":\n";
    for (int i = 1; i <= m; i++) {
      if (ans[i] == inf) cout << "impossible\n";
      else cout << ans[i] - W[i] + 1 << '\n';
    }
  }
  return 0;
}
// https://lightoj.com/contest/loj-homecoming/arena/problem/14