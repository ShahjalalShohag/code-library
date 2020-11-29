#include<bits/stdc++.h>
using namespace std;

const int mod = 1e9 + 7;

// Given two arrays and x
// Matrix M(i, j) = a(i) * b(j) if i != j, (x + a(i) * b(j)) if i == j
// Find the determinant
// Solution = x^n + x^(n - 1) * sum_of(a(i) * b(i))

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, x; 
  while (cin >> n >> x) {
    vector<int> a(n), b(n);
    for (int i = 0; i < n; i++) {
      cin >> a[i];
    }    
    for (int i = 0; i < n; i++) {
      cin >> b[i];
    }
    int ans = 0;
    for (int i = 0; i < n; i++) {
      ans += 1LL * a[i] * b[i] % mod;
      ans %= mod;
    }
    int p = x;
    for (int i = 0; i + 1 < n; i++) {
      ans = 1LL * ans * x % mod;
      p = 1LL * p * x % mod;
    }
    ans = (ans + p) % mod;
    cout << ans << '\n';
  }
  return 0;
}
// https://official.contest.yandex.ru/opencupXXI/contest/23124/problems/D/?success=43031828#7/2020_11_27/m1N1edUCxn