#include <bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9, mod = 330301441;

int POW(long long n, long long k) {
  int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
  while (k) {
      if (k & 1) ans = (long long) ans * n % mod;
      n = (long long) n * n % mod;
      k >>= 1;
  }
  return ans;
}
namespace FWHT {
  const int MXK = 10; //(mod - 1) should be divisible by each base
  struct Matrix {
    int a[MXK][MXK], n, m;
    Matrix(int n, int m) : n(n), m(m) { }
  };
  Matrix operator * (Matrix& p, Matrix& q) {
    Matrix result(p.n, q.m);
    for (int i = 0; i < p.n; i++)
      for (int j = 0; j < q.m; j++) {
        long long val = 0;
        for (int k = 0; k < p.m; k++) val += 1LL * p.a[i][k] * q.a[k][j];
        result.a[i][j] = val % mod;
      }
    return result;
  }
  Matrix constructMatrix(int k, bool invert) {
    vector<int> divs;
    int x = mod - 1;
    for (int i = 2; i * i <= x; i++) if (x % i == 0) {
      divs.push_back((mod - 1) / i);
      while (x % i == 0) x /= i;
    }
    if (x > 1) divs.push_back((mod - 1) / x);
    int g = 2;
    while (true) {
      bool ok = true;
      for (int d : divs) if (POW(g, d) == 1) ok = false;
      if (ok) break;
      g++;
    }
    int root = POW(g, (mod - 1) / k);
    Matrix result(k, k);
    for (int i = 0; i < k; i++) {
      int x = POW(root, invert ? k - i : i);
      for (int j = 0; j < k; j++) result.a[i][j] = POW(x, j);
    }
    return result;
  }
  void transform(vector<int> &a, int n, int k, bool invert) {
    Matrix M = constructMatrix(k, invert);
    for (int len = 1; len < n; len *= k)
      for (int i = 0; i < n; i += k * len)
        for (int j = 0; j < len; j++) {
          Matrix V(1, k);
          for (int p = 0; p < k; p++) V.a[0][p] = a[i + j + p * len];
          V = V * M;
          for (int p = 0; p < k; p++) a[i + j + p * len] = V.a[0][p];
        }
    if (invert) {
      long long d = POW(n, mod - 2);
      for (int i = 0; i < n; i++) a[i] = a[i] * d % mod;
    }
  }
  //O(k^d * k * d) where d = log_k(MAX VALUE)
  vector<int> multiply(vector<int> a, vector<int> b, int k) {
    int n = 1;
    int nw = max((int)a.size() - 1, (int)b.size() - 1);
    while (n <= nw) n *= k;
    a.resize(n); b.resize(n);
    transform(a, n, k, false);
    transform(b, n, k, false);
    for (int i = 0; i < n; i++) a[i] = 1LL * a[i] * b[i] % mod;
    transform(a, n, k, true);
    return a;
  }
  vector<int> pow(vector<int> a, long long p, int k) {
    int n = 1;
    int nw = (int)a.size() - 1;
    while (n <= nw) n *= k;
    a.resize(n);
    transform(a, n, k, false);
    for (int i = 0; i < n; i++) a[i] = POW(a[i], p);
    transform(a, n, k, true);
    return a;
  }
}
int cnt[N];
int main() {
  int t; cin >> t;
  while (t--) {
    memset(cnt, 0, sizeof cnt);
    int n, k; long long x; cin >> n >> k >> x;
    for (int i = 0; i < n; i++) {
      int z; cin >> z; cnt[z]++;
    }
    int cur = POW(2, n), mx = 0;
    vector<int> MEX(N, 0);
    for (int i = 0; cur != 0; i++) {
      cur = 1LL * cur * POW(2, 1LL * cnt[i] * (mod - 2ll)) % mod;
      MEX[i] = cur;
      mx = i;
      cur = 1LL * cur * (POW(2, cnt[i]) - 1) % mod;
    }
    MEX.resize(mx + 1);
    auto res = FWHT::pow(MEX, x, k);
    int sum = 0, ans = 0;
    for (auto x: res) sum = (sum + x) % mod;
    for (int y = 0; y < res.size(); y++) {
      long long p = 1LL * res[y] * POW(sum, mod - 2) % mod;
      p = 1LL * POW(p, 3) * POW(y, 2) % mod;
      ans = (ans + POW(p, y)) % mod;
    }
    cout << ans << '\n';
  }
  return 0;
}
//https://www.codechef.com/problems/XORTREEH
