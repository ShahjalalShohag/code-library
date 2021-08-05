#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

int T, low[N], dis[N], art[N];
vector<int> g[N];
void dfs(int u, int pre = 0) {
  low[u] = dis[u] = ++T;
  int child = 0;
  for(auto v: g[u]) {
    if(!dis[v]) {
      dfs(v, u);
      low[u] = min(low[u], low[v]);
      if(low[v] >= dis[u] && pre != 0) art[u] = 1;
      ++child;
    }
    else if(v != pre) low[u] = min(low[u], dis[v]);
  }
  if(pre == 0 && child > 1) art[u] = 1;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  while(1){
    int n, m; cin >> n >> m;
    if(!n) break;
    while(m--) {
      int u, v; cin >> u >> v;
      g[u].push_back(v);
      g[v].push_back(u);
    }
    dfs(1);
    int ans = 0;
    for(int i = 1; i <= n; i++) ans += art[i];
    cout << ans << '\n';
    T = 0; for(int i = 1; i <= n; i++) low[i] = dis[i] = art[i] = 0, g[i].clear();
  }
  return 0;
}
