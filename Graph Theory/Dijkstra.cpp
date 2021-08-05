#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 998244353;

int n, m;
vector<pair<int, int>> g[N], r[N];
vector<long long> dijkstra(int s, int t, vector<int> &cnt) {
  const long long inf = 1e18;
  priority_queue<pair<long long, int>, vector<pair<long long, int>>, greater<pair<long long, int>>> q;
  vector<long long> d(n + 1, inf);
  vector<bool> vis(n + 1, 0);
  q.push({0, s});
  d[s] = 0;
  cnt.resize(n + 1, 0); // number of shortest paths
  cnt[s] = 1;
  while(!q.empty()) {
    auto x = q.top();
    q.pop();
    int u = x.second;
    if(vis[u]) continue;
    vis[u] = 1;
    for(auto y: g[u]) {
      int v = y.first;
      long long w = y.second;
      if(d[u] + w < d[v]) {
        d[v] = d[u] + w;
        q.push({d[v], v});
        cnt[v] = cnt[u];
      } else if(d[u] + w == d[v]) cnt[v] = (cnt[v] + cnt[u]) % mod;
    }
  }
  return d;
}

int u[N], v[N], w[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int s, t;
  cin >> n >> m >> s >> t;
  for(int i = 1; i <= m; i++) {
    cin >> u[i] >> v[i] >> w[i];
    g[u[i]].push_back({v[i], w[i]});
    r[v[i]].push_back({u[i], w[i]});
  }
  vector<int> cnt1, cnt2;
  auto d1 = dijkstra(s, t, cnt1);
  auto d2 = dijkstra(t, s, cnt2);

  long long ans = d1[t];
  for(int i = 1; i <= m; i++) {
    int x = u[i], y = v[i];
    long long nw = d1[x] + w[i] + d2[y];
    if(nw == ans && 1LL * cnt1[x] * cnt2[y] % mod == cnt1[t]) cout << "YES\n";
    else if(nw - ans + 1 < w[i]) cout << "CAN " << nw - ans + 1 << '\n';
    else cout << "NO\n";
  }
  return 0;
}
