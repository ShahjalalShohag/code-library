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
  int swaps = 0;
  for (int i = 1; i <= n; i++) {
    int min_index = i;
    for (int j = i + 1; j <= n; j++) {
      if (a[j] < a[min_index]) {
        min_index = j;
      }
    }
    if (i != min_index) {
      swap(a[i], a[min_index]);
      ++swaps;
    }
  }
  for (int i = 1; i <= n; i++) {
    if (i > 1) cout << ' ';
    cout << a[i];
  }
  cout << '\n';
  cout << swaps << '\n';
  return 0;
}
// https://vjudge.net/problem/Aizu-ALDS1_2_B
