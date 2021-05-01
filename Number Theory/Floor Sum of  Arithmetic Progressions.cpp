#include <bits/stdc++.h>
using namespace std;

using ll = long long;
ll sumsq(ll n) {
  return n / 2 * ((n - 1) | 1);
}
// \sum_{i = 0}^{n - 1}{(a + d * i) / m}, O(log m)
ll floor_sum(ll a, ll d, ll m, ll n) {
  ll res = d / m * sumsq(n) + a / m * n;
  d %= m; a %= m;
  if (!d) return res;
  ll to = (n * d + a) / m;
  return res + (n - 1) * to - floor_sum(m - 1 - a, m, d, to);
}
// \sum_{i = 0}^{n - 1}{(a + d * i) % m}
ll mod_sum(ll a, ll d, ll m, ll n) {
  a = ((a % m) + m) % m;
  d = ((d % m) + m) % m;
  return n * a + d * sumsq(n) - m * floor_sum(a, d, m, n);
}
int main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  ll a, d, n; 
  while (cin >> a >> n >> d) {
    n = (n - a) / d;
    ll ans = 0;
    for (int k = 0; k < 32; k++) {
      ll cur = mod_sum(a, d, (1LL << k + 1), n + 1);
      cur -= mod_sum(a, d, (1LL << k), n + 1);
      if (cur / (1LL << k) & 1) {
        ans += 1LL << k;
      }
    }
    cout << ans << '\n';
  }
  return 0;
}
// http://poj.org/problem?id=3495