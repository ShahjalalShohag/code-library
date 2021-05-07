#include<bits/stdc++.h>
using namespace std;

const int N = 1 << 18;
const int mod = 998244353;
const int root = 3;
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
  //while((int)a.size() && a.back() == 0) a.pop_back();
  return a;
}
int fact[N], ifact[N];
vector<int> shift(vector<int> &f, int c) { //f(x + c)
  int n=(int)f.size();
  precompute(n + n - 1);
  vector<int> a = f; a.resize(lim);
  for (int i = 0; i < n; ++i) a[i] = (long long) a[i] * fact[i] % mod;
  reverse(a.begin(), a.begin()+n);
  vector<int> b; b.resize(lim); b[0] = 1;
  for (int i = 1; i < n; ++i) b[i] = (long long) b[i - 1] * c % mod;
  for (int i = 0; i < n; ++i) b[i] = (long long) b[i] * ifact[i] % mod;
  ntt(a, 1), ntt(b, 1);
  for (int i = 0; i < lim; ++i) a[i] = (long long) a[i] * b[i] % mod;
  ntt(a, 0), reverse(a.begin(), a.begin() + n);
  vector<int> g; g.resize(n);
  for (int i = 0; i < n; ++i) g[i] = (long long) a[i] * ifact[i] % mod;
  return g;
}
// (x+1)*(x+2)*(x+3)...(x+n)
// O(n log n) only for ntt friendly primes
// otherwise use divide and conquer in O(n log^2 n)
vector<int> range_mul(int n) {
  if (n == 0) return vector<int>({1});
  if (n & 1) {
    vector<int> f = range_mul(n - 1);
    f.push_back(0);
    for (int i = (int)f.size()-1; i; --i) f[i] = (f[i - 1] + (long long) n * f[i]) % mod;
    f[0] = (long long) f[0] * n % mod;
    return f;
  } 
  else {
    int n_ = n >> 1;
    vector<int> f = range_mul(n_);
    vector<int> tmp = shift(f, n_);
    f.resize(n_ + 1);
    tmp.resize(n_ + 1);
    return multiply(f, tmp);
  }
}
// returns stirling1st(n, i) for 0 <= i <= n
vector<int> stirling(int n) {
  if (n == 0) return {1};
  vector<int> ans = range_mul(n - 1);
  ans.resize(n + 1);
  for (int i = n - 1; i >= 0; i--) {
    ans[i + 1] = ans[i];
  }
  ans[0] = 0;
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  fact[0] = 1;
  for (int i = 1; i < N; ++i) fact[i] = (long long) fact[i - 1] * i % mod;
  ifact[N - 1] = POW(fact[N - 1], mod - 2);
  for (int i = N - 1; i; --i) ifact[i - 1] = (long long) ifact[i] * i % mod;
  int n; cin >> n;
  auto ans = stirling(n);
  for (int i = 0; i <= n; i++) {
    cout << ans[i] << ' ';
  }
  return 0;
}
