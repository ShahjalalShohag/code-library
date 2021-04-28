#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
using ll = long long;

map<ll, ll> mp;
ll phi(ll n) {
  if (mp.count(n)) return mp[n];
  ll ans = n, m = n;
  for (ll i = 2; i * i <= m; i++) {
    if (m % i == 0) {
      while (m % i == 0) m /= i;
      ans = ans / i * (i - 1);
    }
  }
  if (m > 1) ans = ans / m * (m - 1);
  return mp[n] = ans;
}
inline ll MOD(ll x, ll m) {
  if (x < m) return x;
  return x % m + m;
}
ll power(ll n, ll k, ll mod) {
  ll ans = MOD(1, mod);
  while (k) {
    if (k & 1) ans = MOD(ans * n, mod);
    n = MOD(n * n, mod);
    k >>= 1;
  }
  return ans;
}
int a[N];
// if x >= log2(m), then a^x = a^(MOD(x, phi(m))) % m
ll yo(ll l, ll r, ll m) {
  if (l == r) return MOD(a[l], m);
  if (m == 1) return 1;
  return power(a[l], yo(l + 1, r, phi(m)), m); 
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, m; cin >> n >> m;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  int q; cin >> q;
  while (q--) {
    int l, r; cin >> l >> r;
    cout << yo(l, r, m) % m << '\n';
  }
  return 0;
}
// https://codeforces.com/contest/906/problem/D