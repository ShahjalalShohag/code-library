#include<bits/stdc++.h>
using namespace std;

const int N = 2e3 + 9;

// Let d = maximum degree of any nodes
// then there exists an edge coloring with <= d + 1 colors
// It find any coloring because finding optimal coloring is NP Hard
// returns edge coloring in col[][]
// graph must need to be simple
// 1 indexed, O(n * m)
namespace Vizing {
int C[N][N], col[N][N];
void clear() {
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) C[i][j] = col[i][j] = 0;
  }
}
int solve(int n, vector<pair<int, int>> &edges) {
  int a;
  vector<int> X(n + 1, 0);
  int ans = 0;
  auto update = [&](int u) {
    for (X[u] = 1; C[u][X[u]]; X[u]++);
  };
  auto color = [&](int u, int v, int c) {
    int p = col[u][v];
    col[u][v] = col[v][u] = c;
    ans = max(ans, c);
    C[u][c] = v;
    C[v][c] = u;
    C[u][p] = C[v][p] = 0;
    if (p) X[u] = X[v] = p;
    else update(u), update(v);
    return p;
  };
  auto flip = [&](int u, int c1, int c2) {
    int p = C[u][c1];
    swap(C[u][c1], C[u][c2]);
    if (p) col[u][p] = col[p][u] = c2;
    if (!C[u][c1]) X[u] = c1;
    if (!C[u][c2]) X[u] = c2;
    return p;
  };
  for (int i = 1; i <= n; i++) X[i] = 1;
  for (int t = 0; t < edges.size(); t++) {
    int u = edges[t].first, v0 = edges[t].second, v = v0, c0 = X[u], c = c0, d;
    vector<pair<int, int>> L;
    vector<int> vis(n + 1, 0);
    while (!col[u][v0]) {
      L.emplace_back(v, d = X[v]);
      if (!C[v][c]) for (a = (int)L.size() - 1; a >= 0; a--) c = color(u, L[a].first, c);
      else if (!C[u][d]) for (a = (int)L.size() - 1; a >= 0; a--) color(u, L[a].first, L[a].second);
      else if (vis[d]) break;
      else vis[d] = 1, v = C[u][d];
    }
    if (!col[u][v0]) {
      for (; v; v = flip(v, c, d), swap(c, d));
      if (C[u][c0]) {
        for (a = (int)L.size() - 2; a >= 0 && L[a].second != c; a--);
        for (; a >= 0; a--) color(u, L[a].first, L[a].second);
      } else t--;
    }
  }
  return ans;
}
}
int u[N], v[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  vector<pair<int, int>> edges;
  for (int i = 1; i <= m; i++) {
    cin >> u[i] >> v[i];
    edges.push_back({u[i], v[i]});
  }
  Vizing::clear();
  cout << Vizing::solve(n, edges) << '\n';
  for (int i = 1; i <= m; i++) {
    cout << Vizing::col[u[i]][v[i]] << ' ';
  }
  return 0;
}
