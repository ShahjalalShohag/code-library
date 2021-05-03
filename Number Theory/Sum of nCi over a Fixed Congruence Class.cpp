#include<bits/stdc++.h>
using namespace std;

const int N = 1 << 16;
const int mod = 998244353;
const int root = 3;
using ll = long long;
int lim, rev[N], w[N], wn[N], inv_lim;
void reduce(int &x) { x = (x + mod) % mod; }
int POW(int x, int y, int ans = 1) {
  for (; y; y >>= 1, x = (long long) x * x % mod) if (y & 1) ans = (long long) ans * x % mod;
  return ans;
}
void precompute(int len) {
  lim = wn[0] = 1; int s = -1;
  while (lim < len) lim <<= 1, ++s;
  for (int i = 0; i < lim; ++i) rev[i] = rev[i >> 1] >> 1 | (i & 1) << s;
  const int g = POW(root, (mod - 1) / lim);
  inv_lim = POW(lim, mod - 2);
  for (int i = 1; i < lim; ++i) wn[i] = (long long) wn[i - 1] * g % mod;
}
void ntt(vector<int> &a, int typ) {
  for (int i = 0; i < lim; ++i) if (i < rev[i]) swap(a[i], a[rev[i]]);
  for (int i = 1; i < lim; i <<= 1) {
    for (int j = 0, t = lim / i / 2; j < i; ++j) w[j] = wn[j * t];
    for (int j = 0; j < lim; j += i << 1)
      for (int k = 0; k < i; ++k) {
        const int x = a[k + j], y = (long long) a[k + j + i] * w[k] % mod;
        reduce(a[k + j] += y - mod), reduce(a[k + j + i] = x - y);
      }
  }
  if (!typ) {
    reverse(a.begin() + 1, a.begin() + lim);
    for (int i = 0; i < lim; ++i) a[i] = (long long) a[i] * inv_lim % mod;
  }
}
vector<int> multiply(vector<int> &f, vector<int> &g) {
  int n=(int)f.size() + (int)g.size() - 1;
  precompute(n);
  vector<int> a = f, b = g;
  a.resize(lim); b.resize(lim);
  ntt(a, 1), ntt(b, 1);
  for (int i = 0; i < lim; ++i) a[i] = (long long) a[i] * b[i] % mod;
  ntt(a, 0);
  while((int)a.size() && a.back() == 0) a.pop_back();
  return a;
}
int m;
void fix(vector<int> &ans) {
  int k = ans.size();
  if (k < m) return;
  for (int i = k - 1; i >= m; i--) {
    ans[i % m] += ans[i];
    ans[i % m] %= mod;
  }
  ans.resize(m);
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  ll n; cin >> n; int x; cin >> x >> m;
  vector<int> a({1, 1}), ans({1});
  while (n) {
    if (n & 1) {
      ans = multiply(ans, a); fix(ans);
    }
    a = multiply(a, a); fix(a);
    n >>= 1;
  }
  ans.resize(m);
  // ans[i] = sum of nCk s.t. k % m = i (0 <= k <= n)
  int cur = 1, res = 0;
  for (int i = 0; i < m; i++) {
    res += 1LL * cur * ans[i] % mod;
    res %= mod;
    cur = 1LL * cur * x % mod;
  }
  cout << res << '\n';
  return 0;
}
// https://toph.co/p/abcdefgh