#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
#include<ext/pb_ds/tree_policy.hpp>
using namespace __gnu_pbds;
using namespace std;

// baby step - giant step
// returns minimum integer x such that a^x = b (mod m)
// a and m are co-prime
int discrete_log(int a, int b, int m) {
  static const int inf = 2e9;
  int n = (int) sqrt (m + .0) + 1;
  int pw = 1;
  for (int i = 0; i < n; ++i) pw = 1LL * pw * a % m;
  gp_hash_table<int, int> vals;
  for (int p = 1, cur = pw; p <= n; ++p) {
    if (!vals[cur]) vals[cur] = p;
    cur = 1LL * cur * pw % m;
  }
  int ans = inf;
  for (int q = 0, cur = b; q <= n; ++q) {
    if (vals.find(cur) != vals.end()) {
      long long nw = 1LL * vals[cur] * n - q;
      if (nw < ans) ans = nw;
    }
    cur = (1LL * cur * a) % m;
  }
  if (ans == inf) ans = -1;
  return ans;
}
using ll = long long;
ll extended_euclid(ll a, ll b, ll &x, ll &y) {
  if (b == 0) {
    x = 1; y = 0;
    return a;
  }
  ll x1, y1;
  ll d = extended_euclid(b, a % b, x1, y1);
  x = y1;
  y = x1 - y1 * (a / b);
  return d;
}
ll inverse(ll a, ll m) {
  ll x, y;
  ll g = extended_euclid(a, m, x, y);
  if (g != 1) return -1;
  return (x % m + m) % m;
}
// discrete log but a and m may not be co-prime
int discrete_log_noncoprime(int a, int b, int m) {
  if (m == 1) return 0;
  if (b == 1) return 0;
  if (__gcd(a, m) == 1) return discrete_log(a, b, m);
  int g = __gcd(a, m);
  if (b % g != 0)  return -1;
  int p = inverse(a / g, m / g);
  int nw = discrete_log_noncoprime(a, 1LL * b / g * p % (m / g), m / g);
  if (nw == -1) return -1;
  return nw + 1;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t = 1000;
  while (t--) {
    int m = rand() % 100000 + 2, a = rand() % m + 1, b = rand() % m;
    cout << discrete_log_noncoprime(a, b, m) << '\n';
  }
  return 0;
}