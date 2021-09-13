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
//flow_through[i] = extra flow beyond 'low' sent through edge i
struct LR_Flow {
  Dinic F;
  int n, s, t;
  struct edge {
    int u, v, l, r, id;
  };
  vector<edge> edges;
  LR_Flow() {}
  LR_Flow(int _n) {
    n = _n + 10;
    s = n - 2, t = n - 1;;
    edges.clear();
  }
  void add_edge(int u, int v, int l, int r, int id = -1) {
    assert(0 <= l && l <= r);
    edges.push_back({u, v, l, r, id});
  }
  bool feasible(int _s = -1, int _t = -1, int L = -1, int R = -1) {
    if (L != -1) edges.push_back({_t, _s, L, R, -1});
    F = Dinic(n);
    long long target = 0;
    for (auto e : edges) {
      int u = e.u, v = e.v, l = e.l, r = e.r, id = e.id;
      if (l != 0) {
        F.add_edge(s, v, l);
        F.add_edge(u, t, l);
        target += l;
      }
      F.add_edge(u, v, r - l, id);
    }
    auto ans = F.max_flow(s, t);
    if (L != -1) edges.pop_back();
    if (ans < target) return 0; //not feasible
    return 1;
  }
  int max_flow(int _s, int _t) { //-1 means flow is not feasible
    int mx = 1e5 + 9;
    if (!feasible(_s, _t, 0, mx)) return -1;
    return F.max_flow(_s, _t);
  }
  int min_flow(int _s, int _t) { //-1 means flow is not feasible
    int mx = 1e9;
    int ans = -1, l = 0, r = mx;
    while (l <= r) {
      int mid = l + r >> 1;
      if (feasible(_s, _t, 0, mid)) ans = mid, r = mid - 1;
      else l = mid + 1;
    }
    return ans;
  }
};
int get_id(map<int, int> &mp, int k) {
  if (mp.find(k) == mp.end()) mp[k], mp[k] = mp.size();
  return mp[k];
}
int Lx[N], Rx[N], Ly[N], Ry[N], degx[N], degy[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  LR_Flow F(2 * n + 10);
  int r, b;
  cin >> r >> b;
  int sp = r > b;
  if (sp) swap(r, b);
  map<int, int> mx, my;
  for (int i = 1; i <= n; i++) {
    int x, y;
    cin >> x >> y;
    if (sp) swap(x, y);
    F.add_edge(get_id(mx, x), get_id(my, y) + n, 0, 1, i);
    degx[mx[x]]++;
    degy[my[y]]++;
  }
  for (int i = 1; i <= mx.size(); i++) Lx[i] = 0, Rx[i] = degx[i];
  for (int i = 1; i <= my.size(); i++) Ly[i] = 0, Ry[i] = degy[i];
  while (m--) {
    int ty, x, d;
    cin >> ty >> x >> d;
    ty--;
    ty ^= sp;
    if (ty == 0) {
      if (mx.find(x) != mx.end()) {
        int i = mx[x];
        int p = degx[i];
        int l = (p - d + 1) / 2, r = (p + d) / 2;
        l = max(0, l);
        r = min(p, r);
        Lx[i] = max(Lx[i], l);
        Rx[i] = min(Rx[i], r);
      }
    } else {
      if (my.find(x) != my.end()) {
        int i = my[x];
        int p = degy[i];
        int l = (p - d + 1) / 2, r = (p + d) / 2;
        l = max(0, l);
        r = min(p, r);
        Ly[i] = max(Ly[i], l);
        Ry[i] = min(Ry[i], r);
      }
    }
  }
  int s = 2 * n + 2, t = s + 1;
  for (int i = 1; i <= mx.size(); i++) {
    if (Lx[i] > Rx[i]) return cout << -1 << '\n', 0;
    F.add_edge(s, i, Lx[i], Rx[i]);
  }
  for (int i = 1; i <= my.size(); i++) {
    if (Ly[i] > Ry[i]) return cout << -1 << '\n', 0;
    F.add_edge(i + n, t, Ly[i], Ry[i]);
  }
  int c = F.max_flow(s, t);
  if (c == -1) return cout << -1 << '\n', 0;
  long long ans = 1LL * c * r + 1LL * (n - c) * b;
  cout << ans << '\n';
  for (int i = 1; i <= n; i++) {
    cout << "br"[F.F.flow_through[i] ^ sp];
  }
  cout << '\n';
  return 0;
}
//https://codeforces.com/contest/704/problem/D
