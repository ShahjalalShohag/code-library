#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, LG = 16;
using ll = long long;

vector<array<int, 2>> g[N];
// take the longest path from the root and do the same thing
// recursively for the decomposed subtrees
struct LongPathDecomposition{
  int n, par[N][LG + 1], child[N]; // the subtree of child contains the node having the max dep
  ll dep[N], mx_dep[N];
  void dfs(int u, int p = 0) {
    par[u][0] = p;
    for (int i = 1; i <= LG; i++) {
      par[u][i] = par[par[u][i - 1]][i - 1];
    }
    mx_dep[u] = dep[u];
    child[u] = 0;
    for (auto [v, w]: g[u]) {
      if (v != p) {
        dep[v] = dep[u] + w;
        dfs(v, u);
        if (mx_dep[v] >= mx_dep[u]) {
          mx_dep[u] = mx_dep[v];
          child[u] = v;
        }
      }
    }
  }
  int head[N];
  void decomp(int u, int p, int h) {
    head[u] = h;
    if (child[u]) {
      decomp(child[u], u, h);
    }
    for (auto [v, w]: g[u]) {
      if (v != p and v != child[u]) {
        decomp(v, u, v);
      }
    }
  }
  int pos[N], head_cnt;
  ll sum[N];
  void build(int _n, int root) {
    n = _n;
    dep[0] = dep[root] = 0;
    dfs(root);
    decomp(root, 0, root);
    vector<pair<ll, int>> heads;
    for (int u = 1; u <= n; u++) {
      if (head[u] == u) {
        heads.push_back({mx_dep[u] - dep[par[u][0]], u});
      }
    }
    sort(heads.rbegin(), heads.rend());
    head_cnt = heads.size();
    for (int i = 0; i < head_cnt; i++) {
      pos[heads[i].second] = i;
      sum[i] = (i ? sum[i - 1] : 0) + heads[i].first;
    }
  }
  int get(int u, int p) {
    for (int k = LG; k >= 0; k--) {
      if (pos[head[par[u][k]]] > p) {
        u = par[u][k];
      }
    }
    return par[u][0];
  }
  // max sum of the union of $path_cnt paths such that their union contains the node u
  // so just take the longest 2 * path_cnt leaves
  ll query(int u, int path_cnt) {
    int leaves = min(head_cnt, path_cnt * 2 - 1); // considering root as a leaf for this problem
    if (leaves <= 0) return 0;
    if (pos[head[u]] < leaves) {
      return sum[leaves - 1];
    }
    int anc = get(u, leaves - 1);
    ll ans = sum[leaves - 1] + mx_dep[u] - mx_dep[anc];
    if (leaves > 1) {
      ans = max(ans, sum[leaves - 2] + mx_dep[u] - dep[get(u, leaves - 2)]);
    }
    return ans;
  }
}t1, t2;
ll d[N], mx; int x;
void dfs(int u, int p = 0) {
  if (d[u] >= mx) {
    mx = d[u];
    x = u;
  }
  for (auto [v, w]: g[u]) {
    if (v ^ p) {
      d[v] = d[u] + w;
      dfs(v, u);
    }
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  for (int i = 1; i < n; i++) {
    int u, v, w; cin >> u >> v >> w;
    g[u].push_back({v, w});
    g[v].push_back({u, w});
  }
  dfs(1);
  int y = x;
  mx = 0;
  dfs(x);
  t1.build(n, x);
  t2.build(n, y);
  ll last = 0;
  while (q--) {
    int u, k; cin >> u >> k;
    u = (u + last - 1) % n + 1;
    k = (k + last - 1) % n + 1;
    last = max(t1.query(u, k), t2.query(u, k));
    cout << last << '\n';
  }
  return 0;
}
// https://codeforces.com/contest/526/problem/G
