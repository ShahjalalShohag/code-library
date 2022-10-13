#include<bits/stdc++.h>
using namespace std;

const int N = 1 << 16;
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
    for (int j = 0; j < lim; j += i << 1) {
      for (int k = 0; k < i; ++k) {
        const int x = a[k + j], y = (long long) a[k + j + i] * w[k] % mod;
        reduce(a[k + j] += y - mod), reduce(a[k + j + i] = x - y);
      }
    }
  }
  if (!typ) {
    reverse(a.begin() + 1, a.begin() + lim);
    for (int i = 0; i < lim; ++i) a[i] = (long long) a[i] * inv_lim % mod;
  }
}
vector<int> multiply(vector<int> f, vector<int> g) {
  int n=(int)f.size() + (int)g.size() - 1;
  precompute(n);
  vector<int> a = f, b = g;
  a.resize(lim); b.resize(lim);
  ntt(a, 1), ntt(b, 1);
  for (int i = 0; i < lim; ++i) a[i] = (long long) a[i] * b[i] % mod;
  ntt(a, 0);
  return a;
}

using poly = vector<int>;

struct Mat {
  int n, m;
  vector<vector<poly>> a;
  Mat() { }
  Mat(int _n, int _m) {n = _n; m = _m; a.assign(n, vector<poly>(m, poly({0}))); }
  Mat(vector< vector<poly> > v) { n = v.size(); m = n ? v[0].size() : 0; a = v; }
  inline void make_unit() {
    assert(n == m);
    for (int i = 0; i < n; i++)  {
      for (int j = 0; j < n; j++) a[i][j] = poly({i == j});
    }
  }
  inline Mat operator + (const Mat &b) {
    assert(n == b.n && m == b.m);
    Mat ans = Mat(n, m);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < m; j++) {
        int sz = max(a[i][j].size(), b.a[i][j].size());
        auto &p = ans.a[i][j];
        p.resize(sz);
        for (int k = 0; k < sz; k++) {
          if (k < a[i][j].size()) p[k] = a[i][j][k];
          if (k < b.a[i][j].size()) {
            p[k] += b.a[i][j][k];
            if (p[k] >= mod) {
              p[k] -= mod;
            }
          }
        }
      }
    }
    return ans;
  } 
  inline Mat operator - (const Mat &b) {
    assert(n == b.n && m == b.m);
    Mat ans = Mat(n, m);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < m; j++) {
        int sz = max(a[i][j].size(), b.a[i][j].size());
        auto &p = ans.a[i][j];
        p.resize(sz);
        for (int k = 0; k < sz; k++) {
          if (k < a[i][j].size()) p[k] = a[i][j][k];
          if (k < b.a[i][j].size()) {
            p[k] -= b.a[i][j][k];
            if (p[k] < 0) {
              p[k] += mod;
            }
          }
        }
      }
    }
    return ans;
  }
  inline Mat operator * (const Mat &b) {
    assert(m == b.n);
    Mat ans = Mat(n, b.m);
    for(int i = 0; i < n; i++) {
      for(int j = 0; j < b.m; j++) {
        for(int k = 0; k < m; k++) {
          poly prod = multiply(a[i][k], b.a[k][j]);
          auto &p = ans.a[i][j];
          if (p.size() < prod.size()) {
            p.resize(prod.size());
          }
          for (int x = 0; x < prod.size(); x++) {
            p[x] += prod[x];
            if (p[x] >= mod) {
              p[x] -= mod;
            }
          }
        }
      }
    }
    return ans;
  }
  void reduce(int z) { // % x^z
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        if (a[i][j].size() > z) {
          a[i][j].resize(z);
        }
      }
    }
  }
  inline Mat pow(long long k, int z) { // M^k % x^z
    assert(n == m);
    Mat ans(n, n), t = a; ans.make_unit();
    while (k) {
      if (k & 1) {
        ans = ans * t;
        ans.reduce(z);
      }
      t = t * t;
      t.reduce(z);
      k >>= 1;
    }
    return ans;
  }
  inline Mat& operator += (const Mat& b) { return *this = (*this) + b; }
  inline Mat& operator -= (const Mat& b) { return *this = (*this) - b; }
  inline Mat& operator *= (const Mat& b) { return *this = (*this) * b; }
};

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, k; cin >> n >> k;
  Mat M(2, 2);
  M.a[0][0] = poly({1, 1});
  M.a[0][1] = poly({0, 1});
  M.a[1][0] = poly({1});
  M.a[1][1] = poly({0});
  M = M.pow(n - 1, k + 1);
  poly p0 = poly({1});
  poly p1 = poly({1, 1});
  poly a = multiply(M.a[0][0], p1);
  poly b = multiply(M.a[0][1], p0);
  poly ans(k + 1);
  a.resize(k + 1);
  b.resize(k + 1);
  for (int i = 0; i <= k; i++) {
    ans[i] = a[i] + b[i];
    ans[i] %= mod;
  }
  for (int i = 1; i <= k; i++) {
    cout << ans[i] << ' ';
  }
  cout << '\n';
  return 0;
}
// https://codeforces.com/contest/755/problem/G
