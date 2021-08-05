#include<bits/stdc++.h>
using namespace std;

const int N = 420;

/*
all the edges should be in the same connected component
#undirected graph: euler path: all degrees are even or exactly two of them are odd.
#undirected graph: euler circuit: all degrees are even
*/

//euler path in an undirected graph
//it also finds circuit if it exists
vector<pair<int, int>> g[N];
vector<int> ans;
int done[N];
int vis[N * N]; //number of edges
void dfs(int u) {
  while (done[u] < g[u].size()) {
    auto e = g[u][done[u]++];
    if (vis[e.second]) continue;
    vis[e.second] = 1;
    dfs(e.first);
  }
  ans.push_back(u);
}

int solve(int n) {
  int edges = 0;
  ans.clear();
  memset(done, 0, sizeof done);
  memset(vis, 0, sizeof vis);
  vector<int> deg(n + 1, 0);
  for (int u = 1; u <= n; u++) {
    for (auto e : g[u]) {
      deg[e.first]++, deg[u]++, edges++;
    }
  }
  int odd = 0, root = 0;
  for (int i = 1; i <= n; i++) {
    if (deg[i] & 1) odd++, root = i;
  }
  if (odd > 2) return 0;
  if (root == 0) {
    for (int i = 1; i <= n; i++) if (deg[i]) root = i;
  }
  if (root == 0) return 1; //empty graph
  dfs(root);
  if (ans.size() != edges / 2 + 1) return 0;
  reverse(ans.begin(), ans.end());
  return 1;
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    vector<int> deg(n + 1, 0);
    for (int i = 1; i <= m; i++) {
      int u, v;
      cin >> u >> v;
      g[u].push_back({v, i});
      g[v].push_back({u, i});
      deg[u]++, deg[v]++;
    }
    int sz = m;
    for (int i = 1; i <= n; i++) {
      if (deg[i] & 1) {
        ++sz;
        g[n + 1].push_back({i, sz});
        g[i].push_back({n + 1, sz});
      }
    }
    int ok = solve(n + 1);
    assert(ok);
    vector<int> in(n + 2, 0), out(n + 2, 0);
    for (int i = 0; i + 1 < ans.size(); i++) {
      if (ans[i] != n + 1 && ans[i + 1] != n + 1) {
        in[ans[i + 1]]++;
        out[ans[i]]++;
      }
    }
    int res = 0;
    for (int i = 1; i <= n; i++) res += in[i] == out[i];
    cout << res << '\n';
    for (int i = 0; i + 1 < ans.size(); i++) if (ans[i] != n + 1 &&  ans[i + 1] != n + 1) cout << ans[i] << ' ' << ans[i + 1] << '\n';
    for (int i = 0; i <= n + 1; i++) g[i].clear();
  }
  return 0;
}

// https://codeforces.com/contest/723/problem/E
