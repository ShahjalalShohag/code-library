#include<bits/stdc++.h>
using namespace std;

const int N = 42;

int g[N][N];
int res;
long long edges[N];
//3 ^ (n / 3)
void BronKerbosch(int n, long long R, long long P, long long X) {
  if (P == 0LL && X == 0LL) { //here we will find all possible maximal cliques (not maximum) i.e. there is no node which can be included in this set
    int t = __builtin_popcountll(R);
    res = max(res, t);
    return;
  }
  int u = 0;
  while (!((1LL << u) & (P | X))) u ++;
  for (int v = 0; v < n; v++) {
    if (((1LL << v) & P) && !((1LL << v) & edges[u])) {
      BronKerbosch(n, R | (1LL << v), P & edges[v], X & edges[v]);
      P -= (1LL << v);
      X |= (1LL << v);
    }
  }
}

int max_clique (int n) {
  res = 0;
  for (int i = 1; i <= n; i++) {
    edges[i - 1] = 0;
    for (int j = 1; j <= n; j++)  if (g[i][j]) edges[i - 1] |= ( 1LL << (j - 1) );
  }
  BronKerbosch(n, 0, (1LL << n) - 1, 0);
  return res;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  map<string, int> mp;
  set<int>se;
  int n, m;
  cin >> n >> m;
  for (int i = 1; i <= n + 1; i++) {
    int ty;
    if (i <= n) cin >> ty;
    if (ty == 1 || i > n) {
      for (auto x : se) for (auto y : se) g[x][y] = 1, g[y][x] = 1;
      se.clear();
    } else {
      string s;
      cin >> s;
      if (mp.find(s) == mp.end()) {
        mp[s];
        mp[s] = mp.size();
      }
      int p = mp[s];
      se.insert(p);
    }
  }
  for (int i = 1; i <= m; i++) for (int j = 1; j <= m; j++) g[i][j] = !g[i][j];
  for (int i = 1; i <= m; i++) g[i][i] = 0;
  cout << max_clique(m) << '\n';
  return 0;
}
// https://codeforces.com/contest/1105/problem/E
