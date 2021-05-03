#include<bits/stdc++.h>
using namespace std;

using ll = long long;

/**
given are a[i], b[i], p and q:
sum(x[i] * a[i]) = p --- (i)
sum(x[i] * b[i]) = q --- (ii)
x[i]s can only be integers.

does a solution exist?
**/
ll extended_euclid(ll a, ll b, ll& x, ll& y) {
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
ll a, b, coeff; // possible points that can be generated are of the form (a * m, b * m + k * coeff)
// add (a[i], b[i])
void add(ll c, ll d) {
  if (c == 0 and d == 0) return;
  if (a == 0 and b == 0) {
    a = c; b = d;
    return;
  }
  ll x0, y0;
  // a * x0 + c * y0 = m * gcd(a, c)
  // all solutions are of the form (x0 + k * (c / g), y0 - k * (a / g))
  ll g = extended_euclid(a, c, x0, y0); 
  // replace (x, y) to the equation b * x + d * y = z
  ll tmp_m = b * x0 + d * y0;
  ll tmp_coeff = abs(b * (c / g) - d * (a / g));
  coeff = __gcd(coeff, tmp_coeff);
  a = g;
  b = tmp_m % coeff;
}
// check if solution exists for some (p, q)
bool can(ll x, ll y) {
  if (x == 0 and y == 0) return true;
  if (coeff == 0) {
    if (a == 0 or x % a) return false;
    if (b == 0 or y % b) return false;
    return x / a == y / b;
  }
  if (a == 0 or x % a) return false;
  return (y - (x / a) * b) % coeff == 0;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t, cs = 0; cin >> t;
  while (t--) {
    int q; cin >> q;
    a = b = coeff = 0;
    ll ans = 0;
    while (q--) {
      int ty; cin >> ty;
      if (ty == 1) {
        ll c, d; cin >> c >> d;
        add(c, d);
      }
      else {
        ll x, y, w; cin >> x >> y >> w;
        if (can(x, y)) {
          ans += w;
        }
      }
    }
    cout << "Case #" << ++cs << ": " << ans << '\n';
  }
  return 0;
}
// https://codeforces.com/gym/102769/problem/I
