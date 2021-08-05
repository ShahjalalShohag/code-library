#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;

int n;
vector<pair<int, int>> g[N];
struct PT {
  int x, y, id;
  bool operator < (const PT &p) const {
    return x == p.x ? y < p.y : x < p.x;
  }
} p[N];
struct node {
  int val, id;
} t[N];
struct DSU {
  int p[N];
  void init(int n) { for (int i = 1; i <= n; i++)  p[i] = i; }
  int find(int u) { return p[u] == u ? u : p[u] = find(p[u]); }
  void merge(int u, int v) { p[find(u)] = find(v); }
} dsu;
struct edge {
  int u, v, w;
  bool operator < (const edge &p) const { return w < p.w; }
};
vector<edge> edges;
int query(int x) {
  int r = 2e9 + 10, id = -1;
  for (; x <= n; x += (x & -x)) if (t[x].val < r) r = t[x].val, id = t[x].id;
  return id;
}
void modify(int x, int w, int id) {
  for (; x > 0; x -= (x & -x)) if (t[x].val > w) t[x].val = w, t[x].id = id;
}
int dist(PT &a, PT &b) {
  return abs(a.x - b.x) + abs(a.y - b.y);
}
void add(int u, int v, int w) {
  edges.push_back({u, v, w});
}
long long Kruskal() {
  dsu.init(n);
  sort(edges.begin(), edges.end());
  long long ans = 0;
  for (edge e : edges) {
    int u = e.u, v = e.v, w = e.w;
    if (dsu.find(u) != dsu.find(v)) {
      ans += w;
      g[u].push_back({v, w});
      //g[v].push_back({u, w});
      dsu.merge(u, v);
    }
  }
  return ans;
}
void Manhattan() {
  for (int i = 1; i <= n; ++i) p[i].id = i;
  for (int dir = 1; dir <= 4; ++dir) {
    if (dir == 2 || dir == 4) {
      for (int i = 1; i <= n; ++i) swap(p[i].x, p[i].y);
    } 
    else if (dir == 3) {
      for (int i = 1; i <= n; ++i) p[i].x = -p[i].x;
    }
    sort(p + 1, p + 1 + n);
    vector<int> v;
    static int a[N];
    for (int i = 1; i <= n; ++i) a[i] = p[i].y - p[i].x, v.push_back(a[i]);
    sort(v.begin(), v.end());
    v.erase(unique(v.begin(), v.end()), v.end());
    for (int i = 1; i <= n; ++i) a[i] = lower_bound(v.begin(), v.end(), a[i]) - v.begin() + 1;
    for (int i = 1; i <= n; ++i) t[i].val = 2e9 + 10, t[i].id = -1;
    for (int i = n; i >= 1; --i) {
      int pos = query(a[i]);
      if (pos != -1) add(p[i].id, p[pos].id, dist(p[i], p[pos]));
      modify(a[i], p[i].x + p[i].y, i);
    }
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cin >> n;
  for (int i = 1; i <= n; i++) cin >> p[i].x >> p[i].y;
  Manhattan();
  cout << Kruskal() << '\n';
  for (int u = 1; u <= n; u++) {
    for (auto x: g[u]) cout << u - 1 << ' ' << x.first - 1 << '\n';
  }
  return 0;
}
// https://judge.yosupo.jp/problem/manhattanmst
