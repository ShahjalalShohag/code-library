#include<bits/stdc++.h>
using namespace std;

const int N = 510;

//maximum matching on a general graph
//O(n * m * it) but faster
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct RandomizedMatching {
  vector<int> g[N];
  int n, match[N], vis[N], T = 0;
  RandomizedMatching() {}
  RandomizedMatching(int _n) {
    n = _n;
    T = 0;
    for (int i = 0; i <= n; i++) {
      match[i] = vis[i] = 0;
      g[i].clear();
    }
  }
  void add_edge(int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
  }
  bool dfs(int x) {
    if(x == 0) return true;
    vis[x] = T;
    auto it = g[x].begin();
    shuffle(g[x].begin(), g[x].end(), rnd);
    for(; it != g[x].end(); it++) {
      int u = *it, v = match[u];
      if (vis[v] < T) {
        match[x] = u, match[u] = x, match[v] = 0;
        if (dfs(v)) return true;
        match[u] = v, match[v] = u, match[x] = 0;
      }
    }
    return false;
  }
  int maximum_matching() {
    memset(match, 0, sizeof match);
    int ans = 0;
    for(int it = 5; it; it--) { //increase the iteration value for higher probability
      for(int i = 1; i <= n; i++) {
        if(!match[i]) {
          T++, ans += dfs(i);
        }
      }
    }
    return ans;
  }
};
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  RandomizedMatching M(n);
  while (m--) {
    int u, v;
    cin >> u >> v;
    M.add_edge(u, v);
  }
  cout << M.maximum_matching() << '\n';
  for (int i = 1; i <= n; i++) cout << M.match[i] << ' ';
  cout << '\n';
  return 0;
}
