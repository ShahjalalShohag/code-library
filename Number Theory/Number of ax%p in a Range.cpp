#include<bits/stdc++.h>
using namespace std;

using ll = long long;

// number of integer solutions to ax + by <= c s.t. x, y >= 0
ll lattice_cnt(ll a, ll b, ll c) {
  assert(a >= 0 && b >= 0);
  if (c < 0) return 0;
  if (a == 0 or b == 0) {
    // infinite solutions
    assert(0);
    return -1;
  }
  assert(a > 0 && b > 0);
  if (a > b) swap(a, b);
  ll ans = 0;
  while (c >= 0) {
    ll k = b / a;
    ll l = b % a;
    ll f = c / b;
    ll e = c % b / a;
    ll g = c % b % a;
    ans += (f + 1) * (e + 1) + (f + 1) * f / 2 * k;
    c = f * l - a + g;
    b = a;
    a = l;
  }
  return ans;
}
// returns the number of 0 <= (a * x % m) <= c s.t. 0 <= x <= n
ll mod_count(ll a, ll m, ll c, ll n) {
  ++c; ++n;
  assert(m > 0);
  if (n == 0) return 0;
  a %= m; if (a < 0) a += m;
  ll extra_c = c / m; c %= m;
  if (c < 0) extra_c--, c += m;
  assert(0 <= c && c < m);
  ll ans = extra_c * n;
  ll extra_n = n / m; n %= m;
  if (n < 0) extra_n--, n += m;
  assert(0 <= n && n < m);
  if (extra_n) {
    ans += extra_n * (lattice_cnt(m, a + m, (a + m) * (m - 1)) - lattice_cnt(m, a + m, (a + m) * (m - 1) - c));
  }
  if (n) {
    ans += lattice_cnt(m, a + m, (a + m) * (n - 1)) - lattice_cnt(m, a + m, (a + m) * (n - 1) - c);
  }
  return ans;
}
// returns the count of nlo <= x <= nhi s.t. clo <= (a * x % m) <= chi 
ll mod_count_range(ll a, ll m, ll clo, ll chi, ll nlo, ll nhi) {
  return mod_count(a, m, chi, nhi) - mod_count(a, m, chi, nlo - 1) - mod_count(a, m, clo - 1, nhi) + mod_count(a, m, clo - 1, nlo - 1);
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  
  return 0;
}