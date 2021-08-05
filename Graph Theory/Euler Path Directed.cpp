#include<bits/stdc++.h>
using namespace std;

const int N = 4e5 + 9;

/*
all the edges should be in the same connected component
#directed graph: euler path: for all -> indeg = outdeg or nodes having indeg > outdeg = outdeg > indeg = 1 and for others in = out
#directed graph: euler circuit: for all -> indeg = outdeg
*/

//euler path in a directed graph
//it also finds circuit if it exists
vector<int> g[N], ans;
int done[N];
void dfs(int u) {
  while (done[u] < g[u].size()) dfs(g[u][done[u]++]);
  ans.push_back(u);
}
int solve(int n) {
  int edges = 0;
  vector<int> in(n + 1, 0), out(n + 1, 0);
  for (int u = 1; u <= n; u++) {
    for (auto v : g[u]) in[v]++, out[u]++, edges++;
  }
  int ok = 1, cnt1 = 0, cnt2 = 0, root = 0;
  for (int i = 1; i <= n; i++) {
    if (in[i] - out[i] == 1) cnt1++;
    if (out[i] - in[i] == 1) cnt2++, root = i;
    if (abs(in[i] - out[i]) > 1) ok = 0;
  }
  if (cnt1 > 1 || cnt2 > 1) ok = 0;
  if (!ok) return 0;
  if (root == 0) {
    for (int i = 1; i <= n; i++) if (out[i]) root = i;
  }
  if (root == 0) return 1; //empty graph
  dfs(root);
  if (ans.size() != edges + 1) return 0; //connectivity
  reverse(ans.begin(), ans.end());
  return 1;
}
map<string, int> mp;
string id[N];
int T = 0;
int32_t main() {
  int n;
  cin >> n;
  for (int i = 1; i <= n; i++) {
    string s;
    cin >> s;
    string a = s.substr(0, 2);
    string b = s.substr(1);
    if (!mp.count(a)) mp[a] = ++T, id[T] = a;
    if (!mp.count(b)) mp[b] = ++T, id[T] = b;
    g[mp[a]].push_back(mp[b]);
  }
  int ok = solve(T);
  if (!ok) return cout << "NO\n", 0;
  cout << "YES\n";
  string res = id[ans.front()];
  for (int i = 1; i < ans.size(); i++) res += id[ans[i]][1];
  cout << res << '\n';
  return 0;
}
// https://codeforces.com/contest/508/problem/D
