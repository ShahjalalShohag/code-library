#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

const int N = 205, mod = 1000210433;

int power(long long n, long long k) {
  int ans = 1;
  while (k) {
    if (k & 1) ans = (long long) ans * n % mod;
    n = (long long) n * n % mod;
    k >>= 1;
  }
  return ans;
}
int det(vector<vector<int>> a) {
  int n = a.size(), m = (int)a[0].size();
  int free_var = 0;
  const long long MODSQ = (long long)mod * mod;
  int det = 1, rank = 0;
  for (int col = 0, row = 0; col < m && row < n; col++) {
    int mx = row;
    for (int k = row; k < n; k++) if (a[k][col] > a[mx][col]) mx = k;
    if (a[mx][col] == 0) {
      det = 0;
      continue;
    }
    for (int j = col; j < m; j++) swap(a[mx][j], a[row][j]);
    if (row != mx) det = det == 0 ? 0 : mod - det;
    det = 1LL * det * a[row][col] % mod;
    int inv = power(a[row][col], mod - 2);
    for (int i = 0; i < n && inv; i++) {
      if (i != row && a[i][col]) {
        int x = ((long long)a[i][col] * inv) % mod;
        for (int j = col; j < m && x; j++) {
          if (a[row][j]) a[i][j] = (MODSQ + a[i][j] - ((long long)a[row][j] * x)) % mod;
        }
      }
    }
    row++;
    ++rank;
  }
  return det;
}
struct dsu {
  vector<int> par, rnk, size;
  int c;
  dsu(int n) : par(n + 1), rnk(n + 1, 0), size(n + 1, 1), c(n) {
    for (int i = 1; i <= n; ++i) par[i] = i;
  }
  int find(int i) {
    return (par[i] == i ? i : (par[i] = find(par[i])));
  }
  bool same(int i, int j) {
    return find(i) == find(j);
  }
  int get_size(int i) {
    return size[find(i)];
  }
  int count() {
    return c;    //connected components
  }
  int merge(int i, int j) {
    if ((i = find(i)) == (j = find(j))) return -1;
    else --c;
    if (rnk[i] > rnk[j]) swap(i, j);
    par[i] = j;
    size[j] += size[i];
    if (rnk[i] == rnk[j]) rnk[j]++;
    return j;
  }
};
int w[N][N], deg[N], vis[N];
vector<int> g[N];
//w[u][v]++, w[v][u]++, deg[u]++,deg[v]++
inline void add2(int u, int v) {
  w[u][v]++;
  deg[u]++;
}
int f;
inline void add(int u, int v) {
  if(u == v) return;
  f = 1;
  g[u].push_back(v);
  g[v].push_back(u);
}
gp_hash_table<int, int> mp;
int get_id(int k) {
  if(mp.find(k) == mp.end()) {
    mp[k];
    mp[k] = mp.size();
  }
  return mp[k];
}
void dfs(int u) {
  vis[u] = 1;
  for(auto v : g[u]) {
    if(!vis[v]) dfs(v);
    add2(get_id(u), get_id(v));
  }
}
// number of maximum spanning trees
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t, cs = 0;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    vector< array<int, 3> > ed;
    dsu d(n);
    for(int i = 1; i <= m; i++) {
      int u, v, w;
      cin >> u >> v >> w;
      d.merge(u, v);
      ed.push_back({w, u, v});
    }
    if (d.count() != 1) {
      cout << "Case " << ++cs << ": " << 0 << '\n';
      continue;
    }
    sort(ed.rbegin(), ed.rend());
    d = dsu(n);
    int ans = 1;
    for(int i = 0, j; i < m; i = j) {
      memset(vis, 0, sizeof vis);
      for(int i = 1; i <= n; i++) g[i].clear();
      f = 0;
      add(d.find(ed[i][1]), d.find(ed[i][2]));
      for(j = i + 1; j < m && ed[i][0] == ed[j][0]; j++) add(d.find(ed[j][1]), d.find(ed[j][2]));
      if (!f) continue;
      memset(w, 0, sizeof w);
      memset(deg, 0, sizeof deg);
      for(int u = 1; u <= n; u++) if(!vis[u]) {
          mp.clear();
          dfs(u);
          int sz = mp.size();
          if(sz <= 1) {
            w[1][1] = 0;
            deg[1] = 0;
            continue;
          }
          for(int u = 1; u <= sz; u++) for(int v = 1; v <= sz; v++) {
              if(u == v) w[u][v] = deg[u];
              else w[u][v] = (mod - w[u][v]) % mod;
            }
          vector< vector<int> > mat(sz - 1, vector<int> (sz - 1, 0));
          for(int i = 1; i < sz; i++) for(int j = 1; j < sz; j++) mat[i - 1][j - 1] = w[i][j];
          ans = 1LL * ans * det(mat) % mod;
          for (int i = 1; i <= sz; i++) {
            for (int j = 1; j <= sz; j++) {
              w[i][j] = 0;
            }
            deg[i] = 0;
          }
        }
      for(int k = i; k < j; k++) d.merge(ed[k][1], ed[k][2]);
    }
    cout << "Case " << ++cs << ": " << ans << '\n';
  }
  return 0;
}
// https://lightoj.com/contest/loj-homecoming/arena/problem/13
