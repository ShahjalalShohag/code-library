#include<bits/stdc++.h>
using namespace std;

#define ll long long
#define eb emplace_back
#define nl '\n'
#define deb(x) cerr << #x" = " << x << nl
#define in() ( { int a ; scanf("%d", &a); a; } )

const int N = 1e5 + 9;
const int mod = 1e9 + 7;

struct Line {
  ll k, d;
  ll eval(ll x) {
    return k * x + d;
  }
};

struct LiChaoNode {
  Line line;
  int l, r;
  LiChaoNode() {
    line = Line({0, numeric_limits<long long>::max() / 2});
    l = 0, r = 0;
  }
  LiChaoNode(Line line) : line(line), l(0), r(0) {}
} t[50 * N];

int T;
int upd(int pre, Line nw, int l, int r) {
  int m = (l + r) / 2;
  int id = ++T;
  t[id].line = t[pre].line;
  bool lef = nw.eval(l) < t[id].line.eval(l);
  bool mid = nw.eval(m) < t[id].line.eval(m);
  if(mid) swap(t[id].line, nw);
  if(l == r) return id;
  if(lef != mid) {
    if(!t[pre].l) t[id].l = ++T, t[T] = LiChaoNode(nw);
    else t[id].l = upd(t[pre].l, nw, l, m);
    t[id].r = t[pre].r;
  } else {
    if(!t[pre].r) t[id].r = ++T, t[T] = LiChaoNode(nw);
    else t[id].r = upd(t[pre].r, nw, m + 1, r);
    t[id].l = t[pre].l;
  }
  return id;
}

ll Query(int cur, int x, int l, int r) {
  ll val = t[cur].line.eval(x);
  int m = (l + r) / 2;
  if(l < r) {
    if(x <= m && t[cur].l) val = min(val, Query(t[cur].l, x, l, m));
    if(x > m && t[cur].r) val = min(val, Query(t[cur].r, x, m + 1, r));
  }
  return val;
}

struct PersistentLiChaoTree {
  int L, R;
  vector<int> roots;
  PersistentLiChaoTree() {
    roots = {1};
    T = 1;
    L = -1e9;
    R = 1e9;
  }
  PersistentLiChaoTree(int L, int R) : L(L), R(R) {
    T = 1;
    roots.push_back(1);
  }
  void add_line(Line line) {
    int root = upd(roots.back(), line, L, R);
    roots.push_back(root);
  }
  ll query(int i, int x) {
    return Query(roots[i], x, L, R);
  }
} lt;

ll sum[N];
vector<pair<int, int>> g[N];
void dfs(int u, int pre = 0) {
  for(auto x : g[u]) {
    int v = x.first, d = x.second;
    if(v == pre) continue;
    sum[v] = sum[u] + d;
    dfs(v, u);
  }
}
ll ans[N], p[N], s[N];
void yo(int u, int pre = 0) {
  for(auto x : g[u]) {
    int v = x.first;
    if(v == pre) continue;
    ans[v] = lt.query((int)lt.roots.size() - 1, p[v]) + sum[v] * p[v] + s[v];
    int sz = lt.roots.size();
    lt.add_line({-sum[v], ans[v]});
    yo(v, u);
    lt.roots.pop_back();
  }
}
int main() {
  int n;
  cin >> n;
  for(int i = 1; i < n; i++) {
    int u, v, d;
    cin >> u >> v >> d;
    g[u].eb(v, d);
    g[v].eb(u, d);
  }
  for(int i = 2; i <= n; i++) cin >> s[i] >> p[i];
  dfs(1);
  T = 0;
  lt = PersistentLiChaoTree((ll) - 1e9 - 10, (ll)1e9 + 10);
  lt.add_line({0, 0});
  yo(1);
  for(int i = 2; i <= n; i++) {
    if(i > 2) cout << ' ';
    cout << ans[i];
  }
  cout << nl;
  return 0;
}
