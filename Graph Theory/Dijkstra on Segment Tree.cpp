#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

vector<pair<int, int>> g[N * 9];
inline void add_edge(int u, int v, int w) {
  g[u].push_back({v, w});
}
int add;
void build(int n, int b, int e) {
  if (b == e) {
    add_edge(b, n + add, 0);
    add_edge(n + add * 5, b, 0);
    return;
  }
  int mid = b + e >> 1;
  add_edge(2 * n + add, n + add, 0);
  add_edge(2 * n + 1 + add, n + add, 0);  
  add_edge(n + 5 * add, 2 * n + 5 * add, 0);
  add_edge(n + 5 * add, 2 * n + 1 + 5 * add, 0);
  build(2 * n, b, mid);
  build(2 * n + 1, mid + 1, e);
}
void upd(int n, int b, int e, int i, int j, int dir, int u, int w) {
  if (j < b || e < i) return;
  if (i <= b && e <= j) {
    if (dir) add_edge(u, n + 5 * add, w); // from u to this range
    else add_edge(n + add, u, w); // from this range to u
    return;
  }
  int mid = (b + e) >> 1;
  upd(2 * n, b, mid, i, j, dir, u, w);
  upd(2 * n + 1, mid + 1, e, i, j, dir, u, w);
}

vector<long long> dijkstra(int s) {
  const long long inf = 1e18;
  priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> q;
  vector<long long> d(9 * N + 1, inf); vector<bool> vis(9 * N + 1, 0);
  q.push({0, s});
  d[s] = 0;
  while(!q.empty()){
    auto x = q.top(); q.pop();
    int u = x.second;
    if(vis[u]) continue; vis[u] = 1;
    for(auto y: g[u]){
      int v = y.first; long long w = y.second;
      if(d[u] + w < d[v]){
        d[v] = d[u] + w; q.push({d[v], v});
      }
    }
  }
  return d;
}
long long ans[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q, s; cin >> n >> q >> s;
  add = n;
  build(1, 1, n);
  while (q--) {
    int ty; cin >> ty;
    int u, l, r, w;
    if (ty == 1) {
      cin >> u >> l >> w;
      r = l;
    }
    else {
      cin >> u >> l >> r >> w;
    }
    upd(1, 1, n, l, r, ty <= 2, u, w);
  }
  auto ans = dijkstra(s);
  for (int i = 1; i <= n; i++) {
    if (ans[i] == 1e18) ans[i] = -1;
    cout << ans[i] << ' ';
  }
  return 0;
}
// https://codeforces.com/contest/786/problem/B