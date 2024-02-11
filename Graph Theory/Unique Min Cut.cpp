#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

const long long inf = 1LL << 61;
struct Dinic {
  struct edge {
    int to, rev;
    long long flow, w;
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
  void add_edge(int u, int v, long long w, int id = -1) {
    edge a = {v, (int)g[v].size(), 0, w, id};
    edge b = {u, (int)g[u].size(), 0, 0, -1};//for bidirectional edges cap(b) = w
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
        if (d[v] == -1 && e.flow < e.w) d[v] = d[u] + 1, q.push(v);
      }
    }
    return d[t] != -1;
  }
  long long dfs(int u, long long flow) {
    if (u == t) return flow;
    for (int &i = done[u]; i < (int)g[u].size(); i++) {
      edge &e = g[u][i];
      if (e.w <= e.flow) continue;
      int v = e.to;
      if (d[v] == d[u] + 1) {
        long long nw = dfs(v, min(flow, e.w - e.flow));
        if (nw > 0) {
          e.flow += nw;
          g[v][e.rev].flow -= nw;
          return nw;
        }
      }
    }
    return 0;
  }
  long long max_flow(int _s, int _t) {
    s = _s;
    t = _t;
    long long flow = 0;
    while (bfs()) {
      done.assign(n, 0);
      while (long long nw = dfs(s, inf)) flow += nw;
    }
    flow_through.assign(mxid + 10, 0);
    for(int i = 0; i < n; i++) for(auto e : g[i]) if(e.id >= 0) flow_through[e.id] = e.flow;
    return flow;
  }
} F;
bool vis[N];
void dfs1(int u) {
  if (vis[u]) return;
  vis[u] = 1;
  for (auto e : F.g[u]) {
    if (e.flow < e.w) {
      dfs1(e.to);
    }
  }
}
void dfs2(int v) {
  if (vis[v]) return;
  vis[v] = 1;
  for (auto e : F.g[v]) {
    if (F.g[e.to][e.rev].flow < F.g[e.to][e.rev].w) {
      dfs2(e.to);
    }
  }
}
//it assumes s and t are connected
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  // freopen("attack.in", "r", stdin);
  // freopen("attack.out", "w", stdout);
  int n, m, s, t;
  cin >> n >> m >> s >> t;
  F = Dinic(n);
  while (m--) {
    int u, v, w;
    cin >> u >> v >> w;
    F.add_edge(u, v, w, m);
    F.add_edge(v, u, w, m);
  }
  auto x = F.max_flow(s, t);
  dfs1(s);
  dfs2(t);
  for (int i = 1; i <= n; i++) if (!vis[i]) return cout << "AMBIGUOUS\n", 0;
  cout << "UNIQUE\n";
  return 0;
}
//https://codeforces.com/gym/100200/status/A
