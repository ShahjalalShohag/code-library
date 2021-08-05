#include<bits/stdc++.h>
using namespace std;

const int N = 42;

// 0-indexed
// O(1.38 ^ n) worst case, but works for up to n <= 60 or more
int g[N][N], n;
int mn_deg, comp_size;
bitset<N> st;
vector<int> adj[N];
bool vis[N];
int get_deg(int u) {
  int res = 0;
  for (int v = 0; v < n; v++) if (st[v]) res += g[u][v];
  return res;
}
void dfs(int u) {
  vis[u] = 1;
  mn_deg = min(mn_deg, (int)adj[u].size());
  comp_size++;
  for (int v : adj[u]) if (!vis[v]) dfs(v);
}
int brute() {
  for (int u = 0; u < n; u++) if (st[u]) vis[u] = 0, adj[u].clear();
  for (int u = 0; u < n; u++) {
    if (st[u]) for (int v = 0; v < n; v++) if (g[u][v] && st[v]) adj[u].push_back(v);
  }
  int res = 0;
  for (int u = 0; u < n; u++) {
    if (st[u] && !vis[u]) {
      mn_deg = N;
      comp_size = 0;
      dfs(u);
      if (mn_deg <= 1) res += ((comp_size + 1) / 2);
      else res += (comp_size / 2);
    }
  }
  return res;
}
int yo() {
  if (!st.count()) return 0;
  int d = -1;
  for (int v = 0; v < n; v++) {
    if (st[v] && (d == -1 || get_deg(v) > get_deg(d))) d = v;
  }
  if (get_deg(d) <= 2) return brute();
  int ret = 0;
  bitset<N> prv = st;
  st[d] = 0;
  ret = max(ret, yo());
  st = prv;
  st[d] = 0;
  for (int u = 0; u < n; u++) if (g[u][d]) st[u] = 0;
  ret = max(ret, 1 + yo());
  st = prv;
  return ret;
}
int max_anticlique(int k) {
  n = k;
  for (int i = 0; i < n; i++) st[i] = 1;
  return yo();
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  map<string, int> mp;
  set<int>se;
  int n, m;
  cin >> n >> m;
  for (int i = 1; i <= n + 1; i++) {
    int ty;
    if (i <= n) cin >> ty;
    if (ty == 1 || i > n) {
      for (auto x : se) for (auto y : se) g[x - 1][y - 1] = 1, g[y - 1][x - 1] = 1;
      se.clear();
    } else {
      string s;
      cin >> s;
      if (mp.find(s) == mp.end()) {
        mp[s];
        mp[s] = mp.size();
      }
      int p = mp[s];
      se.insert(p);
    }
  }
  for (int i = 0; i < m; i++) g[i][i] = 0;
  cout << max_anticlique(m) << '\n';
  return 0;
}
// https://codeforces.com/contest/1105/problem/E
