#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;
long long pref_sum[N];
int a[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  for (int i = 1; i <= n; i++) {
    pref_sum[i] = pref_sum[i - 1] + a[i];
  }
  while (q--) {
    int l, r; cin >> l >> r;
    cout << pref_sum[r] - pref_sum[l - 1] << '\n';
  }
  return 0;
}
// https://cses.fi/problemset/task/1646/
