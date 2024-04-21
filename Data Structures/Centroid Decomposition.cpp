#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

vector<int> g[N];
int sz[N];
int tot, done[N], cenpar[N];
void calc_sz(int u, int p) {
  tot ++;
  sz[u] = 1;
  for (auto v : g[u]) {
    if(v == p || done[v]) continue;
    calc_sz(v, u);
    sz[u] += sz[v];
  }
}
int find_cen(int u, int p) {
  for (auto v : g[u]) {
    if(v == p || done[v]) continue;
    else if(sz[v] > tot / 2) return find_cen(v, u);
  }
  return u;
}
void decompose(int u, int pre) {
  tot = 0;
  calc_sz(u, pre);
  int cen = find_cen(u, pre);
  cenpar[cen] = pre;
  done[cen] = 1;
  for(auto v : g[cen]) {
    if(v == pre || done[v]) continue;
    decompose(v, cen);
  }
}
int dep[N];
void dfs(int u, int p = 0) {
  for(auto v : g[u]) {
    if(v == p) continue;
    dep[v] = dep[u] + 1;
    dfs(v, u);
  }
}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n;
  cin >> n;
  for(int i = 1; i < n; i++) {
    int u, v;
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  decompose(1, 0);

  
  for(int i = 1; i <= n; i++) g[i].clear();
  int root;
  for(int i = 1; i <= n; i++) {
    g[cenpar[i]].push_back(i);
    g[i].push_back(cenpar[i]);
    if (cenpar[i] == 0) root = i;
  }
  dfs(root);
  for(int i = 1; i <= n; i++) cout << char(dep[i] + 'A') << ' ';
  return 0;
}
// https://codeforces.com/contest/321/problem/C
