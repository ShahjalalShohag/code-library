#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;
struct ST { // add, del and query for the element that occurs the maximum number of times
  static const int B = 1000;
  int c[N], cc[N / B + 2][N], mx[N / B + 2];
  ST() {
    memset(c, 0, sizeof c);
    memset(cc, 0, sizeof cc);
    memset(mx, 0, sizeof mx);
  }
  void add(int x) {
    cc[x / B][c[x]]--;
    c[x]++;
    cc[x / B][c[x]]++;
    mx[x / B] = max(mx[x / B], c[x]);
  }
  void del(int x) {
    if (mx[x / B] == c[x] and cc[x / B][c[x]] == 1) {
      mx[x / B]--;
    }
    cc[x / B][c[x]]--;
    c[x]--;
    cc[x / B][c[x]]++;
  }
  int query() {
    int mx_oc = 0;
    for (int i = 0; i <= N / B; i++) {
      mx_oc = max(mx_oc, mx[i]);
    }
    for (int i = 0; i <= N / B; i++) {
      if (mx[i] == mx_oc) {
        for (int j = 0; j < B; j++) {
          if (c[i * B + j] == mx_oc) {
            return i * B + j;
          }
        }
        assert(0);
      }
    }
    return 0;
  }
};

const int B = 5000;
struct query {
  int l1, r1, l2, r2, id;
  bool operator < (const query &x) const {
    array<int, 7> a({l1 / B, r1 / B, l2 / B, r2, l1, r1, l2});
    array<int, 7> b({x.l1 / B, x.r1 / B, x.l2 / B, x.r2, x.l1, x.r1, x.l2});
    return a < b;
  }
} Q[N];

int st[N], en[N], T, a[N], b[N];
vector<int> g[N];
void dfs(int u, int p = 0) {
  st[u] = ++T;
  a[T] = b[u];
  for (auto v: g[u]) {
    if (v ^ p) {
      dfs(v, u);
    }
  }
  en[u] = T;
}

ST t;
void add(int i) {
  t.add(a[i]);
}
void del(int i) {
  t.del(a[i]);
}
int ans[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> b[i];
  }
  for (int i = 1; i < n; i++) {
    int u, v; cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  dfs(1);
  int q; cin >> q;
  for (int i = 1; i <= q; i++) {
    int u, v; cin >> u >> v;
    int l1 = st[u], r1 = en[u];
    int l2 = st[v], r2 = en[v];
    Q[i] = {l1, r1, l2, r2, i};
  }
  sort(Q + 1, Q + q + 1);
  int cl1 = 1, cr1 = 1, cl2 = 1, cr2 = 1;
  add(cl1);
  add(cl2);
  for (int i = 1; i <= q; i++) {
    auto [l1, r1, l2, r2, id] = Q[i];
    while (cr1 < r1) {
      ++cr1;
      add(cr1);
    }
    while (cl1 > l1) {
      --cl1;
      add(cl1);
    }
    while (cr1 > r1) {
      del(cr1);
      --cr1;
    }
    while (cl1 < l1) {
      del(cl1);
      ++cl1;
    }
    while (cr2 < r2) {
      ++cr2;
      add(cr2);
    }
    while (cl2 > l2) {
      --cl2;
      add(cl2);
    }
    while (cr2 > r2) {
      del(cr2);
      --cr2;
    }
    while (cl2 < l2) {
      del(cl2);
      ++cl2;
    }
    ans[id] = t.query();
  }
  for (int i = 1; i <= q; i++) {
    cout << ans[i] << '\n';
  }
  return 0;
}
// https://codeforces.com/contest/1767/problem/F
