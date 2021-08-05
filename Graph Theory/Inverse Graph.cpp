#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

// given edges which are not in the graph you need to apply dfs
// 2 coloring for this problem
int par[N], col[N], vis[N], ty[N], n;
vector<int> g[N];
int Find(int x) {
  return par[x] == x ? x : par[x] = Find(par[x]);
}
void dfs(int u, int c) {
  if(vis[u]) return;
  vis[u] = 1;
  par[u] = Find(u + 1);
  col[u] = c;
  if(!ty[u]) for(auto v: g[u]) dfs(v, c ^ 1);
  else {
    int v = 0;
    for(auto nw: g[u]) {
      v = Find(v + 1);
      while(v < nw) dfs(v, c ^ 1), v = Find(v + 1);
      v = nw;
    }
    v = Find(v + 1);
    while(v <= n) dfs(v, c ^ 1), v = Find(v + 1);
  }
}
int32_t main() {
  cin >> n;
  for(int i = 1; i <= n; i++) {
    char ch;
    cin >> ch;
    if(ch == 'N') ty[i] = 1; //'N' means these are the inverse edges
    int k;
    cin >> k;
    if(!ty[i] && k == 0 || ty[i] && k == n - 1) return cout << "Impossible\n", 0;
    vector<int> v;
    int m;
    while(k--) cin >> m, v.push_back(m);
    sort(v.begin(), v.end());
    g[i] = v;
  }
  for(int i = 1; i <= n + 1; i++) par[i] = i; //n + 1 is important
  for(int i = 1; i <= n; i++) if(!vis[i]) dfs(i, 0);
  for(int i = 1; i <= n; i++) cout << (!col[i] ? 'S' : 'V');
  cout << '\n';
  return 0;
}
