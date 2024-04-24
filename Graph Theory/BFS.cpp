#include<bits/stdc++.h>
using namespace std;

const int inf = 1e9;
const int N = 1e5 + 9;
vector<int> g[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  queue<int> q;
  vector<int> d(n + 1, inf), par(n + 1, -1);
  q.push(1);
  d[1] = 0;
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (auto v: g[u]) {
      if (d[u] + 1 < d[v]) {
        d[v] = d[u] + 1;
        par[v] = u;
        q.push(v);
      }
    }
  }
  if (d[n] == inf) {
    cout << "IMPOSSIBLE\n";
    return 0;
  }
  cout << d[n] + 1 << '\n';
  int cur = n;
  vector<int> path;
  while (cur != -1) { 
    path.push_back(cur);
    cur = par[cur];
  }
  reverse(path.begin(), path.end());
  for (auto u: path) {
    cout << u << ' ';
  }
  cout << '\n';
  return 0;
}
// https://cses.fi/problemset/task/1667
