#include<bits/stdc++.h>
using namespace std;
 
int32_t main() {
  ios_base::sync_with_stdio(0);
  cin.tie(0);
  int n, q; cin >> n >> q;
  int a[n + 1];
  for (int i = 1; i <= n; i++) {
    cin >> a[i];
  }
  // a is already sorted
  while (q--) {
    int x; cin >> x;
    int l = 1, r = n;
    bool found = false;
    while (l <= r) {
      int mid = (l + r) / 2;
      if (a[mid] == x) {
        found = true;
        break;
      }
      else if (a[mid] < x) {
        l = mid + 1;
      }
      else {
        r = mid - 1;
      }
    }
    if (found) {
      cout << "YES\n";
    }
    else {
      cout << "NO\n";
    }
  }
  return 0;
}
// https://codeforces.com/edu/course/2/lesson/6/1/practice/contest/283911/problem/A
