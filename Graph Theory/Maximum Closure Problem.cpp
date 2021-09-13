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
};
struct MaximumClosure { //answer could be an empty set and could be disconnected
  MaximumClosure() {}
  int n;
  vector<bool> vis;
  vector<vector<int>> g, r, G;//G is the condensed graph
  vector<int> vec, idx;
  vector<long long> w;
  MaximumClosure(int _n) {
    n = _n ;
    vis.assign(n + 1, 0);
    idx.assign(n + 1, 0);
    g.resize(n + 1);
    r.resize(n + 1);
    G.resize(n + 1);
    vec.clear();
    w.assign(n + 1, 0);
  }
  void add_edge(int u, int v) {
    g[u].push_back(v);
    r[v].push_back(u);
  }
  void dfs1(int u) {
    vis[u] = 1;
    for (auto v : g[u]) if(!vis[v]) dfs1(v);
    vec.push_back(u);
  }
  vector<int> comp;
  void dfs2(int u) {
    comp.push_back(u);
    vis[u] = 1;
    for (auto v : r[u]) if(!vis[v]) dfs2(v);
  }
  long long solve() {
    for(int i = 1; i <= n; i++) if(!vis[i]) dfs1(i);
    reverse(vec.begin(), vec.end());
    vis.assign(n, 0);
    int scc = 0;
    for (auto u : vec) {
      if (!vis[u]) {
        comp.clear();
        dfs2(u);
        scc++;
        for(auto x : comp) idx[x] = scc;
      }
    }
    vector<int> nw(n + 1, 0);
    for (int i = 1; i <= n; i++) nw[idx[i]] += w[i];
    Dinic F(scc + 3);
    int s = scc + 1, t = scc + 2;
    for (int u = 1; u <= n; u++) {
      for (auto v : g[u]) {
        if(idx[u] != idx[v]) {
          F.add_edge(idx[u], idx[v], inf);
        }
      }
    }
    long long sum = 0;
    for (int i = 1; i <= scc; i++) {
      if (nw[i] < 0) F.add_edge(i, t, -nw[i]);
      else F.add_edge(s, i, nw[i]), sum += nw[i];
    }
    return sum - F.max_flow(s, t);
  }
};
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  MaximumClosure M(n);
  for (int i = 1; i <= n; i++) cin >> M.w[i];
  while (m--) {
    int u, v;
    cin >> u >> v;
    M.add_edge(u, v);
  }
  cout << M.solve() << '\n';
  return 0;
}
