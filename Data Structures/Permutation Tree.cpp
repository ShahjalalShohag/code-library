#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, inf = 1e9 + 7, LG = 19;

struct ST {
#define lc (n << 1)
#define rc ((n << 1) | 1)
  int t[4 * N], lazy[4 * N];
  ST() {}
  inline void push(int n, int b, int e) {
    if (lazy[n] == 0) return;
    t[n] = t[n] + lazy[n];
    if (b != e) {
      lazy[lc] = lazy[lc] + lazy[n];
      lazy[rc] = lazy[rc] + lazy[n];
    }
    lazy[n] = 0;
  }
  inline int combine(int a, int b) {
    return min(a, b);
  }
  inline void pull(int n) {
    t[n] = min(t[lc], t[rc]);
  }
  void build(int n, int b, int e) {
    lazy[n] = 0;
    if (b == e) {
      t[n] = 0;
      return;
    }
    int mid = (b + e) >> 1;
    build(lc, b, mid);
    build(rc, mid + 1, e);
    pull(n);
  }
  void upd(int n, int b, int e, int i, int j, int v) {
    push(n, b, e);
    if (j < b || e < i) return;
    if (i <= b && e <= j) {
      lazy[n] = v;
      push(n, b, e);
      return;
    }
    int mid = (b + e) >> 1;
    upd(lc, b, mid, i, j, v);
    upd(rc, mid + 1, e, i, j, v);
    pull(n);
  }
  int query(int n, int b, int e, int i, int j) {
    push(n, b, e);
    if (i > e || b > j) return inf;
    if (i <= b && e <= j) return t[n];
    int mid = (b + e) >> 1;
    return combine(query(lc, b, mid, i, j), query(rc, mid + 1, e, i, j));
  }
} t;

// id of span {i, i} is i
int p[N];
pair<int, int> range[N * 2]; // range of permutation values
pair<int, int> span[N * 2]; // range of permutation indices
vector<int> pt[N * 2]; //directed permutation tree
int par[N * 2];
int ty[N * 2]; // 0 if cut node and 1 if increasing join node, 2 if decreasing join node
int id; //new index to assign to nodes
pair<int, int> get_range(pair<int, int> x, pair<int, int> y) {
  return pair<int, int>(min(x.first, y.first), max(x.second, y.second));
}
void add_edge(int u, int v) { //u is parent of v
  par[v] = u;
  pt[u].push_back(v);
}
bool adjacent(int i, int j) {
  return range[i].second == range[j].first - 1;
}
int length(int i) {
  return range[i].second - range[i].first + 1;
}
// leaf node is a cut node
int build(int n) { //returns root of the tree
  for (int i = 1; i <= 2 * n; i++) {
    pt[i].clear();
    ty[i] = 0;
    par[i] = -1;
  }
  id = n + 1;
  t.build(1, 1, n);
  vector<int> mx = {0}, mn = {0};
  vector<int> nodes; //stack of cut and join nodes
  for (int i = 1; i <= n; i++) {
    while (mx.back() != 0 && p[mx.back()] < p[i]) {
      int r = mx.back();
      mx.pop_back();
      t.upd(1, 1, n, mx.back() + 1, r, p[i] - p[r]);
    }
    mx.push_back(i);

    while (mn.back() != 0 && p[mn.back()] > p[i]) {
      int r = mn.back();
      mn.pop_back();
      t.upd(1, 1, n, mn.back() + 1, r, p[r] - p[i]);
    }
    mn.push_back(i);

    // handle stack updates
    range[i] = {p[i], p[i]};
    span[i] = {i, i};
    int cur = i;

    while (true) {
      if (!nodes.empty() && (adjacent(nodes.back(), cur) || adjacent(cur, nodes.back()))) {
        if ((adjacent(nodes.back(), cur) && ty[nodes.back()] == 1) ||
            (adjacent(cur, nodes.back()) && ty[nodes.back()] == 2)) {
          add_edge(nodes.back(), cur);
          range[nodes.back()] = get_range(range[nodes.back()], range[cur]);
          span[nodes.back()] = get_range(span[nodes.back()], span[cur]);
          cur = nodes.back();
          nodes.pop_back();
        } else { //make a new join node
          ty[id] = (adjacent(nodes.back(), cur) ? 1 : 2);
          add_edge(id, nodes.back());
          add_edge(id, cur);
          range[id] = get_range(range[nodes.back()], range[cur]);
          span[id] = get_range(span[nodes.back()], span[cur]);
          nodes.pop_back();
          cur = id++;
        }
      } else if (i - (length(cur) - 1) && t.query(1, 1, n, 1, i - length(cur)) == 0) {
        int len = length(cur);
        pair<int, int> r = range[cur];
        pair<int, int> s = span[cur];
        add_edge(id, cur);
        do {
          len += length(nodes.back());
          r = get_range(r, range[nodes.back()]);
          s = get_range(s, span[nodes.back()]);
          add_edge(id, nodes.back());
          nodes.pop_back();
        } while (r.second - r.first + 1 != len);
        reverse(pt[id].begin(), pt[id].end());
        range[id] = r;
        span[id] = s;
        cur = id++;
      } else {
        break;
      }
    }
    nodes.push_back(cur);

    t.upd(1, 1, n, 1, i, -1);
  }
  id--;
  assert(id <= 2 * n);
  int r = 0;
  for (int i = 1; i <= id; i++) {
    if (par[i] == -1) r = i;
  }
  assert(r);
  return r;
}
int P[N * 2][LG];
void dfs(int u, int p = 0) {
  P[u][0] = p;
  for (int k = 1; k < LG; k++) {
    P[u][k] = P[P[u][k - 1]][k - 1];
  }
  for (auto v : pt[u]) {
    if (v == p) continue;
    dfs(v, u);
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n;
  cin >> n;
  for (int i = 1; i <= n; i++) cin >> p[i];
  int r = build(n);
  dfs(r);
  int q;
  cin >> q;
  while (q--) {
    int l, r;
    cin >> l >> r;
    if (l == r) {
      cout << l << ' ' << r << '\n';
      continue;
    }
    int u = l;
    for (int k = LG - 1; k >= 0; k--) {
      if (P[u][k] && span[P[u][k]].second < r) u = P[u][k];
    }
    u = P[u][0];
    if (ty[u] == 0) {
      cout << span[u].first << ' ' << span[u].second << '\n';
      continue;
    }
    int curl = -1, curr = pt[u].size();
    for (int k = LG - 1; k >= 0; k--) {
      if (curl + (1 << k) < pt[u].size() && span[pt[u][curl + (1 << k)]].second < l) curl += 1 << k;
      if (curr - (1 << k) >= 0 && r < span[pt[u][curr - (1 << k)]].first) curr -= 1 << k;
    }
    cout << span[pt[u][curl + 1]].first << ' ' << span[pt[u][curr - 1]].second << '\n';
  }
  return 0;
}
// https://codeforces.com/gym/101620, Problem I
// https://codeforces.com/blog/entry/78898
