#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9, M = N * 2 + N * 19 * 2;

vector<pair<int, int>> g[N * 2], G[N];
inline void add_edge(int u, int v, int w) {
  g[u].push_back({v, w});
}
int T;
void binarize(int u, int p = 0) {
  int last = 0, tmp = 0;
  for (auto e : G[u]) {
    int v = e.first, w = e.second;
    if (v == p) continue;
    ++tmp;
    if (tmp == 1) {
      add_edge(u, v, w);
      add_edge(v, u, w);
      last = u;
    }
    else if (tmp == ((int) G[u].size()) - (u != 1)) {
      add_edge(last, v, w);
      add_edge(v, last, w);
    } 
    else {
      T++;
      add_edge(last, T, 0);
      add_edge(T, last, 0);
      last = T;
      add_edge(T, v, w);
      add_edge(v, T, w);
    }
  }
  for (auto e : G[u]) {
    int v = e.first;
    if (v == p) continue;
    binarize(v, u);
  }
}
int sz[N * 2];
int tot, done[N * 2], cenpar[N * 2];
void calc_sz(int u, int p) {
  tot ++; sz[u] = 1;
  for (auto e: g[u]) {
    int v = e.first;
    if(v == p || done[v]) continue;
    calc_sz(v, u);
    sz[u] += sz[v];
  }
}
int find_cen(int u, int p) {
  for (auto e: g[u]) {
    int v = e.first;
    if(v == p || done[v]) continue;
    else if(sz[v] > tot / 2) return find_cen(v, u);
  }
  return u;
}
long long d[20][N * 2];
void yo(int u, int p, long long nw, int l) {
  d[l][u] = nw;
  for(auto e : g[u]) {
    int v = e.first; long long w = e.second;
    if (v == p || done[v]) continue;
    yo(v, u, nw + w, l);
  }
}
int st[N * 2], en[N * 2], DT;
struct node {
  vector<int> ct; //adjacent edges in centroid tree
  int level = 0, id = 0, cnt = 0;
  long long sum = 0, parsum = 0;
}t[M];
int decompose(int u, int p = 0, int l = 0) {
  tot = 0;
  calc_sz(u, p);
  int cen = find_cen(u, p);
  cenpar[cen] = p; done[cen] = 1;
  u = cen; st[u] = ++DT;
  t[u].id = u; t[u].level = l;
  yo(u, p, 0, l);
  for (auto e: g[u]) {
    int v = e.first;
    if(v == p || done[v]) continue;
    int x = decompose(v, u, l + 1);
    t[u].ct.push_back(x);
  }
  en[u] = DT;
  return u;
}
int insub(int r, int c) {
  r = t[r].id, c = t[c].id;
  return st[r] <= st[c] && en[c] <= en[r];
}
int upd(int cur, int u) { //update node u in cur tree
  T++; assert(T < M);
  t[T] = t[cur]; cur = T;
  t[cur].cnt++;
  t[cur].sum += d[t[cur].level][u];
  for(auto &v : t[cur].ct) if(insub(v, u)) {
    v = upd(v, u);
    t[v].parsum += d[t[cur].level][u];
  }
  return cur;
}
long long query(int cur, int u) { //query on cur tree
  long long ans = 0;
  while (t[cur].id != t[u].id) {
    int v = 0;
    for (auto x: t[cur].ct) if(insub(x, u)) v = x;
    assert(v);
    ans += d[t[cur].level][t[u].id] * (t[cur].cnt - t[v].cnt);
    ans += t[cur].sum - t[v].parsum;
    cur = v;
  }
  ans += t[cur].sum;
  return ans;
}
int a[N], root[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  for(int i = 1; i <= n; i++) cin >> a[i];
  for (int i = 1; i < n; i++) {
    int u, v, w; cin >> u >> v >> w;
    G[u].push_back({v, w});
    G[v].push_back({u, w});
  }
  T = n;
  binarize(1);
  root[0] = decompose(1);
  for(int i = 1; i <= n; i++) root[i] = upd(root[i - 1], a[i]);
  long long ans = 0;
  const int mod = 1 << 30;
  while(q--) {
    int ty; cin >> ty;
    if(ty == 1) {
      int l, r, u; cin >> l >> r >> u;
      l ^= ans;
      r ^= ans;
      u ^= ans;
      ans = query(root[r], u) - query(root[l - 1], u);
      cout << ans << '\n';
      ans %= mod;
    }
    else {
      int x; cin >> x;
      x ^= ans;
      root[x] = upd(root[x - 1] , a[x + 1]);
      swap(a[x], a[x + 1]);
    }
  }
  return 0;
}
//https://codeforces.com/contest/757/problem/G
