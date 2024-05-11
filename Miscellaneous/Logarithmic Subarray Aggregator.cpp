#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;
int a[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }

  vector<pair<int, int>> v; // stores the number of times a subarray gcd occurs ending at each index
  map<int, long long> ans;
  for (int i = 1; i <= n; i++) {
    vector<pair<int, int>> u;
    u.push_back({a[i], 1});
    for (auto [x, c]: v) {
      int g = __gcd(x, a[i]);
      if (g == u.back().first) {
        u.back().second += c;
      } else {
        u.push_back({g, c});
      }
    }
    v = u;
    for (auto [x, c]: v) {
      ans[x] += c;
    }
  }

  int q; cin >> q;
  while (q--) {
    int x; cin >> x;
    cout << ans[x] << '\n';
  }
  return 0;
}
// https://codeforces.com/contest/475/problem/D
