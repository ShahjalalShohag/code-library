#include<bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9;

vector<pair<int, int>> ans[N]; //ans[c] = {{a, b}}, s.t. a^2 + b^2 = c^2
int32_t main() {
  vector<pair<int, int>> v;
  int cnt = 0, p = 0;
  for (int m = 1; m * m < N; m++) {
    for (int n = 1; n < m; n++) {
      if ((n & 1) == (m & 1)) continue;
      int a = m * m - n * n, b = 2 * m * n, c = m * m + n * n;
      if (__gcd(a, b) > 1) continue;
      for (int k = 1; k * c < N && k <= 1 ; k++) {
        ans[c * k].emplace_back(a * k, b * k);
        cnt++;
      }
    }
  }
  int sz = 0;
  // for (int i = 1; i < N; i++) sz = max(sz, (int)ans[i].size());
  // cout << cnt << ' ' << sz << '\n';
  for (int i = 1; i <= 1000; i++) {
    for (auto x : ans[i]) cout << x.first << ' ' << x.second << ' ' << i << '\n';
  }
  return 0;
}
// https://en.wikipedia.org/wiki/Pythagorean_triple#Generating_a_triple
