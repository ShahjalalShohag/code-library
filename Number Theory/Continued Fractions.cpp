#include<bits/stdc++.h>
using namespace std;

using ll = long long;
// returns the continued fraction of p / q
// [a[0]; a[1], ..., a[n]] = a[0] + 1 / (a[1] + 1 / (a[2] + ...) + 1 / (a[n - 1] + 1 / a[n]))))
// a[0] is integer number and a[1],a[2],…,a[n] are positive integer numbers and either n = 0 or a[n] != 1
// its unique
vector<ll> get_frac(ll p, ll q) {
  vector<ll> a;
  while (q) {
    a.push_back(p / q);
    p %= q; swap(p, q);
  }
  return a;
}
// ans[k] = [a[0]; a[1], ..., a[k]]
vector<pair<ll, ll>> convergents(vector<ll> a) {
  ll lp = 1, lq = 0;
  vector<pair<ll, ll>> ans({{a[0], 1}});
  for (int i = 1; i < a.size(); i++) {
    ll p = a[i] * ans.back().first + lp;
    ll q = a[i] * ans.back().second + lq;
    // assert(__gcd(p, q) == 1)
    lp = ans.back().first;
    lq = ans.back().second;
    ans.push_back({p, q});
  }
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  ll i = 10, j = 23;
  auto a = get_frac(i, j);
  for (auto x: a) cout << x << ' '; cout << '\n';
  auto  p = convergents(a);
  for (auto [x, y]: p) {
    cout << x << ' ' << y << '\n';
  }
  return 0;
}
// https://codeforces.com/blog/entry/73655