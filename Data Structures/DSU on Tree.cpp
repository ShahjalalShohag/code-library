#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
vector<int> g[N];
int ans[N], col[N], sz[N], cnt[N];
bool big[N];
void dfs(int u, int p) {
  sz[u] = 1;
  for (auto v : g[u]) {
    if (v == p) continue;
    dfs(v, u);
    sz[u] += sz[v];
  }
}
void add(int u, int p, int x) {
  cnt[col[u]] += x;
  for (auto v : g[u]) {
    if (v == p || big[v] == 1) continue;
    add(v, u, x);
  }
}
void dsu(int u, int p, bool keep) {
  int bigchild = -1, mx = -1;
  for (auto v : g[u]) {
    if (v == p) continue;
    if (sz[v] > mx) mx = sz[v], bigchild = v;
  }
  for (auto v : g[u]) {
    if (v == p || v == bigchild) continue;
    dsu(v, u, 0);
  }
  if (bigchild != -1) dsu(bigchild, u, 1), big[bigchild] = 1;
  add(u, p, 1);
  ans[u] = cnt[u];
  if (bigchild != -1) big[bigchild] = 0;
  if (keep == 0) add(u, p, -1);
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t;
  cin >> t;
  return 0;
}
