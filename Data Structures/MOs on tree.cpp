#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

//unique elements on the path from u to v
vector<int> g[N];
int st[N], en[N], T, par[N][20], dep[N], id[N * 2];
void dfs(int u, int p = 0) {
  st[u] = ++T;
  id[T] = u;
  dep[u] = dep[p] + 1;
  par[u][0] = p;
  for(int k = 1; k < 20; k++) par[u][k] = par[par[u][k - 1]][k - 1];
  for(auto v : g[u]) if(v != p) dfs(v, u);
  en[u] = ++T;
  id[T] = u;
}
int lca(int u, int v) {
  if(dep[u] < dep[v]) swap(u, v);
  for(int k = 19; k >= 0; k--) if(dep[par[u][k]] >= dep[v]) u = par[u][k];
  if(u == v) return u;
  for(int k = 19; k >= 0; k--) if(par[u][k] != par[v][k]) u = par[u][k], v = par[v][k];
  return par[u][0];
}

int cnt[N], a[N], ans;
inline void add(int u) {
  int x = a[u];
  if(cnt[x]++ == 0) ans++;
}
inline void rem(int u) {
  int x = a[u];
  if(--cnt[x] == 0) ans--;
}
bool vis[N];
inline void yo(int u) {
  if(!vis[u]) add(u);
  else rem(u);
  vis[u] ^= 1;
}

const int B = 320;
struct query {
  int l, r, id;
  bool operator < (const query &x) const {
    if(l / B == x.l / B) return r < x.r;
    return l / B < x.l / B;
  }
} Q[N];

int res[N];
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n, q;
  while(cin >> n >> q) {
    for(int i = 1; i <= n; i++) cin >> a[i];
    map<int, int> mp;
    for(int i = 1; i <= n; i++) {
      if(mp.find(a[i]) == mp.end()) mp[a[i]], mp[a[i]] = mp.size();
      a[i] = mp[a[i]];
    }
    for(int i = 1; i < n; i++) {
      int u, v;
      cin >> u >> v;
      g[u].push_back(v);
      g[v].push_back(u);
    }
    T = 0;
    dfs(1);
    for(int i = 1; i <= q; i++) {
      int u, v ;
      cin >> u >> v;
      if(st[u] > st[v]) swap(u, v);
      int lc = lca(u, v);
      if(lc == u) Q[i].l = st[u], Q[i].r = st[v];
      else Q[i].l = en[u], Q[i].r = st[v];
      Q[i].id = i;
    }
    sort(Q + 1, Q + q + 1);
    ans = 0;
    int l = 1, r = 0;
    for(int i = 1; i <= q; i++) {
      int L = Q[i].l, R = Q[i].r;
      if(R < l) {
        while (l > L) yo(id[--l]);
        while (l < L) yo(id[l++]);
        while (r < R) yo(id[++r]);
        while (r > R) yo(id[r--]);
      } else {
        while (r < R) yo(id[++r]);
        while (r > R) yo(id[r--]);
        while (l > L) yo(id[--l]);
        while (l < L) yo(id[l++]);
      }
      int u = id[l], v = id[r], lc = lca(u, v);
      if(lc != u && lc != v) yo(lc); //take care of the lca separately
      res[Q[i].id] = ans;
      if(lc != u && lc != v) yo(lc);
    }
    for(int i = 1; i <= q; i++) cout << res[i] << '\n';
    for(int i = 0; i <= n; i++) {
      g[i].clear();
      vis[i] = cnt[i] = 0;
      for(int k = 0; k < 20; k++) par[i][k] = 0;
    }
  }
  return 0;
}
// https://www.spoj.com/problems/COT2/
