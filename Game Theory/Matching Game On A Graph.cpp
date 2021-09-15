#include<bits/stdc++.h>
using namespace std;

const int N = 2e3 + 9;

mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct Blossom {
  int vis[N], par[N], orig[N], match[N], aux[N], t;
  int n;
  bool ad[N];
  vector<int> g[N];
  queue<int> Q;
  Blossom() {}
  Blossom(int _n) {
    n = _n;
    t = 0;
    for (int i = 0; i <= _n; ++i) {
      g[i].clear();
      match[i] = aux[i] = par[i] = vis[i] = aux[i] = ad[i] = orig[i] = 0;
    }
  }
  void add_edge(int u, int v) {
    g[u].push_back(v);
    g[v].push_back(u);
  }
  void augment(int u, int v) {
    int pv = v, nv;
    do {
      pv = par[v];
      nv = match[pv];
      match[v] = pv;
      match[pv] = v;
      v = nv;
    } while (u != pv);
  }
  int lca(int v, int w) {
    ++t;
    while (true) {
      if (v) {
        if (aux[v] == t) return v;
        aux[v] = t;
        v = orig[par[match[v]]];
      }
      swap(v, w);
    }
  }
  void blossom(int v, int w, int a) {
    while (orig[v] != a) {
      par[v] = w;
      w = match[v];
      ad[v] = true;
      if (vis[w] == 1) Q.push(w), vis[w] = 0;
      orig[v] = orig[w] = a;
      v = par[w];
    }
  }
  //it finds an augmented path starting from u
  bool bfs(int u) {
    fill(vis + 1, vis + n + 1, -1);
    iota(orig + 1, orig + n + 1, 1);
    Q = queue<int> ();
    Q.push(u);
    vis[u] = 0;
    while (!Q.empty()) {
      int v = Q.front();
      Q.pop();
      ad[v] = true;
      for (int x : g[v]) {
        if (vis[x] == -1) {
          par[x] = v;
          vis[x] = 1;
          if (!match[x]) return augment(u, x), true;
          Q.push(match[x]);
          vis[match[x]] = 0;
        } else if (vis[x] == 0 && orig[v] != orig[x]) {
          int a = lca(orig[v], orig[x]);
          blossom(x, v, a);
          blossom(v, x, a);
        }
      }
    }
    return false;
  }
  int maximum_matching() {
    int ans = 0;
    vector<int> p(n - 1);
    iota(p.begin(), p.end(), 1);
    shuffle(p.begin(), p.end(), rnd);
    for (int i = 1; i <= n; i++) shuffle(g[i].begin(), g[i].end(), rnd);
    for (auto u : p) {
      if (!match[u]) {
        for(auto v : g[u]) {
          if (!match[v]) {
            match[u] = v, match[v] = u;
            ++ans;
            break;
          }
        }
      }
    }
    for(int i = 1; i <= n; ++i) if (!match[i] && bfs(i)) ++ans;
    return ans;
  }
} M;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    M = Blossom (n);
    while (m--) {
      int u, v;
      cin >> u >> v;
      M.add_edge(u, v);
    }
    int ans = M.maximum_matching();
    if (ans * 2 == n) cout << 0 << '\n';
    else {
      memset(M.ad, 0, sizeof M.ad);
      for (int i = 1; i <= n; i++) if (M.match[i] == 0) M.bfs(i);
      int ans = 0;
      for (int i = 1; i <= n; i++) ans += M.ad[i]; //nodes which are unmatched in some maximum matching
      cout << ans << '\n';
    }
  }
  return 0;
}
//https://www.codechef.com/problems/HAMILG
