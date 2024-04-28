#include<bits/stdc++.h>
using namespace std;

const int N = 1e6 + 9, mod = 1e9 + 7;

int d[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  d[0] = 1; d[1] = 0;
  for (int i = 1; i < N; i++) {
    d[i] = 1LL * (i - 1) * (d[i - 1] + d[i - 2]) % mod;
  }
  int n; cin >> n;
  cout << d[n] << '\n';
  return 0;
}
// https://cses.fi/problemset/task/1717
