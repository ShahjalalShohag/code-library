#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

struct Components {
  vector<int> nxt;
  Components() {}
  Components(int sz) : nxt(sz) {
    iota(nxt.begin(), nxt.end(), 0);
  }
  int size() { return nxt.size(); }
  vector<int> enumerate(int x) {
    assert(x < size());
    vector<int> ret;
    int y = x;
    do {
      ret.push_back(y);
      y = nxt[y];
    } while (y != x);
    return ret;
  }
  vector<vector<int>> get_all() {
    int n = size();
    vector<vector<int>> ans;
    vector<bool> vis(n, false);
    for (int i = 0; i < n; i ++) {
      if (!vis[i]) {
        vector<int> list = enumerate(i);
        for (int j : list) vis[j] = true;
        ans.push_back(list);
      }
    }
    return ans;
  }
  void unite(int x, int y) {
    assert(x < size());
    assert(y < size());
    swap(nxt[x], nxt[y]);
  }
};
// 0 indexed
// returns the list of the three edge connected components
// O(n)
vector<vector<int>> TEC(vector<vector<int>> g) {
  int n = g.size();
  vector<int> in(n), out(n), low(n, n), deg(n, 0), path(n, n);
  vector<bool> vis(n, false);
  Components tcc(n);
  auto absorb = [&](int v, int w) -> void {
    tcc.unite(v, w);
    deg[v] += deg[w];
  };
  int cnt = 0;
  function<void(int, int)> dfs = [&](int v, int p) -> void {
    vis[v] = true;
    in[v] = cnt;
    cnt ++;
    for (int w : g[v]) {
      if (w == v) continue;
      if (w == p) {
        p = n; continue;
      }
      if (vis[w]) {
        if (in[w] < in[v]) {
          deg[v] ++;
          low[v] = min(low[v], in[w]);
        } 
        else {
          deg[v] --;
          int u = path[v];
          while (u != n && in[u] <= in[w] && in[w] < out[u]) {
            absorb(v, u);
            u = path[u];
          }
          path[v] = u;
        }
        continue;
      }
      dfs(w, v);
      if (path[w] == n && deg[w] <= 1) {
        deg[v] += deg[w];
        low[v] = min(low[v], low[w]);
        continue;
      } 
      else if (deg[w] == 0) w = path[w];
      if (low[w] < low[v]) {
        low[v] = low[w];
        swap(w, path[v]);
      }
      while (w != n) {
        absorb(v, w);
        w = path[w];
      }
    }
    out[v] = cnt;
  };
  for (int i = 0; i < n; i ++) if (!vis[i]) dfs(i, n);
  return tcc.get_all();
}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  vector<vector<int>> g(n);
  for (int i = 0; i < m; i ++) {
    int a, b; cin >> a >> b;
    g[a].push_back(b);
    g[b].push_back(a);
  }
  auto ans = TEC(g);
  cout << ans.size() << '\n';
  for (auto &l : ans) {
    cout << l.size();
    for (int e : l) cout << " " << e;
    cout << '\n';
  }
  return 0;
}
// https://judge.yosupo.jp/problem/three_edge_connected_components
