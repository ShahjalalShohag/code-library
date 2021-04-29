#include<bits/stdc++.h>
using namespace std;

using ll = long long;
ll power(ll n, ll k) {
  ll ans = 1;
  while (k--) {
    ans *= n;
  }
  return ans;
}
// returns the number of distinct values of (a^k % p^cnt) over all integers a (p is prime)
// can be optimized by precalculating powers
// current complexity: O(cnt * cnt)
ll f(ll p, ll cnt, ll k) {
  if (cnt <= 0 or k == 0) return 1;
  if (p == 2) {
    if (cnt == 1) return 2;
    ll u = power(2, cnt - 2) / __gcd(k, power(2, cnt - 2));
    if (k % 2) u *= 2;
    return u + f(2, cnt - k, k);
  }
  ll phi = power(p, cnt) - power(p, cnt - 1);
  ll u = phi / __gcd(k, phi);
  return u + f(p, cnt - k, k);
}
// returns the number of distinct values of (a^k % n) over all integers a
ll yo(ll k, ll n) {
  ll ans = 1;
  for (ll i = 2; i * i <= n; i++) {
    if (n % i == 0) {
      int cnt = 0;
      while (n % i == 0) {
        cnt++;
        n /= i;
      }
      ans *= f(i, cnt, k);
    } 
  }
  if (n > 1) ans *= f(n, 1, k);
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  for (int k = 0; k <= 5; k++) {
    for (int p = 1; p <= 12; p++) {
      set<int> se;
      for (int i = 0; i <= p; i++) {
        se.insert(power(i, k) % p);
      }
      assert(se.size() == yo(k, p));
    }
  }
  return 0;
}