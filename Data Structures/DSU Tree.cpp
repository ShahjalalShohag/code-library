#include<bits/stdc++.h>
using namespace std;

const int N = 5e5 + 9;

int st[N], en[N], a[N], par[N], rid[N], root[N];
struct ST {
  pair<int, int> t[4 * N];
  void build(int n, int b, int e) {
    if(b == e) {
      t[n] = {0, b};
      return;
    }
    int mid = (b + e) >> 1,  l = n << 1,  r = l | 1;
    build(l, b, mid);
    build(r, mid + 1, e);
    t[n] = max(t[l], t[r]);
  }
  void upd(int n, int b, int e, int i, int x) {
    if(b > i || e < i) return;
    if(b == e && b == i) {
      t[n] = {x, b};
      return;
    }
    int mid = (b + e) >> 1,  l = n << 1,  r = l | 1;
    upd(l, b, mid, i, x);
    upd(r, mid + 1, e, i, x);
    t[n] = max(t[l], t[r]);
  }
  pair<int, int> query(int n, int b, int e, int i, int j) {
    if(b > j || e < i) return {0, 0};
    if(b >= i && e <= j) return t[n];
    int mid = (b + e) >> 1,  l = n << 1,  r = l | 1;
    auto L = query(l, b, mid, i, j);
    auto R = query(r, mid + 1, e, i, j);
    return max(L, R);
  }
} t;

int find(int u) {
  if (par[u] == u) return u;
  return par[u] = find(par[u]);
}
int T, I;
vector<int> g[N];
void merge(int u, int v) {
  u = find(u);
  v = find(v);
  if (u == v) return;
  ++T;
  g[T].push_back(u);
  g[T].push_back(v);
  par[u] = T;
  par[v] = T;
  return;
}
bool vis[N];
void dfs(int u) {
  vis[u] = 1;
  st[u] = I + 1;
  for (auto v : g[u]) {
    dfs(v);
  }
  if (st[u] == I + 1) {
    ++I;
    rid[I] = u;
  }
  en[u] = I;
}
pair<int, int> ed[N];
int ty[N], x[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, q;
  cin >> n >> m >> q;
  T = n;
  for (int i = 1; i <= 2 * n; i++) par[i] = i;
  for (int i = 1; i <= n; i++) cin >> a[i];
  for (int i = 1; i <= m; i++) {
    int u, v;
    cin >> u >> v;
    ed[i] = {u, v};
  }
  set<int> se;
  for (int i = 1; i <= m; i++) {
    se.insert(i);
  }
  for (int i = 1; i <= q; i++) {
    cin >> ty[i] >> x[i];
    if (ty[i] == 2) {
      se.erase(x[i]);
    }
  }
  for (auto x : se) {
    merge(ed[x].first, ed[x].second);
  }
  for (int i = q; i >= 1; i--) {
    if (ty[i] == 2) {
      merge(ed[x[i]].first, ed[x[i]].second);
    } else root[i] = find(x[i]);
  }
  for (int i = T; i >= 1; i--) {
    if (!vis[i]) dfs(i);
  }
  t.build(1, 1, n);
  for (int i = 1; i <= n; i++) {
    t.upd(1, 1, n, i, a[rid[i]]);
  }
  for (int i = 1; i <= q; i++) {
    if (ty[i] == 1) {
      auto f = t.query(1, 1, n, st[root[i]], en[root[i]]);
      t.upd(1, 1, n, f.second, 0);
      cout << f.first << '\n';
    }
  }
  return 0;
}
// https://codeforces.com/contest/1416/submission/94027308
