#include<bits/stdc++.h>
using namespace std;

const int M = 42;
using ll = long long;
// number of cliques in a graph including null clique
// meet in the middle
// Complexity: O((M/2)*2^(M/2))
int dp[(1 << (M / 2))];
ll g[M];
int32_t main() {
  int n, m; cin >> n >> m;
  for(int i = 0; i < m; i++) {
    int u, v; cin >> u >> v;
    --u;--v;
    g[u] |= 1LL << v;
    g[v] |= 1LL << u;
  }
  for (int i = 0; i < n; i++) g[i] |= 1LL << i;
  int k = n / 2;
  dp[0] = 1;
  for (int i = 1; i < (1 << k); i++) {
    ll nw = (1LL << n) - 1;
    for (int j = 0; j < k; j++) {
      if ((i >> j) & 1) nw &= g[j];
    }
    if ((nw & i) == i) dp[i] = 1;
  }
  for (int i = 0; i < k; i++) {
    for (int mask = 0; mask < (1 << k); mask++) {
      if ((mask >> i) & 1) dp[mask] += dp[mask ^ (1 << i)];
    }
  }
  ll ans = dp[(1 << k) - 1];
  k = n - k;
  for (int i = 1; i < (1 << k); i++) {
    ll nw = (1LL << n) - 1;
    for (int j = 0; j < k; j++) {
      if ((i >> j) & 1) nw &= g[n / 2 + j];
    }
    ll p = (1LL * i) << (n / 2);
    if ((nw & p) == p) {
      ll x = nw & ((1LL << (n / 2)) - 1);
      ans += dp[x];
    }
  }
  cout << ans << '\n';
  return 0;
}