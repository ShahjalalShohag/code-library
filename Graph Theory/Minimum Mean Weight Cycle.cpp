#include<bits/stdc++.h>
using namespace std;

const int N = 26 * 27;

vector<pair<int, int>> g[N];
bool vis[N];
double d[N];
bool spfa(int u, double x) {
  vis[u] = 1;
  for(auto e : g[u]) {
    double w = -x + e.second;
    int v = e.first;
    if(d[u] + w > d[v]) { // d[u] + w < d[v] for minimum mean weight
      if(vis[v]) return 0;
      d[v] = d[u] + w;
      if(!spfa(v, x)) return 0;
    }
  }
  vis[u] = 0;
  return 1;
}

bool ok(int n, double x) {
  for(int i = 1; i <= n; i++) d[i] = 0, vis[i] = 0;
  for(int s = 1; s <= n; s++) {
    if(!spfa(s, x)) return 1;
  }
  return 0;
}
void add_edge(char a, char b, char c, char d, int w) {
  int u = (a - 'a') * 26 + (b - 'a' + 1);
  int v = (c - 'a') * 26 + (d - 'a' + 1);
  g[u].push_back({v, w});
}
//this code is for MAXIMUM mean weight cycle
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n, m;
  while(cin >> n && n) {
    for(int i = 1; i <= n; i++) {
      string s;
      cin >> s;
      int len = s.size();
      if(len < 2) continue;
      add_edge(s[0], s[1], s[len - 2], s[len - 1], len);
    }
    int n = 26 * 26;
    double l = 0, r = 1010;
    int it = 200;
    while(it--) {
      double mid = (l + r) * 0.5;
      if(ok(n, mid)) l = mid; //r = mid for minimum
      else r = mid; // l = mid = for minimum
    }
    if(l == 0.0) cout << "No solution.\n"; //l = 1010.0 for mimimum
    else cout << fixed << setprecision(2) << l << '\n';
    for(int i = 0; i < N; i++) g[i].clear();
  }
  return 0;
}
//https://www.spoj.com/problems/WORDRING/
