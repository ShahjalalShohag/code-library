#include<bits/stdc++.h>
using namespace std;

const int N = 755, L = 1e7 + 9;
bool reach[N][N];
struct AhoCorasick {
  // caution: change 2 to 26 for normal strings
  int link[L], sz, to[L][2], dp[L][2], term[L], last[L]; // each string contains 'a' and 'b'
  AhoCorasick() {
    memset(link, 0, sizeof link);
    memset(to, 0, sizeof to);
    memset(dp, -1, sizeof dp);
    memset(term, 0, sizeof term);
    memset(last, 0, sizeof last);
    sz = 1;
  }
  void insert(string &s, int id) {
    int u = 0;
    for (char c : s) {
      if (!to[u][c - 'a']) to[u][c - 'a'] = sz++;
      u = to[u][c - 'a'];
    }
    term[u] = id;
  }
  void push_links() {
    queue<int> q;
    q.push(0);
    link[0] = -1;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      // caution: change 2 to 26 for normal strings
      for (int c = 0; c < 2; c++) {
        int v = to[u][c];
        if (!v) continue;
        int j = link[u];
        while (j != -1 && !to[j][c]) j = link[j];
        if (j != -1) link[v] = to[j][c];
        else link[v] = 0;
        if (!term[link[v]]) last[v] = last[link[v]];
        else last[v] = link[v];
        q.push(v);
      }
    }
  }
  int go(int v, char ch) {
    int c = ch - 'a';
    if (dp[v][c] == -1) {
      if (to[v][c]) return dp[v][c] = to[v][c];
      return dp[v][c] = (v == 0 ? 0 : go(link[v], ch));
    }
    return dp[v][c];
  }
  void traverse(string &s, int id) {
    int n = s.size(), cur = 0;
    for (int i = 0; i < n; i++) {
      cur = go(cur, s[i]);
      if (term[cur] && term[cur] != id) reach[term[cur]][id] = 1;
      else reach[term[last[cur]]][id] = 1;
    }
  }
} t;
mt19937 rnd(chrono::steady_clock::now().time_since_epoch().count());
struct Kuhn {
  int n;
  vector<vector<int>> g;
  vector<int> l, r;
  vector<bool> vis;
  Kuhn() {}
  Kuhn(int _n, int _m) {
    n = _n;
    g.resize(n + 1);
    vis.resize(n + 1, false);
    l.resize(n + 1, -1);
    r.resize(_m + 1, -1);
  }
  void add_edge(int a, int b) {
    g[a].push_back(b);
  }
  bool yo(int u) {
    if (vis[u])  return false;
    vis[u] = true;
    for (auto v : g[u]) {
      if (r[v] == -1 || yo(r[v])) {
        l[u] = v;
        r[v] = u;
        return true;
      }
    }
    return false;
  }
  int maximum_matching() {
    for (int i = 1; i <= n; i++) shuffle(g[i].begin(), g[i].end(), rnd);
    vector<int> p(n);
    iota(p.begin(), p.end(), 1);
    shuffle(p.begin(), p.end(), rnd);
    bool ok = true;
    while (ok) {
      ok = false;
      vis.assign(n + 1, false);
      for (auto &i : p) if(l[i] == -1) ok |= yo(i);
    }
    int ans = 0;
    for (int i = 1; i <= n; i++) ans += l[i] != -1;
    return ans;
  }
} M;
bool vis[N][2];
vector<int> g[N];
bool vc[N];
void dfs(int u, int side) {
  if (vis[u][side]) return;
  vis[u][side] = 1;
  if (side == 0) {
    for (auto v : g[u]) {
      dfs(v, 1);
    }
  } else dfs(M.r[u], 0);
}
string s[N]; //it assumes all strings are distinct
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> s[i];
    t.insert(s[i], i);
  }
  t.push_links();
  for (int i = 1; i <= n; i++) t.traverse(s[i], i);
  for (int k = 1; k <= n; k++) {
    for (int i = 1; i <= n; i++) {
      for (int j = 1; j <= n; j++) {
        reach[i][j] |= reach[i][k] & reach[k][j];
      }
    }
  }
  M = Kuhn(n, n);
  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      if (reach[i][j]) {
        M.add_edge(i, j);
        g[i].push_back(j);
      }
    }
  }
  int p = M.maximum_matching();
  for (int i = 1; i <= n; i++) if (M.l[i] == -1) dfs(i, 0);
  for (int i = 1; i <= n; i++) {
    if (!vis[i][0] || vis[i][1]) vc[i] = 1;
  }
  cout << n - p << '\n';
  for (int i = 1; i <= n; i++) {
    if (!vc[i]) cout << i << ' ';
  }
  cout << '\n';
  return 0;
}
//https://codeforces.com/contest/590/problem/E
