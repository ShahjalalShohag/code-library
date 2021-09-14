#include<bits/stdc++.h>
using namespace std;

const int N = 1e5 + 9;

int t[N][18], a[N];
void build(int n) {
  for(int i = 1; i <= n; ++i) t[i][0] = a[i];
  for(int k = 1; k < 18; ++k) {
    for(int i = 1; i + (1 << k) - 1 <= n; ++i) {
      t[i][k] = min(t[i][k - 1], t[i + (1 << (k - 1))][k - 1]);
    }
  }
}

int query(int l, int r) {
  int k = 31 - __builtin_clz(r - l + 1);
  return min(t[l][k], t[r - (1 << k) + 1][k]);
}

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);

  int n;
  cin >> n;
  for(int i = 1; i <= n; i++) cin >> a[i];
  build(n);
  int q;
  cin >> q;
  while(q--) {
    int l, r;
    cin >> l >> r;
    ++l;
    ++r;
    cout << query(l, r) << '\n';
  }
  return 0;
}
