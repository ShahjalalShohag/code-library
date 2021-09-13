#include<bits/stdc++.h>
using namespace std;

const int N = 1010;
const double inf = 1e9;
const double eps = 1e-9;
struct Dinic {
  struct edge {
    int to, rev;
    double flow, w;
    int id;
  };
  int n, s, t, mxid;
  vector<int> d, flow_through;
  vector<int> done;
  vector<vector<edge>> g;
  Dinic() {}
  Dinic(int _n) {
    n = _n + 10;
    mxid = 0;
    g.resize(n);
  }
  void add_edge(int u, int v, double w, int id = -1) {
    edge a = {v, (int)g[v].size(), 0, w, id};
    edge b = {u, (int)g[u].size(), 0, 0, -2};//for bidirectional edges cap(b) = w
    g[u].emplace_back(a);
    g[v].emplace_back(b);
    mxid = max(mxid, id);
  }
  bool bfs() {
    d.assign(n, -1);
    d[s] = 0;
    queue<int> q;
    q.push(s);
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (auto &e : g[u]) {
        int v = e.to;
        if (d[v] == -1 && e.w - e.flow > eps) d[v] = d[u] + 1, q.push(v);
      }
    }
    return d[t] != -1;
  }
  double dfs(int u, double flow) {
    if (u == t) return flow;
    for (int &i = done[u]; i < (int)g[u].size(); i++) {
      edge &e = g[u][i];
      if (e.w - e.flow > eps) {
        int v = e.to;
        if (d[v] == d[u] + 1) {
          double nw = dfs(v, fmin(flow, e.w - e.flow));
          if (nw > 0) {
            e.flow += nw;
            g[v][e.rev].flow -= nw;
            return nw;
          }
        }
      }
    }
    return 0;
  }
  double max_flow(int _s, int _t) {
    s = _s;
    t = _t;
    double flow = 0;
    while (bfs()) {
      done.assign(n, 0);
      while (1) {
        double nw = dfs(s, inf);
        if (nw < eps) break;
        flow += nw;
      }
    }
    return flow;
  }
} F;
int s, t, n, m, u[N], v[N], deg[N];
const double M = 1e9;
bool vis[N];
void dfs(int u) {
  if (vis[u]) return;
  vis[u] = 1;
  for (auto e : F.g[u]) {
    if (e.w - e.flow > eps) dfs(e.to);
  }
}
bool yo(double g) {
  F = Dinic(t);
  for (int i = 1; i <= m; i++) {
    F.add_edge(u[i], v[i], 1);
    F.add_edge(v[i], u[i], 1);
  }
  for (int i = 1; i <= n; i++) {
    F.add_edge(s, i, m);
    F.add_edge(i, t, m + 2.0 * g - deg[i]);
  }
  double flow = F.max_flow(s, t); //flow = m * n + 2 * |nodes in s part without s in the mincut| * (g - density)
  return (1.0 * m * n - flow) > eps;
}
int32_t main() {
  //ios_base::sync_with_stdio(0);
  //cin.tie(0);
  bool ff = 0;
  while (~scanf("%d %d", &n, &m)) {
    if (ff) cout << '\n';
    ff = 1;
    if (!m) {
      cout << 1 << '\n' << 1 << '\n';
      continue;
    }
    s = n + 1, t = s + 1;
    memset(deg, 0, sizeof deg);
    for (int i = 1; i <= m; i++) {
      cin >> u[i] >> v[i];
      deg[u[i]]++;
      deg[v[i]]++;
    }
    double l = 0, r = m;
    double f = 1.0 / n / (n - 1);
    vector<int> ans;
    while (r - l >= f) {
      double mid = (l + r) * 0.5;
      int ok = yo(mid); //just check ok if you don't need to extract the nodes
      memset(vis, 0, sizeof vis);
      dfs(s);
      vector<int> nw;
      for (int i = 1; i <= n; i++) if (vis[i]) nw.push_back(i);
      if (!nw.empty()) l = mid, ans = nw;
      else r = mid;
    }
    sort(ans.begin(), ans.end()); //(l + r) * 0.5 is the maximum density
    cout << ans.size() << '\n'; //all nodes of the subgraph
    for (auto x : ans) cout << x << '\n';
  }
  return 0;
}
//https://vjudge.net/problem/UVA-1389
