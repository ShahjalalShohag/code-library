#include<bits/stdc++.h>
using namespace std;

const int N = 5e5 + 9;

vector<pair<int, int>> g[N];
int vis[N], par[N], e_id[N];
vector<int> cycle; // simple cycle, contains edge ids

bool dfs(int u) {
  if (!cycle.empty()) return 1;
  vis[u] = 1;
  for (auto [v, id] : g[u]) {
    if (v != par[u]) {
      if (vis[v] == 0) {
        par[v] = u;
        e_id[v] = id;
        if (dfs(v)) return 1;
      }
      else if (vis[v] == 1) {
        // cycle here
        cycle.push_back(id);
        for (int x = u; x != v; x = par[x]) {
          cycle.push_back(e_id[x]);
        }
        return 1;
      }
    }
  }
  vis[u] = 2;
  return 0;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  for (int i = 1; i <= m; i++) {
    int u, v; cin >> u >> v;
    ++u; ++v;
    g[u].push_back({v, i});
  }
  for (int u = 1; u <= n; u++) {
    if (vis[u] == 0 and dfs(u)) {
      cout << cycle.size() << '\n';
      for (auto x: cycle) cout << x - 1 << '\n';
      return 0;
    }
  }
  cout << -1 << '\n';
  return 0;
}
// https://judge.yosupo.jp/problem/cycle_detection