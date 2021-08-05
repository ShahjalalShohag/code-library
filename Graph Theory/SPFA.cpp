#include<bits/stdc++.h>
using namespace std;

const int N = 5050;

vector<pair<int, int>> g[N];
bool vis[N];
long long d[N];
bool spfa(int u) {
  vis[u] = 1;
  for(auto e: g[u]) {
    int w = e.second, v = e.first;
    if(d[u] + w < d[v]) {
      if(vis[v]) return 0;
      d[v] = d[u] + w;
      if(!spfa(v)) return 0;
    }
  }
  vis[u] = 0;
  return 1;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int t;
  cin >> t;
  while(t--) {
    int n, m;
    cin >> n >> m;
    for(int i = 0; i <= n; i++) g[i].clear();
    int ans = 1e9;
    for(int i = 1; i <= m; i++) {
      int u, v, w; cin >> u >> v >> w;
      g[u].push_back({v, w});
      ans = min(ans, w);
    }
    if(ans >= 0) {
      cout << ans << '\n';
      continue;
    }
    for(int i = 1; i <= n; i++) g[0].push_back({i, 0});
    for(int i = 0; i <= n; i++) d[i] = 1e10, vis[i] = 0;
    int s = 0;
    d[s] = 0;
    if(spfa(s) == 0) cout << "-inf\n"; // negative cycle
    else {
      long long ans = d[1];
      for(int i = 1; i <= n; i++) ans= min(ans, d[i]);
      cout << ans << '\n';
    }
  }
  return 0;
}
// https://codeforces.com/gym/101498/problem/L
