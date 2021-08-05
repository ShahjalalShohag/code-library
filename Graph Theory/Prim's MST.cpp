#include<bits/stdc++.h>
using namespace std;

const int N = 2020;
int g[N][N], w[N], to[N], selected[N];
long long Prims(int n, vector< pair<int, int> > &edges) {
  long long ans = 0;
  for(int i = 1; i <= n; i++) w[i] = 1e9, selected[i] = 0, to[i] = -1;
  w[1] = 0;
  for(int i = 1; i <= n; i++) {
    int u = -1;
    for(int j = 1; j <= n; j++) if(!selected[j] && (u == -1 || w[j] < w[u])) u = j;
    if (w[u] == 1e9) return - 1; //NO MST
    selected[u] = 1;
    ans += w[u];
    if(to[u] != -1) edges.emplace_back(u, to[u]); //order of the edges may be changed
    for(int v = 1; v <= n; v++) if(g[u][v] < w[v]) w[v] = g[u][v], to[v] = u;
  }
  return ans;
}
string s[N];
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n, m; cin >> n >> m;
  for(int i = 1; i <= n; i++) for(int j = 1; j <= n; j++) g[i][j] = 1e9;
  for(int i = 1; i <= n; i++) cin >> s[i];
  for(int i = 1; i <= n; i++){
    for(int j = i + 1; j <= n; j++){
      int w = 0;
      for(int k = 0; k < m; k++) w = max(w, (int)abs(s[i][k] - s[j][k]));
      g[i][j] = min(g[i][j], w);
      g[j][i] = min(g[j][i], w);
    }
  }
  vector< pair<int, int> > ed;
  long long ans = Prims(n, ed);
  int res = 0; for(auto e: ed) res = max(res, g[e.first][e.second]);
  cout << res << '\n';
  return 0;
}
/*
https://www.codechef.com/ICL2016/problems/ICL16A
*/
