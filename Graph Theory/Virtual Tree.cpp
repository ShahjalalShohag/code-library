#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9;

vector<int> g[N];
int par[N][20], dep[N], sz[N], st[N], en[N], T;
void dfs(int u, int pre) {
  par[u][0] = pre;
  dep[u] = dep[pre] + 1;
  sz[u] = 1;
  st[u] = ++T;
  for (int i = 1; i <= 18; i++) par[u][i] = par[par[u][i - 1]][i - 1];
  for (auto v : g[u]) {
    if (v == pre) continue;
    dfs(v, u);
    sz[u] += sz[v];
  }
  en[u] = T;
}
int lca(int u, int v) {
  if (dep[u] < dep[v]) swap(u, v);
  for (int k = 18; k >= 0; k--) if (dep[par[u][k]] >= dep[v]) u = par[u][k];
  if (u == v) return u;
  for (int k = 18; k >= 0; k--) if (par[u][k] != par[v][k]) u = par[u][k], v = par[v][k];
  return par[u][0];
}
int kth(int u, int k) {
  for (int i = 0; i <= 18; i++) if (k & (1 << i)) u = par[u][i];
  return u;
}
int dist(int u, int v) {
  int lc = lca(u, v);
  return dep[u] + dep[v] - 2 * dep[lc];
}
int isanc(int u, int v) {
  return (st[u] <= st[v]) && (en[v] <= en[u]);
}
vector<int> t[N];
// given specific nodes, construct a compressed directed tree with these vertices(if needed some other nodes included)
// returns the nodes of the tree
// nodes.front() is the root
// t[] is the specific tree
vector<int> buildtree(vector<int> v) {
  // sort by entry time
  sort(v.begin(), v.end(), [](int x, int y) {
    return st[x] < st[y];
  });
  // finding all the ancestors, there are few of them
  int s = v.size();
  for (int i = 0; i < s - 1; i++) {
    int lc = lca(v[i], v[i + 1]);
    v.push_back(lc);
  }
  // removing duplicated nodes
  sort(v.begin(), v.end());
  v.erase(unique(v.begin(), v.end()), v.end());
  // again sort by entry time
  sort(v.begin(), v.end(), [](int x, int y) {
    return st[x] < st[y];
  });
  stack<int> st;
  st.push(v[0]);
  for (int i = 1; i < v.size(); i++) {
    while (!isanc(st.top(), v[i])) st.pop();
    t[st.top()].push_back(v[i]);
    st.push(v[i]);
  }
  return v;
}
int ans;
int imp[N];
int yo(int u) {
  vector<int> nw;
  for (auto v : t[u]) nw.push_back(yo(v));
  if (imp[u]) {
    for (auto x : nw) if (x) ans++;
    return 1;
  } else {
    int cnt = 0;
    for (auto x : nw) cnt += x > 0;
    if (cnt > 1) {
      ans++;
      return 0;
    }
    return cnt;
  }
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int i, j, k, n, m, q, u, v;
  cin >> n;
  for (i = 1; i < n; i++) cin >> u >> v, g[u].push_back(v), g[v].push_back(u);
  dfs(1, 0);
  cin >> q;
  while (q--) {
    cin >> k;
    vector<int> v;
    for (i = 0; i < k; i++) cin >> m, v.push_back(m), imp[m] = 1;
    int fl = 1;
    for (auto x : v) if (imp[par[x][0]]) fl = 0;
    ans = 0;
    vector<int> nodes;
    if (fl) nodes = buildtree(v);
    if (fl) yo(nodes.front());
    if (!fl) ans = -1;
    cout << ans << '\n';
    // clear the tree
    for (auto x : nodes) t[x].clear();
    for (auto x : v) imp[x] = 0;
  }
  return 0;
}
// https://codeforces.com/contest/613/problem/D
