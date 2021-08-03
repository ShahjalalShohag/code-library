#include<bits/stdc++.h>
using namespace std;

const int N = 3e5 + 9, mod = 1e9 + 7;
int power(long long n, long long k, const int mod) {
  int ans = 1 % mod; n %= mod; if (n < 0) n += mod;
  while (k) {
    if (k & 1) ans = (long long) ans * n % mod;
    n = (long long) n * n % mod;
    k >>= 1;
  }
  return ans;
}
bool Gauss(vector<vector<int>> a, vector<vector<int>> &ans, const int mod) {
  int n = (int)a.size(); vector<int> col(n);
  vector<vector<int>> tmp(n, vector<int>(n));
  for (int i = 0; i < n; i++) tmp[i][i] = 1, col[i] = i;
  for (int i = 0; i < n; i++) {
    int r = i, c = i;
    for (int j = i; j < n; j++) {
      for (int k = i; k < n; k++) {
        if (a[j][k]) {
          r = j; c = k; goto found;
        }
      }
    }
    return false; //not invertible
    found:
    a[i].swap(a[r]); tmp[i].swap(tmp[r]);
    for (int j = 0; j < n; j++) {
      swap(a[j][i], a[j][c]); swap(tmp[j][i], tmp[j][c]);
    }
    swap(col[i], col[c]);
    int v = power(a[i][i], mod - 2, mod);
    for (int j = i + 1; j < n; j++) {
      int f = 1LL * a[j][i] * v % mod;
      a[j][i] = 0;
      for (int k = i + 1; k < n; k++) {
        a[j][k] -= 1LL * f * a[i][k] % mod;
        if (a[j][k] < 0) a[j][k] += mod;
      }
      for (int k = 0; k < n; k++) {
        tmp[j][k] -= 1LL * f * tmp[i][k] % mod;
        if (tmp[j][k] < 0) tmp[j][k] += mod;
      }
    }
    for (int j = i + 1; j < n; j++) a[i][j] = 1LL * a[i][j] * v % mod;
    for (int j = 0; j < n; j++) tmp[i][j] = 1LL * tmp[i][j] * v % mod;
    a[i][i] = 1;
  }
  for (int i = n - 1; i > 0; --i) {
    for (int j = 0; j < i; j++) {
      int v = a[j][i];
      for (int k = 0; k < n; k++) {
        tmp[j][k] -= 1LL * v * tmp[i][k]%mod;
        if (tmp[j][k] < 0) tmp[j][k] += mod;
      }
    }
  }
  ans.resize(n, vector<int>(n));
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      ans[col[i]][col[j]] = tmp[i][j];
    }
  }
  return true; //invertible
}
int32_t main() {
  int n, mod;
  while (cin >> n >> mod && n && mod) {
    vector<vector<int>> a(n, vector<int>(n, 0));
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) cin >> a[i][j];
    }
    vector<vector<int>> ans;
    int ok = Gauss(a, ans, mod);
    if (!ok) {
      cout << "singular\n";
      continue;
    }
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) cout << ans[i][j] << ' ';
      cout << '\n';
    }
  }
  return 0;
}
