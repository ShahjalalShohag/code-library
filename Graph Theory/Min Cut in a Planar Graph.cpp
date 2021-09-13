#include<bits/stdc++.h>
using namespace std;

const int N = 3e6 + 9;
vector<pair<int, int>> g[N];
vector<long long> dijkstra(int s, int t, vector<pair<int, int>> g[]) {
  int n = t;
  const long long inf = 1e18;
  priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> q;
  vector<long long> d(n + 1, inf);
  vector<bool> vis(n + 1, 0);
  q.push({0, s});
  d[s] = 0;
  while(!q.empty()) {
    auto x = q.top();
    q.pop();
    int u = x.second;
    if(vis[u]) continue;
    vis[u] = 1;
    for(auto y : g[u]) {
      int v = y.first;
      long long w = y.second;
      if(d[u] + w < d[v]) {
        d[v] = d[u] + w;
        q.push({d[v], v});
      }
    }
  }
  return d;
}
int n, s, t, a[N];
inline void add_edge(int u, int v) {
  if (u == -69 || v == -69 || u == v) return;
  int w = a[u] + a[v];
  g[u].push_back({v, w});
  g[v].push_back({u, w});
}
inline int id(int i, int j) {
  if (i < 0 || i >= n) return -69;
  if (j < 0) return s;
  if (j >= n) return t;
  return i * n + j;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> n;
  s = n * n + 1;
  t = s + 1;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      cin >> a[id(i, j)];
    }
  }
  for (int i = 0; i < n; i++) {
    for (int j = 0; j <= n; j++) {
      add_edge(id(i, j), id(i - 1, j));
      add_edge(id(i, j), id(i, j - 1));
    }
  }
  cout << dijkstra(s, t, g)[t] << '\n';
  return 0;
}
//https://www.spoj.com/problems/ADAHOSE/
