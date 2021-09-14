#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, LG = 18;

struct RT {
  int n, par[2 * N], sp[LG + 1][2 * N], val[2 * N], rid[2 * N];
  int T, st[2 * N], en[2 * N];
  vector<int> g[2 * N]; //reachability tree
  int find(int x) {
    if (par[x] == x) return x;
    return par[x] = find(par[x]);
  }
  void dfs(int u) {
    st[u] = T + 1;
    for (int v : g[u]) {
      sp[0][v] = u;
      dfs(v);
    }
    if (st[u] == T + 1) {
      rid[T + 1] = u; //real id of this node
      T++;
    }
    en[u] = T;
  }
  void build(vector<array<int, 3>> e) { //{w, u, v}
    n = e.size() + 1;
    sort(e.begin(), e.end());
    for (int x = 1; x < 2 * n; x++) {
      par[x] = x;
      g[x].clear();
      val[x] = 0;
      sp[0][x] = 0;
    }
    for (int i = 0; i + 1 < n; i++) {
      int u = e[i][1], v = e[i][2];
      int w = e[i][0];
      u = find(u), v = find(v);
      int id = n + i + 1;
      par[u] = par[v] = id;
      val[id] = w;
      g[id].push_back(u);
      g[id].push_back(v);
    }
    val[0] = 1e9; //inf
    T = 0;
    dfs(2 * n - 1);
    for (int k = 1; k <= LG; k++) {
      for (int u = 1; u < 2 * n; u++) sp[k][u] = sp[k - 1][sp[k - 1][u]];
    }
  }
  int lift(int u, int x) { //all nodes reachable from u s.t. edges <= w, returns the cooresponding root of the subtree
    for (int k = LG; k >= 0; k--) if (val[sp[k][u]] <= x) u = sp[k][u];
    return u;
  }
} rt;

pair<int, int> t[8 * N];
int lazy[8 * N];
inline void build(int n, int l, int r) {
  lazy[n] = 0;
  t[n] = make_pair(0, r - l + 1);
  if (l == r) return;
  int mid = l + r >> 1;
  build(2 * n, l, mid);
  build(2 * n + 1, mid + 1, r);
}
inline void push(int n, int l, int r) {
  if (lazy[n]) {
    t[n].first += lazy[n];
    if (l != r) {
      lazy[2 * n] += lazy[n];
      lazy[2 * n + 1] += lazy[n];
    }
    lazy[n] = 0;
  }
}
inline pair<int, int> combine(pair<int, int> a, pair<int, int> b) {
  if (a.first != b.first) return min(a, b);
  return make_pair(a.first, a.second + b.second);
}
inline void upd(int n, int l, int r, int ql, int qr, int val) {
  push(n, l, r);
  if (l > qr || r < ql) return;
  if (l >= ql && r <= qr) {
    lazy[n] += val;
    push(n, l, r);
    return;
  }
  int mid = l + r >> 1;
  upd(2 * n, l, mid, ql, qr, val);
  upd(2 * n + 1, mid + 1, r, ql, qr, val);
  t[n] = combine(t[2 * n], t[2 * n + 1]);
}
inline pair<int, int> query(int n, int l, int r, int ql, int qr) {
  push(n, l, r);
  if (l > qr || r < ql) return make_pair(1e9, 0);
  if (l >= ql && r <= qr) return t[n];
  int mid = l + r >> 1;
  pair<int, int> left = query(2 * n, l, mid, ql, qr);
  pair<int, int> rght = query(2 * n + 1, mid + 1, r, ql, qr);
  return combine(left, rght);
}
queue<pair<int, int>> Q;
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    while (!Q.empty()) Q.pop();
    int n;
    cin >> n;
    vector<array<int, 3>> e(n - 1);
    for (int i = 0; i + 1 < n; i++) cin >> e[i][1] >> e[i][2] >> e[i][0];
    rt.build(e);
    build(1, 1, n);
    int q, x;
    cin >> q >> x;
    while (q--) {
      int d, u, k;
      cin >> d >> u >> k;
      u = rt.lift(u, k);
      while (!Q.empty() and Q.front().first <= d) {
        upd(1, 1, n, rt.st[Q.front().second], rt.en[Q.front().second], -1);
        Q.pop();
      }
      pair<int, int> ans = query(1, 1, n, rt.st[u], rt.en[u]);
      if (ans.first == 0) cout << ans.second << "\n";
      else cout << "0\n";
      upd(1, 1, n, rt.st[u], rt.en[u], 1);
      Q.push(make_pair(d + x, u));
    }
  }
  return 0;
}
//https://www.codechef.com/problems/TULIPS
