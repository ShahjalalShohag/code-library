#include<bits/stdc++.h>
using namespace std;

using ll = long long;
ll power(ll a, ll n, ll p) {
  ll r = 1;
  for (; n > 0; n >>= 1, a = a * a % p) if(n % 2 == 1) r = r * a % p;
  return r;
}
int cnt(ll a, ll base,ll p) {
  int ret = 0;
  while (a != 1) {
    a = power(a, base, p);
    ++ret;
  }
  return ret;
}
ll inverse(ll a, ll p) {
  a %= p;
  ll u = 1, v = 0;
  ll b = p;
  while (b > 0) {
    ll q = a / b;
    a %= b;
    u -= v * q % p;
    u = (u % p + p) % p;
    u ^= v; v ^= u;
    u ^= v; a ^= b;
    b ^= a; a ^= b;
  }
  return u < 0 ? u + p : u;
}
ll gcd(ll a,ll b) {
  return a == 0 ? b : gcd(b % a, a);
}
ll peth_root(ll a, ll p, int e, ll mod) {
  ll q = mod - 1;
  int s = 0;
  while (q % p == 0) {
    q /= p;
    ++s;
  }
  ll pe = power(p, e, mod);
  ll ans = power(a, ((pe - 1) * inverse(q, pe) % pe * q + 1) / pe, mod);
  ll c = 2;
  while (power(c, (mod - 1)/p, mod) == 1) ++c;
  c = power(c, q, mod);
  map<ll, int> mp;
  ll add = 1;
  int v = (int)sqrt((double)(s - e) * p) + 1;
  ll mul = power(c, v * power(p, s - 1, mod - 1) % (mod - 1), mod);
  for (int i = 0; i <= v; ++i) {
    mp[add] = i;
    add = add * mul % mod;
  }
  mul = inverse(power(c, power(p, s - 1, mod - 1), mod), mod);
  for (int i = e; i<s; ++i) {
    ll err = inverse(power(ans, pe, mod), mod) * a % mod;
    ll target = power(err, power(p, s - 1 - i, mod - 1), mod);
    for (int j = 0; j <= v; ++j) {
      if (mp.find(target) != mp.end()) {
        int x = mp[target];
        ans = ans * power(c, (j + v * x) * power(p, i - e, mod - 1) % (mod - 1), mod) % mod;
        break;
      }
      target = target * mul % mod;
      assert(j != v);
    }
  }
  return ans;
}
// returns any x such that x ^ k  =  a (mod p),  p is a prime
// 0^0 = 1 mod p
// Complexity: O(p ^ (1 / 4))
ll discrete_root(ll k, ll a, ll p) {
  if (k > 0 && a == 0) return 0;
  k %= p - 1;
  ll g = gcd(k, p - 1);
  if (power(a, (p - 1) / g, p) != 1) return  -1; // checking existence
  a = power(a, inverse(k / g, (p - 1) / g), p);
  for (ll div = 2; div * div <= g; ++div) {
    int sz = 0;
    while (g % div == 0) g /= div, ++sz;
    if (sz > 0) {
      ll b = peth_root(a, div, sz, p);
      a = b;
    }
  }
  if (g > 1) a = peth_root(a, g, 1, p);
  return a;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int t; cin >> t;
  while (t--) {
    ll k, a, p; cin >> k >> a >> p;
    cout << discrete_root(k, a, p) << '\n';
  }
}
// https://judge.yosupo.jp/problem/kth_root_mod
