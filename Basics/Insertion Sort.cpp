#include<bits/stdc++.h>
using namespace std;

int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n; cin >> n;
  int a[n + 1];
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  for (int i = 1; i <= n; i++) {
    int j = i - 1, x = a[i];
    while (j >= 1 and a[j] > x) {
      a[j + 1] = a[j];
      j--;
    }
    a[j + 1] = x;
  }
  for (int i = 1; i <= n; i++) {
    cout << a[i] << ' ';
  }
  cout << '\n';
  return 0;
}
