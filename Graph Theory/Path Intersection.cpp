#include<bits/stdc++.h>
using namespace std;

const int N = 1e4 + 9;

vector<int> g[N];
int par[N][20], dep[N], sz[N];
void dfs(int u, int pre) {
  par[u][0] = pre;
  dep[u] = dep[pre] + 1;
  sz[u] = 1;
  for (int i = 1; i <= 15; i++) par[u][i] = par[par[u][i - 1]][i - 1];
  for (auto v : g[u]) {
    if (v == pre) continue;
    dfs(v, u);
    sz[u] += sz[v];
  }
}
int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int k = 15; k >= 0; k--) if (dep[par[u][k]] >= dep[v]) u = par[u][k];
  if (u == v) return u;
  for (int k = 15; k >= 0; k--) if (par[u][k] != par[v][k]) u = par[u][k], v = par[v][k];
  return par[u][0];
}
int kth(int u, int k) {
  for (int i = 0; i <= 15; i++) if (k & (1 << i)) u = par[u][i];
  return u;
}
// if g is an ancestor of u
int isanc(int u, int g) {
  int k = dep[u] - dep[g];
  return k >= 0 && kth(u, k) == g;
}
// returns (0,0) for null path
pair<int, int> getCommonPath(int u, int a, int v, int b) { // returns common path between u->a and v->b where u is ancestor a and v is ancestor b
  if ( !isanc(a, v)) return make_pair(0, 0); // v has to be an ancestor of a to have any common path
  int x = lca(a, b);
  if (dep[v] < dep[u]) { // v is an ancestor of u, so the beginning of common path should be from u
    if (isanc(x, u)) // if u is not an ancestor of x no common path exists
      return make_pair(u, x); // u->x is the common path
  } else { // u is an ancestor of v, so the beginning of common path should be from v
    if (isanc(x, v)) // if v is not an ancestor of x no common path exists
      return make_pair(v, x); // v->x is the common path
  }
  return make_pair(0, 0);
}
pair<int, int> path_union_light(pair<int, int> x, pair<int, int> y) {
  if (x.first == 0) return y; //where {0,0} is a null path
  if (y.first == 0) return x;
  vector<int> can = {x.first, x.second, y.first, y.second};
  int a = can[0];
  for (int u : can) if (dep[u] > dep[a]) a = u;
  int b = -1;
  for (int u : can)if (!isanc(a, u)) {
      if (b == -1) b = u;
      if (dep[b] < dep[u]) b = u;
    }
  if (b == -1) {
    b = can[0];
    for (int u : can) if (dep[u] < dep[b]) b = u;
    return {a, b};
  }
  return {a, b};
}
// returns the intersection of two paths (a,b) and (c,d)
// {0,0} for null path
pair<int, int> path_intersection(int a, int b, int c, int d) {
  if (a == 0 || b == 0) return {0, 0};
  int u = lca(a, b), v = lca(c, d); // splits both paths in two chains from lca to one node
  pair<int, int> x, y;
  x = getCommonPath(u, a, v, c);
  y = getCommonPath(u, a, v, d);
  pair<int, int> a1 = path_union_light(x, y);
  x = getCommonPath(u, b, v, c);
  y = getCommonPath(u, b, v, d);
  pair<int, int> a2 = path_union_light(x, y);
  return path_union_light(a1, a2);
}
int32_t main() {
  int t, cs = 0; cin >> t;
  while (t--) {
    int n; cin >> n;;
    for (int i = 1; i < n; i++) {
      int u, v; cin >> u >> v;
      g[u].push_back(v);
      g[v].push_back(u);
    }
    dfs(1, 0);
    int q; cin >> q;
    printf("Case %d:\n", ++cs);
    while (q--) {
      int m; cin >> m;
      pair<int, int> ans;
      if (m) {
        int u, v; cin >> u >> v;
        ans = {u, v};
        m--;
      }
      while (m--) {
        int u, v; cin >> u >> v;
        ans = path_intersection(ans.first, ans.second, u, v);
      }
      if (ans.first == 0) puts("0");
      else cout << dep[ans.first] + dep[ans.second] - 2 * dep[lca(ans.first, ans.second)] + 1 << '\n';
    }
    for (int i = 0; i <= n; i++) {
      g[i].clear();
      memset(par[i], 0, sizeof par[i]);
    }
  }
  return 0;
}
// https://algo.codemarshal.org/contests/icpc-dhaka-18/problems/F