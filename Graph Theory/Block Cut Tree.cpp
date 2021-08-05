#include<bits/stdc++.h>
using namespace std;

const int N = 4e5 + 9;

int T, low[N], dis[N], art[N], sz;
vector<int> g[N], bcc[N], st;
void dfs(int u, int pre = 0) {
  low[u] = dis[u] = ++T;
  st.push_back(u);
  for(auto v: g[u]) {
    if(!dis[v]) {
      dfs(v, u);
      low[u] = min(low[u], low[v]);
      if(low[v] >= dis[u]) {
        sz ++;
        int x;
        do {
          x = st.back();
          st.pop_back();
          bcc[x].push_back(sz);
        } while(x ^ v);
        bcc[u].push_back(sz);
      }
    } else if(v != pre) low[u] = min(low[u], dis[v]);
  }
}

int dep[N], par[N][20], cnt[N], id[N];
vector<int> bt[N];
void dfs1(int u, int pre = 0) {
  dep[u] = dep[pre] + 1;
  cnt[u] = cnt[pre] + art[u];
  par[u][0] = pre;
  for(int k = 1; k <= 18; k++) par[u][k] = par[par[u][k - 1]][k - 1];
  for(auto v: bt[u]) if(v != pre) dfs1(v, u);
}

int lca(int u, int v) {
  if(dep[u] < dep[v]) swap(u, v);
  for(int k = 18; k >= 0; k--) if(dep[par[u][k]] >= dep[v]) u = par[u][k];
  if(u == v) return u;
  for(int k = 18; k >= 0; k--) if(par[u][k] != par[v][k]) u = par[u][k], v = par[v][k];
  return par[u][0];
}

int dist(int u, int v) {
  int lc = lca(u, v);
  return cnt[u] + cnt[v] - 2 * cnt[lc] + art[lc];
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n, m;
  cin >> n >> m;
  while(m--) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs(1);
  for(int u = 1; u <= n; u++) {
    if(bcc[u].size() > 1) { //AP
      id[u] = ++sz;
      art[id[u]] = 1; //if id in BCT is an AP on real graph or not
      for(auto v: bcc[u]) {
        bt[id[u]].push_back(v);
        bt[v].push_back(id[u]);
      }
    } else if(bcc[u].size() == 1) id[u] = bcc[u][0];
  }

  dfs1(1);
  int q;
  cin >> q;
  while(q--) {
    int u, v;
    cin >> u >> v;
    int ans;
    if(u == v) ans = 0;
    else ans = dist(id[u], id[v]) - art[id[u]] - art[id[v]];
    cout << ans << '\n';;//number of articulation points in the path from u to v except u and v
    //u and v are in the same bcc if ans == 0
  }
  return 0;
}
// https://codeforces.com/gym/102512/problem/A
