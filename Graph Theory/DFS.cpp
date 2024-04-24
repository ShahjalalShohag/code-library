#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
vector<int> g[N];
bool vis[N];
void dfs(int u) {
  vis[u] = true;
  for (auto v: g[u]) {
    if (!vis[v]) {
      dfs(v);
    }
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  while (m--) {
    int u, v; cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  vector<int> components;
  for (int u = 1; u <= n; u++) {
    if (!vis[u]) {
      components.push_back(u);
      dfs(u);
    }
  }
  cout << (int)components.size() - 1 << '\n';
  for (int i = 0; i + 1 < (int)components.size(); i++) {
    cout << components[i] << ' ' << components[i + 1] << '\n';
  }
  return 0;
}
// https://cses.fi/problemset/task/1666
