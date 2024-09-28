#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
using ll = long long;

ll extended_euclid(ll a, ll b, ll &x, ll &y) {
  ll xx = y = 0;
  ll yy = x = 1;
  while (b) {
    ll q = a / b;
    ll t = b; b = a % b; a = t;
    t = xx; xx = x - q * xx; x = t;
    t = yy; yy = y - q * yy; y = t;
  }
  return a;
}
// a * x + b * y = c. returns valid x and y if possible.
bool find_any_solution (ll a, ll b, ll c, ll &x0, ll &y0, ll &g) {
  if (a == 0 and b == 0) {
    if (c) return false;
    x0 = y0 = g = 0; 
    return true;
  }
  g = extended_euclid (abs(a), abs(b), x0, y0);
  if (c % g != 0) return false;
  x0 *= c / g;
  y0 *= c / g;
  if (a < 0) x0 *= -1;
  if (b < 0) y0 *= -1;
  return true;
}

// sum(a[i] * x[i]) = c, returns any valid solution. returns empty vector in case of failure
// x[i] can be any integer
// Complexity: O(n log(MAX))
// Optimization: You can reduce the number of variables to O(log (MAX)) instead of n
// by only considering those values which reduces suffix gcds
vector<ll> find_any_solution(vector<ll> a, ll c) {
  int n = a.size();
  vector<ll> x;
  bool all_zero = true;
  for (int i = 0; i < n; i++) {
    all_zero &= a[i] == 0;
  }
  if (all_zero) {
    if (c) return {};
    x.assign(n, 0);
    return x;
  }
  ll g = 0;
  for (int i = 0; i < n; i++) {
    g = __gcd(g, a[i]);
  }
  if (c % g != 0) return {};
  if (n == 1) {
    return {c / a[0]};
  }
  vector<ll> suf_gcd(n);
  suf_gcd[n - 1] = a[n - 1];
  for (int i = n - 2; i >= 0; i--) {
    suf_gcd[i] = __gcd(suf_gcd[i + 1], a[i]);
  }
  ll cur = c;
  for (int i = 0; i + 1 < n; i++) {
    ll x0, y0, g;
    // solve for a[i] * x + suf_gcd[i + 1] * (y / suf_gcd[i + 1]) = cur
    bool ok = find_any_solution(a[i], suf_gcd[i + 1], cur, x0, y0, g);
    assert(ok);
    {
      // trying to minimize x0 in case x0 becomes big
      // it is needed for this problem, not needed in general
      ll shift = abs(suf_gcd[i + 1] / g);
      x0 = (x0 % shift + shift) % shift;
    }
    x.push_back(x0);

    // now solve for the next suffix
    cur -= a[i] * x0;
  }
  x.push_back(a[n - 1] == 0 ? 0 : cur / a[n - 1]);
  return x;
}

void solve() {
  ll x, y; cin >> x >> y;
  int n; cin >> n;
  vector<ll> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  ll c = y - x;
  auto ans = find_any_solution(a, c);
  assert(!ans.empty());
  for (int i = 0; i < n; i++) {
    cout << (ans[i] >= 0 ? ans[i] : 0) << '\n';
  }
  for (int i = 0; i < n; i++) {
    cout << (ans[i] < 0 ? -ans[i] : 0) << '\n';
  }
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t = 1;
  // cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
// https://codeforces.com/gym/102020/problem/I
