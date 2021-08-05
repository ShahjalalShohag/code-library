#include<bits/stdc++.h>
using namespace std;

const int N = 1010;

// Johnson's algorithm for all pair shortest paths with negative edges
// Complexity: O(N * M) + O(N * M * log(N))

const long long INF = 1e18;

struct edge {
  int u, v;
  long long w;
  edge(){}
  edge(int u, int v, long long w) : u(u), v(v), w(w){}
};

bool bellman_ford(int n, int s, vector <struct edge> ed, vector <long long> &d){
  d[s] = 0;
  for (int i = 0; i <= n; i++){
    int flag = 0;
    for (auto e: ed){
      if (d[e.u] + e.w < d[e.v]){
        flag = 1;
        d[e.v] = d[e.u] + e.w;
      }
    }
    if (flag == 0) return 1;
  }
  return 0;
}

vector <long long> dijkstra(int n, int s, vector <struct edge> ed, vector <long long> &h){
  vector <pair<int, long long> > g[n + 1];
  priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> q;
  for (auto e: ed){
    g[e.u].push_back({e.v, e.w + h[e.u] - h[e.v]});
  }
  vector <long long> d(n + 1, INF);vector<bool> vis(n + 1, 0);
  q.push({0, s});
  d[s] = 0;
  while(!q.empty()){
    auto x = q.top(); q.pop();
    int u = x.second;
    if(vis[u]) continue; vis[u] = 1;
    for(auto e: g[u]){
      int v = e.first; long long w = e.second;
      if(d[u] + w < d[v]){
        d[v] = d[u] + w; q.push({d[v], v});
      }
    }
  }
  for(int i = 1; i <= n; i++) if(d[i] < INF) d[i] -= h[s] - h[i];
  return d;
}

long long ans[N][N];
int johnson(int n, vector <struct edge> ed){
  vector <long long> potential(n + 1, INF);
  for (int i = 1; i <= n; i++) ed.push_back(edge(0, i, 0));
  if(bellman_ford(n, 0, ed, potential) == 0) return 0;
  cout << "yes\n\n";
  for(int i = 1; i <= n; i++){
    if(i > 1) cout << ' ';
    cout << potential[i];
  }
  cout << ' ' << potential[0] << "\n\n";
  for (int i = 1; i <= n; i++) ed.pop_back();
  for (int i = 1; i <= n; i++){
    vector <long long> d = dijkstra(n, i, ed, potential);
    for (int j = 1; j <= n; j++) ans[i][j] = d[j];
  }
  return 1;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t, cs = 0; cin >> t;
  while(t--){
    int n , m; cin >> n >> m;
    vector <edge> ed;
    while(m--){
      int u, v, w; cin >> u >> v >> w;
      ed.push_back({u, v, w});
    }
    cout << "graph " << ++cs << ' ';
    int ok = johnson(n, ed);
    if(!ok){
      cout << "no\n";
      continue;
    }
    for (int i = 1; i <= n; i++){
      for (int j = 1; j <= n; j++){
        if(j > 1) cout << ' ';
        if(ans[i][j] < INF) cout << ans[i][j];
        else cout << "#";
      }
      cout << '\n';
    }
  }
  return 0;
}
// https://www.spoj.com/problems/JHNSN/
