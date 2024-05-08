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
  for (int step = 1; step <= n - 1; step++) {
    for (int i = 1; i < n; i++) {
      if (a[i] > a[i + 1]) {
        swap(a[i], a[i + 1]);
        ++swaps;
      }
    }
  }
  cout << "Array is sorted in " << swaps << " swaps.\n";
  cout << "First Element: " << a[1] << '\n';
  cout << "Last Element: " << a[n] << '\n';
  return 0;
}
// https://vjudge.net/problem/HackerRank-ctci-bubble-sort
