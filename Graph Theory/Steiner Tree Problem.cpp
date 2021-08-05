#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

/*
Find the minimum cost connected tree where at least the important nodes are connected
dp(x,i) = minimum cost of a tree rooted at i connecting the important node in bitmask x.
Complexity: O(3^k * n + 2^k * m log m)
*/

int n, k, m;
vector<int> imp;//k important nodes
vector<pair<int, long long>> g[N];
long long d[32][N]; //[2^k][edge count]
const long long inf = LLONG_MAX / 3;
bool vis[N];
long long MST() {
  for(int i = 0; i < (1 << k); i++) fill(d[i], d[i] + N, inf);
  for(int i = 0; i < k; ++i) {
    d[1 << i][imp[i]] = 0;
  }
  priority_queue<pair<long long, int>> q;
  for(int mask = 1; mask < (1 << k); ++mask) {
    for(int a = 0; a < mask; ++a) { //you can still fasten this loop to get exact 3^k complexity
      if((a | mask) != mask) continue; //we only need the subsets
      int b = mask ^ a;
      if(b > a) continue;
      for(int v = 0; v < n; ++v) {
        d[mask][v] = min(d[mask][v], d[a][v] + d[b][v]);
      }
    }
    memset(vis, 0, sizeof vis);
    for(int v = 0; v < n; ++v) {
      if(d[mask][v] == inf) continue;
      q.emplace(-d[mask][v], v);
    }

    while(!q.empty()) {
      long long cost = -q.top().first;
      int v = q.top().second;
      q.pop();
      if(vis[v]) continue;
      vis[v] = true;
      for(auto edge : g[v]) {
        long long ec = cost + edge.second;
        if(ec < d[mask][edge.first]) {
          d[mask][edge.first] = ec;
          q.emplace(-ec, edge.first);
        }
      }
    }
  }

  long long res = inf;
  for(int v = 0; v < n; ++v) {
    res = min(res, d[(1 << k) - 1][v]);
  }
  return res;
}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  cin >> n >> k >> m;
  imp.resize(k);
  for(int i = 0; i < k; ++i) {
    cin >> imp[i];
    --imp[i];
  }
  for(int i = 0; i < m; ++i) {
    int u, v;
    long long w;
    cin >> u >> v >> w;
    --u;
    --v;
    g[u].emplace_back(v, w);
    g[v].emplace_back(u, w);
  }
  cout << MST() << '\n';
  return 0;
}
//https://www.youtube.com/watch?v=BG4vAoV5kWw
