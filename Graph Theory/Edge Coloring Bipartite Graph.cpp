#include<bits/stdc++.h>
using namespace std;

const int N = 5002;

// 1 indexed, O(n * m)
namespace EdgeColoring {
int deg[2][N];
pair<int, int> has[2][N][205]; // (2, number of nodes, number of colors)
int color[5005]; // number of edges
int c[2];
void clear(int n) {
  for (int t = 0; t < 2; t++) {
    for (int i = 0; i <= n; i++) {
      deg[t][i] = 0;
      for (int j = 0; j <= n; j++) {
        has[t][i][j] = pair<int, int>(0, 0);
      }
    }
  }
}
void dfs(int x, int p) {
  auto e = has[p][x][c[!p]];
  if (has[!p][e.first][c[p]].second) dfs(e.first, !p);
  else has[!p][e.first][c[!p]] = pair<int, int>(0, 0);
  has[p][x][c[p]] = e;
  has[!p][e.first][c[p]] = pair<int, int>(x, e.second);
  color[e.second] = c[p];
}
int solve(vector<pair<int, int>> edges, vector<int> &res) {
  int m = edges.size(), ans = 0;
  for (int i = 1; i <= m; i++) {
    int x[2];
    x[0] = edges[i - 1].first;
    x[1] = edges[i - 1].second;
    for (int d = 0; d < 2; d++) {
      deg[d][x[d]] ++;
      ans = max(ans, deg[d][x[d]]);
      for (c[d] = 1; has[d][x[d]][c[d]].second; c[d]++);
    }
    if (c[0] != c[1]) dfs(x[1], 1);
    for (int d = 0; d < 2; d++) has[d][x[d]][c[0]] = pair<int, int>(x[!d], i);
    color[i] = c[0];
  }
  res.resize(m);
  for (int i = 1; i <= m; i++) {
    res[i - 1] = color[i];
    color[i] = 0;
  }
  return ans;
}
};
int deg[N][2], id[N][2], l, r;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, t, k;
  cin >> n >> m >> k >> t;
  vector<pair<int, int>> edges;
  for (int i = 0; i < k; i++) {
    int x, y;
    cin >> x >> y;
    if (deg[x][0] == 0) id[x][0] = ++l;
    if (deg[y][1] == 0) id[y][1] = ++r;
    deg[x][0] = (deg[x][0] + 1) % t;
    deg[y][1] = (deg[y][1] + 1) % t;
    edges.push_back({id[x][0], id[y][1]});
  }
  int ans = 0;
  for (int i = 1; i <= n; i++) ans += !!deg[i][0];
  for (int i = 1; i <= m; i++) ans += !!deg[i][1];
  cout << ans << '\n';
  vector<int> color;
  EdgeColoring::solve(edges, color);
  for (auto x : color) cout << x << ' ';
  return 0;
}
// https://codeforces.com/contest/212/problem/A
