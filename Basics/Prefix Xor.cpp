#include<bits/stdc++.h>
using namespace std;

const int N = 2e5 + 9;
int pref_xor[N];
int a[N];
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  for (int i = 1; i <= n; i++) {
    pref_xor[i] = pref_xor[i - 1] ^ a[i];
  }
  while (q--) {
    int l, r; cin >> l >> r;
    cout << (pref_xor[r] ^ pref_xor[l - 1]) << '\n';
  }
  return 0;
}
// https://cses.fi/problemset/task/1646/
