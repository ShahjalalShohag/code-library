#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;
vector<int> g[N];
int par[N][20], dep[N], sz[N];
void dfs(int u, int pre) {
  par[u][0] = pre;
  dep[u] = dep[pre] + 1;
  sz[u] = 1;
  for (int i = 1; i <= 18; i++) par[u][i] = par[par[u][i - 1]][i - 1];
  for (auto v : g[u]) {
    if (v == pre) continue;
    dfs(v, u);
    sz[u] += sz[v];
  }
}
int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int k = 18; k >= 0; k--) if (dep[par[u][k]] >= dep[v]) u = par[u][k];
  if (u == v) return u;
  for (int k = 18; k >= 0; k--) if (par[u][k] != par[v][k]) u = par[u][k], v = par[v][k];
  return par[u][0];
}
int kth(int u, int k) {
  for (int i = 0; i <= 18; i++) if (k & (1 << i)) u = par[u][i];
  return u;
}
int isanc(int u, int g) {
  int k = dep[u] - dep[g];
  return k >= 0 && kth(u, k) == g;
}
// Given two valid paths in a tree it finds the union of two paths. If the union is not a path it //returns {-1,-1};
pair<int, int> merge(pair<int, int> x, pair<int, int> y) {
  if (x.first == 0)                 return y; //where {0,0} is a null path
  if (y.first == 0)                 return x;
  if (x.first == -1 || y.first == -1)   return { -1, -1};
  vector<int> can = {x.first, x.second, y.first, y.second};
  int a = can[0];
  for (int u : can)
    if (dep[u] > dep[a])
      a = u;
  int b = -1;
  for (int u : can)
    if (!isanc(a, u)) {
      if (b == -1)  b = u;
      if (dep[b] < dep[u])    b = u;
    }
  if (b == -1) {
    b = can[0];
    for (int u : can)
      if (dep[u] < dep[b])
        b = u;
    return {a, b};
  }
  int g = lca(a, b);
  for (int u : can) {
    if (u == a || u == b)   continue;
    if (dep[u] < dep[g] || (!isanc(a, u) && !isanc(b, u)))    return { -1, -1};
  }
  return {a, b};
}
int pos[N];
struct segtree {
  pair<int, int> t[4 * N];
  void build(int n, int b, int e) {
    if (b == e) {
      t[n] = make_pair(pos[b], pos[b]);
      return;
    }
    int l = n << 1, r = l + 1, mid = b + (e - b) / 2;
    build(l, b, mid);
    build(r, mid + 1, e);
    t[n] = merge(t[l], t[r]);
  }
  void upd(int n, int b, int e, int i, int x) {
    if (b > i || e < i) return;
    if (b == e && b == i) {
      t[n] = make_pair(pos[x], pos[x]);
      return;
    }
    int l = n << 1, r = l + 1, mid = b + (e - b) / 2;
    upd(l, b, mid, i, x);
    upd(r, mid + 1, e, i, x);
    t[n] = merge(t[l], t[r]);
  }
  int query(int n, int b, int e, pair<int, int> p) {
    if (b == e) return (merge(p, t[n]).first == -1 ? b - 1 : b);
    int l = n << 1, r = l + 1, mid = b + (e - b) / 2;
    pair<int, int> nw = merge(p, t[l]);
    if (nw.first == -1) return query(l, b, mid, p);
    else return query(r, mid + 1, e, nw);
  }
} t;
int a[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int i, j, k, n, m, q;
  cin >> n;
  for (i = 1; i <= n; i++) cin >> k, pos[k + 1] = i, a[i] = k + 1;
  for (i = 2; i <= n; i++) cin >> k, g[i].push_back(k), g[k].push_back(i);
  dfs(1, 0);
  t.build(1, 1, n);
  cin >> q;
  while (q--) {
    int ty;
    cin >> ty;
    if (ty == 1) {
      cin >> j >> k;
      t.upd(1, 1, n, a[j], a[k]);
      t.upd(1, 1, n, a[k], a[j]);
      swap(a[j], a[k]);
      swap(pos[a[j]], pos[a[k]]);
    } else cout << t.query(1, 1, n, make_pair(0, 0)) << '\n';
  }
  return 0;
}
// https://codeforces.com/problemset/problem/1083/C