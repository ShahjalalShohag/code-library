#include<bits/stdc++.h>
using namespace std;

using ll = long long;

// number of integer solutions to ax + by <= c s.t. x, y >= 0
// number of nonnegative integer lattice points under or on the line ax + by = c
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
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  cout << lattice_cnt(2, 3, 5) << '\n';
  return 0;
}
