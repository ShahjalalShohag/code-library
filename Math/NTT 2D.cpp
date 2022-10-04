#include<bits/stdc++.h>
using namespace std;

const int N = 1 << 13;
const int mod = 998244353;
const int root = 3;
using Mat = vector<vector<int>>;
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
// a is of size n * n
// b is of size m * m
// max(n, m)^2 * log(max(n, m));
Mat multiply(Mat a, Mat b) {
  int n = a.size(), m = b.size();
  int len = n + m - 1;
  precompute(len);
  a.resize(lim);
  for (int i = 0; i < lim; i++) {
    a[i].resize(lim, 0);
  }
  b.resize(lim);
  for (int i = 0; i < lim; i++) {
    b[i].resize(lim, 0);
  }
  // convert rows to point value form
  for (int i = 0; i < lim; i++) {
    ntt(a[i], 1);
    ntt(b[i], 1);
  }
  Mat ans(lim, vector<int> (lim, 0));
  for (int j = 0; j < lim; j++) {
    vector<int> col_a(lim), col_b(lim);
    for (int i = 0; i < lim; i++) {
      col_a[i] = a[i][j];
      col_b[i] = b[i][j];
    }
    // convert columns to point value form
    ntt(col_a, 1);
    ntt(col_b, 1);
    // so everything is in point value form,
    // so compute the product easily
    for (int i = 0; i < lim; i++) {
      col_a[i] = 1LL * col_a[i] * col_b[i] % mod;
    }
    // inverse fft on columns
    ntt(col_a, 0);
    for (int i = 0; i < lim; i++) {
      a[i][j] = col_a[i];
    }
  }
  // inverse fft on rows
  for (int i = 0; i < lim; i++) {
    ntt(a[i], 0);
  }
  a.resize(n + m - 1);
  for (int i = 0; i < n + m - 1; i++) {
    a[i].resize(n + m - 1);
  }
  return a;
}
Mat multiply_brute(Mat a, Mat b) {
  int n = a.size(), m = b.size();
  Mat ans(n + m - 1, vector<int> (n + m - 1, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      for (int r = 0; r < m; r++) {
        for (int c = 0; c < m; c++) {
          ans[i + r][j + c] += 1LL * a[i][j] * b[r][c] % mod;
          ans[i + r][j + c] %= mod;
        }
      }
    }
  }
  return ans;
}
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n = 30, m = 119;
  vector<vector<int>> a(n, vector<int>(n, 0)), b(m, vector<int>(m, 0));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      a[i][j] = 1LL * rand() * rand() % mod;
    }
  }
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      b[i][j] = 1LL * rand() * rand() % mod;
    }
  }
  Mat brute = multiply_brute(a, b);
  for (int i = 0; i < n + m - 1; i++) {
    for (int j = 0; j < n + m - 1; j++) {
      cout << brute[i][j] << ' ';
    }
    cout << '\n';
  }
  cout << '\n';
  Mat fast = multiply(a, b);
  for (int i = 0; i < n + m - 1; i++) {
    for (int j = 0; j < n + m - 1; j++) {
      cout << fast[i][j] << ' ';
    }
    cout << '\n';
  }
  assert(brute == fast);
  return 0;
}
