#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;

struct DSU {
  vector<int> par, sz, w;
  vector<array<int, 3>> op;
  bool flag;
  DSU() {}
  DSU(int n) {
    par.resize(n + 1);
    sz.resize(n + 1);
    w.resize(n + 1);
    flag = true;
    for (int i = 1; i <= n; i++) {
      par[i] = i; 
      sz[i] = 1; w[i] = 0;
    }
  }
  bool is_bipartite() {
    return flag;
  }
  pair<int, int> find(int u) {
    int ans = 0;
    while (par[u] != u)  {
      ans ^= w[u];
      u = par[u];
    }
    return make_pair(u, ans);
  }
  bool merge(int u, int v) {
    auto pu = find(u), pv = find(v);
    u = pu.first;
    v = pv.first;
    int last = flag;
    int z = pu.second ^ pv.second ^ 1;
    if (u == v) {
      if (z) {
        flag = false;
      }
      op.push_back({-1, -1, last});
      return false;
    }
    if (sz[u] > sz[v]) {
      swap(u, v);
    }
    op.push_back({u, w[u], last});
    par[u] = v;
    w[u] = z;
    sz[v] += sz[u];
    return true;
  }
  void undo() {
    assert(!op.empty());
    auto x = op.back();
    flag = x[2];
    int u = x[0];
    if (u != -1) {
      sz[par[u]] -= sz[u];
      par[u] = u;
      w[u] = x[1];
    }
    op.pop_back();
  }
};
struct update {
  bool type;
  int x, y;
  update(int _x, int _y) {
    x = _x; y = _y;
    type = 0;
  }
};
struct DSUQueue {
  DSU D;
  vector<update> S;
  DSUQueue(int n) {
    D = DSU(n);
  }
  void push(update u) {
    D.merge(u.x, u.y);
    S.push_back(u);
  }
  void pop() {
    assert(!S.empty());
    vector<update> t[2];
    do {
      t[S.back().type].push_back(S.back());
      S.pop_back();
      D.undo();
    } while (t[1].size() < t[0].size() && !S.empty());
    if (t[1].empty()) {
      for (auto &u : t[0]) {
        u.type = 1;
        push(u);
      }
    } 
    else {
      for (int i : {0, 1}) {
        reverse(t[i].begin(), t[i].end());
        for (auto &u : t[i]) push(u);
      }
    }
    S.pop_back();
    D.undo();
  }
  bool is_bipartite() {
    return D.is_bipartite();
  }
};
int u[N], v[N], a[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m, q; cin >> n >> m >> q;
  DSU P(n);
  for (int i = 1; i <= m; i++) {
    cin >> u[i] >> v[i];
  }
  DSUQueue D(n);
  for (int i = 1; i <= m; i++) {
    D.push(update(u[i], v[i]));
  }
  for (int l = 1, r = 1; l <= m; l++) {
    while (r < l || (!D.is_bipartite() && r <= m)) {
      D.pop();
      ++r;
    }
    if (D.is_bipartite()) a[l] = r - 1;
    else a[l] = m + 1;
    D.push(update(u[l], v[l]));
  }
  while (q--) {
    int l, r; cin >> l >> r;
    if (a[l] <= r) {
      cout << "NO\n";
    }
    else {
      cout << "YES\n";
    }
  }
  return 0;
}
// https://codeforces.com/contest/1386/problem/C
// https://codeforces.com/blog/entry/83467
