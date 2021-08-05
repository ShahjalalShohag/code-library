#include <bits/stdc++.h>
using namespace std;
typedef long long ll;

const int N = 505;
const int M = 200010;
const ll INF = 1e18 + 5;

ll d[N][N], W[M];
vector <int> g[N];
int n, m, U[M], V[M], id[N], par[N];
ll ed, far, global = LLONG_MAX, dist[N];

int main() {
  cin >> n >> m;
  memset(d, 63, sizeof d);
  for (int i = 1; i <= n; ++i) {
    d[i][i] = 0, id[i] = par[i] = i;
  }
  for (int i = 1; i <= m; ++i) {
    scanf("%d %d %lld", U + i, V + i, W + i);
    if (U[i] > V[i]) swap(U[i], V[i]);
    W[i] <<= 1LL; // double the weight
    int u = U[i], v = V[i]; ll w = W[i];
    d[u][v] = min(d[u][v], w);
    d[v][u] = min(d[v][u], w);
    g[u].emplace_back(i), g[v].emplace_back(i);
  }
  if (n == 1) {
    cout << 0 << '\n';
    return 0;
  }
  for (int k = 1; k <= n; ++k) {
    for (int i = 1; i <= n; ++i) {
      for (int j = 1; j <= n; ++j) {
        d[i][j] = min(d[i][j], d[i][k] + d[k][j]);
      }
    }
  }
  for (int u = 1; u <= n; ++u) {
    sort(id + 1, id + n + 1, [&] (int i, int j) {return d[u][i] > d[u][j];});
    for (int e : g[u]) {
      int v = U[e] ^ u ^ V[e]; ll w = W[e];
      if (v < u) continue; 
      int last = id[1];
      ll opt = 0, minVal = d[u][last];
      for (int it = 2; it <= n; ++it) {
        int i = id[it];
        if (d[v][i] <= d[v][last]) continue; 
        ll curX = (w - d[u][i] + d[v][last]) / 2;
        ll curY = (w + d[u][i] + d[v][last]) / 2;
        if (minVal > curY) minVal = curY, opt = curX; 
        last = i;
      }
      if (minVal > d[v][last]) minVal = d[v][last], opt = w;
      if (minVal < global) global = minVal, ed = e, far = opt;
    }
  }
  printf("%lld\n", global);
  // lets extract the tree
  ++n;
  ++m, U[m] = U[ed], V[m] = n, W[m] = far;
  g[U[m]].emplace_back(m), g[V[m]].emplace_back(m);
  ++m, U[m] = V[ed], V[m] = n, W[m] = W[ed] - far; 
  g[U[m]].emplace_back(m), g[V[m]].emplace_back(m);
  priority_queue <pair <ll, int>> pq;
  pq.emplace(0, n);
  for (int i = 1; i < n; ++i) dist[i] = INF;
  while (!pq.empty()) {
    int u = pq.top().second; pq.pop();
    for (int e : g[u]) if (e ^ ed) {
      int v = U[e] ^ u ^ V[e]; ll w = W[e];
      if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w, par[v] = u, pq.emplace(-dist[v], v);
      }
    }
  }
  if (U[ed] > V[ed]) swap(U[ed], V[ed]);
  printf("%d %d\n", U[ed], V[ed]);
  for (int i = 1; i < n; ++i) if (par[i] ^ n) {
    int u = i, v = par[i];
    if (u > v) swap(u, v); 
    printf("%d %d\n", u, v);
  }
  return 0;
}
// https://www.spoj.com/problems/PT07C/
