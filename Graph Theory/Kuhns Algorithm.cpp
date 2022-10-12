#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct Kuhn {
  int n;
  vector<vector<int>> g;
  vector<int> l, r;
  vector<bool> vis;
  Kuhn(int _n, int _m) {
    n = _n;
    g.resize(n + 1);
    vis.resize(n + 1, false);
    l.resize(n + 1, -1);
    r.resize(_m + 1, -1);
  }
  void add_edge(int a, int b) {
    g[a].push_back(b);
  }
  bool yo(int u) {
    if (vis[u])  return false;
    vis[u] = true;
    for (auto v : g[u]) {
      if (r[v] == -1 || yo(r[v])) {
        l[u] = v;
        r[v] = u;
        return true;
      }
    }
    return false;
  }
  int maximum_matching() {
    for (int i = 1; i <= n; i++) shuffle(g[i].begin(), g[i].end(), rnd);
    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    shuffle(p.begin(), p.end(), rnd);
    bool ok = true;
    while (ok) {
      ok = false;
      vis.assign(n + 1, false);
      for (auto &i : p) if(l[i] == -1) ok |= yo(i);
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) ans += l[i] != -1;
    return ans;
  }
};
bool vis[N];
vector<int> g[N], r[N], G[N], vec; //G is the condensed graph
void dfs1(int u) {
  vis[u] = 1;
  for(auto v : g[u]) if(!vis[v]) dfs1(v);
  vec.push_back(u);
}

vector<int> comp;
void dfs2(int u) {
  comp.push_back(u);
  vis[u] = 1;
  for(auto v : r[u]) if(!vis[v]) dfs2(v);
}
int idx[N];

// Given a graph having perfect matching, find the number of edges which are not in all Maximum Matchings in a Bipartite Graph
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m;
  cin >> n >> m;
  Kuhn M(n, n);
  multiset<pair<int, int>> edges;
  for (int i = 1; i <= m; i++) {
    int u, v;
    cin >> u >> v; //u -> left side, v -> right side
    M.add_edge(u, v);
    edges.insert({u, v});
  }
  int cnt = M.maximum_matching();
  assert(cnt == n);
  for (int i = 1; i <= n; i++) {
    int u = i, v = M.l[i];
    edges.erase(edges.find({u, v})); //u -> v for matched edges
    g[u].push_back(v + n);
    r[v + n].push_back(u);
  }
  for (auto e : edges) {
    g[e.second + n].push_back(e.first); //v -> u for unmatched edges
    r[e.first].push_back(e.second + n);
  }
  for(int i = 1; i <= 2 * n; i++) if(!vis[i]) dfs1(i);
  reverse(vec.begin(), vec.end());
  memset(vis, 0, sizeof vis);
  int scc = 0;
  for(auto u : vec) {
    if(!vis[u]) {
      comp.clear();
      dfs2(u);
      scc++;
      for(auto x : comp) idx[x] = scc;
    }
  }
  int ans = m - n;
  for (int i = 1; i <= n; i++) {
    int u = i, v = M.l[i];
    if (idx[u] == idx[v + n]) ans++; //if nodes of a matched edge are in the same SCC then we can replace it
  }
  cout << ans << '\n';
  return 0;
}
//https://open.kattis.com/problems/justenoughbridges
